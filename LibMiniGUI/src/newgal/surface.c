/*
**  $Id: surface.c 7344 2007-08-16 03:56:40Z xgwang $
**  
**  Copyright (C) 2003 ~ 2007 Feynman Software.
**  Copyright (C) 2001 ~ 2002 Wei Yongming.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "newgal.h"
#include "sysvideo.h"
#include "blit.h"
#include "RLEaccel_c.h"
#include "pixels_c.h"
#include "memops.h"
#include "leaks.h"

/* Public routines */
/*
 * Create an empty RGB surface of the appropriate depth
 */
GAL_Surface * GAL_CreateRGBSurface (Uint32 flags,
            int width, int height, int depth,
            Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    GAL_VideoDevice *video = current_video;
    GAL_VideoDevice *this  = current_video;
    GAL_Surface *screen;
    GAL_Surface *surface;

    /* Check to see if we desire the surface in video memory */
    if ( video ) {
        screen = GAL_PublicSurface;
    } else {
        screen = NULL;
    }
    if ( screen && ((screen->flags&GAL_HWSURFACE) == GAL_HWSURFACE) ) {
        if ( (flags&(GAL_SRCCOLORKEY|GAL_SRCALPHA)) != 0 ) {
            flags |= GAL_HWSURFACE;
        }
        if ( (flags & GAL_SRCCOLORKEY) == GAL_SRCCOLORKEY ) {
            if ( ! current_video->info.blit_hw_CC ) {
                flags &= ~GAL_HWSURFACE;
            }
        }
        if ( (flags & GAL_SRCALPHA) == GAL_SRCALPHA ) {
            if ( ! current_video->info.blit_hw_A ) {
                flags &= ~GAL_HWSURFACE;
            }
        }
    } else {
        flags &= ~GAL_HWSURFACE;
    }

    /* Allocate the surface */
    surface = (GAL_Surface *)malloc(sizeof(*surface));
    if ( surface == NULL ) {
        GAL_OutOfMemory();
        return(NULL);
    }
    if ((flags & GAL_HWSURFACE) == GAL_HWSURFACE)
        surface->video = current_video;
    else
        surface->video = NULL;

    surface->flags = GAL_SWSURFACE;

    if ( (flags & GAL_HWSURFACE) == GAL_HWSURFACE ) {
        depth = screen->format->BitsPerPixel;
        Rmask = screen->format->Rmask;
        Gmask = screen->format->Gmask;
        Bmask = screen->format->Bmask;
        Amask = screen->format->Amask;
    }
    surface->format = GAL_AllocFormat(depth, Rmask, Gmask, Bmask, Amask);
    if ( surface->format == NULL ) {
        free(surface);
        return(NULL);
    }
    if ( Amask ) {
        surface->flags |= GAL_SRCALPHA;
    }
    surface->w = width;
    surface->h = height;
    surface->pitch = GAL_CalculatePitch(surface);
    surface->pixels = NULL;
    surface->offset = 0;
    surface->hwdata = NULL;
    surface->map = NULL;
    surface->format_version = 0;
    GAL_SetClipRect(surface, NULL);

    /* Get the pixels */
    if ( ((flags&GAL_HWSURFACE) == GAL_SWSURFACE) || 
                (video->AllocHWSurface(this, surface) < 0) ) {
        if ( surface->w && surface->h ) {
            surface->pixels = malloc(surface->h*surface->pitch);
            if ( surface->pixels == NULL ) {
                GAL_FreeSurface(surface);
                GAL_OutOfMemory();
                return(NULL);
            }
            /* This is important for bitmaps */
            memset(surface->pixels, 0, surface->h*surface->pitch);
            surface->flags &= ~GAL_HWSURFACE;
        }
    }

    /* Allocate an empty mapping */
    surface->map = GAL_AllocBlitMap();
    if ( surface->map == NULL ) {
        GAL_FreeSurface(surface);
        return(NULL);
    }

    /* The surface is ready to go */
    surface->refcount = 1;
#ifdef CHECK_LEAKS
    ++surfaces_allocated;
#endif
    return(surface);
}
/*
 * Create an RGB surface from an existing memory buffer
 */
GAL_Surface * GAL_CreateRGBSurfaceFrom (void *pixels,
            int width, int height, int depth, int pitch,
            Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    GAL_Surface *surface;

    surface = GAL_CreateRGBSurface(GAL_SWSURFACE, 0, 0, depth,
                                   Rmask, Gmask, Bmask, Amask);
    if ( surface != NULL ) {
        surface->flags |= GAL_PREALLOC;
        surface->pixels = pixels;
        surface->w = width;
        surface->h = height;
        surface->pitch = pitch;
        GAL_SetClipRect(surface, NULL);
    }
    return(surface);
}
/*
 * Set the color key in a blittable surface
 */
