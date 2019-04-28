#pragma once
#ifndef _CAPSTONE_H_
#define _CAPSTONE_H_
#include "capstone\include\capstone.h"
#include "MasterExceptionHandler.h"


#ifdef _WIN64 // 64λƽ̨���������Զ����������
#pragma comment(lib,"capstone_x64.lib")
#else
#pragma comment(lib,"capstone\\lib\\capstone_x86.lib")
#endif

class Capstone
{
public:
	Capstone();
	virtual ~Capstone();

public:
	// ��ʼ�����������
	void InitCapstone();
	// ��ʾ�������Ϣ
	void ShowAssembly(const void* pAddr, int nLen);

private:
	// �����������
	csh Handle;
	// ������Ϣ
	cs_err err;	
	// ���淴���õ���ָ��Ļ������׵�ַ
	cs_insn* pInsn; 
	// �ṹ���ڴ�
	cs_opt_mem OptMem;
};

#endif