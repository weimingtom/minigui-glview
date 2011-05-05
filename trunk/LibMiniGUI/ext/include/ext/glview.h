/**
* program	: MiniGLView with TinyGL under MiniGUI
* author	: Wei (David) Wu
* E-mail	: KyosukeNo1@gmail.com
* 
* 
* MiniGUI MiniGLView includes three separated parts of work:
* 		1. External control of MiniGUI (http://www.minigui.org), 
* 		   named GLView. MiniGLView embedded a third party software 
* 		   renderer, TinyGL(http://bellard.org/TinyGL), which is a
* 		   subset of OpenGL(http://www.opengl.org). MiniGLView provides
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
* Copyright for MiniGLView:
*		MiniGUI MiniGLView is protected by GPL license. It is free for those
* 		who are 100% GPL and those who never copy, modify and distribute
* 		MiniGUI MiniGLView. 
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
* "If you redistribute MiniGLView, I would appreciate that you
* include in the files history information documenting your changes."
* 
**/

#ifndef EXT_GLVIEW_H
#define EXT_GLVIEW_H

#ifdef  __cplusplus
extern "C" {
#endif

    /**
     * \addtogroup mgext_fns
     * @{
     */

    /**
     * \addtogroup mgext_controls
     * @{
     */

    /**
     * \defgroup mgext_ctrl_glview GLView control
     * @{
     */
/** 
 * \def CTRL_GLVIEW
 * \brief The class name of glview control.
 */
#define CTRL_GLVIEW       ("GLView")

/** GLview return value */
#define GV_OKAY                 0
/** GLview return value */
#define GV_ERR                  (-1)
/** GLview return value */
#define GV_ERRSPACE             (-2)

/** Translate 0-9, a-z, A-Z to ASCII */
#define ENABLE_KEY_MAPPING


typedef void (*pFuncGLViewOnResize_t)(int, int);
typedef void (*pFuncGLViewDraw_t)(void);
typedef void (*pFuncGLViewKey_t)(unsigned int, DWORD);
typedef void (*pFuncGLViewTouch_t)(unsigned short, unsigned short, int, int);

typedef struct
{
	pFuncGLViewOnResize_t	pFuncGLViewOnResize;
	pFuncGLViewDraw_t		pFuncGLViewDraw;
	pFuncGLViewKey_t		pFuncGLViewKey;
	pFuncGLViewTouch_t		pFuncGLViewTouch;
} glviewCtrlFunc_t;


    /** @} end of mgext_ctrl_iconview_ncs */

    /** @} end of mgext_ctrl_iconview */

    /** @} end of mgext_controls */

    /** @} end of mgext_fns */

#ifdef  __cplusplus
}
#endif

#endif /* EXT_GLVIEW_H */

