//////////////////////////////////////////////////////////////////////
// BWndBase.h: CBWndBase 类的定义
// 包含常规窗口功能，作为 CBForm、CBControl 类的基类
//
// 支持：
//     需要 BWindows 模块的支持
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BWindows.h"

// 定义常量（这些常量头文件没有）
#define	WS_EX_LAYERED	0x80000
#define	LWA_COLORKEY	0x1
#define	LWA_ALPHA		0x2

class CBWndBase
{
public:
	// 构造函数和析构函数
	CBWndBase( HWND hWndToManage = NULL, HWND hWndParentForm = NULL );	
	~CBWndBase();

	// 获得和设置窗口的 使能 状态
	void EnabledSet(bool enabledValue) const;	
	bool Enabled() const;	
	
	// 获得和设置窗口的 隐藏 状态
	void VisibleSet(bool visibleValue) const;	
	bool Visible() const;
	
	// 返回和设置窗口是否有滚动条
	bool HScrollBar();
	void HScrollBarSet(bool bValue);
	bool VScrollBar();
	void VScrollBarSet(bool bValue);
	
	// 返回和设置 控件是否接收按 Tab 键移动焦点
	bool TabStop();
	void TabStopSet(bool bValue);
	
	// 返回和设置Group属性
	// 从第一个 Group=true 的控件开始 至下一个 Group=True 的控
    //   件为止，之间的所有控件为一组
	bool Group();
	void GroupSet(bool bValue);


	// ================================================
	// 位置、大小
	// ================================================

	// 获得和改变窗口大小、位置（移动窗口、改变窗口大小）
	void Move(int left=0x7FFFFFFF, int top=0x7FFFFFFF, 
		int width=0x7FFFFFFF, int height=0x7FFFFFFF) const;
	int Left() const;
	int Top() const;
	int Width() const;
	int Height() const;
	int ClientWidth() const;
	int ClientHeight() const;
	void LeftSet(int left) const;
	void TopSet(int top) const;
	void WidthSet(int width) const;
	void HeightSet(int height) const;


	// ================================================
	// 文本
	// ================================================

	// 设置窗口文本（各种重载版本）
	void TextSet(LPCTSTR newText) const;
	void TextSet(char valueChar) const;
	void TextSet(unsigned short int valueInt) const;	// TCHAR
	void TextSet(int valueInt) const;	
	void TextSet(unsigned int valueInt) const; 
	void TextSet(unsigned long valueInt) const;
	void TextSet(float valueSng) const;
	void TextSet(double valueDbl) const;
	void TextSet(long double valueDbl) const;
	
	// 设置窗口文本（各种重载版本）
	void TextAdd(LPCTSTR newText) const;
	void TextAdd(char valueChar) const;
	void TextAdd(unsigned short int valueInt) const;
	void TextAdd(int valueInt) const;	
	void TextAdd(unsigned int valueInt) const;
	void TextAdd(unsigned long valueInt) const;
	void TextAdd(float valueSng) const;
	void TextAdd(double valueDbl) const;
	void TextAdd(long double valueDbl) const;
	
	// 获得窗口文本，返回字符串
	TCHAR * Text() const;
	// 获得窗口文本转换为的 double 型数
	double TextVal() const; 



	// ================================================
	// 字体
	// ================================================
	
	// 返回和设置 窗口字体的字体名称字符串
	// 字符串缓冲区自动管理
	LPTSTR FontName();
	void   FontNameSet(LPCTSTR szFontName);

	// 返回和设置 窗口字体的大小
	float FontSize();
	void  FontSizeSet(float fSize);

	// 返回和设置 窗口字体是否加粗
	bool FontBold();
	void FontBoldSet(bool value);

	// 返回和设置 窗口字体是否加下划线
	bool FontUnderline();
	void FontUnderlineSet(bool value);

	// 返回和设置 窗口字体是否倾斜
	bool FontItalic();
	void FontItalicSet(bool value);

	// 返回和设置 窗口字体的旋转角度（单位：1/10 度）
	float FontRotateDegree();
	void  FontRotateDegreeSet(float fDegree);

	// 返回和设置 窗口字体的字符集
	BYTE FontCharSet();
	void FontCharSetSet(BYTE ucValue);


	// 返回目前窗口所使用的字体对象的句柄，0 表示正在使用默认字体
	HFONT hFont();
	



	
	// ================================================
	// 边框
	// ================================================

	// 返回和设置窗口是否有边框
	bool Border();
	void BorderSet(bool bValue);
	
	// 返回和设置 是否是对话框类型的边框
	// Creates a window that has a border of a style typically used 
	//    with dialog boxes. A window with this style cannot have a 
	//    title bar.
	bool BorderFrameDlg();
	void BorderFrameDlgSet(bool bValue);
	
	// 返回和设置 边框可以被拖动以改变窗口大小
	// Creates a window that has a sizing border. 
	//   Same as the WS_SIZEBOX style.
	bool BorderFrameThick();
	void BorderFrameThickSet(bool bValue);

