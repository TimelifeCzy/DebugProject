#pragma once
#ifndef _COMMANDHELPDOCUMENT_H_
#define _COMMANDHELPDOCUMENT_H_
#include <vector>
#include <string>


using std::vector;	using std::string;

class ComHelpDocument
{
public:
	ComHelpDocument();
	virtual ~ComHelpDocument();

public:
	// ��������ĵ���ʾ
	void puShowComandHelp() { this->ShowComandHelp(); }

private:
	// �������
	void ComandManage();
	// ��������ĵ���ʾ
	void ShowComandHelp();
	// ����Ԥ���ӿ�(��ɾ�Ĳ�)

private:
	vector<string> CmdManage;
};

#endif