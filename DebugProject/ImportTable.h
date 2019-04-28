#pragma once
#ifndef _IMPORT_TABLE_H_
#define _IMPORT_TABLE_H_
#include <windows.h>

class ImportTab
{
public:
	ImportTab();
	virtual ~ImportTab();

public:
	void puShowTmportTab(){ this->ShowTmportTab(); }
	void puSetProcess(const HANDLE process) { this->m_process = process; }

private:
	// ��ʼ�������
	void InitImportTab();
	// ��ʾ�����
	void ShowTmportTab();
	// NTͷ(DWORD)
	DWORD pNtheaders;
	// pData
	PIMAGE_DATA_DIRECTORY pData;
	// �ļ����ػ�ַ
	char* m_lpBase;
	// ������ṹ��
	PIMAGE_IMPORT_DESCRIPTOR pImport;
	// ����THUNK_DATA32
	PIMAGE_THUNK_DATA32 pTunk;
	// �������
	HANDLE m_process;


};

#endif