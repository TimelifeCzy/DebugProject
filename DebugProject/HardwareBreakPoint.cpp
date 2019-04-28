#include "HardwareBreakPoint.h"
#include "debugRegisters.h"

HANDLE HardwareBreakP::hHardThread = 0;

HardwareBreakP::HardwareBreakP()
{

}

HardwareBreakP::~HardwareBreakP()
{

}

// ����Ӳ��ִ�жϵ�
BOOL HardwareBreakP::voidHardwareBreakP(const DWORD uAddress, CONTEXT *m_conText)
{
	DBG_REG7* pDr7 = (DBG_REG7 *)&m_conText->Dr7;
	if (0 == pDr7->L0) {
		m_conText->Dr0 = uAddress;
		pDr7->RW0 = 0;		// �ϵ�����
		pDr7->LEN0 = 0;		// �ϵ㳤��
		pDr7->L0 = 1;
	}
	else if (0 == pDr7->L1){
		m_conText->Dr1 = uAddress;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
		pDr7->L1 = 1;
	}
	else if (0 == pDr7->L2){
		m_conText->Dr2 = uAddress;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
		pDr7->L2 = 1;
	}
	else if (0 == pDr7->L3){
		m_conText->Dr3 = uAddress;
		pDr7->RW3 = 0;
		pDr7->LEN3 = 0;
		pDr7->L3 = 1;
	}
	else
		return FALSE;
	hwBreakPoint.addRess = uAddress;
	hwBreakPoint.oldData = 0;
	strcpy(hwBreakPoint.Type, "Ӳ�ϵ�(��ִ��)");
	SetThreadContext(HardwareBreakP::hHardThread, m_conText);
	return TRUE;
}

// ����Ӳ����д�ϵ�
BOOL HardwareBreakP::ReadWriteHardbp(CONTEXT* m_conText, DWORD uAddress, const WORD type, const WORD dwLen)
{
	// ��������
	if (dwLen == 1)
		uAddress = uAddress - (uAddress % 2);
	else if (dwLen == 3)
		uAddress = uAddress - (uAddress % 4);
	else if (dwLen > 3)
		return FALSE;
	// �ж���Щ�Ĵ���û�б�ʹ��
	DBG_REG7* pDr7 = (DBG_REG7 *)&m_conText->Dr7;
	if (0 == pDr7->L0) {
		m_conText->Dr0 = uAddress;
		pDr7->RW0 = type;
		pDr7->LEN0 = dwLen;
		pDr7->L0 = 1;
	}
	else if (0 == pDr7->L1){
		m_conText->Dr1 = uAddress;
		pDr7->RW1 = type;
		pDr7->LEN1 = dwLen;
		pDr7->L1 = 1;
	}
	else if (0 == pDr7->L2){
		m_conText->Dr2 = uAddress;
		pDr7->RW2 = type;
		pDr7->LEN2 = dwLen;
		pDr7->L2 = 1;
	}
	else if (0 == pDr7->L3){
		m_conText->Dr3 = uAddress;
		pDr7->RW3 = type;
		pDr7->LEN3 = dwLen;
		pDr7->L3 = 1;
	}
	else
		return FALSE;
	hwBreakPoint.addRess = uAddress;
	hwBreakPoint.oldData = dwLen;
	if (1 == type)
		strcpy(hwBreakPoint.Type, "Ӳ�ϵ�(��)");
	else if (3 == type)
		strcpy(hwBreakPoint.Type, "Ӳ�ϵ�(д)");
	SetThreadContext(HardwareBreakP::hHardThread, m_conText);
	return TRUE;
}
