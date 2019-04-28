#include "CommandHelpDocument.h"
#include "UWindows.h"
#include <iostream>
#include <string>

using std::cout; using std::endl;

ComHelpDocument::ComHelpDocument()
{

}

ComHelpDocument::~ComHelpDocument()
{

}

// �������
void ComHelpDocument::ComandManage()
{
	cout << "���\t    ";
	cout << "˵����\t\t\t";
	cout << "���䣺" << endl;
	string strName;
	strName = "1.  t       ";
	strName = strName + "    �����ж�            ";
	strName = strName + "\t    TFλ��1";
	CmdManage.push_back(strName);
	strName = "2.  g       ";
	strName = strName + "    ����                ";
	strName = strName + "\t    δ����1";
	CmdManage.push_back(strName);
	strName = "3.  bp      ";
	strName = strName + "    ���öϵ�            ";
	strName = strName + "\t    TFint3 �ϵ� 0xCC";
	CmdManage.push_back(strName);
	strName = "4.  md      ";
	strName = strName + "    �޸Ļ��ָ��        ";
	strName = strName + "\t    address / ModifyData�������������޸ĵĵ�ַ �� �޸Ļ��ָ��";
	CmdManage.push_back(strName);
	strName = "5.  help    ";
	strName = strName + "    �������ѯ          ";
	strName = strName + "\t    �оٵ�ǰ��ָ����Ϣ�����ܽ���";
	CmdManage.push_back(strName);
	strName = "6.  rmodify ";
	strName = strName + "    �޸ļĴ���          ";
	strName = strName + "\t    ��ʽ: rmodify ESP 0x00000000 ���ƴ�д";
	CmdManage.push_back(strName);
	strName = "7.  u       ";
	strName = strName + "    �鿴���ָ��        ";
	strName = strName + "\t    �鿴��ǰECS:EIPָ��Ϊ��ʼ�Ļ���ָ�Ĭ��6����";
	CmdManage.push_back(strName);
	strName = "8.  stack   ";
	strName = strName + "    ��ȡջ��Ϣ          ";
	strName = strName + "\t    �鿴��ǰESS:EIPָ��Ϊ��ʼ��ջ�ڴ���Ϣ��Ĭ��10����";
	CmdManage.push_back(strName);
	strName = "9.  d       ";
	strName = strName + "    �������ڴ����ݻ�ȡ   ";
	strName = strName + "\t    ��ʽ:d 0x000000��Ĭ��10����";
	CmdManage.push_back(strName);
	strName = "10. dd      ";
	strName = strName + "    ��ȡĬ���ڴ�����     ";
	strName = strName + "\t    �鿴��ǰEDS:EIPָ��Ϊ��ʼ�������ڴ���Ϣ��Ĭ��10����";
	CmdManage.push_back(strName);
	strName = "11. dmodify ";
	strName = strName + "    �޸��ڴ�����         ";
	strName = strName + "\t    �޸�ָ���ĵ�ַ�ڴ�����";
	CmdManage.push_back(strName);
	strName = "12. export  ";
	strName = strName + "    ������               ";
	strName = strName + "\t    �鿴��ǰ���̵�����";
	CmdManage.push_back(strName);
	strName = "13. import  ";
	strName = strName + "    �����               ";
	strName = strName + "\t    �鿴��ǰ���̵����";
	CmdManage.push_back(strName);
	strName = "14. reload  ";
	strName = strName + "    ���س���             ";
	strName = strName + "\t    ���¼��س���";
	CmdManage.push_back(strName);
	strName = "15. bl      ";
	strName = strName + "    �ϵ�鿴             ";
	strName = strName + "\t    ��Ӳ��ϵͳ�ϵ��б�";
	CmdManage.push_back(strName);	
	strName = "16. hwxbp   ";
	strName = strName + "    Ӳ�ϵ㣨ִ�У�       ";
	strName = strName + "\t    Ӳ��ִ�жϵ�";
	CmdManage.push_back(strName);
	strName = "17. hwrwbp  ";
	strName = strName + "    Ӳ�ϵ㣨��д��       ";
	strName = strName + "\t    ����1. ��  ����2. д ��ʽ��(hwrwbp 0x00000000 Type Len)";
	CmdManage.push_back(strName);
	strName = "18. ml      ";
	strName = strName + "    ģ��鿴             ";
	strName = strName + "\t    �鿴�����Գ������ģ��";
	CmdManage.push_back(strName);
	strName = "19. mu      ";
	strName = strName + "    ���ָ���          ";
	strName = strName + "\t    ֻ�ܳ�����һ�ε��޸�ָ��";
	CmdManage.push_back(strName);
	strName = "20. memorybp";
	strName = strName + "    �ڴ�ϵ�             ";
	strName = strName + "\t    ����0. ��	����1. д ����8. ��ִ�� ��ʽ��(memorybp 0x00000000 Type)";
	CmdManage.push_back(strName);

	

}

// ��ʾ��������ĵ�
void ComHelpDocument::ShowComandHelp()
{
	// ��ʼ����������
	ComandManage();
	// ������ӡ���� ���� ˵��
	for (size_t i = 0; i < CmdManage.size(); ++i)
	{
		cout << CmdManage[i] << endl;
	}
}