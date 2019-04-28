#ifndef _MODULEENTRY_H_
#define _MODULEENTRY_H_
#include <Windows.h>
#include <atlstr.h>

class ModuleEntry
{
public:
	ModuleEntry();
	virtual ~ModuleEntry();

public:
	void puShowProcessModule(){ this->ShowProcessModule(); }
	void SetProcessHandle(const DWORD processid){ this->SaveMasterProessId = processid; }

private:
	// ��ʾ����ģ��
	BOOL ShowProcessModule();
	// ������̾��
	DWORD SaveMasterProessId = 0;
	// �����ļ��汾��Ϣ
	char VersionInfobuf[MAX_PATH];
	// �����ļ��汾��Ϣ
	char* pVerValue = NULL;
	// ��������
	CString strSubBlock, strTranslation, strTemp;
};


#endif