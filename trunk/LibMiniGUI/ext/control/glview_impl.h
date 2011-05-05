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


#ifndef _GLVIEW_IMPL_H_
#define _GLVIEW_IMPL_H_


#include "../../../LibTinyGL/src/include/zgl.h"


#ifdef __cplusplus
extern "C"
{
#endif


struct GAL_Surface;

typedef struct
{
	GLContext   *gl_context;
	struct GAL_Surface *surface;
	unsigned int pitch;
	unsigned int *palette;
	unsigned char *indexes;

	unsigned char *pDitherBuf;				// David: buffer for dithering, only used for 8-bit systems

	glviewCtrlFunc_t* pFuncCallback;		// David: function callbacks in user app
} minigl_Context;



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _GLVIEW_IMPL_H_ */

