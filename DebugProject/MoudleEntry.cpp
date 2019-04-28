#include "ModuleEntry.h"
#include <TlHelp32.h>
#include <stdio.h>
#include <Psapi.h>
#include <iostream>

#pragma comment (lib, "Version.lib")

using std::wcout; using std::endl; using std::cout;

ModuleEntry::ModuleEntry()
{

}

ModuleEntry::~ModuleEntry()
{

}

BOOL ModuleEntry::ShowProcessModule()
{
	LPVOID VersionInfo = 0;
	HANDLE Moduhandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->SaveMasterProessId);
	HANDLE processhandle = OpenProcess(PROCESS_ALL_ACCESS, GENERIC_READ, SaveMasterProessId);
	MODULEENTRY32 module32 = { sizeof(MODULEENTRY32) };
	Module32First(Moduhandle, &module32);
	// ��ȡģ�����
	MODULEINFO mi = { 0 };
	DWORD dwSize = 0;
	cout << "��ַ\t\t";	cout << "��С\t\t";	cout << "���\t\t";
	cout << "����\t\t\t";	// cout << "�ļ��汾\t\t";	
	cout << "�ļ�·��" << endl;
	// ֧�ֿ��ַ��������
	_wsetlocale(LC_ALL, L"CHS");
	do{
		auto hModle = GetModuleHandle(module32.szExePath);
		// auto hModles = GetModuleHandle(module32.szModule);
		GetModuleInformation(processhandle, hModle, &mi, sizeof(MODULEINFO));
		// ��ַ
		printf("%08X\t", module32.modBaseAddr);
		// ��С
		printf("%08X\t", mi.SizeOfImage);
		// ���
		printf("%08X\t", mi.lpBaseOfDll);
		// ����
		wprintf(L"%s", module32.szModule);
		printf("\t\t   ");
		// �ļ��汾
		// LPVOID size = 0;
		// UINT nSize = 0;
		// dwSize = 0;
		// VersionInfobuf[0] = 0;
		// GetFileVersionInfo(module32.szExePath, NULL, 1024, VersionInfo);
		// strSubBlock.Format("\\StringFileInfo\\%s\\ProductVersion ", strTranslation);
		// VerQueryValue(pBlock, strSubBlock.GetBufferSetLength(256), (LPVOID*)&pVerValue, &nSize);
		// VerQueryValue(VersionInfo, L"\\StringFileInfo\\%s\\ProductVersion ", (LPVOID*)&VersionInfobuf, &nSize);
		//if (VersionInfo != 0)
		//	printf("%s\t", VersionInfobuf);
		// ·��
		wprintf(L"%s", module32.szExePath);
		printf("\n");
	} while (Module32Next(Moduhandle, &module32));
	CloseHandle(Moduhandle);
	return TRUE;
}