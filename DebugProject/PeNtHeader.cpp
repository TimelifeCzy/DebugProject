#include "PeNtHeader.h"
#include "iostream"

using std::cout; using std::endl;

DWORD PeNtHeader::PeNtHeaders = 0;

char* PeNtHeader::m_pSbase = 0;

HANDLE PeNtHeader::hMasterprocess = 0;

HANDLE PeNtHeader::hFilehand = 0;

HANDLE PeNtHeader::Imagebase = 0;

PeNtHeader::PeNtHeader()
{

}

PeNtHeader::~PeNtHeader()
{

}

//  ���ڴ�
DWORD PeNtHeader::LoadFile(DWORD lpAddress)
{
	// ��ȡ�ļ���С
	DWORD dwSize = GetFileSize(PeNtHeader::hFilehand, NULL);
	DWORD nRead = 0;
	PeNtHeader::m_pSbase = new char[dwSize]{0};
	BOOL ErrorCode =
		ReadProcessMemory(PeNtHeader::hMasterprocess, (LPVOID)PeNtHeader::Imagebase, PeNtHeader::m_pSbase, dwSize, &nRead);
	if (ErrorCode)
		return 1;
	return -1;
 }

// ��ȡNtͷ
BOOL PeNtHeader::GetPeNtHeader()
{
	// ��ȡDos
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)PeNtHeader::m_pSbase;
	// ��ȡNt
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + PeNtHeader::m_pSbase);
	// ����Ntͷ
	PeNtHeader::PeNtHeaders = (DWORD)pNt;
	return PeNtHeader::PeNtHeaders;
}

// RVAתFOA
DWORD PeNtHeader::RvaofFoa(const DWORD Rvaddress)
{
	// ��ȡDos
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pSbase;
	// ��ȡNt
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_pSbase);
	// ��ȡ����ͷ
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// ��ȡ��������
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	// ����ת��
	for (DWORD i = 0; i < dwCount; ++i)
	{
		// ��������
		if ((pSection->VirtualAddress <= Rvaddress) && ((pSection->VirtualAddress + pSection->SizeOfRawData) > Rvaddress))
		{
			return Rvaddress - pSection->VirtualAddress + pSection->PointerToRawData;
		}
		++pSection;
	}
	return -1;
}