/**
* program	: GLView with TinyGL under MiniGUI
* author	: Wei (David) Wu
* E-mail	: KyosukeNo1@gmail.com
* 
* 
* MiniGUI GLView includes three separated parts of work:
* 		1. External control of MiniGUI (http://www.minigui.org), 
* 		   named GLView. GLView embedded a third party software 
* 		   renderer, TinyGL(http://bellard.org/TinyGL), which is a
* 		   subset of OpenGL(http://www.opengl.org). GLView provides
* 		   some basic but fast and lightweight rendering 
* 		   algorithms (powered by TinyGL) as well as keyboard and mouse
* 		   interactive controlls (powered by MiniGUI).
* 		2. Modified copy of TinyGL. New functions included:
* 				a. Offseted block copying for direct-screen-access
* 				   (from TinyGL internal buffer to screen framebuffer);
* 				b. Pixel line copying which supports non-4-bytes alignment
* 		3. Three rendering examples from official TinyGL examples with
* 		   minimum changes.
* 
* Copyright for GLView:
*		MiniGUI GLView is protected by GPL license. It is free for those
* 		who are 100% GPL and those who never copy, modify and distribute
* 		MiniGUI GLView. 
* 
* Copyright for MiniGUI:
* 		Feynman Software (http://www.minigui.com) releases some MiniGUI
* 		versions under GPL license. This is the licensing policy for the
* 		GPL'd MiniGUI versions.
*
* Copyright for TinyGL:
* 		(C) 1997-1998 Fabrice Bellard (http://bellard.org/TinyGL)
* 
* Copyright for examples:
*		main.c: Created by Wei Wu. It's a modified version of some
* 				MiniGUI official examples with limited functionalities.
* 		gears.c, mech.c, spin.c: Official examples from TinyGL with
* 				minimum changes.
* 
* 
* "If you redistribute MiniGUI GLView, I would appreciate that you
* include in the files history information documenting your changes."
* 
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#ifdef __MINIGUI_LIB__
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "control.h"
#else
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#endif

#ifdef _EXT_CTRL_GLVIEW


#include "tinygl/GL/gl.h"

#include "ext/glview.h"
#include "scrolled.h"
#include "listmodel.h"
#include "glview_impl.h"

#include "../../src/include/newgal.h"
#include "../../src/include/cliprect.h"
#include "../../src/include/internals.h"
#include "../../src/include/ctrlclass.h"
#include "../../src/include/dc.h"


#define ID_TIMER   100


#ifdef ENABLE_KEY_MAPPING
	// Map 0-9, a-z, A-Z to ASCII code
	const Uint8 SCAN_ASCII[128] =
	{
			0, 0,
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '=', 0, 0,
			'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
			'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '.', 0, 0, 0,
			'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
#endif	// ENABLE_KEY_MAPPING


// + MiniGL related functions & variables --------------------------------------------


static int minigl_resize_viewport( GLContext *c, int *xsize_ptr, int *ysize_ptr );


minigl_Context *minigl_CreateContext (LPARAM pFuncCallback)
{
	minigl_Context *ctx;

	ctx = (minigl_Context*)malloc( sizeof(minigl_Context) );
	if( ctx == NULL ){
		return NULL;
	}
	ctx->gl_context = NULL;
	ctx->indexes = NULL;
	ctx->palette = NULL;
	ctx->pDitherBuf = NULL;
	ctx->pFuncCallback = (glviewCtrlFunc_t*)pFuncCallback;
	return ctx;
}

void minigl_DestroyContext( minigl_Context *ctx )
{
	if( ctx->gl_context != NULL )
	{
		glClose();
	}

	if ( ctx->indexes != NULL)		free(ctx->indexes);
	if ( ctx->palette != NULL)		free(ctx->palette);
	if (ctx->pDitherBuf != NULL)	free(ctx->pDitherBuf);

	free( ctx );
}

int minigl_MakeCurrent(GAL_Surface *surface, minigl_Context *ctx, unsigned short client_width, unsigned short client_height)
{
	int	mode;
	int	xsize;
	int	ysize;
	int	n_colors = 0;
	ZBuffer *zb;

	if( ctx->gl_context == NULL )
	{
		// create the TinyGL context

		xsize = client_width;
		ysize = client_height;

		// we ensure that xsize and ysize are multiples of 2 for the zbuffer.
		xsize &= ~3;
		ysize &= ~3;

		switch( surface->format->BitsPerPixel )
		{
		case  8:
			{
				ctx->indexes = (unsigned char *)malloc(ZB_NB_COLORS);
				if (ctx->indexes == NULL)
					return 0;
				for(mode=0;mode<ZB_NB_COLORS;mode++)
					ctx->indexes[mode]=mode;

				ctx->palette = (unsigned int *)calloc (ZB_NB_COLORS, sizeof(int));
				if (ctx->palette == NULL)
				{
					free(ctx->indexes);
					ctx->indexes = NULL;
					return 0;
				}

				ctx->pitch = surface->pitch * 2;
				n_colors = ZB_NB_COLORS;
				mode = ZB_MODE_INDEX;

				ctx->pDitherBuf = (unsigned char *)malloc(xsize * ysize);
			}
			break;

		case 16:
			{
				ctx->pitch = surface->pitch;
				mode = ZB_MODE_5R6G5B;
			}
			break;

		case 24:
			{
				ctx->pitch = ( surface->pitch * 2 ) / 3;
				mode = ZB_MODE_RGB24;
			}
			break;

		case 32:
			{
				ctx->pitch = surface->pitch / 2;
				mode = ZB_MODE_RGBA;
			}
			break;

		default:
			return 0;
		}

		zb = ZB_open( xsize, ysize, mode, n_colors, (unsigned char*)ctx->indexes, (int *)ctx->palette, NULL);

		if( zb == NULL ){
//			fprintf( stderr, "Error while initializing Z buffer\n" );
			if (ctx->indexes != NULL )
			{
				free(ctx->indexes);
				ctx->indexes = NULL;
			}
			if (ctx->palette != NULL )
			{
				free(ctx->palette);
				ctx->palette = NULL;
			}
			return 0;
		}

		if (ctx->palette != NULL)
		{
			GAL_Color *pal=(GAL_Color *)calloc(ZB_NB_COLORS,sizeof(GAL_Color));
			if (pal!=NULL)
			{
				for (mode=0; mode<ZB_NB_COLORS; mode++)
				{
					pal[mode].r = (ctx->palette[mode]>>16) & 0xFF;
					pal[mode].g = (ctx->palette[mode]>>8) & 0xFF;
					pal[mode].b = (ctx->palette[mode]) & 0xFF;
				}

				GAL_SetColors(surface, pal, 0, ZB_NB_COLORS);
				free(pal);
			}
		}

		// initialization of the TinyGL interpreter
		glInit( zb );
		ctx->gl_context                     = gl_get_context();
		ctx->gl_context->opaque             = (void *) ctx;
		ctx->gl_context->gl_resize_viewport = minigl_resize_viewport;

		// set original size
		ctx->gl_context->viewport.xsize = xsize;
		ctx->gl_context->viewport.ysize = ysize;
		glViewport( 0, 0, xsize, ysize );
	}

	ctx->surface = surface;

	return 1;
}

void minigl_SwapBuffers(const minigl_Context* gl_ctx, const Sint32 xOffset, const Sint32 yOffset)
{
	Sint32 wBlt, hBlt;
	const Sint32 w = gl_ctx->gl_context->zb->xsize;
	const Sint32 h = gl_ctx->gl_context->zb->ysize;

	if (
#ifdef TGL_FEATURE_8_BITS
		gl_ctx->gl_context->zb->mode != ZB_MODE_INDEX &&
#endif
		xOffset >= 0 && yOffset >= 0 && (xOffset+w) < gl_ctx->surface->w && (yOffset+h) < gl_ctx->surface->h)
	{
	    ZB_copyFrameBufferWithOffset(gl_ctx->gl_context->zb, gl_ctx->surface->pixels, gl_ctx->pitch, xOffset, yOffset);
	    wBlt = w;
	    hBlt = h;
	}
	else
	{
		Sint16 i, j;
		Sint32 xSrc, ySrc, xDst, yDst;
		Sint32 wSrcInc, wDstInc;
		Uint8  *pSrc, *pDst;

		if (xOffset < 0)
		{
			xSrc = -xOffset;
			xDst = 0;
		}
		else
		{
			xSrc = 0;
			xDst = xOffset;
		}

		if (xOffset + w >= gl_ctx->surface->w)
		{
			wBlt = gl_ctx->surface->w - xDst;
		}
		else
		{
			wBlt = w - xSrc;
		}

		if (yOffset < 0)
		{
			ySrc = -yOffset;
			yDst = 0;
		}
		else
		{
			ySrc = 0;
			yDst = yOffset;
		}

		if (yOffset + h >= gl_ctx->surface->h)
		{
			hBlt = gl_ctx->surface->h - yDst;
		}
		else
		{
			hBlt = h - ySrc;
		}

		switch (gl_ctx->gl_context->zb->mode)
		{
#ifdef TGL_FEATURE_8_BITS
		case ZB_MODE_INDEX:
			{
				// Src: 8-bit
				// Dst: 8-bit
				const Uint8 bpp = 1;

				// Convert the whole image to buffer: pDitherBuf
				ZB_copyFrameBuffer(gl_ctx->gl_context->zb, gl_ctx->pDitherBuf, w*2);

				// bpp is 1, so we ignore it
				wSrcInc = w;
				wDstInc = (gl_ctx->pitch>>1);

				pSrc = ((Uint8*)gl_ctx->pDitherBuf)      + wSrcInc * ySrc + xSrc;
				pDst = ((Uint8*)gl_ctx->surface->pixels) + wDstInc * yDst + xDst;

				for (i=0; i<hBlt; i++)
				{
					memcpy(pDst, pSrc, wBlt);
					pSrc += wSrcInc;
					pDst += wDstInc;
				}
			}
			break;
#endif

#ifdef TGL_FEATURE_16_BITS
		case ZB_MODE_5R6G5B:
			{
				// Src: 16-bit
				// Dst: 16-bit
				const Uint8 bpp = 2;

				wBlt *= bpp;
				wSrcInc = w * bpp;
				wDstInc = (gl_ctx->pitch>>1) * bpp;

				pSrc = ((Uint8*)gl_ctx->gl_context->zb->pbuf) + wSrcInc * ySrc + xSrc * bpp;
				pDst = ((Uint8*)gl_ctx->surface->pixels)      + wDstInc * yDst + xDst * bpp;

				for (i=0; i<hBlt; i++)
				{
					memcpy(pDst, pSrc, wBlt);
					pSrc += wSrcInc;
					pDst += wDstInc;
				}

				wBlt /= bpp;
			}
			break;
#endif

#ifdef TGL_FEATURE_24_BITS
		case ZB_MODE_RGB24:
			{
				// Src: 16-bit
				// Dst: 24-bit
				const Uint8 bppSrc = 2;
				const Uint8 bppDst = 3;

				wSrcInc = w * bppSrc;
				wDstInc = (gl_ctx->pitch>>1) * bppDst;

				pSrc = ((Uint8*)gl_ctx->gl_context->zb->pbuf) + wSrcInc * ySrc + xSrc * bppSrc;
				pDst = ((Uint8*)gl_ctx->surface->pixels)      + wDstInc * yDst + xDst * bppDst;

				for (i=0; i<hBlt; i++)
				{
					ZB_copyFrameBufferRGB24_OneLine(pSrc, pDst, wBlt);
					pSrc += wSrcInc;
					pDst += wDstInc;
				}
			}
			break;
#endif

#ifdef TGL_FEATURE_32_BITS
		case ZB_MODE_RGBA:
			{
				// Src: 16-bit
				// Dst: 32-bit
				const Uint8 bppSrc = 2;
				const Uint8 bppDst = 4;

				wSrcInc = w * bppSrc;
				wDstInc = (gl_ctx->pitch>>1) * bppDst;

				pSrc = ((Uint8*)gl_ctx->gl_context->zb->pbuf) + wSrcInc * ySrc + xSrc * bppSrc;
				pDst = ((Uint8*)gl_ctx->surface->pixels)      + wDstInc * yDst + xDst * bppDst;

				for (i=0; i<hBlt; i++)
				{
					ZB_copyFrameBufferRGB32_OneLine(pSrc, pDst, wBlt);
					pSrc += wSrcInc;
					pDst += wDstInc;
				}
			}
			break;
#endif

		default:
			return;
		}
	}

	GAL_UpdateRect(gl_ctx->surface, xOffset>=0 ? xOffset : 0, yOffset>=0 ? yOffset : 0, wBlt, hBlt);
}


static int minigl_resize_viewport( GLContext *c, int *xsize_ptr, int *ysize_ptr )
{
	minigl_Context *ctx;
	int            xsize;
	int            ysize;

	ctx = (minigl_Context *)c->opaque;

	xsize = *xsize_ptr;
	ysize = *ysize_ptr;

	// we ensure that xsize and ysize are multiples of 2 for the zbuffer.
	xsize &= ~3;
	ysize &= ~3;

	if (xsize == 0 || ysize == 0) return -1;

	if (xsize != c->zb->xsize || ysize != c->zb->ysize)
	{
		*xsize_ptr = xsize;
		*ysize_ptr = ysize;

		// resize the Z buffer
		ZB_resize( c->zb, ctx->surface->pixels, xsize, ysize );
	}
	return 0;
}


void InitGL(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// - MiniGL related functions & variables --------------------------------------------



static int GLViewCtrlProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    RECT rcClient;
	minigl_Context *gl_ctx = NULL;

    if (message != MSG_CREATE)
    {
    	gl_ctx = (minigl_Context*) GetWindowAdditionalData2 (hWnd);
    }

    switch (message)
    {
	case MSG_CREATE:
		{
			gl_ctx = minigl_CreateContext(lParam);
			if (!gl_ctx)
				return -1;

			SetWindowAdditionalData2 (hWnd, (DWORD) gl_ctx);

			GetClientRect (hWnd, &rcClient);
			minigl_MakeCurrent(__gal_screen, gl_ctx, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

			// Init MiniGL
			InitGL();

			// Set timer
			KillTimer (hWnd, ID_TIMER);
			SetTimer (hWnd, ID_TIMER, 1);
		}
		break;

    case MSG_DESTROY:
    	minigl_DestroyContext (gl_ctx);
        break;
    
    case MSG_GETDLGCODE:
        return DLGC_WANTARROWS;

//	case MSG_KEYDOWN:
//		{
//			int a = SCANCODE_C;
//			if (gl_ctx->pFuncCallback && gl_ctx->pFuncCallback->pFuncGLViewKey)
//			{
//				gl_ctx->pFuncCallback->pFuncGLViewKey(keycode_to_scancode(LOWORD(wParam)), lParam);
//			}
//		}
//		break;

    case MSG_CHAR:
		{
			if (gl_ctx->pFuncCallback && gl_ctx->pFuncCallback->pFuncGLViewKey)
			{
				gl_ctx->pFuncCallback->pFuncGLViewKey(wParam, lParam);
			}
		}
		break;

    case MSG_MOUSEMOVE:
		{
			if (gl_ctx->pFuncCallback && gl_ctx->pFuncCallback->pFuncGLViewTouch)
			{
				int xPos = LOSWORD (lParam);
				int yPos = HISWORD (lParam);
				gl_ctx->pFuncCallback->pFuncGLViewTouch(0, 0, xPos, yPos);
			}
		}
		break;

    case MSG_TIMER:
		{
			if (gl_ctx->pFuncCallback)
			{
				GetClientRect (hWnd, &rcClient);
				InvalidateRect (hWnd, &rcClient, FALSE);
			}
		}
        return 0;

    case MSG_PAINT:
		{
			PDC pdc;

			HDC hdc = BeginPaint (hWnd);

			GetClientRect (hWnd, &rcClient);


			// David:	if MiniGL area is smaller than current client rect (MiniGL rect should be multiples of 2)
			//			fill the gap with some color here
//			SetBrushColor(hdc, GetWindowElementColorEx (hWnd, FGC_MENUITEM_NORMAL));
//			nClientW = gl_ctx->gl_context->zb->xsize;
//			nClientH = gl_ctx->gl_context->zb->ysize;
//			if (nClientW < rcClient.right - rcClient.left)
//			{
//				const int w = rcClient.right - rcClient.left - nClientW;
//				FillBox(hdc, nClientW, 0, w, nClientH);
//			}
//			if (nClientH < rcClient.bottom - rcClient.top)
//			{
//				const int h = rcClient.bottom - rcClient.top - nClientH;
//				FillBox(hdc, 0, nClientH, rcClient.right - rcClient.left, h);
//			}



			if (!(pdc = dc_HDC2PDC (hdc)))
				return -1;

			if (gl_ctx->pFuncCallback && gl_ctx->pFuncCallback->pFuncGLViewDraw)
			{
				Sint32 xOffset, yOffset;

				gl_ctx->pFuncCallback->pFuncGLViewDraw();

				xOffset = rcClient.left;
				yOffset = rcClient.top;
				coor_LP2SP (pdc, &xOffset, &yOffset);
				minigl_SwapBuffers(gl_ctx, xOffset, yOffset);
			}

			EndPaint (hWnd, hdc);
		}
		return 0;

    case MSG_SIZECHANGING:
		{
			int w, h;
			const RECT* rcExpect = (const RECT*)wParam;
			RECT* rcResult = (RECT*)lParam;

			// must be the power of 4!
			w = (rcExpect->right - rcExpect->left) & (~3);
			h = (rcExpect->bottom - rcExpect->top) & (~3);

			rcResult->left = rcExpect->left;
			rcResult->top = rcExpect->top;
			rcResult->right = rcExpect->left + w;
			rcResult->bottom = rcExpect->top + h;
		}
		return 0;

    case MSG_SIZECHANGED:
		{
			int xsize, ysize;

			GetClientRect (hWnd, &rcClient);
			xsize = rcClient.right - rcClient.left;
			ysize = rcClient.bottom - rcClient.top;
			minigl_resize_viewport(gl_ctx->gl_context, &xsize, &ysize);

			if (gl_ctx->pFuncCallback && gl_ctx->pFuncCallback->pFuncGLViewOnResize)
			{
				gl_ctx->pFuncCallback->pFuncGLViewOnResize(xsize, ysize);
			}
		}
		return 0;

    }	// end switch

    return DefaultControlProc (hWnd, message, wParam, lParam);
}

BOOL RegisterGLViewControl (void)
{
	WNDCLASS WndClass;

	WndClass.spClassName = CTRL_GLVIEW;
	WndClass.dwStyle     = WS_NONE;
	WndClass.dwExStyle   = WS_EX_NONE;
	WndClass.hCursor     = GetSystemCursor (IDC_ARROW);
	WndClass.iBkColor    = GetWindowElementColor (BKC_CONTROL_DEF);
	WndClass.WinProc     = GLViewCtrlProc;

	return RegisterWindowClass (&WndClass);
}

#endif /* _EXT_CTRL_GLVIEW */

