#include <iostream>

using std::cout; using std::endl;

// ������������
void SetpOver()
{
	printf("��������");
}

int main(int argc, char** argv)
{
	// ������������
	SetpOver();
	// ��д�ϵ����
	static int b = 10;
	b += 10;
	// ����ϵ���ԣ������ԣ�
	for (int i = 0; i < 10; i++)
	{
		printf("���Զϵ�\n");
	}
	// �ڴ��д����
	system("pause");
	return 0;
}