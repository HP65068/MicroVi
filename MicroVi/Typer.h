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

	void setFocus(); //显示插入符号
	void killFocus(); //隐藏插入符号

	void setCurrentPos(int, int); //设置当前插入符号位置
	void setClientSize(int, int); //设置客户区大小

	//获取参数
	int getCxChar() { return this->cxChar; }
	int getCyChar() { return this->cyChar; }
	int getCxBuffer() { return this->cxBuffer; }
	int getCyBuffer() { return this->cyBuffer; }
	int getXCurrent() { return this->xCurrent; }
	int getYCurrent() { return this->yCurrent; }
	int getXClient() { return this->xClient; }
	int getYClient() { return this->yClient; }

	void writeBuffer(char); //写入缓冲区
	TCHAR readBuffer(int, int); //读取缓冲区

	void deleteChar(); //删除字符

private:
	vector<char> c_buffer; //字符缓冲区
	HWND m_hWnd;
	int cxChar; //字符宽度
	int cyChar; //字符高度
	int cxBuffer; //x字符数
	int cyBuffer; //y字符数
	int xCurrent; //x位置
	int yCurrent; //y位置
	int xClient; //客户区宽度
	int yClient; //客户区高度
};

