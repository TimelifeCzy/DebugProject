#include "MasterExceptionHandler.h"
#include "debugRegisters.h"
#include <iostream>
#include <stdio.h>
#include "Capstone.h"
#include "UKeystone.h"
#include "UWindows.h"
#include "CommandHelpDocument.h"
#include "RegisterInfoData.h"
#include "StackInfoData.h"
#include "MemoryInfoData.h"
#include "PeNtHeader.h"
#include "ExportTable.h"
#include "ImportTable.h"
#include "HardwareBreakPoint.h"
#include "MemoryBreakPoint.h"
#include "ModuleEntry.h"
#include "winternl.h"
#include "DbgHelp.h"

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "ntdll.lib")

using std::cout; using std::endl; using std::cin; 
using std::wcout; using std::wcin;

BOOL MasterExceptionHandler::IsOneLoadProcess = TRUE;

BOOL MasterExceptionHandler::MemFlag = FALSE;

// ����ϵͳ�ϵ�״̬����һ�Σ�
BOOL MasterExceptionHandler::m_SystemBreakPointFlag = TRUE;

int MasterExceptionHandler::nCountRow = 0;

#ifdef _DEBUG
// �������DebugInfo
#define ERRORDEBUG_INFO(errorMsg) cout << "%s\n" << __FILE__ << "%s\n"  \
										<< __FUNCTION__ << "%s\n" << "%d\n" \
											<< __LINE__ << "%s\n" << errorMsg << endl;
#else
#define ERRORDEBUG_INFO()
#endif

HANDLE MasterExceptionHandler::m_hProcess = 0;

MasterExceptionHandler::MasterExceptionHandler()
{
}

MasterExceptionHandler::~MasterExceptionHandler()
{

}

// �����쳣��,
INT MasterExceptionHandler::ResponsetoDragMessage()
{
	// ��ȡȨ��
	this->GetSystempermissions();
	// �ж��ǲ��ǵ�һ�μ��س���
	if (IsOneLoadProcess == TRUE){
		// 1. �ȴ�Cmd����·������
		cin >> m_Executable;
	}
	// �����´����Ĺ��̼�����Ҫ��Ϣ�̡߳�
	pRocessInfo = { 0 };
	// ָ������,����,�����ڵ���۱�׼����,����ʱ��Ĺ���.
	STARTUPINFOA sTartInfo = { sizeof(STARTUPINFOA) };
	// ���ļ�
	Fopen = fopen("DUMP.txt", "a+");
	// 2. ��.exe����
	INT nRet = CreateProcessA(
		m_Executable,
		NULL,
		NULL,
		NULL,
		FALSE,
		DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&sTartInfo, &pRocessInfo);
	if (0 >= nRet) {
		ERRORDEBUG_INFO("CreateProcessA() failuer!");
		exit(EXIT_FAILURE);
	}
	// pRocessInfo.hProcess = m_hProcess;
	// 3. ������һ�������¼�
	DebugEvent = { 0 }; 
	// 4. �����¼��ṹ��
	while (1)
	{
		// 1. �ȴ�
		WaitForDebugEvent(&DebugEvent, INFINITE);
		// 2. �򿪽��̡��̻߳�ȡ���
		m_dExceptionHandleFlag = DBG_CONTINUE;
		m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
		m_hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEvent.dwThreadId);
		// 3. �������ϻ���
		m_conText.ContextFlags = CONTEXT_ALL;
		// 4. ����ָ���̵߳�������
		if (!GetThreadContext(m_hThread, &m_conText)) {
			ERRORDEBUG_INFO("GetThreadContext() failuer!");
		}
		// 5. ʶ��Ĵ�������¼�������
		switch (DebugEvent.dwDebugEventCode)
		{
			// 5.1 �쳣�¼�
		case EXCEPTION_DEBUG_EVENT:
		{
			// ������쳣��¼����(EXCEPTION_RECORD)
			m_dExceptionHandleFlag = OneofExceptionDispacth(&DebugEvent.u.Exception.ExceptionRecord);
		}
		break;
		case CREATE_THREAD_DEBUG_EVENT: cout << "�̴߳����¼�" << endl; break;			// �̴߳����¼�
		case EXIT_THREAD_DEBUG_EVENT:   cout << "�߳��˳��¼�" << endl; break;			// �˳��߳��¼�
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			// ��������
			InitParSymbol();
			// �����ļ�����������״Σ�
			this->Filehandle = DebugEvent.u.CreateProcessInfo.hFile;
			this->Imagebase = DebugEvent.u.CreateProcessInfo.lpBaseOfImage;
			fprintf(Fopen, "%d %s\n", nCountRow++, "���̴����¼�: ���汻�����ļ������lpBaseOfImage(Code��124)");
			OEPflag = FALSE;
			// ������ڵ�
			m_StartAddr = (DWORD)DebugEvent.u.CreateProcessInfo.lpStartAddress;
			fprintf(Fopen, "%d %s\n", nCountRow++, "���̴����¼�: ���汻����lpStartAddress(Code��129)");
			cout << "���̴����¼�" << endl;												// ���̴����¼�
			fflush(Fopen);
		}
		break;
		case EXIT_PROCESS_DEBUG_EVENT:	cout << "�����˳��¼�\n" << endl; break;			// �˳������¼�
		case LOAD_DLL_DEBUG_EVENT:
		{
			char ModuleFile[MAX_PATH] = { 0 };
			// ��ȡģ������
			GetModuleFileNameA((HMODULE)DebugEvent.u.LoadDll.lpBaseOfDll, ModuleFile, MAX_PATH);
			// ��ʼ���ṹ����Ϣ
			SymLoadModule64(
				pRocessInfo.hProcess,
				DebugEvent.u.CreateProcessInfo.hFile,
				ModuleFile,
				NULL,
				(DWORD64)DebugEvent.u.LoadDll.lpBaseOfDll,
				0
				);
			cout << "DLL�����¼�" << endl;
		}	
		break;			// ӳ��DLL�¼�
		case UNLOAD_DLL_DEBUG_EVENT:    cout << "DLLж���¼�" << endl; break;			// ж��DLL�¼� 
		case OUTPUT_DEBUG_STRING_EVENT: cout << "�����ַ�������¼�" << endl; break;		// �����ַ�������¼�
		case RIP_EVENT:
		default:
			break;
		}
		// ����֮ǰ�����߳�������
		if (!SetThreadContext(m_hThread, &m_conText)) {
			ERRORDEBUG_INFO("SetThreadContext() failuer!");
		}
		CloseHandle(m_hProcess);
		CloseHandle(m_hThread);
		// �ظ�������ϵͳ
		ContinueDebugEvent(
			DebugEvent.dwProcessId,
			DebugEvent.dwThreadId,
			m_dExceptionHandleFlag);
	}
	// �ر��ļ�ָ��
	fclose(Fopen);
	return 0;
}

