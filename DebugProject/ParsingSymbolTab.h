#pragma once
#ifndef _PARSINGSYSBOLTAB_H_
#define _PARSINGSYSBOLTAB_H_
#include <windows.h>

class ParSymbol
{
public:
	ParSymbol();
	virtual ~ParSymbol();

public:
	void puSetProcessHandle(HANDLE process){ this->ObjectProcess = process; }

private:
	// ��ʼ�����Է���
	void InitParSymbol();
	// ��ʾ����
	void ShowParSymbol();

private:
	// ������Ч���̾��
	HANDLE ObjectProcess;
};

#endif 