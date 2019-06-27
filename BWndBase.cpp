//////////////////////////////////////////////////////////////////////
// BWndBase.cpp: CBWndBase 类的实现
// 包含常规窗口功能，作为 CBForm、CBControl 类的基类
//   
//////////////////////////////////////////////////////////////////////

#include "BWndBase.h"

CBWndBase::CBWndBase( HWND hWndToManage /*= NULL*/, 
					  HWND hWndParentForm /*= NULL */ )
{
	m_hWnd = hWndToManage;
	m_hWndDlg = hWndParentForm;	

	HM.ZeroMem(m_ClassName, sizeof(m_ClassName));
	m_atom = 0;
}


CBWndBase::~CBWndBase()
{
	// 禁止一切释放系统资源操作，因 CBControl 类继承本类时，
	//   CBControl 是动态绑定的，对象被卸载时，对应的控件不一定被卸载
	
}



// 设置和返回 Enabled
void CBWndBase::EnabledSet( bool enabledValue ) const
{
	EnableWindow(m_hWnd, (BOOL)enabledValue);
}

bool CBWndBase::Enabled() const
{
	if (IsWindowEnabled(m_hWnd)) return true; else return false;
}

// 设置和返回 Visible
void CBWndBase::VisibleSet( bool visibleValue ) const
{
	if (visibleValue)
		ShowWindow(m_hWnd, SW_SHOWNA);
	else
		ShowWindow(m_hWnd, SW_HIDE);
}

bool CBWndBase::Visible() const
{
	if (IsWindowVisible(m_hWnd)) return true; else return false;
}


bool CBWndBase::HScrollBar()
{
	if (Style() & WS_HSCROLL) return true; else return false;
}

void CBWndBase::HScrollBarSet(bool bValue)
{
	if (bValue) 
		StyleSet(WS_HSCROLL);
	else
		StyleSet(WS_HSCROLL, -1);
	Refresh();
}

bool CBWndBase::VScrollBar()
{
	if (Style() & WS_VSCROLL) return true; else return false;
}

void CBWndBase::VScrollBarSet(bool bValue)
{
	if (bValue) 
		StyleSet(WS_VSCROLL);
	else
		StyleSet(WS_VSCROLL, -1);
	Refresh();
}




bool CBWndBase::TabStop()
{
	if (Style() & WS_TABSTOP) return true; else return false;
}

void CBWndBase::TabStopSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_TABSTOP);
	else
		StyleSet(WS_TABSTOP, -1);
	Refresh();
}


bool CBWndBase::Group()
{
	if (Style() & WS_GROUP) return true; else return false;
}

void CBWndBase::GroupSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_GROUP);
	else
		StyleSet(WS_GROUP, -1);
	Refresh();
}


// ================================================
// 位置大小
// ================================================

// 控件大小和位置
int CBWndBase::Left() const
{
	RECT rect;
	POINT pt;
	if (GetWindowRect(m_hWnd, &rect)) 
	{
		pt.x = rect.left; pt.y = rect.top; 
		// 若 m_hWndDlg 不为 0，说明是控件，转换为窗体坐标系
		// 若 m_hWndDlg 为 0，说明是窗体，不转换直接返回
		if (m_hWndDlg) ScreenToClient(m_hWndDlg, &pt);	
		return pt.x;
	}
	else
	{
		return 0;
	}
}

int CBWndBase::Top() const
{
	RECT rect;
	POINT pt;
	if (GetWindowRect(m_hWnd, &rect)) 
	{
		pt.x = rect.left; pt.y = rect.top; 
		// 若 m_hWndDlg 不为 0，说明是控件，转换为窗体坐标系
		// 若 m_hWndDlg 为 0，说明是窗体，不转换直接返回
		if (m_hWndDlg) ScreenToClient(m_hWndDlg, &pt);	
		return pt.y;
	}
	else
	{
		return 0;
	}
}

int CBWndBase::Width() const
{
	RECT rect;
	POINT pt1, pt2;
	if (GetWindowRect(m_hWnd, &rect)) 
	{
		pt1.x = rect.left; pt1.y = rect.top; 
		pt2.x = rect.right; pt2.y = rect.bottom; 
		// 若 m_hWndDlg 不为 0，说明是控件，转换为窗体坐标系
		// 若 m_hWndDlg 为 0，说明是窗体，不转换直接返回
		if (m_hWndDlg)
		{
			ScreenToClient(m_hWndDlg, &pt1);	
			ScreenToClient(m_hWndDlg, &pt2);
		}
		return pt2.x - pt1.x;
	}
	else
	{
		return 0;
	}
}