// ��������
void MasterExceptionHandler::SetBreakPoint()
{
	// IF���ⲿ�������жϣ��жϲ��ù�IF��־λ
	// 1. �����ж����� �����쳣TF = 1
	EFLAGS *pEflage = (EFLAGS *)&m_conText.EFlags;
	pEflage->TF = 1;
}

// ��������
void MasterExceptionHandler::StepOverBp()
{
	BYTE OpCodeBuf[16] = { 0 };
	DWORD processAdd = (DWORD)DebugEvent.u.Exception.ExceptionRecord.ExceptionAddress;
	DWORD read = 0;
	ReadProcessMemory(m_hProcess, (LPVOID)processAdd, OpCodeBuf, sizeof(OpCodeBuf), &read);
	
	if ((0xE8 == OpCodeBuf[0]) || (0xF3 == OpCodeBuf[0]))
	{
		m_conText.Eip += OpCodeLen(m_conText.Eip);
		SetInt3BreakPoint(m_conText.Eip);
	}
	else
		SetBreakPoint();
	SetThreadContext(m_hThread, &m_conText);
}

// ��ȡ�����볤��
short MasterExceptionHandler::OpCodeLen(DWORD address)
{
	csh Handle;
	// ��һ�����
	cs_open(CS_ARCH_X86, CS_MODE_32, &Handle);
	// ���û�����
	BYTE* OpCode = (BYTE *)malloc(16);
	memset(OpCode, 0, sizeof(BYTE) * 16);
	cs_insn* ins = nullptr;
	DWORD dwSize = 0;
	// ����ǰָ������
	ReadProcessMemory(m_hProcess, (LPCVOID)address, OpCode, 16, &dwSize);
	// int count = cs_disasm(Handle, (uint8_t*)pOpCode, nLen * 16, (uint64_t)pAddr, 0, &ins);
	int count = cs_disasm(Handle, (uint8_t *)OpCode, 16, (uint64_t)m_conText.Eip, 0, &ins);
	// ����ָ��ȴ�С
	return ins->size;
}