int GAL_SetColorKey (GAL_Surface *surface, Uint32 flag, Uint32 key)
{
    /* Sanity check the flag as it gets passed in */
    if ( flag & GAL_SRCCOLORKEY ) {
        if ( flag & (GAL_RLEACCEL|GAL_RLEACCELOK) ) {
            flag = (GAL_SRCCOLORKEY | GAL_RLEACCELOK);
        } else {
            flag = GAL_SRCCOLORKEY;
        }
    } else {
        flag = 0;
    }

    /* Optimize away operations that don't change anything */
    if ( (flag == (surface->flags & (GAL_SRCCOLORKEY|GAL_RLEACCELOK))) &&
         (key == surface->format->colorkey) ) {
        return(0);
    }

    /* UnRLE surfaces before we change the colorkey */
    if ( surface->flags & GAL_RLEACCEL ) {
            GAL_UnRLESurface(surface, 1);
    }

    if ( flag ) {
        GAL_VideoDevice *video = current_video;
        GAL_VideoDevice *this  = current_video;

        surface->flags |= GAL_SRCCOLORKEY;
        surface->format->colorkey = key;
        if ( (surface->flags & GAL_HWACCEL) == GAL_HWACCEL ) {
            if ( (video->SetHWColorKey == NULL) ||
                 (video->SetHWColorKey(this, surface, key) < 0) ) {
                surface->flags &= ~GAL_HWACCEL;
            }
        }
        if ( flag & GAL_RLEACCELOK ) {
            surface->flags |= GAL_RLEACCELOK;
        } else {
            surface->flags &= ~GAL_RLEACCELOK;
        }
    } else {
        surface->flags &= ~(GAL_SRCCOLORKEY|GAL_RLEACCELOK);
        surface->format->colorkey = 0;
    }
    GAL_InvalidateMap(surface->map);
    return(0);
}
int GAL_SetAlpha (GAL_Surface *surface, Uint32 flag, Uint8 value)
{
    Uint32 oldflags = surface->flags;
    Uint32 oldalpha = surface->format->alpha;

    /* Sanity check the flag as it gets passed in */
    if ( flag & GAL_SRCALPHA ) {
        if ( flag & (GAL_RLEACCEL|GAL_RLEACCELOK) ) {
            flag = (GAL_SRCALPHA | GAL_RLEACCELOK);
        } else {
            flag = GAL_SRCALPHA;
        }
    } else {
        flag = 0;
    }

    /* Optimize away operations that don't change anything */
    if ( (flag == (surface->flags & (GAL_SRCALPHA|GAL_RLEACCELOK))) &&
         (!flag || value == oldalpha) ) {
        return(0);
    }

    if(!(flag & GAL_RLEACCELOK) && (surface->flags & GAL_RLEACCEL))
        GAL_UnRLESurface(surface, 1);

    if ( flag ) {
        GAL_VideoDevice *video = current_video;
        GAL_VideoDevice *this  = current_video;

        surface->flags |= GAL_SRCALPHA;
        surface->format->alpha = value;
        if ( (surface->flags & GAL_HWACCEL) == GAL_HWACCEL ) {
            if ( (video->SetHWAlpha == NULL) ||
                 (video->SetHWAlpha(this, surface, value) < 0) ) {
                surface->flags &= ~GAL_HWACCEL;
            }
        }
        if ( flag & GAL_RLEACCELOK ) {
                surface->flags |= GAL_RLEACCELOK;
        } else {
                surface->flags &= ~GAL_RLEACCELOK;
        }
    } else {
        surface->flags &= ~GAL_SRCALPHA;
        surface->format->alpha = GAL_ALPHA_OPAQUE;
    }
    /*
     * The representation for software surfaces is independent of
     * per-surface alpha, so no need to invalidate the blit mapping
     * if just the alpha value was changed. (If either is 255, we still
     * need to invalidate.)
     */
    if((surface->flags & GAL_HWACCEL) == GAL_HWACCEL
       || oldflags != surface->flags
       || (((oldalpha + 1) ^ (value + 1)) & 0x100))
        GAL_InvalidateMap(surface->map);
    return(0);
}

/*
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */
static inline
GAL_bool GAL_IntersectRect(const GAL_Rect *A, const GAL_Rect *B, GAL_Rect *intersection)
{
    int Amin, Amax, Bmin, Bmax;

    /* Horizontal intersection */
    Amin = A->x;
    Amax = Amin + A->w;
    Bmin = B->x;
    Bmax = Bmin + B->w;
    if(Bmin > Amin)
            Amin = Bmin;
    intersection->x = Amin;
    if(Bmax < Amax)
            Amax = Bmax;
    intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

    /* Vertical intersection */
    Amin = A->y;
    Amax = Amin + A->h;
    Bmin = B->y;
    Bmax = Bmin + B->h;
    if(Bmin > Amin)
            Amin = Bmin;
    intersection->y = Amin;
    if(Bmax < Amax)
            Amax = Bmax;
    intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;

    return (intersection->w && intersection->h);
}
/*
 * Set the clipping rectangle for a blittable surface
 */
