/*
** $Id: custom.c 6566 2006-05-10 01:44:57Z xwyan $
**
** custom.c: The IAL engine for customer's engine.
** 
** This IAL engine is a template to support various customer's boards.
** We add this IAL engine is just for your convenience.
**
** If you configure MiniGUI with in-core resource mode, you can enable this
** IAL engine, and modify the file "../sysres/mgetc-custom.c" to meet your 
** run-time configuration.
**
** Modifying an existing IAL engine and using the name of that IAL engine to
** configure MiniGUI is a very bad habit.
**
** Copyright (C) 2007 Feynman Software.
**
** Created by Wei Yongming, 2007/06/06.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef _CUSTOM_IAL

#include "misc.h"
#include "ial.h"
#include "custom.h"

static int mouse_x, mouse_y, mouse_button;

typedef struct tagPOS
{
    short x;
    short y;
    short b;
} POS;

/************************  Low Level Input Operations **********************/
/*
 * Mouse operations -- Event
 */
static int mouse_update(void)
{
    return 1;
}

static void mouse_getxy (int* x, int* y)
{
    *x = mouse_x;
    *y = mouse_y;
}

static int mouse_getbutton(void)
{
    return mouse_button;
}

static int keyboard_update(void)
{
    return 0;
}

static const char * keyboard_get_state (void)
{
    return NULL;
}

#ifdef _LITE_VERSION 
static int wait_event (int which, int maxfd, fd_set *in, fd_set *out, 
                fd_set *except, struct timeval *timeout)
{
    fd_set rfds;
    int    e;

    if (!in) {
        in = &rfds;
        FD_ZERO (in);
    }

    e = select (maxfd + 1, in, out, except, timeout) ;

    if (e < 0) {
        return -1;
    }
    return 0;
}
#else
static int wait_event (int which, fd_set *in, fd_set *out, fd_set *except,
                struct timeval *timeout)
{
    __mg_os_time_delay (300);
    return 0;
}
#endif

BOOL InitCustomInput (INPUT* input, const char* mdev, const char* mtype)
{
    input->update_mouse = mouse_update;
    input->get_mouse_xy = mouse_getxy;
    input->set_mouse_xy = NULL;
    input->get_mouse_button = mouse_getbutton;
    input->set_mouse_range = NULL;

    input->update_keyboard = keyboard_update;
    input->get_keyboard_state = keyboard_get_state;
    input->set_leds = NULL;

    input->wait_event = wait_event;
    mouse_x = 0;
    mouse_y = 0;
    mouse_button= 0;
    return TRUE;
}

void TermCustomInput (void)
{
}

#endif /* _CUSTOM_IAL */