// ����Int3�ϵ�
void MasterExceptionHandler::SetInt3BreakPoint(DWORD address)
{
	size_t Flag = 0;
	// ����ԭ�����ݣ����ݣ�
	BREAKPOINT BreakPoint = { 0 };
	SIZE_T read = 0;
	int ErrorCode = 0;
	BreakPoint.addRess = address;
	ErrorCode = ReadProcessMemory(m_hProcess, (LPVOID)address, &BreakPoint.oldData, 1, &read);
	if (0 >= ErrorCode) {
		ERRORDEBUG_INFO("Int3 ReadProceeMemory() faliuer!");
	}
	ErrorCode = WriteProcessMemory(m_hProcess, (LPVOID)address, "\xCC", 1, &read);
	if (0 >= ErrorCode) {
		ERRORDEBUG_INFO("Int3 WriteProcessMemory() failuer!");
	}
	if (this->OEPflag == FALSE)
		strcpy(BreakPoint.Type, "OEP");
	else
		strcpy(BreakPoint.Type, "��ϵ㣨int 3��");
	if (0 == m_SaveBreakPoint.size())
		m_SaveBreakPoint.push_back(BreakPoint);
	// �����ж������ǰ�����������ַ�����
	for (size_t i = 0; i < m_SaveBreakPoint.size(); ++i)
	{
		if (BreakPoint.addRess != m_SaveBreakPoint[i].addRess)
			Flag += 1;
	}
	if (Flag == m_SaveBreakPoint.size())
	{
		m_SaveBreakPoint.push_back(BreakPoint);
		fprintf(Fopen, "%d %s\n", nCountRow++, "Int 3��ϵ�ɹ�: �ɹ�(Code��237)");
		fflush(Fopen);
	}
}

// ���û�����
void MasterExceptionHandler::SetAssemblyCode(const DWORD address, const char* ModifyData)
{
	// ��ȡ��ǰ��ַ��OPECODE
	KeysTone obj_Keystone;
	obj_Keystone.puSetBpHandle(m_hProcess);
	if (address == 0)
	{
		obj_Keystone.puAttData();
		printf("�ָ����ָ��ɹ��ɹ�!\n");
	}
	else
	{
		BOOL ErrorCode = obj_Keystone.puModifyOpenCode(m_hThread, address, ModifyData);
		if (0 >= ErrorCode) {
			ERRORDEBUG_INFO("Modify Assembly Data failuer!");
			printf("Alter of Assembly Sucess!\n");
			return;
		}
	}

}

// �޸ļĴ����ж�
int SwitchModifyRegister(char *reg)
{	
	if (NULL == reg)
		return 0;
	else if (0 == _stricmp("EAX", reg))
		return 2; 
	else if (0 == _stricmp("ECX", reg))
		return 3;
	else if (0 == _stricmp("EDX", reg))
		return 4; 
	else if (0 == _stricmp("EBX", reg))
		return 5; 
	else if (0 == _stricmp("ESP", reg))
		return 6; 
	else if (0 == _stricmp("EBP", reg))
		return 7;
	else if (0 == _stricmp("ESI", reg))
		return 8; 
	else if (0 == _stricmp("EDI", reg))
		return 9;
	else if (0 == _stricmp("EIP", reg))
		return 10;
	else if (0 == _stricmp("TF", reg))
		return 11;
	else if (0 == _stricmp("IF", reg))
		return 12;
	else if (0 == _stricmp("DF", reg))
		return 13;
	else if (0 == _stricmp("CF", reg))
		return 14;
	else if (0 == _stricmp("ZF", reg))
		return 15;
	else if (0 == _stricmp("PF", reg))
		return 16;
	else if (0 == _stricmp("SF", reg))
		return 17;
	else if (0 == _stricmp("AF", reg))
		return 18;
	else if (0 == _stricmp("OF", reg))
		return 19;
	return 0;
}

// �鿴��ǰ�ϵ�(��Ӳ)
void MasterExceptionHandler::ShowCurrBreakPoint()
{
	cout << "�ϵ��ַ    ";
	cout << "������\t\t";
	cout << "�ϵ�����" << endl;
	for (size_t i = 0; i < m_SaveBreakPoint.size(); ++i)
	{
		printf("%08X\t", m_SaveBreakPoint[i].addRess);
		wcout << m_SaveBreakPoint[i].oldData << "\t\t\t";
		wcout << m_SaveBreakPoint[i].Type << endl;
	}
	for (size_t i = 0; i < m_SaveHardBp.size(); ++i)
	{
		printf("%08X\t", m_SaveHardBp[i].addRess);
		wcout << m_SaveHardBp[i].oldData << "\t\t\t";
		wcout << m_SaveHardBp[i].Type << endl;
	}
	for (size_t i = 0; i < m_SaverMemoryBp.size(); ++i)
	{
		printf("%08X\t", m_SaverMemoryBp[i].addRess);
		wcout << m_SaverMemoryBp[i].oldData << "\t\t\t";
		wcout << m_SaverMemoryBp[i].Type << endl;
	}
	for (size_t i = 0; i < m_SaveCpBp.size(); ++i)
	{
		printf("%08X\t", m_SaveCpBp[i].addRess);
		wcout << m_SaveCpBp[i].oldData << "\t\t\t";
		wcout << m_SaveCpBp[i].Type << endl;
	}
}

// �����Զϵ�����
void MasterExceptionHandler::PermanentBreakPoint(DWORD address)
{
	for (size_t i = 0; i < m_SaveBreakPoint.size(); ++i)
	{
		// �����ǰ��ַС�ڶϵ��ַ����û�����е��ϵ㴦 ��������
		SetInt3BreakPoint(m_SaveBreakPoint[i].addRess);
	}
	for (size_t i = 0; i < m_SaveHardBp.size(); ++i)
	{
		m_conText.Dr7 = 1;
	}
}

