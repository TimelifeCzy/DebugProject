#pragma once
#ifndef _REGISTER_H_
#define _REGISTER_H_
#include <windows.h>

class RegisterData
{
public:
	RegisterData();
	~RegisterData();

public:
	// �����߳̾��
	static HANDLE hReThread;
	// ��ʾ�Ĵ�����Ϣ
	void RegisterInfo();
	// �޸Ĵ�����Ϣ
	void RegisterInfo(int nRegisterFlag, DWORD dRegisterFlagedata);

private:
	// ����Ĵ�����Ϣ
	CONTEXT ct;
};

#endif