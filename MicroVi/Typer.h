#pragma once

#include <Windows.h>
#include <vector>

using std::vector;

class Typer
{
public:
	Typer();
	~Typer();

	void init(HWND);

	void setFocus(); //��ʾ�������
	void killFocus(); //���ز������

	void setCurrentPos(int, int); //���õ�ǰ�������λ��
	void setClientSize(int, int); //���ÿͻ�����С

	//��ȡ����
	int getCxChar() { return this->cxChar; }
	int getCyChar() { return this->cyChar; }
	int getCxBuffer() { return this->cxBuffer; }
	int getCyBuffer() { return this->cyBuffer; }
	int getXCurrent() { return this->xCurrent; }
	int getYCurrent() { return this->yCurrent; }
	int getXClient() { return this->xClient; }
	int getYClient() { return this->yClient; }

	void writeBuffer(char); //д�뻺����
	TCHAR readBuffer(int, int); //��ȡ������

	void deleteChar(); //ɾ���ַ�

private:
	vector<char> c_buffer; //�ַ�������
	HWND m_hWnd;
	int cxChar; //�ַ����
	int cyChar; //�ַ��߶�
	int cxBuffer; //x�ַ���
	int cyBuffer; //y�ַ���
	int xCurrent; //xλ��
	int yCurrent; //yλ��
	int xClient; //�ͻ������
	int yClient; //�ͻ����߶�
};