// ɾ�������Զϵ�
void MasterExceptionHandler::DeletePermanentBp(DWORD address)
{
	SIZE_T write = 0;
	// �ָ�Int3�ϵ�
	for (size_t i = 0; i < m_SaveBreakPoint.size(); ++i)
	{
		if (address == m_SaveBreakPoint[i].addRess)
		{
			// �ָ�0xCC�ϵ� û�лָ��ɹ�
			WriteProcessMemory(m_hProcess, &address, &m_SaveBreakPoint[i].oldData, 1, &write);
			// ɾ�������Զϵ�ϵ�
			auto index = find(m_SaveBreakPoint.begin(), m_SaveBreakPoint.end(), m_SaveBreakPoint[i]);
			m_SaveBreakPoint.erase(index);
			cout << "ɾ�������Զϵ�ɹ�" << endl;
			fprintf(Fopen, "%d %s\n", nCountRow++, "ɾ�������Զϵ�ɹ�: �ɹ�(Code��374)");
			break;
		}
	}
	// Ӳ�ϵ�
	for (size_t i = 0; i < m_SaveHardBp.size(); ++i)
	{
		if (address == m_SaveHardBp[i].addRess)
		{
			m_conText.Dr7 = 0;
			// ɾ�������Զϵ�ϵ�
			auto index = find(m_SaveHardBp.begin(), m_SaveHardBp.end(), m_SaveHardBp[i]);
			m_SaveHardBp.erase(index);
			cout << "ɾ��������Ӳ�ϵ�ɹ�" << endl;
			fprintf(Fopen, "%d %s\n", nCountRow++, "ɾ��������Ӳ�ϵ�ɹ�: �ɹ�(Code��388)");
			break;
		}
	}
	// �ڴ�ϵ�
	for (size_t i = 0; i < m_SaverMemoryBp.size(); ++i)
	{
		if (address == m_SaverMemoryBp[i].addRess)
		{
			// �ָ��ڴ�ҳ���ԭ������
			WriteProcessMemory(m_hProcess, &address, &m_SaverMemoryBp[i].oldData, 1, &write);
			// ɾ�������Զϵ�ϵ�
			auto index = find(m_SaverMemoryBp.begin(), m_SaverMemoryBp.end(), m_SaverMemoryBp[i]);
			m_SaverMemoryBp.erase(index);
			cout << "ɾ���������ڴ�ϵ��ɹ�" << endl;
			fprintf(Fopen, "%d %s\n", nCountRow++, "ɾ���������ڴ�ϵ��ɹ�: �ɹ�(Code��403)");
			break;
		}
	}
	fflush(Fopen);
}

