#pragma once
#ifndef _UWINDOWS_H_
#define _UWINDOWS_H_
#include <windows.h>
#include <stdio.h>

/*�ַ�������*/
void WriteChar(int x, int y, char * szInfo, int color);

/*���ξ�����*/
void WriteInt(int x, int y, int Info, int color);

/*���ع��*/
void HideCursor();

/*��������*/
void Musics_Game();

int SetWindowsSize(char* pszWindowTitle, int nX, int nY);

#endif