int CBWndBase::Height() const
{
	RECT rect;
	POINT pt1, pt2;
	if (GetWindowRect(m_hWnd, &rect)) 
	{
		pt1.x = rect.left; pt1.y = rect.top; 
		pt2.x = rect.right; pt2.y = rect.bottom; 
		// 若 m_hWndDlg 不为 0，说明是控件，转换为窗体坐标系
		// 若 m_hWndDlg 为 0，说明是窗体，不转换直接返回
		if (m_hWndDlg)
		{
			ScreenToClient(m_hWndDlg, &pt1);	
			ScreenToClient(m_hWndDlg, &pt2);
		}
		return pt2.y - pt1.y;
	}
	else
	{
		return 0;
	}

}


int CBWndBase::ClientWidth() const
{
	RECT rect;
	if (GetClientRect(m_hWnd, &rect))
		return rect.right - rect.left;
	else
		return 0;
}

int CBWndBase::ClientHeight() const
{
	RECT rect;
	if (GetClientRect(m_hWnd, &rect))
		return rect.bottom - rect.top;
	else
		return 0;	
}


void CBWndBase::LeftSet(int left) const
{
	Move(left);
}

void CBWndBase::TopSet(int top) const
{
	Move(0x7FFFFFFF, top);
}

void CBWndBase::WidthSet(int width) const
{
	Move(0x7FFFFFFF, 0x7FFFFFFF, width);
}

void CBWndBase::HeightSet(int height) const
{
	Move(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, height);
}

void CBWndBase::Move( int left/*=0x7FFFFFFF*/, 
					  int top/*=0x7FFFFFFF*/, 
					  int width/*=0x7FFFFFFF*/, 
					  int height/*=0x7FFFFFFF*/ ) const
{
	RECT rect;
	POINT pt1, pt2;
	if (! GetWindowRect(m_hWnd, &rect)) 
	{
		// 不能获得窗口当前大小，为容错，设置为一个默认大小
		rect.left=0; rect.top=0;
		rect.right=100; rect.bottom=100;
	}
	pt1.x = rect.left; pt1.y = rect.top; 
	pt2.x = rect.right; pt2.y = rect.bottom; 

	// 若 m_hWndDlg 不为 0，说明是控件，转换为窗体坐标系
	// 若 m_hWndDlg 为 0，说明是窗体，不转换直接返回
	if (m_hWndDlg)
	{
		ScreenToClient(m_hWndDlg, &pt1);	
		ScreenToClient(m_hWndDlg, &pt2);
	}

	// 设置新的位置和大小 => rect
	if (left!=0x7FFFFFFF) { pt2.x += (left-pt1.x); pt1.x = left;  }
	if (top!=0x7FFFFFFF) { pt2.y += (top-pt1.y); pt1.y = top; }
	if (width!=0x7FFFFFFF) pt2.x = pt1.x + width;
	if (height!=0x7FFFFFFF) pt2.y = pt1.y + height;
	
	// 移动控件位置和调整大小
	MoveWindow(m_hWnd, pt1.x, pt1.y, 
		pt2.x-pt1.x, pt2.y-pt1.y, 1);

	InvalidateRect(m_hWnd, NULL, 0);
}



// ================================================
// 文本
// ================================================