GAL_bool GAL_SetClipRect(GAL_Surface *surface, GAL_Rect *rect)
{
    GAL_Rect full_rect;

    /* Don't do anything if there's no surface to act on */
    if ( ! surface ) {
        return GAL_FALSE;
    }

    /* Set up the full surface rectangle */
    full_rect.x = 0;
    full_rect.y = 0;
    full_rect.w = surface->w;
    full_rect.h = surface->h;

    /* Set the clipping rectangle */
    if ( ! rect ) {
        surface->clip_rect = full_rect;
        return 1;
    }
    return GAL_IntersectRect(rect, &full_rect, &surface->clip_rect);
}
void GAL_GetClipRect(GAL_Surface *surface, GAL_Rect *rect)
{
    if ( surface && rect ) {
        *rect = surface->clip_rect;
    }
}
/* 
 * Set up a blit between two surfaces -- split into three parts:
 * The upper part, GAL_UpperBlit(), performs clipping and rectangle 
 * verification.  The lower part is a pointer to a low level
 * accelerated blitting function.
 *
 * These parts are separated out and each used internally by this 
 * library in the optimimum places.  They are exported so that if
 * you know exactly what you are doing, you can optimize your code
 * by calling the one(s) you need.
 */
int GAL_LowerBlit (GAL_Surface *src, GAL_Rect *srcrect,
                GAL_Surface *dst, GAL_Rect *dstrect)
{
    GAL_blit do_blit;

    /* Check to make sure the blit mapping is valid */
    if ( (src->map->dst != dst) ||
             (src->map->dst->format_version != src->map->format_version) ) {
        if ( GAL_MapSurface(src, dst) < 0 ) {
            return(-1);
        }
    }

    /* Figure out which blitter to use */
    if ( (src->flags & GAL_HWACCEL) == GAL_HWACCEL ) {
        do_blit = src->map->hw_blit;
    } else {
        do_blit = src->map->sw_blit;
    }
    return(do_blit(src, srcrect, dst, dstrect));
}


int GAL_UpperBlit (GAL_Surface *src, GAL_Rect *srcrect,
           GAL_Surface *dst, GAL_Rect *dstrect)
{
    GAL_Rect fulldst;
    int srcx, srcy, w, h;

    if ( ! src || ! dst ) {
        GAL_SetError("NEWGAL: GAL_UpperBlit: passed a NULL surface.\n");
        return(-1);
    }

    /* If the destination rectangle is NULL, use the entire dest surface */
    if ( dstrect == NULL ) {
        fulldst.x = fulldst.y = 0;
        dstrect = &fulldst;
    }

    /* clip the source rectangle to the source surface */
    if(srcrect) {
        int maxw, maxh;
    
        srcx = srcrect->x;
        w = srcrect->w;
        if(srcx < 0) {
            w += srcx;
            dstrect->x -= srcx;
            srcx = 0;
        }
        maxw = src->w - srcx;
        if(maxw < w)
            w = maxw;

        srcy = srcrect->y;
        h = srcrect->h;
        if(srcy < 0) {
                h += srcy;
            dstrect->y -= srcy;
            srcy = 0;
        }
        maxh = src->h - srcy;
        if(maxh < h)
            h = maxh;
        
    } else {
        srcx = srcy = 0;
        w = src->w;
        h = src->h;
    }

    /* clip the destination rectangle against the clip rectangle */
    {
        GAL_Rect *clip = &dst->clip_rect;
        int dx, dy;

        dx = clip->x - dstrect->x;
        if(dx > 0) {
            w -= dx;
            dstrect->x += dx;
            srcx += dx;
        }
        dx = dstrect->x + w - clip->x - clip->w;
        if(dx > 0)
            w -= dx;

        dy = clip->y - dstrect->y;
        if(dy > 0) {
            h -= dy;
            dstrect->y += dy;
            srcy += dy;
        }
        dy = dstrect->y + h - clip->y - clip->h;
        if(dy > 0)
            h -= dy;
    }

    if(w > 0 && h > 0) {
        GAL_Rect sr;
        sr.x = srcx;
        sr.y = srcy;
        sr.w = dstrect->w = w;
        sr.h = dstrect->h = h;
        return GAL_LowerBlit(src, &sr, dst, dstrect);
    }
    dstrect->w = dstrect->h = 0;
    return 0;
}

/* 
 * This function performs a fast fill of the given rectangle with 'color'
 */
