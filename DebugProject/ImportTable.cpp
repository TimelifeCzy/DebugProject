#include "ImportTable.h"
#include "PeNtHeader.h"
#include "stdio.h"
#include <string>

using std::string;

PeNtHeader obj_Impentheader;

ImportTab::ImportTab()
{
	
}

ImportTab::~ImportTab()
{

}

// ��ʼ�������
void ImportTab::InitImportTab()
{
	pImport = { 0 };
	pNtheaders = obj_Impentheader.puGetPeNtHeader();
	m_lpBase = obj_Impentheader.puGetLoadFileBaseAddress();
	// ��ȡExportͷ
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)pNtheaders;
	pData = (PIMAGE_DATA_DIRECTORY)&pNt->OptionalHeader.DataDirectory[1];
	if (pData->VirtualAddress == 0)
	{
		printf("��ִ���ļ�û�е����");
		return;
	}
	m_lpBase = obj_Impentheader.puGetLoadFileBaseAddress();
	pImport = (PIMAGE_IMPORT_DESCRIPTOR)(pData->VirtualAddress + m_lpBase);
	// // BUG���޸�pImportû��ֵ���� ���ܸ���ͬ��������й�
	pTunk = (PIMAGE_THUNK_DATA32)(pImport->FirstThunk + m_lpBase);
}

// ��ʾ�����
void ImportTab::ShowTmportTab()
{
	InitImportTab();
	if (pData->VirtualAddress == 0)
	{
		printf("��ִ���ļ�û�е����");
		return;
	}
	while (pImport->Name != NULL)
	{
		printf("--------------�������Ϣ-------------------\n");
		printf("DLL����: %s\t", (pImport->Name + m_lpBase));
		printf("Name��RVA: %08X\t", pImport->Name);
		printf("�����ַ���RVA: %08X\n", pImport->FirstThunk);
		printf("==============�������������Ϣ============\n");
		char szGetName[200] = { 0 };
		while (pTunk->u1.Ordinal != 0)
		{
			if (!IMAGE_SNAP_BY_ORDINAL32(pTunk->u1.Ordinal))
			{
				szGetName[0] = 0;
				DWORD read = 0;
				ReadProcessMemory(m_process, (LPVOID)(m_lpBase + pTunk->u1.Ordinal),
					szGetName, 200, &read);
				PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)szGetName;
				printf("%s\t", pName->Name);
				printf("%08X", pTunk->u1.Ordinal);
			}
			else
			{
				printf("\t\t  %08X", pTunk->u1.Ordinal & 0xFFFF);
			}
			++pTunk;
		}
		++pImport;
	}
}