// 直接设置文本（有各种重载版本）
void CBWndBase::TextSet( LPCTSTR newText ) const
{
	SetWindowText(m_hWnd, newText);
}
void CBWndBase::TextSet(char valueChar) const
{  
	LPTSTR buff=Str(valueChar);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(unsigned short int valueInt) const	// TCHAR
{
	LPTSTR buff=Str(valueInt);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(int valueInt) const
{ 
	LPTSTR buff=Str(valueInt);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}	
void CBWndBase::TextSet(unsigned int valueInt) const
{ 
	LPTSTR buff=Str(valueInt);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(unsigned long valueInt) const
{ 
	LPTSTR buff=Str(valueInt);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(float valueSng) const
{
	LPTSTR buff=Str(valueSng);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(double valueDbl) const
{
	LPTSTR buff=Str(valueDbl);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextSet(long double valueDbl) const
{
	LPTSTR buff=Str(valueDbl);
	TextSet(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}

// 现有文本基础上添加文本和设置（有各种重载版本）
void CBWndBase::TextAdd( LPCTSTR newText ) const
{
	int lenText;
	lenText=GetWindowTextLength(m_hWnd);
		
	TCHAR * pText=new TCHAR[lenText+lstrlen(newText)+1];
	GetWindowText(m_hWnd, pText, lenText+1);
	_tcscat(pText, newText);

	SetWindowText(m_hWnd, pText);
	delete []pText;
}
void CBWndBase::TextAdd(char valueChar) const
{
	LPTSTR buff=Str(valueChar);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(unsigned short int valueInt) const
{
	LPTSTR buff=Str(valueInt);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(int valueInt) const
{
	LPTSTR buff=Str(valueInt);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(unsigned int valueInt) const
{
	LPTSTR buff=Str(valueInt);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(unsigned long valueInt) const
{
	LPTSTR buff=Str(valueInt);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(float valueSng) const
{
	LPTSTR buff=Str(valueSng);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(double valueDbl) const
{
	LPTSTR buff=Str(valueDbl);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}
void CBWndBase::TextAdd(long double valueDbl) const
{
	LPTSTR buff=Str(valueDbl);
	TextAdd(buff);
	HM.Free(buff);	// 可及时释放 buff 的空间
}


// 获得文本（可获得字符串的，以及转换为整型、double型的）
LPTSTR CBWndBase::Text() const
{
	int lenText;
	lenText=GetWindowTextLength(m_hWnd);

	TCHAR * pText=new TCHAR[lenText+1];
	HM.AddPtr(pText);

	GetWindowText(m_hWnd, pText, lenText+1);
	return pText;
}

double CBWndBase::TextVal() const
{
	int lenText;
	double dblRet=0.0;
	lenText=GetWindowTextLength(m_hWnd);

	TCHAR * pText=new TCHAR[lenText+1];
	GetWindowText(m_hWnd, pText, lenText+1);
	
	dblRet = Val(pText);
	delete []pText;
	return dblRet;
}



// ================================================
// 字体
// ================================================



LPTSTR CBWndBase::FontName()
{
	LOGFONT lf;  GetFontMLF(&lf);
	TCHAR * pStr = new TCHAR [lstrlen(lf.lfFaceName) + 1];
	HM.AddPtr(pStr);
	_tcscpy(pStr, lf.lfFaceName);
	return pStr;
}

void CBWndBase::FontNameSet( LPCTSTR szFontName )
{
	LOGFONT lf;  GetFontMLF(&lf);

	if ( lstrlen(szFontName) > LF_FACESIZE )
	{
		// 若字体名字符串过长，截断之
		_tcsncpy(lf.lfFaceName, szFontName, LF_FACESIZE-1 );
		*(lf.lfFaceName + LF_FACESIZE - 1) = 0;	
	}
	else
	{
		_tcscpy( lf.lfFaceName, szFontName );
	}

	SetFontMLF(&lf);
}

float CBWndBase::FontSize()
{
	LOGFONT lf;  GetFontMLF(&lf);

	HDC hDC=GetDC(m_hWnd);
	float fheight =
		(float)(lf.lfHeight * (-72.0) / GetDeviceCaps(hDC, LOGPIXELSY));
	fheight = (float)((int)(fheight * 100 + 0.5)/100.0);	// 保留 2 位小数
	ReleaseDC(m_hWnd, hDC);

	return fheight;
}

void CBWndBase::FontSizeSet( float fSize )
{
	LOGFONT lf;  GetFontMLF(&lf);

	HDC hDC=GetDC(m_hWnd);
	float fheight = 
		(float)(- fSize * GetDeviceCaps(hDC, LOGPIXELSY) / 72.0);
	lf.lfHeight = (long)(fheight+0.5);	// 四舍五入 fheight
	ReleaseDC(m_hWnd, hDC);

	SetFontMLF(&lf);
}

bool CBWndBase::FontBold()
{
	LOGFONT lf;  GetFontMLF(&lf);
	return (lf.lfWeight >= 700);
}

void CBWndBase::FontBoldSet( bool value )
{
	LOGFONT lf;  GetFontMLF(&lf);

	if (value)
		lf.lfWeight = 700;
	else
		lf.lfWeight = 400;

	SetFontMLF(&lf);
}

bool CBWndBase::FontUnderline()
{
	LOGFONT lf;  GetFontMLF(&lf);
	return (lf.lfUnderline != 0);
}

void CBWndBase::FontUnderlineSet( bool value )
{
	LOGFONT lf;  GetFontMLF(&lf);

	if (value)
		lf.lfUnderline = 1;
	else
		lf.lfUnderline = 0;

	SetFontMLF(&lf);
}

bool CBWndBase::FontItalic()
{
	LOGFONT lf;  GetFontMLF(&lf);
	return (lf.lfItalic != 0);	
}

void CBWndBase::FontItalicSet( bool value )
{
	LOGFONT lf;  GetFontMLF(&lf);
	
	if (value)
		lf.lfItalic = 1;
	else
		lf.lfItalic = 0;

	SetFontMLF(&lf);
}

float CBWndBase::FontRotateDegree()
{
	LOGFONT lf;  GetFontMLF(&lf);
	return (float)(lf.lfEscapement / 10);
}

void CBWndBase::FontRotateDegreeSet( float fDegree )
{
	LOGFONT lf;  GetFontMLF(&lf);
	lf.lfEscapement = (long)(fDegree * 10+0.5);
	SetFontMLF(&lf);
}

BYTE CBWndBase::FontCharSet()
{
	LOGFONT lf;  GetFontMLF(&lf);
	return lf.lfCharSet;
}

void CBWndBase::FontCharSetSet( BYTE ucValue )
{
	LOGFONT lf;  GetFontMLF(&lf);
	lf.lfCharSet = ucValue;
	SetFontMLF(&lf);
}


HFONT CBWndBase::hFont()
{
	return GetFontMLF(NULL);
}


// protected:
HFONT CBWndBase::GetFontMLF( LOGFONT * lpLf /*=NULL*/ )
{
	HFONT hFont = (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0);
	if (lpLf==0) return hFont;	// 若 lpLf 为空指针，直接返回字体句柄，不获取字体信息
	GetObject( (HGDIOBJ)hFont, sizeof(LOGFONT), lpLf );
	return hFont;
}

HFONT CBWndBase::SetFontMLF( LOGFONT * lpLf )
{
	// 仅实现创建新字体、将窗口设置为使用新字体的功能
	// 原来的 hFont 是有继承类（CBForm、CBControl）记录的，将由
	// 继承类（CBForm、CBControl）的本函数的重载函数负责 DeleteObject
	// 本函数不管 DeleteObject 旧字体
	HFONT hFont = CreateFontIndirect(lpLf);
	// causes the control to redraw itself immediately upon setting the font
	LRESULT l = SendMessage (m_hWnd, WM_SETFONT, (WPARAM)hFont, 1);
	HFONT hFont2 = (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0);
	// 返回新字体对象句柄
	return hFont;
}






// ================================================
// 边框
// ================================================

bool CBWndBase::Border()
{
	if (Style() & WS_BORDER) return true; else return false;
}

void CBWndBase::BorderSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_BORDER);
	else
		StyleSet(WS_BORDER, -1);
	Refresh();
}

bool CBWndBase::BorderFrameDlg()
{
	if (Style() & WS_DLGFRAME) return true; else return false;
}

void CBWndBase::BorderFrameDlgSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_DLGFRAME);
	else
		StyleSet(WS_DLGFRAME, -1);
	Refresh();
}

bool CBWndBase::BorderFrameThick()
{
	if (Style() & WS_THICKFRAME) return true; else return false;
}

void CBWndBase::BorderFrameThickSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_THICKFRAME);
	else
		StyleSet(WS_THICKFRAME, -1);
	Refresh();
}

bool CBWndBase::BorderRaised()
{
	if (StyleEx() & WS_EX_WINDOWEDGE) return true; else return false;
}

void CBWndBase::BorderRaisedSet( bool bValue )
{
	if (bValue) 
	{
		// 必须有 WS_THICKFRAME 或 WS_DLGFRAME，才能起作用
		if ( ! ( Style() & (WS_THICKFRAME | WS_DLGFRAME) ) )
			StyleSet(WS_DLGFRAME);
		StyleExSet(WS_EX_WINDOWEDGE);
	}
	else
		StyleExSet(WS_EX_WINDOWEDGE, -1);
	Refresh();
}


bool CBWndBase::BorderSunken()
{
	if (StyleEx() & WS_EX_CLIENTEDGE) return true; else return false;
}

void CBWndBase::BorderSunkenSet( bool bValue )
{
	if (bValue) 
		StyleExSet(WS_EX_CLIENTEDGE);
	else
		StyleExSet(WS_EX_CLIENTEDGE, -1);
	Refresh();
}

bool CBWndBase::BorderStatic()
{
	if (StyleEx() & WS_EX_STATICEDGE) return true; else return false;
}

void CBWndBase::BorderStaticSet( bool bValue )
{
	if (bValue) 
		StyleExSet(WS_EX_STATICEDGE);
	else
		StyleExSet(WS_EX_STATICEDGE, -1);
	Refresh();
}

bool CBWndBase::BorderTitleBar()
{
	if (Style() & WS_CAPTION) return true; else return false;
}

void CBWndBase::BorderTitleBarSet( bool bValue )
{
	if (bValue) 
		StyleSet(WS_CAPTION);
	else
		StyleSet(WS_CAPTION, -1);
	Refresh();
}


bool CBWndBase::BorderToolWindow()
{
	if (StyleEx() & WS_EX_TOOLWINDOW) return true; else return false;
}

void CBWndBase::BorderToolWindowSet( bool bValue )
{
	if (bValue) 
		StyleExSet(WS_EX_TOOLWINDOW);
	else
		StyleExSet(WS_EX_TOOLWINDOW, -1);
	Refresh();
}



















// ================================================
// 高级
// ================================================


bool CBWndBase::Transparent()
{
	if (StyleEx() & WS_EX_TRANSPARENT) return true; else return false;
}

void CBWndBase::TransparentSet( bool bTransparent )
{
	if (bTransparent)
		StyleExSet(WS_EX_TRANSPARENT);
	else
		StyleExSet(WS_EX_TRANSPARENT, -1);
}	

int CBWndBase::Opacity()
{
	typedef BOOL (WINAPI *PFUNType)(HWND, COLORREF *, BYTE *, DWORD *);

	if (StyleEx() & WS_EX_LAYERED)
	{
		PFUNType pfun = NULL;
		pfun = (PFUNType)GetProcAddress(GetModuleHandle(TEXT("USER32.dll")), 
			"GetLayeredWindowAttributes");
		if (pfun == 0) return -2;	// 无法获得 GetLayeredWindowAttributes 函数的入口地址

		BYTE ret; DWORD retFlags; 
		(*pfun)(m_hWnd, 0, &ret, &retFlags);
		if (retFlags & LWA_ALPHA)
			return (int)ret; 
		else
			return -1;	// 窗口未被设置此样式
	}
	else
	{
		return -1;		// 窗口未被设置此样式
	}
}

void CBWndBase::OpacitySet( int iOpacity )
{
	typedef BOOL (WINAPI *PFUNType)(HWND, COLORREF, BYTE, DWORD);

	if (iOpacity < 0)
	{
		// 取消 WS_EX_LAYERED 样式
		StyleExSet(WS_EX_LAYERED, -1);
	}
	else
	{
		PFUNType pfun = NULL;
		pfun = (PFUNType)GetProcAddress(GetModuleHandle(TEXT("USER32.dll")), 
			"SetLayeredWindowAttributes");
		if (pfun == 0) return;	// 无法获得 SetLayeredWindowAttributes 函数的入口地址

		BYTE byt = (BYTE)iOpacity;
		// 设置 WS_EX_LAYERED 样式
		StyleExSet(WS_EX_LAYERED, 1);
		if (iOpacity > 255) iOpacity = 255;	// 限制范围在 0～255
		(*pfun)(m_hWnd, 0, byt, LWA_ALPHA);
	}
}

COLORREF CBWndBase::TransparencyKey()
{
	typedef BOOL (WINAPI *PFUNType)(HWND, COLORREF *, BYTE *, DWORD *);

	if (StyleEx() & WS_EX_LAYERED)
	{
		PFUNType pfun = NULL;
		pfun = (PFUNType)GetProcAddress(GetModuleHandle(TEXT("USER32.dll")), 
			"GetLayeredWindowAttributes");
		if (pfun == 0) return -2;	// 无法获得 GetLayeredWindowAttributes 函数的入口地址

		BYTE ret; DWORD retFlags; COLORREF color;
		(*pfun)(m_hWnd, &color, &ret, &retFlags);
		if (retFlags & LWA_COLORKEY)
			return (int)ret; 
		else
			return -1;	// 窗口未被设置此样式
	}
	else
	{
		return -1;		// 窗口未被设置此样式
	}
}

void CBWndBase::TransparencyKeySet(COLORREF iTransColor)
{
	typedef BOOL (WINAPI *PFUNType)(HWND, COLORREF, BYTE, DWORD);
	
	if (iTransColor < 0)
	{
		// 取消 WS_EX_LAYERED 样式
		StyleExSet(WS_EX_LAYERED, -1);
	}
	else
	{
		PFUNType pfun = NULL;
		pfun = (PFUNType)GetProcAddress(GetModuleHandle(TEXT("USER32.dll")), 
			"SetLayeredWindowAttributes");
		if (pfun == 0) return;	// 无法获得 SetLayeredWindowAttributes 函数的入口地址
		
		// 设置 WS_EX_LAYERED 样式
		StyleExSet(WS_EX_LAYERED, 1);
		(*pfun)(m_hWnd, iTransColor, 0, LWA_COLORKEY);
	}
}


// 获得窗口的类名（不能通过指针改变类名字符串的内容）
const TCHAR * CBWndBase::ClassName() const
{
	return m_ClassName;
}


bool CBWndBase::IsClassName( LPCTSTR lpTestClassName ) const
{
	return (_tcscmp(m_ClassName, lpTestClassName)==0); 
}



unsigned long CBWndBase::Style()
{
	return (unsigned long)GetWindowLong(m_hWnd, GWL_STYLE);
}

void CBWndBase::StyleSet( unsigned long newStyle, int bOr/*=1*/ )
{
	unsigned long r, rNew;
	r = (unsigned long)GetWindowLong(m_hWnd, GWL_STYLE);
	if (bOr > 0) 
		rNew = r | newStyle;	// 若 bOr > 0， 则在现有风格上增加
	else if (bOr < 0)
		rNew = r & ~newStyle;	// 若 bOr < 0，则在现有风格上取消 newStyle
	else	// bOr == 0
		rNew = newStyle;		// 若 bOr == 0，则将现有风格改为 newStyle

	if (rNew != r) 
		SetWindowLong(m_hWnd, GWL_STYLE, (LONG)rNew);	
}

unsigned long CBWndBase::StyleEx()
{
	return (unsigned long)GetWindowLong(m_hWnd, GWL_EXSTYLE);	
}

void CBWndBase::StyleExSet( unsigned long newStyleEx, int bOr/*=1*/ )
{
	unsigned long r, rNew;
	r = (unsigned long)GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bOr > 0) 
		rNew = r | newStyleEx;	// 若 bOr > 0， 则在现有扩展风格上增加
	else if (bOr < 0)			
		rNew = r & ~newStyleEx;	// 若 bOr < 0，则在现有扩展风格上取消 newStyleEx
	else
		rNew = newStyleEx;		// 若 bOr < 0，则在现有扩展风格上取消 newStyleEx

	if (rNew != r) 
		SetWindowLong(m_hWnd, GWL_EXSTYLE, (LONG)rNew);
}




// ================================================
// 方法
// ================================================

void CBWndBase::Refresh()
{
	SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, 
		SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE); 
	InvalidateRect(m_hWnd, NULL, 1);
}

void CBWndBase::SetFocus()
{
	::SetFocus(m_hWnd);		// 调用全局 API 函数（与本方法同名）
}

void CBWndBase::ZOrder( int position/*=0*/ )
{
	// 设置窗口或控件的 Z-顺序，即是覆盖其他控件，还是被其他控件所覆盖
	// position=0，则位于其他控件的最前面；否则 位于最后面
	if (m_hWnd==NULL) return;
	UINT flags = SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE;
	if (position==0)
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, flags);
	else
		SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, flags);
}




void CBWndBase::Cut()
{
	SendMessage(m_hWnd, WM_CUT, 0, 0);
}

void CBWndBase::Copy()
{
	SendMessage(m_hWnd, WM_COPY, 0, 0);
}

void CBWndBase::Paste()
{
	SendMessage(m_hWnd, WM_PASTE, 0, 0);
}











