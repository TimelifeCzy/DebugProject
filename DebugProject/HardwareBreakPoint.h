#ifndef _HARDWAREBREAKPOINT_H_
#define _HARDWAREBREAKPOINT_H
#include "windows.h"

// ����ԭ���ݽṹ�壬Int3��ԭʹ��
typedef struct _BREAKPOINT
{
	DWORD addRess;
	BYTE oldData;
	char Type[20];
}BREAKPOINT, *PBREAKPOINT;

class HardwareBreakP
{
public:
	HardwareBreakP();
	virtual ~HardwareBreakP();
	// ����HardwareThread
	static HANDLE hHardThread;

public:
	// ��ʼ��Ӳ���ϵ�
	BREAKPOINT puInitHardwareBreakP(const DWORD uAddress, CONTEXT* m_conText){
		hwBreakPoint = { 0 };
		this->voidHardwareBreakP(uAddress, m_conText);
		return hwBreakPoint;
	}
	// ����Ӳ����д�ϵ�
	BREAKPOINT puReadWriteHardbp(CONTEXT* m_conText, DWORD uAddress, const WORD type, const WORD dwLen) {
		hwBreakPoint = { 0 };
		this->ReadWriteHardbp(m_conText, uAddress, type, dwLen);
		return hwBreakPoint;
	}

private:
	// ��ʼ��Ӳ���ϵ�
	BOOL voidHardwareBreakP(const DWORD uAddress, CONTEXT* m_conText);
	// ����Ӳ����д�ϵ�
	BOOL ReadWriteHardbp(CONTEXT* m_conText, DWORD uAddress, const WORD type, const WORD dwLen);
	// ����Ӳ�ϵ����Ϣ
	BREAKPOINT hwBreakPoint;

private:
	CONTEXT ct;

};

#endif