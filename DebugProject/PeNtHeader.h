#pragma once
#ifndef _PENTHEADER_H_
#define _PENTHEADER_H_
#include "windows.h"

class PeNtHeader
{
public:
	PeNtHeader();
	virtual ~PeNtHeader();

public:
	// �ļ����
	static HANDLE hFilehand;
	// ������ػ�ַ
	static HANDLE Imagebase;
	// ��ȡFOA
	DWORD puRvaofFoa(const DWORD Rvaddress) { 
		return this->RvaofFoa(Rvaddress); 
	}
	// ��ȡNTͷ
	DWORD puGetPeNtHeader(){ 
		return this->GetPeNtHeader(); 
	}
	// �����ļ����ڴ�
	void puLoadFile(DWORD lpAddress) {
		this->LoadFile(lpAddress);
	}
	// �����ļ�·���ӿ�
	void puSetProcessHeader(HANDLE ProcessBases){ PeNtHeader::hMasterprocess = ProcessBases; }
	// ��ȡ���ػ�ַ
	char* puGetLoadFileBaseAddress(){ return PeNtHeader::m_pSbase; }
	
private:
	// �����ļ����ڴ�
	DWORD LoadFile(DWORD lpAddress);
	// ��ȡNtͷ
	BOOL GetPeNtHeader();
	// RVAתFOA
	DWORD RvaofFoa(const DWORD Rvaddress);

private:
	// ��ȡ�ļ��׵�ַ
	static char* m_pSbase;
	// ����NTͷ
	static DWORD PeNtHeaders;
	// �����������ƣ�
	static HANDLE hMasterprocess;
};


#endif