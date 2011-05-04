/*
** $Id: systext.c 8013 2007-10-30 05:11:41Z xwyan $
**
** systext.c: GetSysText function.
**
** Copyright (C) 2003 ~ 2007 Feynman Software.
** Copyright (C) 1999 ~ 2002 Wei Yongming.
**
** All rights reserved by Feynman Software.
** 
**
** Current maintainer: Wei Yongming.
**
** Create date: 2000/12/31
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"

/*
 * This function translates system strings.
 * You can use gettext to return the text.
 *
 * System text as follows:
 *
 const char* SysText [] =
 {
    "Windows",                  // 0
    "Start",                    // 1
    "Refresh Background",       // 2
    "Close All Windows",        // 3
    "End Session",              // 4
    "Operations",               // 5
    "Minimize",                 // 6
    "Maximize",                 // 7
    "Restore",                  // 8
    "Close",                    // 9
    "OK",                       // 10
    "Next",                     // 11
    "Cancel",                   // 12
    "Previous",                 // 13
    "Yes",                      // 14
    "No",                       // 15
    "Abort",                    // 16
    "Retry",                    // 17
    "Ignore",                   // 18
    "About MiniGUI...",         // 19
    "Open File",                // 20
    "Save File",                // 21
    "Color Selection",          // 22
    "Switch Layer",             // 23
    "Delete Layer",             // 24
    "Error",                     // 25
    "LOGO",                      // 26 
    "Current Path",              // 27 
    "File",                      // 28 
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "Location",                 //0 + 29
    "Up",                       //1 + 29
    "Name",                     //2 + 29
    "Size",                     //3 + 29
    "Access Mode",              //4 + 29
    "Last Modify Time",         //5 + 29
    "Open",                     //6 + 29
    "File Name",                //7 + 29
    "File Type",                //8 + 29
    "Show Hide File",           //9 + 29
    "Sorry! not find %s ", //10 + 29
    "Can't Read %s !",        //11 + 29
    "Can't Write  %s !",      //12 + 29
    "Information",              //13 + 29
    "R",                        //14 + 29
    "W",                        //15 + 29
    "WR",                       //16 + 29
    "Save",                     //17 + 29
    "File %s exists, Replace or not?", //18 + 29
#endif
    NULL
};
*/

const char** local_SysText;

const char* SysText [] =
{
    "Windows",                  // 0
    "Start",                    // 1
    "Refresh Background",       // 2
    "Close All Windows",        // 3
    "End Session",              // 4
    "Operations",               // 5
    "Minimize",                 // 6
    "Maximize",                 // 7
    "Restore",                  // 8
    "Close",                    // 9
    "OK",                       // 10
    "Next",                     // 11
    "Cancel",                   // 12
    "Previous",                 // 13
    "Yes",                      // 14
    "No",                       // 15
    "Abort",                    // 16
    "Retry",                    // 17
    "Ignore",                   // 18
    "About MiniGUI...",         // 19
    "Open File",                // 20
    "Save File",                // 21
    "Color Selection",          // 22
    "Switch Layer",             // 23
    "Delete Layer",             // 24
    "Error",                     // 25
    "LOGO",                      // 26 
    "Current Path",              // 27 
    "File",                      // 28 
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "Location",                 //0 + 29
    "Up",                       //1 + 29
    "Name",                     //2 + 29
    "Size",                     //3 + 29
    "Access Mode",              //4 + 29
    "Last Modify Time",         //5 + 29
    "Open",                     //6 + 29
    "File Name",                //7 + 29
    "File Type",                //8 + 29
    "Show Hide File",           //9 + 29
    "Sorry! not find %s ", //10 + 29
    "Can't Read %s !",        //11 + 29
    "Can't Write  %s !",      //12 + 29
    "Information",              //13 + 29
    "R",                        //14 + 29
    "W",                        //15 + 29
    "WR",                       //16 + 29
    "Save",                     //17 + 29
    "File %s exists, Replace or not?", //18 + 29
#endif
    NULL
};

