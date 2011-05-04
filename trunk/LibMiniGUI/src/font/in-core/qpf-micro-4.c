/*
** $Id: qpf-micro-4.c 8944 2007-12-29 08:29:16Z xwyan $
** 
** qpf-micro-4.c: font data for 4-point micro QPF.
**
** Copyright (C) 2005 ~ 2007 Feynman Software
**
** All right reserved by Feynman Software.
*/

/*
** This file is created by 'dumpqpf2' by FMSoft (http://www.fmsoft.cn).
** Please do not modify it manually.
*/

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "devfont.h"

#ifdef _MGFONT_QPF
#ifdef _MGINCORE_RES

#include "qpf.h"

static const GLYPHMETRICS glyph_metrics [] = {
	{1, 4, 3, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 3, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 4, 0}, 
	{1, 4, 2, 0, 0, 4, 3, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 3, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 4, 0}, 
	{1, 4, 0, 0, 0, 4, 0, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 2, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 2, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 4, 0}, 
	{1, 4, 2, 0, 0, 4, 2, 0}, 
	{1, 4, 1, 0, 0, 4, 3, 0}, 
	{1, 4, 2, 0, 0, 4, 2, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 3, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 2, 0, 0, 4, 5, 0}, 
	{1, 4, 1, 0, 0, 4, 1, 0}, 
	{1, 4, 2, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 4, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
	{1, 4, 2, 0, 0, 4, 4, 0}, 
	{1, 4, 5, 0, 0, 4, 5, 0}, 
};

static const unsigned char glyph_data [] = {
	0xe0, 0xe0, 0xe0, 
	0x40, 0x40, 0x40, 0x60, 0x40, 
	0xa0, 0xc0, 0xa0, 
	0x60, 0xa0, 0xe0, 0xa0, 0xc0, 
	0x40, 0xe0, 0xa0, 
	0xe0, 0x20, 
	0x40, 0x80, 0xc0, 0x80, 0x40, 
	0xe0, 0xa0, 0xa0, 
	0x80, 0x40, 0x40, 0x40, 0xa0, 
	0xa0, 0x40, 0xc0, 0x80, 
	0x60, 0x80, 0x40, 0xa0, 0xc0, 
	0x40, 0x60, 0x40, 0x40, 0x40, 
	0x40, 0xe0, 0x40, 0xe0, 
	0xa0, 0x00, 0x40, 0xe0, 0x40, 
	0x40, 0x00, 0x40, 0x00, 0x40, 
	0xc0, 0x20, 0x60, 0xa0, 0xc0, 
	0x60, 0x80, 0x80, 0x60, 
	0xc0, 0x20, 0x20, 0xc0, 
	0x40, 0xa0, 0xa0, 0xa0, 
	0xa0, 0xa0, 0xa0, 0x40, 
	0xa0, 0xe0, 0xa0, 0xa0, 0x40, 
	0xe0, 0x20, 0x60, 0x20, 0xe0, 
	0xa0, 0x00, 0xa0, 0x40, 0xa0, 
	0x40, 0x20, 0xe0, 0x80, 0x40, 
	0x20, 0x60, 0xe0, 0x60, 0x20, 
	0x80, 0xc0, 0xe0, 0xc0, 0x80, 
	0x20, 0xe0, 0x40, 0xe0, 0x80, 
	0x40, 0xe0, 0xa0, 0xe0, 0x40, 
	0x20, 0x40, 0x80, 0x40, 0xe0, 
	0x80, 0x40, 0x20, 0x40, 0xe0, 
	0xe0, 0x00, 0xe0, 0x00, 0xe0, 
	0xa0, 0xa0, 0x40, 
	0xc0, 0xc0, 0xc0, 0x00, 0xc0, 
	0xa0, 0xa0, 
	0xa0, 0xe0, 0xa0, 0xe0, 0xa0, 
	0x40, 0xe0, 0xc0, 0x60, 0xe0, 
	0xa0, 0x60, 0xc0, 0xa0, 
	0x40, 0x40, 0xe0, 0xc0, 0x40, 
	0x20, 0x60, 
	0x20, 0x40, 0x40, 0x40, 0x20, 
	0x80, 0x40, 0x40, 0x40, 0x80, 
	0xa0, 0x40, 0xe0, 0x40, 0xa0, 
	0x40, 0xe0, 0x40, 
	0x40, 0xc0, 
	0xe0, 
	0xc0, 0xc0, 
	0x20, 0x20, 0x40, 0x40, 0x80, 
	0x40, 0xa0, 0xa0, 0xa0, 0x40, 
	0x40, 0xc0, 0x40, 0x40, 0x40, 
	0xc0, 0x20, 0x40, 0x80, 0xe0, 
	0xc0, 0x20, 0x60, 0x20, 0xe0, 
	0xa0, 0xa0, 0xa0, 0xe0, 0x20, 
	0xe0, 0x80, 0xc0, 0x20, 0xc0, 
	0x60, 0x80, 0xe0, 0xa0, 0xe0, 
	0xe0, 0x20, 0x20, 0x40, 0x40, 
	0xe0, 0xa0, 0xe0, 0xa0, 0xe0, 
	0xe0, 0xa0, 0xe0, 0x20, 0xc0, 
	0xc0, 0xc0, 0x00, 0xc0, 0xc0, 
	0xc0, 0xc0, 0x00, 0x40, 0xc0, 
	0x20, 0x40, 0x80, 0x40, 0x20, 
	0xe0, 0x00, 0xe0, 
	0x80, 0x40, 0x20, 0x40, 0x80, 
	0xe0, 0x20, 0x60, 0x00, 0x40, 
	0x60, 0xa0, 0xc0, 0x80, 0x60, 
	0xe0, 0xa0, 0xe0, 0xa0, 0xa0, 
	0xe0, 0xa0, 0xc0, 0xa0, 0xe0, 
	0xe0, 0x80, 0x80, 0x80, 0xe0, 
	0xc0, 0xa0, 0xa0, 0xa0, 0xc0, 
	0xe0, 0x80, 0xe0, 0x80, 0xe0, 
	0xe0, 0x80, 0xe0, 0x80, 0x80, 
	0xe0, 0x80, 0xa0, 0xa0, 0xe0, 
	0xa0, 0xa0, 0xe0, 0xa0, 0xa0, 
	0xe0, 0x40, 0x40, 0x40, 0xe0, 
	0x20, 0x20, 0x20, 0xa0, 0xe0, 
	0xa0, 0xa0, 0xc0, 0xa0, 0xa0, 
	0x80, 0x80, 0x80, 0x80, 0xe0, 
	0xa0, 0xe0, 0xa0, 0xa0, 0xa0, 
	0xe0, 0xa0, 0xa0, 0xa0, 0xa0, 
	0xe0, 0xa0, 0xa0, 0xa0, 0xe0, 
	0xe0, 0xa0, 0xe0, 0x80, 0x80, 
	0xe0, 0xa0, 0xa0, 0xc0, 0x60, 
	0xe0, 0xa0, 0xc0, 0xa0, 0xa0, 
	0xe0, 0x80, 0xe0, 0x20, 0xe0, 
	0xe0, 0x40, 0x40, 0x40, 0x40, 
	0xa0, 0xa0, 0xa0, 0xa0, 0xe0, 
	0xa0, 0xa0, 0xa0, 0xa0, 0x40, 
	0xa0, 0xa0, 0xa0, 0xe0, 0xa0, 
	0xa0, 0xe0, 0x40, 0xe0, 0xa0, 
	0xa0, 0xa0, 0xe0, 0x40, 0x40, 
	0xe0, 0x20, 0x40, 0x80, 0xe0, 
	0x60, 0x40, 0x40, 0x40, 0x60, 
	0x80, 0x80, 0x40, 0x40, 0x20, 
	0xc0, 0x40, 0x40, 0x40, 0xc0, 
	0x40, 0xa0, 
	0xe0, 
	0x40, 0x60, 
	0xe0, 0x60, 0xa0, 0xe0, 
	0x80, 0xe0, 0xa0, 0xa0, 0xe0, 
	0xe0, 0x80, 0x80, 0xe0, 
	0x20, 0xe0, 0xa0, 0xa0, 0xe0, 
	0xe0, 0xa0, 0xc0, 0xe0, 
	0x60, 0x80, 0xc0, 0x80, 0x80, 
	0xe0, 0xa0, 0x60, 0xe0, 
	0x80, 0xe0, 0xa0, 0xa0, 0xa0, 
	0x40, 0x40, 0x40, 0x40, 
	0x20, 0x20, 0xa0, 0xe0, 
	0x80, 0xa0, 0xc0, 0xc0, 0xa0, 
	0xc0, 0x40, 0x40, 0x40, 0x40, 
	0xa0, 0xe0, 0xa0, 0xa0, 
	0xe0, 0xa0, 0xa0, 0xa0, 
	0xe0, 0xa0, 0xa0, 0xe0, 
	0xe0, 0xa0, 0xe0, 0x80, 
	0xe0, 0xa0, 0xe0, 0x20, 
	0xe0, 0xa0, 0x80, 0x80, 
	0xe0, 0xc0, 0x60, 0xe0, 
	0x40, 0xe0, 0x40, 0x40, 0x60, 
	0xa0, 0xa0, 0xa0, 0xe0, 
	0xa0, 0xa0, 0xa0, 0x40, 
	0xa0, 0xa0, 0xe0, 0xa0, 
	0xa0, 0x40, 0x40, 0xa0, 
	0xa0, 0xa0, 0x60, 0xc0, 
	0xe0, 0x60, 0xc0, 0xe0, 
	0x60, 0x40, 0xc0, 0x40, 0x60, 
	0x40, 0x40, 0x40, 0x40, 0x40, 
	0xc0, 0x40, 0x60, 0x40, 0xc0, 
	0xc0, 0x60, 
	0x60, 0x40, 0x40, 0x40, 0xc0, 
};

static GLYPHTREE glyph_tree_0 = {
	0x0000, 0x007f, NULL, NULL, NULL, glyph_metrics + 0, glyph_data + 0
};

static QPFMETRICS fm = {
	5, 0, 0, 0, 4, 0, 0, 0, 0, 0
};

QPFINFO __mgif_qpf_micro_4 = {
	"qpf-micro-rrncnn-4-4-ISO8859-1,ISO8859-15,UTF-8,UTF-16LE", 
	0, 0, 0, &fm, 
	&glyph_tree_0, 
};

#endif /* _MGINCORE_RES */
#endif /* _MGFONT_QPF */