int GAL_FillRect(GAL_Surface *dst, const GAL_Rect *dstrect, Uint32 color)
{
    GAL_VideoDevice *video = dst->video;
    GAL_VideoDevice *this  = dst->video;
    int x, y;
    Uint8 *row;
    GAL_Rect my_dstrect;

    if (!video)
    {
        video = current_video;
        this = current_video;
    }

    /* If 'dstrect' == NULL, then fill the whole surface */
    if ( dstrect ) {
        /* Perform clipping */
        if ( !GAL_IntersectRect(dstrect, &dst->clip_rect, &my_dstrect) ) {
            return(0);
        }
    } else {
        my_dstrect = dst->clip_rect;
    }

    /* Check for hardware acceleration */
    if ( ((dst->flags & GAL_HWSURFACE) == GAL_HWSURFACE) &&
                    video->info.blit_fill ) {
        return(video->FillHWRect(this, dst, &my_dstrect, color));
    }

    row = (Uint8 *)dst->pixels+my_dstrect.y*dst->pitch+
            my_dstrect.x*dst->format->BytesPerPixel;
    if ( dst->format->palette || (color == 0) ) {
        x = my_dstrect.w*dst->format->BytesPerPixel;
        if ( !color && !((long)row&3) && !(x&3) && !(dst->pitch&3) ) {
            int n = x >> 2;
            for ( y=my_dstrect.h; y; --y ) {
                GAL_memset4(row, 0, n);
                row += dst->pitch;
            }
        } else {
#if 0
            /*
             * memset() on PPC (both glibc and codewarrior) uses
             * the dcbz (Data Cache Block Zero) instruction, which
             * causes an alignment exception if the destination is
             * uncachable, so only use it on software surfaces
             */
            if((dst->flags & GAL_HWSURFACE) == GAL_HWSURFACE) {
                if(my_dstrect.w >= 8) {
                    /*
                     * 64-bit stores are probably most
                     * efficient to uncached video memory
                     */
                    double fill;
                    memset(&fill, color, (sizeof fill));
                    for(y = my_dstrect.h; y; y--) {
                        Uint8 *d = row;
                        unsigned n = x;
                        unsigned nn;
                        Uint8 c = color;
                        double f = fill;
                        while((unsigned long)d
                              & (sizeof(double) - 1)) {
                            *d++ = c;
                            n--;
                        }
                        nn = n / (sizeof(double) * 4);
                        while(nn) {
                            ((double *)d)[0] = f;
                            ((double *)d)[1] = f;
                            ((double *)d)[2] = f;
                            ((double *)d)[3] = f;
                            d += 4*sizeof(double);
                            nn--;
                        }
                        n &= ~(sizeof(double) * 4 - 1);
                        nn = n / sizeof(double);
                        while(nn) {
                            *(double *)d = f;
                            d += sizeof(double);
                            nn--;
                        }
                        n &= ~(sizeof(double) - 1);
                        while(n) {
                            *d++ = c;
                            n--;
                        }
                        row += dst->pitch;
                    }
                } else {
                    /* narrow boxes */
                    for(y = my_dstrect.h; y; y--) {
                        Uint8 *d = row;
                        Uint8 c = color;
                        int n = x;
                        while(n) {
                            *d++ = c;
                            n--;
                        }
                        row += dst->pitch;
                    }
                }
            } else
#endif /* __powerpc__ */
            {
                for(y = my_dstrect.h; y; y--) {
                    memset(row, color, x);
                    row += dst->pitch;
                }
            }
        }
    } else {
        switch (dst->format->BytesPerPixel) {
            case 2:
            for ( y=my_dstrect.h; y; --y ) {
                Uint16 *pixels = (Uint16 *)row;
                Uint16 c = color;
                Uint32 cc = (Uint32)c << 16 | c;
                int n = my_dstrect.w;
                if((unsigned long)pixels & 3) {
                    *pixels++ = c;
                    n--;
                }
                if(n >> 1)
                    GAL_memset4(pixels, cc, n >> 1);
                if(n & 1)
                    pixels[n - 1] = c;
                row += dst->pitch;
            }
            break;

            case 3:
            if(GAL_BYTEORDER == GAL_BIG_ENDIAN)
                color <<= 8;
            for ( y=my_dstrect.h; y; --y ) {
                Uint8 *pixels = row;
                for ( x=my_dstrect.w; x; --x ) {
                    memcpy(pixels, &color, 3);
                    pixels += 3;
                }
                row += dst->pitch;
            }
            break;

            case 4:
            for(y = my_dstrect.h; y; --y) {
                GAL_memset4(row, color, my_dstrect.w);
                row += dst->pitch;
            }
            break;
        }
    }

    /* We're done! */
    return(0);
}

/* 
 * Calculate the pad-aligned box sizew in a surface
 */
Uint32 GAL_GetBoxSize (GAL_Surface *surface, Uint32 w, Uint32 h, Uint32* pitch_p)
{
    Uint32 pitch;

    /* Box should be 4-byte aligned for speed */
    pitch = w * surface->format->BytesPerPixel;
    switch (surface->format->BitsPerPixel) {
        case 1:
            pitch = (pitch+7)/8;
            break;
        case 4:
            pitch = (pitch+1)/2;
            break;
        default:
            break;
    }

    pitch = (pitch + 3) & ~3;    /* 4-byte aligning */

    if (pitch_p)
        *pitch_p = pitch;

    return pitch * h;
}

/* 
 * This function performs a fast HW -> SW box copying.
 */
