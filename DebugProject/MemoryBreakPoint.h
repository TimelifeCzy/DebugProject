#ifndef _MEMORYBREAKPOINT_H_
#define _MEMORYBREAKPOINT_H_
#include <windows.h>
#include "stdio.h"


class MemoryBp
{
public:
	MemoryBp();
	virtual ~MemoryBp();
	// ����ԭ���ݽṹ�壬Int3��ԭʹ��
	typedef struct _BREAKPOINT
	{
		DWORD addRess;
		BYTE oldData;
		char Type[20];
	}BREAKPOINT, *PBREAKPOINT;


public:
	// ��������Ϣ
	static HANDLE ProcessHandle;
	BREAKPOINT puSetMeoryBp(DWORD Address, BYTE Type, BOOL *MemFlag) {
		*MemFlag = this->SetrwxMemoryBp(Address, Type);
		return breakPoint;
	}
	// ���þ��
	void puSetHadnle(HANDLE MasterHandler) { 
		this->MasterBreakBpHandle = MasterHandler; 
	}
	// �ָ����
	void puRestorMempryPro(BOOL *MemFlag){
		if (!this->RestoreMemoryPropert()) {
			printf("�����ָ�ʧ���ˣ�");
			*MemFlag = TRUE;
		}
		else
			*MemFlag = FALSE;
	}

private:
	// �ṹ��
	EXCEPTION_RECORD excepRecord;
	// ��ʼ���ڴ�ϵ�
	BOOL InitMemoryBp();
	// �����ڴ��д�ϵ�
	BOOL SetrwxMemoryBp(DWORD Address, BYTE Type);
	// �ָ��ڴ�����
	BOOL RestoreMemoryPropert();
	// ����ϵ�����
	BREAKPOINT breakPoint;
	// ������
	HANDLE MasterBreakBpHandle;
	// �����ڴ�ϵ��ַ��������
	static DWORD BpMemoryAddress;
	static DWORD m_dwAtt;

};


#endif