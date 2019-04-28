#pragma once
#ifndef _STACKINFODATA_H_
#define _STACKINFODATA_H_
#include <windows.h>

class StackData
{
public:
	StackData();
	virtual ~StackData();


public:
	// ���浱ǰ���̾��
	static HANDLE hStackThread;
	// ������̾��
	static HANDLE hStackProcess;
	// ��ʾջ��Ϣ�ӿ�
	void puShowStackInfoData() { this->ShowStackInfoData(); }

private:
	// ��ʼ��ջ��Ϣ
	void InitStackInfoData();
	// ��ʾջ��Ϣ
	void ShowStackInfoData();

private:
	// �����߳�������Ϣ
	CONTEXT ct;
	// ����ջ�е�����
	BYTE StackDatabuf[521];
	// ����ջ��ַ
	DWORD stackAddress = 0;
};

#endif