// �û�������Ϣ����
void MasterExceptionHandler::UserInfoInput()
{
	// ��һ���ֽڳ�ʼ��0����ʼ������
	char cmd[10] = { 0 };
	regName[0] = 0;
	// �����޸ĵ���ֵ
	DWORD NewRegData = 0;
	// ѭ��������Ϣ
	while (true)
	{
		cout << "��������>> ";
		cin >> cmd;
		// ���õ������뿪ѭ��
		if (0 == _stricmp(cmd, "t")) {
			SetBreakPoint();
			HardBpFlag = TRUE;
			IsCpBp = TRUE;
			FlgasFlou = TRUE;
			break;
		}
		// ��������
		else if (0 == _stricmp(cmd, "s")) {
			StepOverBp();
			FlgasFlou = TRUE;
			IsCpBp = TRUE;
			HardBpFlag = TRUE;
			break;
		}
		// ���¼��س���
		else if (0 == _stricmp(cmd, "reload")) {
			CloseHandle(m_hProcess);
			CloseHandle(m_hThread);
			MasterExceptionHandler::IsOneLoadProcess = FALSE;
			MasterExceptionHandler::m_SystemBreakPointFlag = TRUE;
			cout << "���¼��ؿ�ִ�г���......." << endl;
			Sleep(500);
			ResponsetoDragMessage();
			FlgasFlou = TRUE;
			break;
		}
		else if (0 == _stricmp(cmd, "oep")) {
			OEPflag = FALSE;
			SetInt3BreakPoint(m_StartAddr);
			break;
		}
		// ����
		else if (0 == _stricmp(cmd, "cls")) {
			system("cls");
		}
		// ������ϵ�
		else if (0 == _stricmp(cmd, "bp")) {
			OEPflag = TRUE;
			DWORD addRes = 0;
			scanf_s("%x", &addRes);
			SetInt3BreakPoint(addRes);
		}
		// ��ִ��Ӳ�ϵ�
		else if (0 == _stricmp(cmd, "hwxbp")) {
			IsCpBp = FALSE;
			HardBpFlag = TRUE;
			char str[10] = { 0 };
			cin >> str;
			DWORD hwBpAddress = strtoul(str, NULL, 16);
			HardwareBreakP obj_hardBp;
			HardwareBreakP::hHardThread = this->m_hThread;
			auto BreakPoints = obj_hardBp.puInitHardwareBreakP(hwBpAddress, &m_conText);
			BREAKPOINT Temp = { 0 };
			Temp.addRess = BreakPoints.addRess;
			Temp.oldData = BreakPoints.oldData;
			strcpy(Temp.Type, BreakPoints.Type);
			m_SaveHardBp.push_back(Temp);
		}
		// ��дӲ�ϵ�
		else if (0 == _stricmp(cmd, "hwrwbp")){
			HardBpFlag = TRUE;
			IsCpBp = FALSE;
			DWORD hwBpAddress = 0;
			WORD hwBpType = 0;
			WORD hwBpLen = 0;
			char str[10] = { 0 };
			cin >> str;
			hwBpAddress = strtoul(str, NULL, 16);
			cin >> hwBpType; cin >> hwBpLen;
			HardwareBreakP obj_hardBp;
			HardwareBreakP::hHardThread = this->m_hThread;
			auto Bpreturn = obj_hardBp.puReadWriteHardbp(&m_conText, hwBpAddress, hwBpType, hwBpLen);
			BREAKPOINT Temp1 = { 0 };
			Temp1.addRess = Bpreturn.addRess;
			Temp1.oldData = Bpreturn.oldData;
			strcpy(Temp1.Type, Bpreturn.Type);
			m_SaveHardBp.push_back(Temp1);
		}
		// �����ڴ�ϵ�
		else if (0 == _stricmp(cmd, "memorybp")) {
			if (MemoryBpFlag)
			{
				MemoryBp obj_memorybp;
				obj_memorybp.puSetHadnle(this->m_hProcess);
				DWORD Addr = 0;
				scanf_s("%x", &Addr);
				auto Bp = obj_memorybp.puSetMeoryBp(Addr, 0, &MemFlag);
				BREAKPOINT Temp1 = { 0 };
				Temp1.addRess = Bp.addRess;
				Temp1.oldData = Bp.oldData;
				strcpy(Temp1.Type, Bp.Type);
				m_SaverMemoryBp.push_back(Temp1);
				MemoryBpFlag = FALSE;
			}
			else{
				cout << "ֻ����һ���ڴ�ϵ�,����ɾ�������ڴ�ϵ�" << endl;
			}
		}
		// �����ϵ��ж�
		else if (0 == _stricmp(cmd, "cdit")) {
			IsCpBp = TRUE;
			// ��֤Ϊ
			HardBpFlag = FALSE;
			DWORD Address = 0;
			char Temp[10] = { 0 };
			char Reg[10] = { 0 };
			cin >> Temp;
			Address = strtoul(Temp, NULL, 16);
			cin >> Reg;
			cin >> nCount;
			SetCpbp(Address, nCount);
		}
		// �鿴��ǰ�ϵ�
		else if (0 == _stricmp(cmd, "bl")) {
			ShowCurrBreakPoint();
		}
		// ɾ�������Զϵ�
		else if (0 == _stricmp(cmd, "dbp")) {
			DWORD InputAddress = 0;
			scanf("%x", &InputAddress);
			DeletePermanentBp(InputAddress);
		}
		// ���г��򣨻�����������һ���ϵ㣩
		else if (0 == _stricmp(cmd, "g")) {
			FlgasFlou = TRUE;
			break;
		}
		// �޸Ļ�����
		else if (0 == _stricmp(cmd, "md")) {
			char Address[10];
			char AssemblyData[] = { 0 };
			cin >> Address;
			DWORD dMemoryPath = strtoul(Address, NULL, 16);
			getchar();
			scanf("%[^\n]", AssemblyData);
			SetAssemblyCode(dMemoryPath, AssemblyData);
		}
		// �����ĵ��鿴
		else if (0 == _stricmp(cmd, "help")) {
			ComHelpDocument obj_Comhelpdct;
			obj_Comhelpdct.puShowComandHelp();
		}
		// �Ĵ����鿴
		else if (0 == _stricmp(cmd, "r")) {
			// ��ʼ������
			RegisterData obj_register;
			RegisterData::hReThread = m_hThread;
			obj_register.RegisterInfo();
		}
		// �޸ļĴ���
		else if (0 == _stricmp(cmd, "rmodify")) {
			cin >> regName;
			scanf("%x", &NewRegData);
			RegisterData obj_register;
			RegisterData::hReThread = m_hThread;
			int nFlag = SwitchModifyRegister(regName);
			if (nFlag > 0)
				obj_register.RegisterInfo(nFlag, NewRegData);
			else
				cout << "��������ȷ�ļĴ�����ע��:����ȫ��д��" << endl;
		}
		// �鿴���ָ��
		else if (0 == _stricmp(cmd, "uu")) {
			Capstone obj_capstone;
			obj_capstone.ShowAssembly((void *)m_conText.Eip, 10);
		}
		// �鿴�ڴ��ַ���
		else if (0 == _stricmp(cmd, "u")) {
			DWORD Address = 0;
			Capstone obj_capstone;
			scanf("%x", &Address);
			obj_capstone.ShowAssembly((void *)Address, 10);
		}
		// �鿴ģ������
		else if (0 == _stricmp(cmd, "ml")) {
			ModuleEntry obj_module;
			obj_module.SetProcessHandle(DebugEvent.dwProcessId);
			obj_module.puShowProcessModule();
		}
		// ��ʾջ��Ϣ
		else if (0 == _stricmp(cmd, "stack")) {
			StackData obj_stack;
			StackData::hStackThread = this->m_hThread;
			StackData::hStackProcess = this->m_hProcess;
			obj_stack.puShowStackInfoData();
		}
		// ��ʾ�ڴ���Ϣ
		else if (0 == _stricmp(cmd, "d")) {
			char Address[10] = { 0 };
			cin >> Address;
			DWORD MemoryAddress = strtoul(Address, NULL, 16);
			MemoryData obj_memory;
			MemoryData::hMemThread = this->m_hThread;
			MemoryData::hMemProcess = this->m_hProcess;
			obj_memory.MemoryDatas(MemoryAddress);
		}
		// ��ʾĬ�ϼĴ���Eip�ڴ�����
		else if (0 == _stricmp(cmd, "dd")) {
			MemoryData obj_memory;
			MemoryData::hMemThread = this->m_hThread;
			MemoryData::hMemProcess = this->m_hProcess;
			obj_memory.MemoryDatas();
		}
		// �޸��ڴ���Ϣ
		else if (0 == _stricmp(cmd, "dmodify")) {
			char Temp1[8] = { 0 };
			char MemoryModifyData[MAX_PATH] = { 0 };
			cin >> Temp1;
			cin >> MemoryModifyData;
			DWORD MemoryAddress = strtoul(Temp1, NULL, 16);
			DWORD ModifyData = strtoul(MemoryModifyData, NULL, 16);
			// DWORD64 MemoryModifyData = strtoul(Temp, NULL, 16);
			MemoryData obj_memory;
			MemoryData::hMemThread = this->m_hThread;
			MemoryData::hMemProcess = this->m_hProcess;
			obj_memory.MemoryDatas(MemoryAddress, ModifyData);
		}
		// �鿴�������뵼���
		else if ((0 == _stricmp(cmd, "export")) || (0 == _stricmp(cmd, "import")))
		{
			if (TRUE == IsInitTabFlag)
			{
				IsInitTabFlag = FALSE;
				PeNtHeader obj_pentheader;
				PeNtHeader::hFilehand = Filehandle;
				PeNtHeader::Imagebase = this->Imagebase;
				obj_pentheader.puSetProcessHeader(MasterExceptionHandler::m_hProcess);
				obj_pentheader.puLoadFile((DWORD)DebugEvent.u.CreateProcessInfo.lpBaseOfImage);
			}
			if (0 == _stricmp(cmd, "export")) {
				ExportTab obj_export;
				obj_export.puShowExportTable();
			}
			if (0 == _stricmp(cmd, "import")) {
				ImportTab obj_import;
				obj_import.puSetProcess(this->m_hProcess);
				obj_import.puShowTmportTab();
			}
		}
		// ���Ž�����API��ѯ
		else if (0 == _stricmp(cmd, "findapi")) {
			// InitParSymbol();
			char ApiName[MAX_PATH] = { 0 };
			printf("�������ѯ��API��");
			cin >> ApiName;
			ApiofAddress(ApiName);
		}
		// ������ַ
		else if (0 == _stricmp(cmd, "findaddress")) {
			//InitParSymbol();
			DWORD address = 0;
			printf("�������ѯ��API��");
			scanf("%x", &address);
			AddressofApi(address);
		}
	}
}

