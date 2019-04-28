#pragma once
#ifndef _MEMORYINFODATA_H_
#define _MEMORYINFODATA_H_
#include <windows.h>

class MemoryData
{
public:
	MemoryData();
	virtual ~MemoryData();

public:
	// �����߳̾��
	static HANDLE hMemThread;
	// ������̾��
	static HANDLE hMemProcess;
	// Ĭ��Eip�鿴
	void MemoryDatas();
	// �������鿴
	void MemoryDatas(const DWORD dAddress);
	// �޸��ڴ�����
	void MemoryDatas(const DWORD dAddress, const DWORD ModifyMemData);
	
private:
	// ��ʾ�ڴ�����
	void ShowMemoryDatas();

private:
	// �����ȡ���ڴ�����
	BYTE MemoryArrData[100];
	// ԭ��ַ����ʹ��
	DWORD Address = 0;

};

#endif