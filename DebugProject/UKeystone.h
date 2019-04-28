#pragma once
#ifndef _UKEYSTONE_H_
#define _UKEYSTONE_H_
#include "keystone\keystone.h"
#include "windows.h"

#pragma comment (lib, "keystone\\x86\\keystone_x86.lib")

class KeysTone
{
public:
	KeysTone();
	virtual ~KeysTone();

public:
	BOOL puModifyOpenCode(const HANDLE hthread, const DWORD MemoryAddress, const char *ModifyData) {
		auto nRet = this->ModifyOpCode(hthread, MemoryAddress, ModifyData);
		return nRet;
	}
	void puSetBpHandle(const HANDLE masterhandle){ this->MasterBpHandle = masterhandle; }
	// �ָ�ԭ���ϵ�
	void puAttData() { this->AttData(); }

private:
	ks_engine *pengine = NULL;
	// ��ʼ���������
	void InitAssembly();
	// �޸�OpenCode
	BOOL ModifyOpCode(const HANDLE hthread, const DWORD MemoryAddress, const char *ModifyData);
	// �ָ�ԭ���ϵ�
	BOOL KeysTone::AttData();

private:
	// ���õ���opcode�Ļ������׵�ַ
	unsigned char* opcode = NULL;
	// ��������opcode���ֽ���
	unsigned int nOpCcodeSize = 0;
	// ���溯���ķ���ֵ�������жϺ����Ƿ�ִ�гɹ�
	int nRet = 0;
	// ����ɹ�����ָ�������
	size_t stat_count = 0;
	// ���洫�����ı����Ծ��
	HANDLE MasterBpHandle;
	// ����ԭ�ڴ��ַ����
	unsigned char* Oldopcode = NULL;
	// ������˶���OpCode
	DWORD read = 0;
	// ���泷�����ڴ��ַ
	DWORD SavaMemoryAdd;
};

#endif