int GAL_GetBox (GAL_Surface *src, const GAL_Rect *rect, BITMAP* box)
{
    Sint32 dst_x, dst_y, off_x, off_y, y;
    Uint8 *srcrow, *dstrow;
    Uint32 linelen;
    GAL_Rect srcrect = {0, 0, 0, 0};
    srcrect.w = src->w;
    srcrect.h = src->h;

    if (rect->h <= 0 || rect->w <= 0)
        return -1;

    box->bmType = BMP_TYPE_NORMAL;
    box->bmBitsPerPixel = src->format->BitsPerPixel;
    box->bmBytesPerPixel = src->format->BytesPerPixel;
    box->bmWidth = rect->w;
    box->bmHeight = rect->h;
    box->bmAlphaPixelFormat = NULL;

    dst_x = rect->x;
    dst_y = rect->y;

    /* Perform clipping */
    if (!GAL_IntersectRect (rect, &srcrect, &srcrect)) {
        return 0;
    }

    off_x = srcrect.x - dst_x;
    off_y = srcrect.y - dst_y;

    if ((box->bmPitch == 0) || (box->bmBits == NULL)) {
        box->bmBits = malloc (GAL_GetBoxSize (src, box->bmWidth, box->bmHeight, &box->bmPitch));
        if (box->bmBits == NULL) {
            GAL_OutOfMemory ();
            return -1;
        }
    }

    linelen = srcrect.w * box->bmBytesPerPixel;
    srcrow = (Uint8 *)src->pixels + srcrect.y * src->pitch +
            srcrect.x * src->format->BytesPerPixel;
    dstrow = box->bmBits + off_y * box->bmPitch +
            off_x * box->bmBytesPerPixel;

    switch (box->bmBytesPerPixel) {
        case 1:
            if (((DWORD)srcrow & 3) || ((DWORD)dstrow & 3) 
                    || (linelen & 3) || (box->bmPitch & 3) || (src->pitch & 3))
                goto slow_copy;
            else {
                int n = linelen >> 2;
                for (y = srcrect.h; y; --y) {
                    GAL_memcpy4 (dstrow, srcrow, n);
                    srcrow += src->pitch;
                    dstrow += box->bmPitch;
                }
            }
            break;

        case 2:
            if ((((DWORD)srcrow & 3) != ((DWORD)dstrow & 3)) 
                    || (box->bmPitch & 3) || (src->pitch & 3))
                goto slow_copy;

            for (y = srcrect.h; y; --y) {
                Uint16 *dstpixels = (Uint16 *)dstrow;
                Uint16 *srcpixels = (Uint16 *)srcrow;
                int n = srcrect.w;
                if ((DWORD)dstpixels & 3) {
                    *dstpixels = *srcpixels;
                    dstpixels++;
                    srcpixels++;
                    n--;
                }

                if (n >> 1)
                    GAL_memcpy4 (dstpixels, srcpixels, n >> 1);
                if (n & 1)
                    dstpixels [n - 1] = srcpixels [n - 1];

                srcrow += src->pitch;
                dstrow += box->bmPitch;
            }
            break;

        case 3:
            goto slow_copy;

        case 4:
            for (y = srcrect.h; y; --y) {
                GAL_memcpy4 (dstrow, srcrow, srcrect.w);
                srcrow += src->pitch;
                dstrow += box->bmPitch;
            }
            break;
    }

    return(0);

slow_copy:
    for (y = srcrect.h; y; --y) {
        GAL_memcpy (dstrow, srcrow, linelen);
        srcrow += src->pitch;
        dstrow += box->bmPitch;
    }

    return(0);
}

/* 
 * This function performs a fast SW -> HW box copying.
 */

// helpers

#include "blit.h"

static int _PutBoxAlpha (GAL_Surface* dst, BYTE* dstrow, BYTE* srcrow, Uint32 w, Uint32 h, BITMAP* box)
{
    BYTE* dstpixels = dstrow;
    BYTE* srcpixels = srcrow;
    GAL_PixelFormat *dstfmt = dst->format;
    GAL_PixelFormat *srcfmt = box->bmAlphaPixelFormat;
    int bpp = dstfmt->BytesPerPixel;
    unsigned alpha = dstfmt->Amask ? GAL_ALPHA_OPAQUE : 0;

    if (bpp == 1)
        return -1;

    while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            unsigned sA;
            unsigned dR;
            unsigned dG;
            unsigned dB;
            DISEMBLE_RGBA (srcpixels, bpp, srcfmt, pixel, sR, sG, sB, sA);
            DISEMBLE_RGB (dstpixels, bpp, dstfmt, pixel, dR, dG, dB);
		    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB);
            ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, dR, dG, dB, alpha);
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }

    return 0;
}

