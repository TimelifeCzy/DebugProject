#include "Capstone.h"
#include "windows.h"
#include "iostream"

using std::cout; using std::endl;

Capstone::Capstone()
{

}

Capstone::~Capstone()
{

}

// ��ʼ�����������
void Capstone::InitCapstone()
{
	// ���öѿռ�Ļص�����
	OptMem.free = free;
	OptMem.calloc = calloc;
	OptMem.malloc = malloc;
	OptMem.realloc = realloc;
	OptMem.vsnprintf = (cs_vsnprintf_t)vsprintf_s;
	// ע��ѿռ�����麯��
	cs_option(NULL, CS_OPT_MEM, (size_t)&OptMem);
	// ��һ�����
	cs_open(CS_ARCH_X86, CS_MODE_32, &Handle);
}

// �������Ϣ���
void Capstone::ShowAssembly(const void* pAddr, int nLen)
{
	this->InitCapstone();
	// ����OpCode��С ���16�������ָ��			
	BYTE* pOpCode = (BYTE *)malloc(nLen * 16);
	memset(pOpCode, 0, (sizeof(BYTE) * 16 * nLen) );
	SIZE_T read = 0;			
	// �����ָ�����������
	// ������ȡָ��λ���ڴ�Ļ�������Ϣ
	cs_insn* ins = nullptr;

	// ��ȡָ�����ȵ��ڴ�ռ�
	DWORD dwCount = 0;
	ReadProcessMemory(MasterExceptionHandler::m_hProcess, pAddr, pOpCode, nLen * 16, &dwCount);

	int count = cs_disasm(Handle, (uint8_t*)pOpCode, nLen * 16, (uint64_t)pAddr, 0, &ins);

	for (int i = 0; i < nLen; ++i)
	{
		printf("%08X\t", ins[i].address);
		for (uint16_t j = 0; j < 16; ++j)
		{
			if (j < ins[i].size)
				printf("%02X", ins[i].bytes[j]);
			else
				printf(" ");
		}
		printf("\t");
		printf("%s  ", ins[i].mnemonic);
		cout << ins[i].op_str << endl;  
	}
	printf("\n");
	// �ͷŶ�̬����Ŀռ�
	delete[] pOpCode;
	cs_free(ins, count);
}