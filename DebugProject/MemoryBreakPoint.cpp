#include "MemoryBreakPoint.h"
#include "debugRegisters.h"

HANDLE MemoryBp::ProcessHandle = 0;
DWORD MemoryBp::BpMemoryAddress = 0;
DWORD MemoryBp::m_dwAtt = 0;

MemoryBp::MemoryBp()
{

}

MemoryBp::~MemoryBp()
{

}

// ��ʼ���ڴ�ϵ�
BOOL MemoryBp::InitMemoryBp()
{
	excepRecord = { 0 };
	return 0;
}

// �����ڴ�ϵ�
BOOL MemoryBp::SetrwxMemoryBp(DWORD Address, BYTE Type)
{
	// ��������쳣����ʱ������� ��14
	// 0 ���쳣  1 д�쳣		8 ִ���쳣 
	// typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;
	char buf[MAX_PATH] = { 0 };
	DWORD dwSize = 0;
	// �����쳣�ϵ�
	VirtualProtectEx(MasterBreakBpHandle, (LPVOID)Address, 4, PAGE_NOACCESS, &m_dwAtt);
	BpMemoryAddress = Address;
	breakPoint.addRess = Address;
	breakPoint.oldData = m_dwAtt;
	strcpy(breakPoint.Type, "�ڴ�ϵ�");
	return TRUE;
}

BOOL MemoryBp::RestoreMemoryPropert()
{
	DWORD dwAtter;
	// �ָ���ǰҳ���ڴ�����
	auto ErrorCode = VirtualProtectEx(MasterBreakBpHandle, (LPVOID)BpMemoryAddress, 4, m_dwAtt, &dwAtter);
	if (ErrorCode > 0)
		return TRUE;
	else
		return FALSE;
}

