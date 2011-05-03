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

extern void init(void);
extern void display(void);
extern void animation(void);
extern void reshape(int w, int h);
extern void key(int key);


