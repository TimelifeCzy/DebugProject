#include "ParsingSymbolTab.h"
#include <DbgHelp.h>
#pragma comment (lib, "Dbghelp.lib")

ParSymbol::ParSymbol()
{
	
}

ParSymbol::~ParSymbol()
{
	 
}

// ��ʼ�����Է���
void ParSymbol::InitParSymbol()
{
	// ��ʼ������
	SymInitialize(ObjectProcess, NULL , FALSE);
	// ��ʼ���ṹ����Ϣ
}


// ��ʾ����
void ParSymbol::ShowParSymbol()
{

}