#if defined(_GB_SUPPORT) | defined (_GBK_SUPPORT) | defined (_GB18030_SUPPORT)
static const char* SysText_GB [] =
{
    "����",              // 0
    "��ʼ",              // 1
    "ˢ�±���",             // 2
    "�ر����д���",         // 3
    "�����Ự",             // 4
    "���ڲ���",             // 5
    "��С��",               // 6
    "���",               // 7
    "�ָ�",                 // 8
    "�ر�",                 // 9
    "ȷ��",                 // 10
    "��һ��",               // 11
    "ȡ��",                 // 12
    "��һ��",               // 13
    "��(Y)",                // 14
    "��(N)",                // 15
    "��ֹ(A)",              // 16
    "����(R)",              // 17
    "����(I)",              // 18
    "���� MiniGUI...",      // 19
    "���ļ�",             // 20
    "�����ļ�",             // 21
    "��ɫѡ��",             // 22
    "�л���",               // 23
    "ɾ����",               // 24
    "����",                 // 25
    "ͼ��",                 // 26
    "��ǰ·��",             // 27
    "�ļ�",                 // 28
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "����λ��",                                 //0 + 29
    "��һ��",                                   //1 + 29
    "����",                                     //2 + 29
    "��С",                                     //3 + 29
    "����Ȩ��",                                 //4 + 29
    "�ϴ��޸�ʱ��",                             //5 + 29
    "��",                                     //6 + 29
    "�� �� ��",                                 //7 + 29
    "�ļ�����",                                 //8 + 29
    "��ʾ�����ļ�",                             //9 + 29
    "�Բ���δ�ҵ�ָ����Ŀ¼�� %s ",     //10 + 29
    "���ܶ�ȡ %s !",                          //11 + 29
    "�� %s û��дȨ��!",                      //12 + 29
    "��ʾ��Ϣ",                                 //13 + 29
    "��",                                       //14 + 29
    "д",                                       //15 + 29
    "��д",                                     //16 + 29
    "����",                                     //17 + 29
    "�ļ� %s ���ڣ��Ƿ��滻?"                   //18 + 29
#endif
};
#endif

#ifdef _BIG5_SUPPORT
static const char* SysText_BIG5 [] =
{
    "���f",
    "�۩l",
    "��s�I��",
    "�����Ҧ����f",
    "�����|��",
    "���f�ާ@",
    "�̤p��",
    "�̤j��",
    "���`",
    "����",
    "�̩w",
    "�U�@�B",
    "����",
    "�W�@�B",
    "�O(Y)",
    "�_(N)",
    "�פ�(A)",
    "����(R)",
    "����(I)",
    "���_ MiniGUI...",
    "���}���",
    "�O�s���",
	"�C����",
    "�����h",
    "�R���h",
    "���~",                                 // 25
    "�ϼ�",                                 // 26 
    "��e���|",                             // 27 
    "���",                                 // 28 
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "�d���_",                             //0 + 29
    "�W�@��",                               //1 + 29
    "�W��",                                 //2 + 29
    "�j�p",                                 //3 + 29
    "�X���v��",                             //4 + 29
    "�W���ק�ɶ�",                         //5 + 29
    "���}",                                 //6 + 29
    "�� �� �W",                             //7 + 29
    "�������",                             //8 + 29
    "������ä��",                         //9 + 29
    "�藍�_�A�������w���ؿ��G %s ", //10 + 29
    "����Ū�� %s !",                      //11 + 29
    "�� %s �S���g�v��",                   //23 + 29
    "���ܫH��",                             //13 + 29
    "Ū",                                   //14 + 29
    "�g",                                   //15 + 29
    "Ū�g",                                 //16 + 29
    "�O�s",                                 //17 + 29
    "��� %s �s�b�A�O�_����?",              //18 + 29
#endif
    NULL
};
#endif

void __mg_init_local_sys_text (void)
{
    const char* charset = GetSysCharset (TRUE);

    local_SysText = SysText;

    if (charset == NULL)
        charset = GetSysCharset (FALSE);

#ifdef _GB_SUPPORT
    if (strcmp (charset, FONT_CHARSET_GB2312_0) == 0) {
        local_SysText = SysText_GB;
	}
#endif

#ifdef _GBK_SUPPORT
    if (strcmp (charset, FONT_CHARSET_GBK) == 0) {
        local_SysText = SysText_GB;
	}
#endif

#ifdef _GB18030_SUPPORT
    if (strcmp (charset, FONT_CHARSET_GB18030_0) == 0) {
        local_SysText = SysText_GB;
	}
#endif

#ifdef _BIG5_SUPPORT
    if (strcmp (charset, FONT_CHARSET_BIG5) == 0) {
        local_SysText = SysText_BIG5;
	}
#endif

}