static int _PutBoxAlphaChannel (GAL_Surface* dst, BYTE* dstrow, BYTE* srcrow, Uint32 w, Uint32 h, BITMAP* box)
{
    BYTE* dstpixels = dstrow;
    BYTE* srcpixels = srcrow;
    GAL_PixelFormat *dstfmt = dst->format;
    int bpp = dstfmt->BytesPerPixel;
    unsigned alpha = box->bmAlpha;

    if (bpp == 1)
        return -1;

    while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            unsigned dR;
            unsigned dG;
            unsigned dB;
            DISEMBLE_RGB (srcpixels, bpp, dstfmt, pixel, sR, sG, sB);
            DISEMBLE_RGB (dstpixels, bpp, dstfmt, pixel, dR, dG, dB);
		    ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB);
            ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, dR, dG, dB, alpha);
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }

    return 0;
}

static int _PutBoxKeyAlpha (GAL_Surface* dst, BYTE* dstrow, BYTE* srcrow, Uint32 w, Uint32 h, BITMAP* box)
{
    BYTE* dstpixels = dstrow;
    BYTE* srcpixels = srcrow;
    Uint32 ckey = box->bmColorKey;
    GAL_PixelFormat *dstfmt = dst->format;
    GAL_PixelFormat *srcfmt = box->bmAlphaPixelFormat;
    int bpp = dstfmt->BytesPerPixel;
    Uint32 rgbmask = ~srcfmt->Amask;
    unsigned alpha = dstfmt->Amask ? GAL_ALPHA_OPAQUE : 0;

    if (bpp == 1)
        return -1;

    while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            unsigned sA;
            unsigned dR;
            unsigned dG;
            unsigned dB;
            RETRIEVE_RGB_PIXEL (srcpixels, bpp, pixel);
            if ((pixel & rgbmask) != ckey ) {
                RGBA_FROM_PIXEL (pixel, srcfmt, sR, sG, sB, sA);
                DISEMBLE_RGB (dstpixels, bpp, dstfmt, pixel, dR, dG, dB);
		        ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB);
                ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, dR, dG, dB, alpha);
            }
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }

    return 0;
}

static int _PutBoxKey (GAL_Surface* dst, BYTE* dstrow, BYTE* srcrow, Uint32 w, Uint32 h, BITMAP* box)
{
    BYTE* dstpixels = dstrow;
    BYTE* srcpixels = srcrow;
    Uint32 ckey = box->bmColorKey;
    GAL_PixelFormat *dstfmt = dst->format;
    int bpp = dstfmt->BytesPerPixel;
    unsigned alpha = dstfmt->Amask ? GAL_ALPHA_OPAQUE : 0;

    if (bpp == 1) {
        while (h--) {
            int i;
            dstpixels = dstrow;
            srcpixels = srcrow;
            for (i = 0; i < w; i++) {
                if (*srcpixels != ckey)
                    *dstpixels = *srcpixels;
                dstpixels += bpp;
                srcpixels += bpp;
            }
            srcrow += box->bmPitch;
            dstrow += dst->pitch;
        }
    }
#ifndef _FOR_MONOBITMAP
    else while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            RETRIEVE_RGB_PIXEL (srcpixels, bpp, pixel);
            if ( pixel != ckey ) {
                RGB_FROM_PIXEL (pixel, dstfmt, sR, sG, sB);
                ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, sR, sG, sB, alpha);
            }
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }
#else
    else while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            RETRIEVE_RGB_PIXEL (srcpixels, bpp, pixel);
            if (pixel != ckey || ( pixel == ckey && box->bmType & BMP_TYPE_MONOKEY) ) {
                if (pixel == ckey)
                    pixel = box->bmColorRep;
                RGB_FROM_PIXEL (pixel, dstfmt, sR, sG, sB);
                ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, sR, sG, sB, alpha);
            }
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }
#endif

    return 0;
}

static int _PutBoxKeyAlphaChannel (GAL_Surface* dst, BYTE* dstrow, BYTE* srcrow, Uint32 w, Uint32 h, BITMAP* box)
{
    BYTE* dstpixels = dstrow;
    BYTE* srcpixels = srcrow;
    Uint32 ckey = box->bmColorKey;
    GAL_PixelFormat *dstfmt = dst->format;
    int bpp = dstfmt->BytesPerPixel;
    unsigned alpha = box->bmAlpha;

    if (bpp == 1)
        return -1;

    while ( h-- ) {
        dstpixels = dstrow;
        srcpixels = srcrow;
        DUFFS_LOOP(
        {
            Uint32 pixel;
            unsigned sR;
            unsigned sG;
            unsigned sB;
            unsigned dR;
            unsigned dG;
            unsigned dB;
            RETRIEVE_RGB_PIXEL (srcpixels, bpp, pixel);
            if ( pixel != ckey ) {
                RGB_FROM_PIXEL (pixel, dstfmt, sR, sG, sB);
                DISEMBLE_RGB (dstpixels, bpp, dstfmt, pixel, dR, dG, dB);
		        ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB);
                ASSEMBLE_RGBA (dstpixels, bpp, dstfmt, dR, dG, dB, alpha);
            }
            dstpixels += bpp;
            srcpixels += bpp;
        },
        w);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }

    return 0;
}

