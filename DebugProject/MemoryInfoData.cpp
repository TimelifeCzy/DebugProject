#include "MemoryInfoData.h"
#include <stdio.h>
#include <ctype.h>

HANDLE MemoryData::hMemThread = nullptr;
HANDLE MemoryData::hMemProcess = nullptr;

MemoryData::MemoryData()
{
}

MemoryData::~MemoryData()
{
}

// Ĭ��Eip�鿴
void MemoryData::MemoryDatas()
{
	//MemoryArrData[0] = 0;
	CONTEXT ct = { 0 };
	DWORD dwSize = 0;
	ct.ContextFlags = CONTEXT_ALL;
	GetThreadContext(MemoryData::hMemThread, &ct);
	Address = ct.Eip;
	auto ErrorCode = ReadProcessMemory(
		MemoryData::hMemProcess,
		(LPVOID)ct.Eip,
		MemoryArrData,
		sizeof(MemoryArrData),
		&dwSize
		);
	ShowMemoryDatas();
}

// �������鿴
void MemoryData::MemoryDatas(const DWORD dAddress)
{
	// ������ת��ASCII�룺toascii()
	MemoryArrData[0] = 0;
	DWORD dwSize = 0;
	Address = dAddress;
	auto ErrorCode = ReadProcessMemory(
		MemoryData::hMemProcess,
		(LPVOID)dAddress,
		MemoryArrData,
		sizeof(MemoryArrData),
		&dwSize
		);
	ShowMemoryDatas();
}

// �޸��ڴ�����
void MemoryData::MemoryDatas(const DWORD dAddress, const DWORD ModifyMemData)
{
	DWORD dwSize = 0;
	DWORD dwOldAttrubet = 0;
	// �ڴ�ɶ���д
	// VirtualProtect((LPVOID)dAddress, sizeof(ModifyMemData), PAGE_READWRITE, &dwSize);
	// д���ڴ�����
	if (WriteProcessMemory(MemoryData::hMemProcess, (LPVOID)dAddress, &ModifyMemData, sizeof(ModifyMemData), &dwSize))
		printf("�޸��ڴ�ɹ�\n");
	else
		printf("�޸��ڴ�ʧ��,�����ڴ�λ����Ч\n");
}

// ��ʾ�ڴ�����
void MemoryData::ShowMemoryDatas()
{
	int ndCount = 0;
	int naCount = 0;
	BOOL Flag = FALSE;
	while (1)
	{
		// 1. �����ʾ��ַ
		printf("%08X    ", Address);
		// 2. ѭ���������
		for (int i = 0; i < 16; ++i)
		{
			printf("%02X ", MemoryArrData[ndCount]);
			++ndCount;
			if (96 == ndCount)
			{
				Flag = TRUE;
				break;
			}
		}
		printf("    ");
		// 3. ���ascci
		for (int i = 0; i < 16; ++i)
		{
			if (MemoryArrData[naCount] >= 0x00 && MemoryArrData[naCount] <= 0x1F) {
				printf(".");
			}
			else if (MemoryArrData[naCount] >= 0x81 && MemoryArrData[naCount] <= 0xFF) {
				printf("?");
			}
			else
				printf("%X", __toascii(MemoryArrData[naCount]));
			++naCount;
			if (96 == naCount)
			{
				Flag = TRUE;
				break;
			}
		}
		printf("\n");
		Address += 16;
		if (Flag)
			break;
	}
}