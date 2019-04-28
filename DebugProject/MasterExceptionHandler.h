#pragma once
#ifndef _MASTEREXCEPTIONHANDLER_H_
#define _MASTEREXCEPTIONHANDLER_H_
#include <windows.h>
#include <vector>
#include <string>

using std::vector; using std::string;

class MasterExceptionHandler
{
public:
	MasterExceptionHandler();
	virtual ~MasterExceptionHandler();

// public  method interface
public:
	// ��Ӧ.exe��ק��Ϣ�����ӿ�
	void ResponsetoDragMessages(){ this->ResponsetoDragMessage(); }
	// ���̾��
	static HANDLE m_hProcess;

// private function method
private:
	// ����ԭ���ݽṹ�壬Int3��ԭʹ��
	typedef struct _BREAKPOINT
	{
		DWORD addRess;
		BYTE oldData;
		char Type[20];
		bool operator ==(_BREAKPOINT obj)
		{
			return obj.addRess == this->addRess;
		}
	}BREAKPOINT, *PBREAKPOINT;


	// ��Ӧ.exe��ק��Ϣ
	INT ResponsetoDragMessage();
	// ��һ���ж������쳣
	DWORD OneofExceptionDispacth(const EXCEPTION_RECORD* ExceptionRecord);
	// �û�������Ϣ����
	void UserInfoInput();
	// ���õ���ѭ��
	void SetBreakPoint();
	// ��������
	void StepOverBp();
	// ����int3�ж�0XCC
	void SetInt3BreakPoint(DWORD address);
	// �޸Ļ�����
	void SetAssemblyCode(const DWORD address, const char* ModifyData);
	// �鿴��ǰ�ϵ�(��Ӳ)
	void ShowCurrBreakPoint();
	// �������뼰�����������ָ�0xCC��
	void PermanentBreakPoint(DWORD address);
	// ɾ�������Զϵ�
	void DeletePermanentBp(DWORD address);
	// ��ȡϵͳȨ��
	void GetSystempermissions();
	// ��ȡ�����볤��
	short OpCodeLen(DWORD address);
	// �����ڴ�ϵ�
	void MemoryExecuteBp();
	// �ڴ洦������
	DWORD MemorydwSize = 0;
	// ��ʼ������
	void InitParSymbol();
	// APIת��ַ
	void ApiofAddress(char *ApiName);
	// ��ַתAPI
	void AddressofApi(DWORD ApiAddress);
	// PEB����
	void MasterExceptionHandler::PEBGo();
	// ���������ϵ�
	void MasterExceptionHandler::SetCpbp(DWORD address, int Count);

// ��������
private:
	// ��ִ���ļ�·����Ϣ
	char m_Executable[MAX_PATH];
	// �߳̾��
	HANDLE m_hThread = 0;
	// �̻߳�����
	CONTEXT m_conText;
	// �����쳣�����־���
	DWORD m_dExceptionHandleFlag = DBG_CONTINUE;
	// �޸ļĴ������ֱ���
	char regName[MAX_PATH];
	// �ж��ǲ��ǵ�һ�μ��س���
	static BOOL IsOneLoadProcess;
	// ������һ�������¼�
	DEBUG_EVENT DebugEvent;
	// �ж��Ƿ�ϵͳ�ϵ�
	static BOOL m_SystemBreakPointFlag;
	// ���뵼�����ʼ�����
	BOOL IsInitTabFlag = TRUE;
	// �����ļ����
	HANDLE Filehandle = 0;
	// ���ػ�ַ
	HANDLE Imagebase = 0;
	// �ж��Ƿ���OEP
	BOOL OEPflag = TRUE;
	// װ���쳣�ϵ�������int 3��ϵ㣩
	vector<BREAKPOINT>  m_SaveBreakPoint, m_SaveHardBp,m_SaverMemoryBp, m_SaveCpBp;
	// ֻ����һ���ڴ�ϵ�
	BOOL MemoryBpFlag = TRUE;
	// ����������Ϣ(�ڴ����)
	BOOL FlgasFlou = FALSE;
	// ������ַ����
	DWORD m_StartAddr;
	// �����ϵ��ж�����
	INT nCount = 0;
	// �ڴ��������ж�
	static BOOL MemFlag;
	// �ļ�ָ��
	FILE *Fopen;
	// ��������ָ��
	static int nCountRow;
	// �����ϵ�����
	BOOL IsCpBp = FALSE;
	// ʵ��Ӳ������
	BOOL HardBpFlag = FALSE;
	// �����߳̽ṹ��
	PROCESS_INFORMATION pRocessInfo;

};

#endif