// ��һ���쳣�¼��ַ�
DWORD MasterExceptionHandler::OneofExceptionDispacth(const EXCEPTION_RECORD* ExceptionRecord)
{
	// �����ϵ����û�ȥ
	if (IsCpBp)
	{
		DWORD write = 0;
		IsCpBp = FALSE;
		for (auto&i : m_SaveCpBp)
		{
			WriteProcessMemory(m_hProcess, (PVOID)i.addRess,
				"\xCC", 1, &write);
		}
	}
	// ���������Զϵ�
	PermanentBreakPoint(m_conText.Eip);

	// ��������
	Capstone capstone;
	// ���öϵ��
	BOOL BreakPointFlag = FALSE;
	// 1. ����Ϊ�쳣���δ����
	DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED;
	// 3. �ж�EXCEPTION_DEBUG_EVENT�¼�����ʲô�����쳣
	switch (ExceptionRecord->ExceptionCode)
	{
		// 3.1 �쳣��������/���壩
	case EXCEPTION_BREAKPOINT:
	{
		// ���˵�PEB
		PEBGo();
		if (TRUE == MasterExceptionHandler::m_SystemBreakPointFlag) {
			// ����֮��Ͳ����ǵ�һ��
			MasterExceptionHandler::m_SystemBreakPointFlag = FALSE;
			cout << "����/���壺ϵͳ�ϵ�\n" << endl;
			dwRet = DBG_CONTINUE;
			fprintf(Fopen, "%d %s\n", nCountRow++, "ϵͳ�ϵ��¼�: �ɹ�(Code��673)");
			break;
		}
		for (auto&i : m_SaveBreakPoint)
		{
			// �����쳣
			SIZE_T write = 0;
			if (i.addRess == (DWORD)ExceptionRecord->ExceptionAddress) {
				// ��0xCC�޸���ȥ
				if (!WriteProcessMemory(m_hProcess, (void *)i.addRess, &i.oldData, 1, &write)) {
					ERRORDEBUG_INFO("WriteProcessMemory() failuer!");
					fprintf(Fopen, "%d %s\n", nCountRow++, "�޸�Int 3�����쳣: д��ʧ��(Code��683)");
				}
				// ���õ����ϵ�
				SetBreakPoint();
				// �����쳣 
				m_conText.Eip--;
				BreakPointFlag = TRUE;
				dwRet = DBG_CONTINUE;
			}
		}
		// �ж������ϵ�
		for (auto&i : m_SaveCpBp)
		{
			if (i.addRess = (DWORD)ExceptionRecord->ExceptionAddress)
			{
				// HardBpFlag = TRUE;
				dwRet = DBG_CONTINUE;
				SIZE_T write = 0;
				// ���õ���
				SetBreakPoint();
				IsCpBp = TRUE;
				// �ָ�
				WriteProcessMemory(m_hProcess, (PVOID)i.addRess,
					&i.oldData, 1, &write);
				m_conText.Eip--;
				// isDbgBreakPoint = TRUE;
				if (m_conText.Eax == this->nCount)
				{
					BreakPointFlag = DBG_CONTINUE;
					FlgasFlou = TRUE;
					HardBpFlag = FALSE;
					break;
				}
				else
				{
					return DBG_CONTINUE;
				}
			}

		}
		if (!BreakPointFlag) {
			dwRet = DBG_EXCEPTION_NOT_HANDLED;
		}
	}
	break;
	// �����ڴ�ϵ�
	case EXCEPTION_ACCESS_VIOLATION:
	{
		MemoryExecuteBp();
		dwRet = DBG_CONTINUE;
	}
	break;
	case EXCEPTION_SINGLE_STEP:
		// �ָ�Ӳ�ϵ�
		dwRet = DBG_CONTINUE;
		for (auto&i : m_SaveHardBp)
		{
			// HardBpFlag = FALSE;
			if (i.addRess == (DWORD)ExceptionRecord->ExceptionAddress) {
				m_conText.Dr7 = 0;
				SetThreadContext(m_hThread, &m_conText);
				// ���õ����ϵ�
				SetBreakPoint();
				HardBpFlag = TRUE;
				break;
			}
		}
		// Ӳ����д�ϵ��ж�
		if (HardBpFlag)
			break;
		// �����ϵ��ж�
		if (!IsCpBp)
			return dwRet;
		else
			break;
	default:
		dwRet = DBG_EXCEPTION_NOT_HANDLED;
		break;
	}
	if (FlgasFlou = TRUE) {
		cout << "�쳣�¼�" << endl;
		printf("Eip : %08X\n", m_conText.Eip);
		// ����������Ϣ
		capstone.ShowAssembly((void *)m_conText.Eip, 6);
	}
	UserInfoInput();
	fflush(Fopen);
	return dwRet;
}