int GAL_PutBox (GAL_Surface *dst, const GAL_Rect *dstrect, BITMAP* box)
{
    Uint32 box_x, box_y, off_x, off_y;
    int y, w, h;
    Uint8 *srcrow, *dstrow;
    Uint32 linelen;
    GAL_Rect my_dstrect;

    /* If 'dstrect' == NULL, then put to (0, 0) */
    if (dstrect) {
        box_x = dstrect->x;
        box_y = dstrect->y;

        /* Perform clipping */
        if (!GAL_IntersectRect (dstrect, &dst->clip_rect, &my_dstrect)) {
            return 0;
        }
    } else {
        box_x = 0;
        box_y = 0;

        my_dstrect = dst->clip_rect;
    }

    off_x = my_dstrect.x - box_x;
    off_y = my_dstrect.y - box_y;
    if (off_x >= box->bmWidth || off_y >= box->bmHeight ||
            box->bmBytesPerPixel != dst->format->BytesPerPixel) {
        return 0;
    }

    dstrow = (Uint8 *)dst->pixels + my_dstrect.y * dst->pitch +
            my_dstrect.x * dst->format->BytesPerPixel;
    srcrow = (Uint8 *)box->bmBits + off_y * box->bmPitch +
            off_x * box->bmBytesPerPixel;
    w = MIN (my_dstrect.w, box->bmWidth);
    h = MIN (my_dstrect.h, box->bmHeight);
    linelen = w * box->bmBytesPerPixel;

    /* TODO: Check for hardware acceleration here */

    if (box->bmAlphaPixelFormat != NULL 
                    && box->bmAlphaPixelFormat != dst->format) {
        if ((box->bmType & BMP_TYPE_ALPHA) && (box->bmType & BMP_TYPE_COLORKEY)) {
            return _PutBoxKeyAlpha (dst, dstrow, srcrow, w, h, box);
        }
        else if (box->bmType & BMP_TYPE_ALPHA) {
            return _PutBoxAlpha (dst, dstrow, srcrow, w, h, box);
        }
    }
    else if ((box->bmType & BMP_TYPE_ALPHACHANNEL) && (box->bmType & BMP_TYPE_COLORKEY)) {
        return _PutBoxKeyAlphaChannel (dst, dstrow, srcrow, w, h, box);
    }
    else if (box->bmType & BMP_TYPE_COLORKEY) {
        return _PutBoxKey (dst, dstrow, srcrow, w, h, box);
    }
    else if (box->bmType & BMP_TYPE_ALPHACHANNEL) {
        return _PutBoxAlphaChannel (dst, dstrow, srcrow, w, h, box);
    }

    switch (dst->format->BytesPerPixel) {
        case 1:
            if (((DWORD)srcrow & 3) || ((DWORD)dstrow & 3) 
                    || (linelen & 3) || (box->bmPitch & 3) || (dst->pitch & 3))
                goto slow_copy;
            else {
                int n = linelen >> 2;
                for (y = h; y; --y) {
                    GAL_memcpy4 (dstrow, srcrow, n);
                    srcrow += box->bmPitch;
                    dstrow += dst->pitch;
                }
            }
            break;

        case 2:
            if ((((DWORD)dstrow & 3) != ((DWORD)srcrow & 3)) 
                    || (box->bmPitch & 3) || (dst->pitch & 3))
                goto slow_copy;

            for (y = h; y; --y) {
                Uint16 *dstpixels = (Uint16 *)dstrow;
                Uint16 *srcpixels = (Uint16 *)srcrow;
                int n = w;
                if ((DWORD)dstpixels & 3) {
                    *dstpixels = *srcpixels;
                    dstpixels++;
                    srcpixels++;
                    n--;
                }

                if (n >> 1)
                    GAL_memcpy4 (dstpixels, srcpixels, n >> 1);
                if (n & 1)
                    dstpixels [n - 1] = srcpixels [n - 1];

                srcrow += box->bmPitch;
                dstrow += dst->pitch;
            }
            break;

        case 3:
            goto slow_copy;

        case 4:
            for (y = h; y; --y) {
                GAL_memcpy4 (dstrow, srcrow, w);
                srcrow += box->bmPitch;
                dstrow += dst->pitch;
            }
            break;
    }

    return 0;

slow_copy:
    for (y = h; y; --y) {
        GAL_memcpy (dstrow, srcrow, linelen);
        srcrow += box->bmPitch;
        dstrow += dst->pitch;
    }

    return 0;
}

/* 
 * Convert a surface into the specified pixel format.
 */
