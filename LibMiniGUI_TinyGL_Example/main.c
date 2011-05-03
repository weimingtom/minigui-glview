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
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/mgext.h>
#include <minigui/ext/glview.h>

#include <tinygl/GL/gl.h>
#include "samples.h"

#define IDC_GL_VIEW	351

#define SCRN_W		425
#define SCRN_H		385
#define BTN_W		100
#define BTN_H		25
#define GLVIEW_W	403
#define GLVIEW_H	302



void OnGLViewOnResize(int w, int h)
{
	reshape(w, h);
}


void TestRender(void)
{
	static float fRot = 0;
	const float SIZE = 0.7f;

	fRot += M_PI/180.0f*30;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(fRot, 0, 0, 1.0f);

	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);		glVertex3f(-SIZE, -SIZE, 0);
		glColor3f(0.0, 1.0, 0.0);		glVertex3f( SIZE, -SIZE, 0);
		glColor3f(0.0, 0.0, 1.0);		glVertex3f( 0,     SIZE, 0);
	glEnd();

	glPopMatrix();
}

void OnGLViewDraw(void)
{
#define ENABLE_TEST_MECH

#ifdef ENABLE_TEST_MECH
	animation();
	display();
#else	// ENABLE_TEST_MECH
	TestRender();
#endif	// ENABLE_TEST_MECH
}


void OnGLViewKey(unsigned int nKey, DWORD nFlag)
{
	key(nKey);
}


void OnGLViewTouch(unsigned short nID, unsigned short nFlag, int x, int y)
{
}


glviewCtrlFunc_t g_ViewCtrlFunc = {
	OnGLViewOnResize,
	OnGLViewDraw,
	OnGLViewKey,
	OnGLViewTouch
};


static CTRLDATA CtrlMyDate[] =
{
    {
		CTRL_GLVIEW,
		WS_CHILD | SS_RIGHT | WS_VISIBLE,
		10, 10, GLVIEW_W, GLVIEW_H,
		IDC_GL_VIEW,
		"",
		(DWORD)&g_ViewCtrlFunc
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        (SCRN_W-BTN_W)/2, SCRN_H-BTN_H-35, BTN_W, BTN_H,
        IDOK,
        "OK",
        0
    },
};


static DLGTEMPLATE DlgMyDate =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    10, 10, SCRN_W, SCRN_H,
    "GLView Test",
    0, 0,
    TABLESIZE(CtrlMyDate),
	NULL,
    0
};


static int MyDateBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITDIALOG:
		{
			init();
		}
        return 1;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            EndDialog (hDlg, wParam);
            break;
        }
        break;

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "GLViewTest" , 0 , 0);
#endif

    if (!InitMiniGUIExt()) {
        return 2;
    }

    DlgMyDate.controls = CtrlMyDate;

    DialogBoxIndirectParam (&DlgMyDate, HWND_DESKTOP, MyDateBoxProc, 0L);

    return 0;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif
