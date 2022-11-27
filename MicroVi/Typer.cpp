#include "Typer.h"

Typer::Typer()
{
	m_hWnd = nullptr;
	cxChar = 0;
	cyChar = 0;
	cxBuffer = 0;
	cyBuffer = 0;
	xCurrent = 0;
	yCurrent = 0;
	xClient = 0;
	yClient = 0;
}

Typer::~Typer()
{

}

void Typer::init(HWND hWnd)
{
	this->m_hWnd = hWnd;

	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	this->xClient = rect.bottom - rect.top;
	this->yClient = rect.right - rect.left;

	HDC hdc = GetDC(m_hWnd);
	TEXTMETRIC tm;
	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
	GetTextMetrics(hdc, &tm);
	this->cxChar = tm.tmAveCharWidth;
	this->cyChar = tm.tmHeight;
	ReleaseDC(m_hWnd, hdc);

	this->cxBuffer = max(1, this->xClient / this->cxChar);
	this->cyBuffer = max(1, this->yClient / this->cyChar);
}

void Typer::setFocus()
{
	CreateCaret(m_hWnd, NULL, cxChar, cyChar);
	SetCaretPos(xCurrent * cxChar, yCurrent * cyChar);
	ShowCaret(m_hWnd);
}

void Typer::killFocus()
{
	HideCaret(m_hWnd);
	DestroyCaret();
}

void Typer::setCurrentPos(int x, int y)
{
	this->xCurrent = (x == -1 ? this->xCurrent : x);
	this->yCurrent = (y == -1 ? this->yCurrent : y);
	if (this->xCurrent < 0)
	{
		this->yCurrent -= 1;
		this->xCurrent = this->cxBuffer - 1;
	}
	else if (this->xCurrent >= this->cxBuffer)
	{
		if (this->yCurrent + 1 < this->cyBuffer)
		{
			this->xCurrent = 0;
			this->yCurrent++;
		}
		else
		{
			this->xCurrent = this->cxBuffer - 1;
		}
	}
	this->yCurrent = max(this->yCurrent, 0);
	this->yCurrent = min(this->yCurrent, this->cyBuffer - 1);
}

void Typer::setClientSize(int x, int y)
{
	//设置当前位置
	int pos = this->yCurrent * this->cxBuffer + this->xCurrent;

	this->xClient = (x == -1 ? this->xClient : x);
	this->yClient = (y == -1 ? this->yClient : y);

	this->cxBuffer = max(1, this->xClient / this->cxChar);
	this->cyBuffer = max(1, this->yClient / this->cyChar);

	/*if (pos > this->cxBuffer * cyBuffer)
	{
		this->xCurrent = this->cxBuffer;
		this->yCurrent = this->cyBuffer;
	}*/
	/*else
	{
		this->xCurrent = pos % this->cxBuffer;
		this->yCurrent = pos / this->cxBuffer;
	}*/

	if (this->c_buffer.empty())
	{
		//this->c_buffer.clear();
		this->c_buffer.insert(c_buffer.begin(), cxBuffer * cyBuffer, ' ');
	}
}

void Typer::writeBuffer(char c)
{
	int size = cxBuffer * yCurrent + xCurrent;
	if (size >= c_buffer.size())
	{
		c_buffer.insert(c_buffer.end(), size - c_buffer.size() + 1, ' ');
	}
	this->c_buffer[size] = c;
}

TCHAR Typer::readBuffer(int x,int y)
{
	int size = cxBuffer * y + x;
	if (size >= c_buffer.size())
	{
		c_buffer.insert(c_buffer.end(), size - c_buffer.size() + 1, ' ');
	}
	wchar_t ch = this->c_buffer.at(size);
	return TCHAR(ch);
}

void Typer::deleteChar()
{
	int pos = this->yCurrent * this->cxBuffer + this->xCurrent - 1;
	pos = max(pos, 0);
	this->c_buffer.erase(c_buffer.begin() + pos);
	this->setCurrentPos(this->xCurrent - 1, -1);
	InvalidateRect(m_hWnd, nullptr, true);
	UpdateWindow(m_hWnd);
}
