#include "UKeystone.h"
#include "iostream"
#include "XEDParse/XEDParse.h"
#pragma comment(lib, "XEDParse\\x86\\XEDParse_x86.lib")

using std::cout; using std::endl;

KeysTone::KeysTone()
{

}

KeysTone::~KeysTone()
{

}

// ��ʼ��Assembly
void KeysTone::InitAssembly()
{
	// ��ʼ��ks_open
	if (KS_ERR_OK != ks_open(KS_ARCH_X86, KS_MODE_32, &pengine)) {
		cout << "ks_open failuer()";
		return;
	}
}

// �޸Ļ��ָ���Դ��
BOOL KeysTone::ModifyOpCode(const HANDLE hthread, const DWORD MemoryAddress, const char *ModifyData)
{
	this->InitAssembly();
	nRet = ks_asm(
		pengine,					/* �����������ͨ��ks_open�����õ�*/
		ModifyData,					/* Ҫת���Ļ��ָ��*/
		NULL,						/* ���ָ�����ڵĵ�ַ*/
		&opcode,					/* �����opcode*/
		&nOpCcodeSize,				/* �����opcode���ֽ���*/
		&stat_count					/* ����ɹ�����ָ�������*/
		);

	// 1. ����ֵ����-1ʱ��������
	if (nRet == -1)
	{
		// ���������Ϣ
		// ks_errno ��ô�����
		// ks_strerror ��������ת�����ַ���������������ַ���
		printf("������Ϣ��%s\n", ks_strerror(ks_errno(pengine)));
		return FALSE;
	}
	DWORD dwSize = 0;
	DWORD dwAtt = 0;
	// 2. ��ֵ����
	SavaMemoryAdd = MemoryAddress;
	// 3. �޸��ڴ��ַ����
	VirtualProtectEx(MasterBpHandle, (LPVOID)MemoryAddress, 16, PAGE_READWRITE, &dwAtt);
	// 4. ����ԭ���Ļ��OpCode
	ReadProcessMemory(MasterBpHandle, (LPVOID)MemoryAddress, Oldopcode, 16, &read);
	// 5. д���µĻ��OpCode
	if (!WriteProcessMemory(MasterBpHandle, (LPVOID)MemoryAddress, opcode, nOpCcodeSize, &dwSize)){
		printf("д����ָ��ʧ�ܣ��ڴ������޸ģ�Ȩ�޲���\n");
		return FALSE;
	}
	// 6. �ָ�ԭ������
	VirtualProtectEx(MasterBpHandle, (LPVOID)MemoryAddress, 16, dwAtt, &dwAtt);
	// 7. �ͷſռ�
	ks_free(opcode);
	// 8. �رվ��
	ks_close(pengine);
	return TRUE;
}

// �ָ�ԭ������
BOOL KeysTone::AttData()
{
	DWORD dwSize = 0;
	if (!WriteProcessMemory(MasterBpHandle, (LPVOID)SavaMemoryAdd, &Oldopcode, read, &dwSize)){
		printf("д����ָ��ʧ�ܣ� ��дȨ���Ѹ��ģ�Ȩ�޲���");
		return FALSE;
	}
	printf("�ѳ����޸�����");
	return TRUE;
}