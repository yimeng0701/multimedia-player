//////////////////////////////////////////////////////////////////////
// BControl.h: CBControl 类的定义
// 实现对话框中的一个控件的各种功能
// 本类对象用全局 HM 管理动态分配的内存
//
// 支持：
//     需要 BWindows、BForm、BMenuItem 模块的支持
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BWndBase.h"
// BControl.cpp 将包含 #include "BForm.h"，因 BControl.cpp
//   中的 CBControl 类的实现中，将用到 CBForm 类

class CBForm;	// 类的声明

class CBControl : public CBWndBase
{
public:
	// 构造函数。hWndDialog 为对话框的句柄，idResControl 为控件的资源id
	// ptrHashObjs 为控件隶属 Form 对象（CBForm类）中维护的位图、光标、图标等对象的
	//   句柄哈希表的地址，用于加载菜单相关位图等资源时将句柄存入其中
	CBControl(HWND hWndDialog=0, unsigned short int idResControl=0, CBHashLK * ptrHashObjs=NULL);		
	
	// 析构函数
	~CBControl();

	// 设置使用本对象管理的一个控件，如未在构造函数中设置，也可用本函数中设置
	// hWndDialog 为对话框的句柄，idResControl 为控件的资源id
	// 成功返回子窗口控件句柄，失败返回0
	HWND SetResID(HWND hWndDialog=0, unsigned short int idResControl=0, CBHashLK * ptrHashObjs=NULL);	


	// 设置静态控件的图片（各种重载版本）
	void PictureSet(UINT bmpResID) const;
	void PictureSet(LPCTSTR bmpFile) const;

	// PictureSetIcon 可加载图标，也可加载动画光标
	//   typeRes 不为0时，指定自定义资源的类型（为0表示Icon类型）
	// ==== 加载动画光标的两种方法：=====
	// ① Form1.Control(ID_pic1).PictureSetIcon(TEXT("APPLE.ANI")); 
	//	// 加载动画光标文件
	// ② Form1.Control(ID_pic1).PictureSetIcon(IDR_MYANI1, TEXT("MYANI"));   
	//	// 从资源文件中加载 "MYANI" 类型的一个资源（ani文件事先被导入为该资源）
	void PictureSetIcon(EStandardIcon iconStd) const;
	void PictureSetIcon(UINT iconResID, LPCTSTR typeRes=0) const;
	void PictureSetIcon(LPCTSTR iconFile) const;
	
	// 直接以句柄设置图片，
	// picType 为图片类型：0,1,2,3 分别表示 位图、图标、光标、enh图元
	void PictureSet(HANDLE hPic, UINT picType) const;	


	// ======== 附加属性 ========
	// 用附加的 STRControlProp 来存储
	// CBForm::ms_hashCtrls.ItemLong2 为动态开辟的 STRControlProp 空间的地址

	// 返回和设置控件鼠标光标形状的索引值（不修改窗口类，通过 WM_SETCURSOR 实现）
	//   typeRes 为 0 时，设置 idResCursor 为 Cursor 类型的资源 ID
	//   也可设置其他类型的资源，此时 typeRes 为类型字符串（动画光标时使用）
	long MousePointer();
	void MousePointerSet(EStandardCursor cursor);		
	void MousePointerSet(long idResCursor, LPCTSTR typeRes=0);

	// 返回和设置控件背景色
	COLORREF BackColor();
	void BackColorSet(COLORREF color);

	// 返回和设置控件前景色
	COLORREF ForeColor();
	void ForeColorSet(COLORREF color);

	// 获得控件文本，但返回的不是字符串，而是转换为的整数
	// retSuccess 为一个指针，指向数据表示函数是否执行成功
	// 仅有 CBControl 类有 TextInt；
	// Text、TextVal 继承 CBWndBase 基类
	int TextInt(BOOL * retSuccess=0);

	// 附加字符串数据
	LPTSTR TagString();
	void TagStringSet(LPCTSTR tagString);


	// ======== Static 类控件功能 ========


	// ======== 按钮类控件功能 ========

	// 返回和设置单选或复选按钮是否被勾选了：0=未选；1=已选；2=三态
	unsigned int ValueChecked() const;			
	void ValueCheckedSet( UINT uCheck ) const;	


	// ======== 组合框、列表框类控件功能 ========

