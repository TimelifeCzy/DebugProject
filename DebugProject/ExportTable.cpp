#include "ExportTable.h"
#include "PeNtHeader.h"
#include "stdio.h"

ExportTab::ExportTab()
{
}

ExportTab::~ExportTab()
{

}

// ��ȡ������
PeNtHeader obj_pentheader;

// ��ʼ��������
void ExportTab::InitExportTable()
{
	pNtheaders = obj_pentheader.puGetPeNtHeader();
	// ��ȡExportͷ
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)pNtheaders;
	pData = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[0];
	if (pData->VirtualAddress == 0)
		return;
	m_lpBase = obj_pentheader.puGetLoadFileBaseAddress();
	pExport = (PIMAGE_EXPORT_DIRECTORY)(pData->VirtualAddress + m_lpBase);
	// 1.��ַ��
	pFunAdd = (DWORD*)(pExport->AddressOfFunctions + m_lpBase);
	// 2.�������Ʊ�
	pNameAddr = (DWORD*)(pExport->AddressOfNames + m_lpBase);
	// 3.��ű�
	pOrdinalAddr = (DWORD*)(pExport->AddressOfNameOrdinals + m_lpBase);
}

// ��ʾ������
void ExportTab::ShowExportTable()
{
	InitExportTable();
	if (pData->VirtualAddress == 0)
	{
		printf("��ִ���ļ�û�е�����\n");
		return;
	}
	printf("--------------��������Ϣ-------------------\n");
	printf("����ֵ: %08X\t", pExport->TimeDateStamp);
	printf("��Ż�ַ: %08X\t", pExport->Base);
	printf("����: %08X\n", pExport->Base);
	printf("������ַ(RVA):\t   %08X\n", pExport->AddressOfFunctions);
	printf("�������ص�ַ(RVA): %08X\n", pExport->AddressOfNames);
	printf("������ŵ�ַ(RVA): %08X\n", pExport->AddressOfNameOrdinals);
	printf("��������: %08X\t", pExport->NumberOfFunctions);
	printf("������������: %08X\n", pExport->NumberOfNames);
	printf("-------------------------------------------\n");
	DWORD dwCount = (DWORD)pExport->NumberOfFunctions;
	DWORD dwCounts = (DWORD)pExport->NumberOfNames;
	// ѭ��������Ϣ
	for (DWORD i = 0; i < dwCount; ++i)
	{
		if (0 == pFunAdd[i])
			continue;
		BOOL bFlag = FALSE;
		for (DWORD j = 0; j < dwCounts; j++)
		{
			bFlag = TRUE;
			DWORD dwNameRVA = pNameAddr[j];
			printf("������ţ�%08d\t", pOrdinalAddr[j]);
			printf("��������%s\n", (char *)(dwNameRVA + m_lpBase));
		}
		if (!bFlag) {
			printf("������ţ�%08d\t", (i + pExport->Base));
			printf("��������NULL\n");
		}
	}
}