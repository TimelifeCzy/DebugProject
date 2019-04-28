#pragma once
#ifndef _EXPORT_TABLE_H_
#define _EXPORT_TABLE_H_
#include <windows.h>

class ExportTab
{
public:
	ExportTab();
	virtual ~ExportTab();

public:
	void puShowExportTable(){ this->ShowExportTable(); }

private:
	// ������ػ�ַ
	char* m_lpBase;
	// ��ʼ��������
	void InitExportTable();
	// ��ʾ������
	void ShowExportTable();
	// ����Ntͷ
	DWORD pNtheaders = 0;
	// ������ṹ��
	PIMAGE_EXPORT_DIRECTORY pExport;
	// ������������ַ����š���������FOA
	DWORD* pFunAdd;
	DWORD* pNameAddr;
	DWORD* pOrdinalAddr;
	// ���浼����
	PIMAGE_DATA_DIRECTORY pData;
};


#endif