	// 添加一个条目，返回添加的条目编号（从1开始），出错返回 0
	// index=0 时，在末尾添加；否则在指定的 index 位置添加，后者不能自动排序
	int AddItem(LPCTSTR item, int index = 0) const;	

	// 删除一个条目，编号从1开始；返回删除后的列表共有条目数，出错返回0
	int RemoveItem(int index) const;

	// 返回共有条目数，出错返回<0的值
	int ListCount() const;

	// 返回当前选择的条目编号（编号从1开始），出错返回0
	// 若对多选列表框使用，则返回的是具有焦点的项目编号
	//   所有项目都没被选择时，返回1（表示第1个项目有焦点）
	int ListIndex() const;

	// 选择一个条目，条目编号从1开始
	// 该函数对多选列表框使用无效
	void ListIndexSet(int index) const;

	// 获得一个条目的内容，index 从1开始
	LPTSTR List(int index) const;

	// 获取和设置 列表框中各项目的附加整数数据，index 从1开始
	int ListItemData(int index) const;
	void ListItemDataSet(int index, int itemData);

	// 获取和设置 多选列表的一个项目是否被选中的状态，index 从1开始
	bool ListSeleted(int index) const;
	void ListSeletedSet(int index, bool bSel);
	
	// 全选/取消选中 多选列表的所有项目
	void ListSelAll(bool bSel);

	// 返回和设置列表中第一个可见项的索引号（从 1 开始）
	int ListTopIndex();
	void ListTopIndexSet(int idxTop);

	// 返回多选列表中已选条目数
	int ListSelCount() const;

	// 获得一个列表框中所有选定项目的索引号，放入一个 int 型数组
	//   各项目索引号从 1 开始
	// 数组下标从 1 开始，由本函数自动开辟数组空间
	// 函数返回数组首地址，没有选中项或出错返回 0
	// 数组元素个数返回到参数 pSelCount 所指空间，pSelCount 为0时不返回
	//   可用 ListSelCount 获得选择的元素个数
	// 对单选列表框数组中只有一个元素
	int * ListSelItems(int *pSelCount=0) const;

	// 清除列表的所有内容
	void ListClear() const;

	// 获取和设置列表项高度
	int ListItemsHeight() const;
	void ListItemsHeightSet(int newHeight);


	// ======== 高级通用功能 ========
	HWND hWnd() const;					// 获得子窗口控件的窗口句柄
	HWND hWndParentForm() const;		// 获得所属父窗体的窗口句柄
	// 返回一个父窗体的 CBForm 对象的地址指向，用于通过控件访问父窗体
	// 例如：Form1.Control(ID_cmd1).ParentFormPtr()->BackColorSet(...);
	CBForm * ParentFormPtr() const;		
	// 获得子窗口控件的默认窗口程序的地址（注实际此地址的函数可能不被使用，因已被子类化）
	unsigned long PtrWndProcDef() const;			
	
private:
	// HWND m_hWnd：子窗口控件的句柄，使用基类 BWndBase 的成员，此值在 SetResID 函数中被设置
	// HWND m_hWndDlg： 控件所在窗体的 hwnd，使用基类 BWndBase 的成员，此值在 SetResID 函数中被设置

	// TCHAR m_ClassName[128]：窗口控件的类名，使用基类 BWndBase 的成员
	// long m_atom：窗口控件类的 atom 号，唯一标识一种类，使用基类 BWndBase 的成员
	//				这两个值都在 SetResID 后即自动被设置

	unsigned short int m_ResCtrlID;	// 控件的资源id，此值在 SetResID 函数中被设置
	CBHashLK * m_ptrHashObjs;		// 子窗口控件隶属 Form 对象（CBForm类）中维护的位图、光标、图标等对象的句柄哈希表的地址，在Form 对象析构时被自动释放，菜单项相关的资源加载（如位图）应将所加载的资源句柄存入其中
	
	virtual HFONT SetFontMLF(LOGFONT * lpLf);	// 根据 lpLf 所指向的结构创建一种字体，并设置窗口使用该种字体；
												//   函数返回新字体对象的句柄
												// 这里为重载 CBWndBase 基类的虚函数
												// CBControl 类重载后，要在重载函数中将新字体对象地址存入 STRControlProp.hFont
};