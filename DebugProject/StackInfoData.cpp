#include "StackInfoData.h"
#include <stdio.h>

HANDLE StackData::hStackThread = nullptr;

HANDLE StackData::hStackProcess = nullptr;

StackData::StackData()
{

}

StackData::~StackData()
{

}

// ��ʼ��ջ��Ϣ
void StackData::InitStackInfoData()
{
	// ��ʼ����Ϣ����
	ct = { 0 };
	StackDatabuf[0] = 0;
	ct.ContextFlags = CONTEXT_ALL;
	DWORD dwsize = 0;
	GetThreadContext(StackData::hStackThread, &ct);
	ReadProcessMemory(StackData::hStackProcess, (LPVOID)ct.Esp, StackDatabuf, sizeof(StackDatabuf), &dwsize);
	stackAddress = ct.Esp;
}

// ��ʾջ��Ϣ
void StackData::ShowStackInfoData()
{
	InitStackInfoData();
	printf(" ջ��ַ\t");
	printf("\t  ����\n");
	for (int i = 0; i < 10; ++i)
	{
		printf("%08X\t", stackAddress);
		// ��ӡջ����
		printf("%08X\n", ((DWORD *)StackDatabuf)[i]);
		stackAddress += 4;
	}
}