const char* GUIAPI GetSysText (unsigned int id)
{
    if (id > IDS_MGST_MAXNUM)
        return NULL;
        
    return local_SysText [id];
}

#ifdef _UNICODE_SUPPORT
static const char* SysText_GB_UTF8 [] =
{
    "窗口",              // 0
    "开始",              // 1
    "刷新背景",             // 2
    "关闭所有窗口",         // 3
    "结束会话",             // 4
    "窗口操作",             // 5
    "最小化 ",
    "最大化 ",
    "恢复",                 // 8
    "关闭",                 // 9
    "确定",                 // 10
    "下一步 ", 
    "取消",                 // 12
    "上一步 ",
    "是(Y)",                // 14
    "否(N)",                // 15
    "终止(A)",              // 16
    "重试(R)",              // 17
    "忽略(I)",              // 18
    "关于 MiniGUI...",      // 19
    "打开文件",             // 20
    "保存文件",             // 21
    "颜色选择",             // 22
    "切换层 ",
    "删除层 ",
    "错误",                 // 25
    "图标",                 // 26
    "当前路径",             // 27
    "文件",                 // 28
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "查找位于",                                 //0 + 29
    "上一级 ",
    "名称",                                     //2 + 29
    "大小",                                     //3 + 29
    "访问权限",                                 //4 + 29
    "上次修改时间",                             //5 + 29
    "打开",                                     //6 + 29
    "文 件 名 ",                                 //7 + 29
    "文件类型",                                 //8 + 29
    "显示隐藏文件",                             //9 + 29
    "对不起，未找到指定的目录：  %s ",
    "不能读取 %s !",                          //11 + 29
    "对 %s 没有写权限!",                      //12 + 29
    "提示信息",                                 //13 + 29
    "读 ",
    "写 ",
    "读写",                                     //16 + 29
    "保存",                                     //17 + 29
    "文件 %s 存在，是否替换?"                   //18 + 29
#endif
};

static const char* SysText_BIG5_UTF8 [] =
{
	"窗口",              // 0
    "開始",              // 1
    "刷新背景",             // 2
    "關閉所有窗口",         // 3
    "結束會話",             // 4
    "窗口操作",             // 5
    "最小化 ",               // 6
    "最大化 ",               // 7
    "恢復",                 // 8
    "關閉",                 // 9
    "確定",                 // 10
    "下一步 ",               // 11
    "取消",                 // 12
    "上一步 ",               // 13
    "是(Y)",                // 14
    "否(N)",                // 15
    "終止(A)",              // 16
    "重試(R)",              // 17
    "忽略(I)",              // 18
    "關于 MiniGUI...",      // 19
    "打開文件",             // 20
    "保存文件",             // 21
    "顏色選擇",             // 22
    "切換層 ",               // 23
    "刪除層 ",               // 24
    "錯誤",                 // 25
    "圖標",                 // 26
    "當前路徑",             // 27
    "文件",                 // 28
#if (!defined (__NOUNIX__) || defined (WIN32)) && defined (_EXT_CTRL_LISTVIEW)
    "查找位于",                                 //0 + 29
    "上一級 ",                                   //1 + 29
    "名稱",                                     //2 + 29
    "大小",                                     //3 + 29
    "訪問權限",                                 //4 + 29
    "上次修改時間",                             //5 + 29
    "打開",                                     //6 + 29
    "文 件 名 ",                                 //7 + 29
    "文件類型",                                 //8 + 29
    "顯示隱藏文件",                             //9 + 29
    "對不起，未找到指定的目錄：%s ",     //10 + 29
    "不能讀取 %s !",                          //11 + 29
    "對 %s 沒有寫權限!",                      //12 + 29
    "提示信息",                                 //13 + 29
    "讀 ",                                       //14 + 29
    "寫 ",                                       //15 + 29
    "讀寫",                                     //16 + 29
    "保存",                                     //17 + 29
    "文件 %s 存在，是否替換?",                  //18 + 29
#endif
    NULL
};

const char** GUIAPI GetSysTextInUTF8 (const char* language)
{
    if (strncasecmp (language, "zh_CN", 5) == 0) {
        return SysText_GB_UTF8;
	}
    else if (strncasecmp (language, "zh_TW", 5) == 0) {
        return SysText_BIG5_UTF8;
	}

    return SysText;
}
#endif /* _UNICODE_SUPPORT */