GAL_Surface * GAL_ConvertSurface (GAL_Surface *surface,
                    GAL_PixelFormat *format, Uint32 flags)
{
    GAL_Surface *convert;
    Uint32 colorkey = 0;
    Uint8 alpha = 0;
    Uint32 surface_flags;
    GAL_Rect bounds;

    /* Check for empty destination palette! (results in empty image) */
    if ( format->palette != NULL ) {
        int i;
        for ( i=0; i<format->palette->ncolors; ++i ) {
            if ( (format->palette->colors[i].r != 0) ||
                 (format->palette->colors[i].g != 0) ||
                 (format->palette->colors[i].b != 0) )
                break;
        }
        if ( i == format->palette->ncolors ) {
            GAL_SetError("NEWGAL: Empty destination palette.\n");
            return(NULL);
        }
    }

    /* Create a new surface with the desired format */
    convert = GAL_CreateRGBSurface(flags,
                surface->w, surface->h, format->BitsPerPixel,
        format->Rmask, format->Gmask, format->Bmask, format->Amask);
    if ( convert == NULL ) {
        return(NULL);
    }

    /* Copy the palette if any */
    if ( format->palette && convert->format->palette ) {
        memcpy(convert->format->palette->colors,
                format->palette->colors,
                format->palette->ncolors*sizeof(GAL_Color));
        convert->format->palette->ncolors = format->palette->ncolors;
    }

    /* Save the original surface color key and alpha */
    surface_flags = surface->flags;
    if ( (surface_flags & GAL_SRCCOLORKEY) == GAL_SRCCOLORKEY ) {
        /* Convert colourkeyed surfaces to RGBA if requested */
        if((flags & GAL_SRCCOLORKEY) != GAL_SRCCOLORKEY
           && format->Amask) {
            surface_flags &= ~GAL_SRCCOLORKEY;
        } else {
            colorkey = surface->format->colorkey;
            GAL_SetColorKey(surface, 0, 0);
        }
    }
    if ( (surface_flags & GAL_SRCALPHA) == GAL_SRCALPHA ) {
        alpha = surface->format->alpha;
        GAL_SetAlpha(surface, 0, 0);
    }

    /* Copy over the image data */
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = surface->w;
    bounds.h = surface->h;
    GAL_LowerBlit(surface, &bounds, convert, &bounds);

    /* Clean up the original surface, and update converted surface */
    if ( convert != NULL ) {
        GAL_SetClipRect(convert, &surface->clip_rect);
    }
    if ( (surface_flags & GAL_SRCCOLORKEY) == GAL_SRCCOLORKEY ) {
        Uint32 cflags = surface_flags&(GAL_SRCCOLORKEY|GAL_RLEACCELOK);
        if ( convert != NULL ) {
            Uint8 keyR, keyG, keyB;

            GAL_GetRGB(colorkey,surface->format,&keyR,&keyG,&keyB);
            GAL_SetColorKey(convert, cflags|(flags&GAL_RLEACCELOK),
                GAL_MapRGB(convert->format, keyR, keyG, keyB));
        }
        GAL_SetColorKey(surface, cflags, colorkey);
    }
    if ( (surface_flags & GAL_SRCALPHA) == GAL_SRCALPHA ) {
        Uint32 aflags = surface_flags&(GAL_SRCALPHA|GAL_RLEACCELOK);
        if ( convert != NULL ) {
                GAL_SetAlpha(convert, aflags|(flags&GAL_RLEACCELOK),
                alpha);
        }
        GAL_SetAlpha(surface, aflags, alpha);
    }

    /* We're ready to go! */
    return(convert);
}

/*
 * Free a surface created by the above function.
 */
void GAL_FreeSurface (GAL_Surface *surface)
{
    /* Free anything that's not NULL, and not the screen surface */
    if ((surface == NULL) ||
        (current_video && (surface == GAL_VideoSurface))) {
        return;
    }
    if ( --surface->refcount > 0 ) {
        return;
    }
    if ( (surface->flags & GAL_RLEACCEL) == GAL_RLEACCEL ) {
            GAL_UnRLESurface(surface, 0);
    }
    if ( surface->format ) {
        GAL_FreeFormat(surface->format);
        surface->format = NULL;
    }
    if ( surface->map != NULL ) {
        GAL_FreeBlitMap(surface->map);
        surface->map = NULL;
    }
    if ( (surface->flags & GAL_HWSURFACE) == GAL_HWSURFACE ) {
        GAL_VideoDevice *video = current_video;
        GAL_VideoDevice *this  = current_video;
        video->FreeHWSurface(this, surface);
    }
    if ( surface->pixels &&
         ((surface->flags & GAL_PREALLOC) != GAL_PREALLOC) ) {
        free(surface->pixels);
    }
    free(surface);
#ifdef CHECK_LEAKS
    --surfaces_allocated;
#endif
}

#if defined(_LITE_VERSION) && !defined(_STAND_ALONE)
void GAL_RequestHWSurface (const REQ_HWSURFACE* request, REP_HWSURFACE* reply)
{
    if (current_video->RequestHWSurface) {
        current_video->RequestHWSurface (current_video, request, reply);
    }
    else if (request->offset == 0) {
        reply->offset = 0;
    }
}

#endif