// ��ȡϵͳȨ��
void MasterExceptionHandler::GetSystempermissions()
{
	HANDLE hToken = NULL;
	// ��ȡα��������̣߳�
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	// ��Ȩ����
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

// �ڴ�ϵ㴦��
void MasterExceptionHandler::MemoryExecuteBp()
{
	FlgasFlou = FALSE;
	Capstone capstone;
	MemoryBp obj_Memory;
	// �ڶ���ѭ���Ž��뵽���� �ж��Ƿ�����
	for (size_t i = 0; i < m_SaverMemoryBp.size(); ++i)
	{
		// ��ʱ�쳣
		if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0] == 0) {
			if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1] == m_SaverMemoryBp[i].addRess){
				// �ָ��쳣�ϵ�
				VirtualProtect(&m_SaverMemoryBp[i].addRess, 1, m_SaverMemoryBp[i].oldData, &MemorydwSize);
				// ����������Ϣ
				capstone.ShowAssembly((void *)m_conText.Eip, 6);
				FlgasFlou = TRUE;
				// ���õ����ϵ�
				SetBreakPoint();
				break;
			}
		}
		// дʱ�쳣
		if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0] == 1) {
			if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1] == m_SaverMemoryBp[i].addRess) {
				// �ָ��쳣�ϵ�
				VirtualProtect(&m_SaverMemoryBp[i].addRess, 1, m_SaverMemoryBp[i].oldData, &MemorydwSize);
				// ����������Ϣ
				capstone.ShowAssembly((void *)m_conText.Eip, 6);
				FlgasFlou = TRUE;
				// ���õ����ϵ�
				SetBreakPoint();
				break;
			}
		}
		// ִ���쳣
		if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0] == 8) {
			if (DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1] == m_SaverMemoryBp[i].addRess) {
				// �ָ��쳣�ϵ�
				VirtualProtect(&m_SaverMemoryBp[i].addRess, 1, m_SaverMemoryBp[i].oldData, &MemorydwSize);
				// ����������Ϣ
				capstone.ShowAssembly((void *)m_conText.Eip, 6);
				FlgasFlou = TRUE;
				// ���õ����ϵ�
				SetBreakPoint();
				break;
			}
		}
	}
	// �����������������˻ָ��ڴ�ҳ���ԣ�
	obj_Memory.puSetHadnle(m_hProcess);
	obj_Memory.puRestorMempryPro(&MemFlag);
}