	// 返回和设置 是否为有凸起感的边框
	// Specifies that a window has a border with a raised edge.
	bool BorderRaised();
	void BorderRaisedSet(bool bValue);

	// 返回和设置 是否为有凹陷感的边框
	// Specifies that a window has a 3D look 
	//   — that is, a border with a sunken edge.
	bool BorderSunken();
	void BorderSunkenSet(bool bValue);

	// 返回和设置 是否为静态边框
	// Creates a window with a three-dimensional border style
	//   intended to be used for items that do not accept user input.
	bool BorderStatic();
	void BorderStaticSet(bool bValue);

	// 返回和设置窗口是否有标题栏
	bool BorderTitleBar();
	void BorderTitleBarSet(bool bValue);
	
	// 返回和设置 是否为浮动工具窗口样式（窄标题栏）
	// Creates a tool window, which is a window intended to be 
	//   used as a floating toolbar. A tool window has a title bar 
	//   that is shorter than a normal title bar, and the window 
	//   title is drawn using a smaller font. A tool window does not 
	//   appear in the task bar or in the window that appears when 
	//   the user presses ALT+TAB.
	bool BorderToolWindow();
	void BorderToolWindowSet(bool bValue);



	// ================================================
	// 高级
	// ================================================

	// 返回和设置控件是否透明
	bool Transparent();
	void TransparentSet(bool bTransparent);
	
	// 返回和设置 窗体半透明度：0-255，0为完全透明，255为不透明；
	//							设置为负数取消此效果
	//            窗口未被设置此样式返回-1，系统不支持返回 -2
	// win2000以上可设置；winXP以上可返回
	int Opacity();
	void OpacitySet(int iOpacity);

	// 返回和设置 窗体“挖空”的颜色：设置为 -1 取消此效果
	// 返回-1表无此效果
	// win2000以上可设置；winXP以上可返回
	COLORREF TransparencyKey();
	void TransparencyKeySet(COLORREF iTransColor);
	
	// 获得窗口的类名（不能通过指针改变类名字符串的内容）
	const TCHAR * ClassName() const;	
	
	// 判断窗口的类是否是一种类名
	bool IsClassName(LPCTSTR lpTestClassName) const;


	// 返回或设置窗口风格；
	// 设置时：若 bOr > 0， 则在现有风格上增加
	//         若 bOr < 0，则在现有风格上取消 newStyle
	//         若 bOr == 0，则将现有风格改为 newStyle
	unsigned long Style();
	void StyleSet(unsigned long newStyle, int bOr=1);
	
	// 返回或设置窗口的扩展风格；
	// 设置时：若 bOr > 0， 则在现有扩展风格上增加
	//         若 bOr < 0，则在现有扩展风格上取消 newStyleEx
	//         若 bOr == 0，则将现有扩展风格改为 newStyleEx
	unsigned long StyleEx();
	void StyleExSet(unsigned long newStyleEx, int bOr=1);


	// ================================================
	// 方法
	// ================================================

	// 刷新窗口显示
	void Refresh();
	
	// 设置焦点到本窗口
	void SetFocus();
	
	// 设置窗口或控件的 Z-顺序，即是覆盖其他窗口控件，还是被其他窗口控件所覆盖
	// position=0，则位于其他窗口控件的最前面；否则 位于最后面
	void ZOrder(int position=0);


	
	
	// 剪切复制粘贴
	void Cut();
	void Copy();
	void Paste();

protected:	// 将变为继承类的保护成员
	HWND m_hWnd;		// 窗口句柄
	HWND m_hWndDlg;		// 如果本类的继承类的对象是控件，此表示控件所在窗体的 hwnd
						// 如果本类的继承类的对象是窗体，不用此值（保持为0）

	TCHAR m_ClassName[128];	// 窗口控件的类名
	long m_atom;			// 窗口控件类的 atom 号，唯一标识一种类，使用基类 BWndBase 的成员
							// CBControl 类继承时，在 SetResID 后即自动用 GetClassName 设置
							// CBForm 类继承时，在 CBForm_DlgProc 的 WM_INITDIALOG 中设置
	
	HFONT GetFontMLF(LOGFONT * lpLf=NULL);		// 将窗口当前所用字体信息获取到 lpLf 所指向的结构，
												//   并返回目前所用字体对象句柄
												// 若 lpLf 为空指针，直接返回字体句柄，不获取字体信息
	virtual HFONT SetFontMLF(LOGFONT * lpLf);	// 根据 lpLf 所指向的结构创建一种字体，并设置窗口使用该种字体；
												//   函数返回新字体对象的句柄
												// 虚函数，继承类必须重载该函数
												// CBControl 类重载后，应在重载函数中将新字体对象地址存入 STRControlProp.hFont
												// CBForm 类重载后，应在重载函数中将新字体对象地址存入 m_hFont 私有成员
};