/*���Ž���*/
// ��ʼ�����Է���
void MasterExceptionHandler::InitParSymbol()
{
	char ModuleFile[MAX_PATH] = { 0 };
	// ��ʼ������
	SymInitialize(m_hProcess, NULL, FALSE);
	// GetModuleHandle()
}

// APIת��ַ
void MasterExceptionHandler::ApiofAddress(char *ApiName)
{
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;
	// �������ֲ�ѯ
	if (!SymFromName(pRocessInfo.hProcess, ApiName, pSymbol))
		return;
	printf("������ַ�� %U\n", pSymbol->Address);
}

// ��ַתAPI
void MasterExceptionHandler::AddressofApi(DWORD ApiAddress)
{
	DWORD64 dwDisplacement = 0;
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;
	// ���ݵ�ַ��ȡ������Ϣ
	if (!SymFromAddr(m_hProcess, ApiAddress, &dwDisplacement, pSymbol))
		return;
	printf("��������: %s\n", pSymbol->Name);
}

// PEB����
void MasterExceptionHandler::PEBGo()
{
	// ��������PEB(��ȡĿ����̸�����Ϣ)
	PROCESS_BASIC_INFORMATION hProcessDbug = { 0 };
	HANDLE dwProces = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
	// The PebBaseAddress member points to a PEB structure.
	NtQueryInformationProcess(dwProces, ProcessBasicInformation, &hProcessDbug, sizeof(hProcessDbug), NULL);
	// ��ȡPEB��ַ���ڴ���Ϣ
	DWORD dwSize = 0;
	PEB peb = { 0 };
	ReadProcessMemory(dwProces, hProcessDbug.PebBaseAddress, &peb, sizeof(PEB), &dwSize);
	// �õ���PEB��ַ �ѵ���״̬��λ0
	peb.BeingDebugged = 0;
	WriteProcessMemory(dwProces, hProcessDbug.PebBaseAddress, &peb, sizeof(PEB), &dwSize);
	cout << "PEB�Ѿ�����" << endl;
}

// ���������ϵ�int 3
void MasterExceptionHandler::SetCpbp(DWORD address, int Count)
{
	this->nCount = Count;
	size_t Flag = 0;
	// ����ԭ�����ݣ����ݣ�
	BREAKPOINT BreakPoint = { 0 };
	SIZE_T read = 0;
	int ErrorCode = 0;
	BreakPoint.addRess = address;
	ErrorCode = ReadProcessMemory(m_hProcess, (LPVOID)address, &BreakPoint.oldData, 1, &read);
	if (0 >= ErrorCode) {
		ERRORDEBUG_INFO("�����ϵ� ReadProceeMemory() faliuer!");
	}
	ErrorCode = WriteProcessMemory(m_hProcess, (LPVOID)address, "\xCC", 1, &read);
	if (0 >= ErrorCode) {
		ERRORDEBUG_INFO("�����ϵ� WriteProcessMemory() failuer!");
	}
	strcpy(BreakPoint.Type, "�����ϵ�");
	m_SaveCpBp.push_back(BreakPoint);
	fprintf(Fopen, "%d %s\n", nCountRow++, "�����ϵ�ɹ�: �ɹ�(Code��921)");
	fflush(Fopen);
	IsCpBp = FALSE;
}
