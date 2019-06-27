//////////////////////////////////////////////////////////////////////
// BControl.cpp: CBControl 类的实现
// 实现对话框中的一个控件的各种功能
//   
//////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <stdio.h>	// 用 _stprintf
#include "BForm.h"

// 仅本模块使用的全局空串字符串空间，例如用于 TagString 为空时的返回值
TCHAR mEmptyStr[2]={0};	


// 构造函数
CBControl::CBControl( HWND hWndDialog/*=0*/, 
					 unsigned short int idResControl/*=0*/, 
					 CBHashLK * ptrHashObjs/*=NULL*/)
{
	// 成员变量清零
	m_ResCtrlID = 0;
	m_hWndDlg = 0;
	m_hWnd = 0;
	m_ptrHashObjs = NULL;
	
	// 如果参数中给出了对话框句柄和控件ID，现在设置
	if (hWndDialog && idResControl)
		SetResID(hWndDialog, idResControl, ptrHashObjs);
}


// 析构函数
CBControl::~CBControl()
{
	// 禁止一切释放系统资源操作，因 CBControl 是动态绑定的
	//   本类对象被卸载时，对应的控件不一定被卸载
	;
}


// 设置使用本对象管理的一个控件，如未在构造函数中设置，也可用本函数中设置
// hWndDialog 为对话框的句柄，idResControl 为控件的资源id
// 成功返回子窗口控件句柄，失败返回0
HWND CBControl::SetResID( HWND hWndDialog/*=0*/, 
						  unsigned short int idResControl/*=0*/, 
						  CBHashLK * ptrHashObjs/*=NULL*/ )
{
	HWND hWndCtrl;

	// 清空 类名字符串
	memset(m_ClassName, 0, sizeof(m_ClassName));
	
	// 获得控件 hWnd
	hWndCtrl=GetDlgItem(hWndDialog, (int)idResControl);
	if (hWndCtrl)
	{
		// 将相关信息记录到成员变量
		m_ResCtrlID = idResControl;
		m_hWndDlg = hWndDialog;
		m_hWnd = hWndCtrl;
		m_ptrHashObjs = ptrHashObjs;

		// 获得类的唯一标识 Atom
		m_atom = GetClassLong(hWndCtrl, GCW_ATOM);

		// 获得类名字符串 => m_ClassName
		GetClassName(m_hWnd, m_ClassName, sizeof(m_ClassName)/sizeof(TCHAR)-1);

	}

	return hWndCtrl;
}











// ============ 设置图片 ============

void CBControl::PictureSet( UINT bmpResID ) const
{
	HANDLE hBmp = LoadImage(pApp->hInstance, (LPCTSTR)bmpResID, IMAGE_BITMAP, 0, 0, LR_SHARED);
	PictureSet(hBmp, IMAGE_BITMAP);
}

void CBControl::PictureSetIcon( EStandardIcon iconStd ) const
{
	HANDLE hIco = (HANDLE)LoadIcon(NULL, (LPCTSTR)iconStd);
	PictureSet(hIco, IMAGE_ICON);
}

void CBControl::PictureSet( LPCTSTR bmpFile ) const
{
	HANDLE hBmp = LoadImage(NULL, bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	PictureSet(hBmp, IMAGE_BITMAP);
}

void CBControl::PictureSetIcon( UINT iconResID, LPCTSTR typeRes/*=0*/ ) const
{
	HANDLE hIco = 0; 
	if (typeRes==0)
	{
		// 加载 Icon 类型的资源
		hIco = LoadImage(pApp->hInstance, (LPCTSTR)iconResID, IMAGE_ICON, 0, 0, LR_SHARED);	
	}
	else
	{
		// 加载自定义类型的资源（typeRes 类型的资源）
		unsigned long size=0; 
		unsigned char * p= LoadResData(iconResID, typeRes,&size);
		hIco = CreateIconFromResource(p, size, 0, 0x00030000);
	}
	PictureSet(hIco, IMAGE_ICON);
}

void CBControl::PictureSetIcon( LPCTSTR iconFile ) const
{
	HANDLE hIco = LoadImage(NULL, iconFile, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	PictureSet(hIco, IMAGE_ICON);
}

void CBControl::PictureSet( HANDLE hPic, UINT picType ) const
{
	LONG stl = GetWindowLong(m_hWnd, GWL_STYLE);
	bool bRestoreCENTERIMAGE=false; 

	// 若有 SS_CENTERIMAGE 风格在显示图片时可能会有问题：
	//   如 hPic 为 0 表示删除图片，不能立即刷新删除
	//   转换为 SS_ICON 风格时原位图不能被清除 
	// 若有此风格，这里要取消 SS_CENTERIMAGE 之后再恢复
	if (stl & SS_CENTERIMAGE)
	{
		// 暂时取消 SS_CENTERIMAGE
		stl = stl & ~SS_CENTERIMAGE;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
		bRestoreCENTERIMAGE=true;
	}

	// 根据图片类型，调整控件风格
	if ( picType == IMAGE_BITMAP )
	{
		// 调整 Static 控件为 SS_BITMAP 风格
		stl = stl & ~SS_ICON;
		stl = stl & ~SS_ENHMETAFILE;
		stl = stl | SS_BITMAP;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
	}
	else if ( picType == IMAGE_ICON )
	{
		// 调整 Static 控件为 SS_ICON 风格
		stl = stl & ~SS_BITMAP;
		stl = stl & ~SS_ENHMETAFILE;
		stl = stl | SS_ICON;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
	}

	// 管理句柄
	if (hPic)	
		m_ptrHashObjs->Add((long)hPic, (long)hPic, (long)picType, 0, 0, 0, 0, false);

	// 设置位图
	SendMessage(m_hWnd, STM_SETIMAGE, (WPARAM)picType, (LPARAM)hPic);

	// 如需要，恢复 SS_CENTERIMAGE
	if (bRestoreCENTERIMAGE)
		SetWindowLong(m_hWnd, GWL_STYLE, stl | SS_CENTERIMAGE);
}







long CBControl::MousePointer()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->cursorIndex); 
	else
		return 0;	// 没有设置过附加属性
}

void CBControl::MousePointerSet( EStandardCursor cursor )
{
	MousePointerSet( (long)cursor );
}

void CBControl::MousePointerSet( long idResCursor, LPCTSTR typeRes/*=0*/ )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// 第二个参数为 true，若没有设置过附加属性，现在开辟附加属性的空间
	if (pProp==NULL) return; //上条语句第二个参数已为 true，若 pProp 仍为 0 表示出错
	
	// 用新分配的空间的 cursorIndex 成员记录鼠标光标形状Index
	pProp->cursorIndex = (long)idResCursor;
	if (pProp->cursorIndex)
	{
		// 加载光标，句柄存入 pProp->hCursor
		// =============================================================
		// LoadCursor 函数即使重复被调用，也不会重复加载资源；系统会判断
		//   如果对应光标已经加载，LoadCursor 直接返回句柄
		// =============================================================
		if ( pProp->cursorIndex > gc_IDStandCursorIDBase)
		{
			// 标准光标
			// indexCursor-gc_IDStandCursorIDBase 才是标准光标的ID号
			 pProp->hCursor = 
				 LoadCursor(NULL, 
				   MAKEINTRESOURCE(pProp->cursorIndex-gc_IDStandCursorIDBase));
		}
		else
		{
			// 资源光标
			// pProp->cursorIndex 就是资源 ID
			if (typeRes==0)
			{
				// 加载 Cursor 类型的资源
				pProp->hCursor = LoadCursor(pApp->hInstance, MAKEINTRESOURCE(pProp->cursorIndex));
			}
			else
			{
				// 加载自定义类型的资源（typeRes 类型的资源）
				unsigned long size=0; 
				unsigned char * p= LoadResData(pProp->cursorIndex, typeRes, &size);
				pProp->hCursor = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (pProp->cursorIndex)
	{
		// 不特殊设置光标，使用默认：设置 pProp->hCursor 为 0
		pProp->hCursor = 0;
	}		// end if (pProp->cursorIndex)

	// 向本子窗口控件发送 WM_SETCURSOR，以使光标立即生效
	SendMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, 0);
	// 在本子窗口控件不断接收到的 WM_SETCURSOR 消息中会改变鼠标光标
}



COLORREF CBControl::BackColor()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->backColor); 
	else
		return 0;	// 没有设置过附加属性	
}


void CBControl::BackColorSet( COLORREF color )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// 第二个参数为 true，若没有设置过附加属性，现在开辟附加属性的空间
	if (pProp==NULL) return; //上条语句第二个参数已为 true，若 pProp 仍为 0 表示出错
	
	// 用新分配的空间的 backColor 成员记录控件背景色
	pProp->backColor = color;
	// 同时建立刷子，并用新空间的 hBrushBack 成员记录刷子句柄
	if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);
	pProp->hBrushBack = CreateSolidBrush(color); 
}

COLORREF CBControl::ForeColor()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->foreColor); 
	else
		return 0;	// 没有设置过附加属性	
}

void CBControl::ForeColorSet( COLORREF color )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// 第二个参数为 true，若没有设置过附加属性，现在开辟附加属性的空间
	if (pProp==NULL) return; //上条语句第二个参数已为 true，若 pProp 仍为 0 表示出错
	
	// 用新分配的空间的 foreColor 成员记录控件背景色
	pProp->foreColor = color;
}

// 仅有 CBControl 类有 TextInt；
// Text、TextVal 继承 CBWndBase 基类
int CBControl::TextInt(BOOL * retSuccess/*=0*/)
{
	return GetDlgItemInt(m_hWndDlg, m_ResCtrlID, retSuccess , 1);	
}


LPTSTR CBControl::TagString()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
	{
		if (pProp->tagString) return pProp->tagString;
	}

	// 没有设置过附加属性，返回全局 mEmptyStr 空间的 ""
	mEmptyStr[0]=0; return mEmptyStr; 
}

void CBControl::TagStringSet( LPCTSTR tagString )
{
	if (tagString==NULL) // 若是空指针就设为空字符串
		{ mEmptyStr[0]=0; tagString=mEmptyStr;	} 
	
	// 如果是空字符串，且还未设置过附加属性，保持还未设置即可
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp==NULL && *tagString==0) return;

	// 设置附加属性，获取空间（如附加属性的空间尚未开辟现在开辟）
	pProp = CBForm::PropPtr(m_hWnd, true);
	// 第二个参数为 true，若没有设置过附加属性，现在开辟附加属性的空间
	if (pProp==NULL) return; //上条语句第二个参数已为 true，若 pProp 仍为 0 表示出错
	
	if (pProp->tagString) delete [](pProp->tagString);	// 删除原字符串的空间，如果有的话
	pProp->tagString = new TCHAR[lstrlen(tagString)+1];
	_tcscpy(pProp->tagString, tagString);
}




// =================== 按钮类控件的功能 ===================
// 返回单选或复选按钮是否被勾选了：0=未选；1=已选；2=三态
unsigned int CBControl::ValueChecked() const
{
	return IsDlgButtonChecked(m_hWndDlg, m_ResCtrlID);
}

// 设置单选或复选按钮的勾选状态：0=未选；1=已选；2=三态
// 要设置为“2=三态”，复选框必须具有 tri-STATE 风格
void CBControl::ValueCheckedSet( UINT uCheck ) const
{
	
	CheckDlgButton(m_hWndDlg, m_ResCtrlID, uCheck);
}


// =================== 组合框列表框类控件的功能 ===================

// 添加一个条目，返回添加的条目编号（从1开始），出错返回 0
// index=0 时，在末尾添加；否则在指定的 index 位置添加，后者不能自动排序
int CBControl::AddItem( LPCTSTR item, int index /*= -1*/ ) const
{
	// ************************
	// If you create the combo box with an owner-drawn style but without the CBS_HASSTRINGS style, 
	// the value of the lpsz parameter is stored as item data rather than the string it would otherwise point to. 
	
	if ( IsClassName( TEXT("ComboBox") ) )
	{
		// ComboBox
		if (index<=0)
			return SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)item)+1;
		else
			return SendMessage(m_hWnd, CB_INSERTSTRING, index-1, (LPARAM)item)+1;
	}
	else if ( IsClassName( TEXT("ListBox") ) ) 
	{
		// ListBox
		if (index<=0)
			return SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)item)+1;
		else
			return SendMessage(m_hWnd, LB_INSERTSTRING, index-1, (LPARAM)item)+1;
	}
	

	// 该控件不能用于 AddItem
	return -3;
}


// 删除一个条目，编号从1开始；返回删除后的列表共有条目数，出错返回0
int CBControl::RemoveItem( int index ) const
{
	if ( IsClassName( TEXT("ComboBox") ) )   
	{
		// ComboBox
		return SendMessage(m_hWnd, CB_DELETESTRING, index-1, 0);
	}
	else if ( IsClassName( TEXT("ListBox") ) )    
	{
		// ListBox
		return SendMessage(m_hWnd, LB_DELETESTRING, index-1, 0);
	}
	
	// 该控件不能用于 RemoveItem
	return -3;
}


// 返回共有条目数，出错返回<0的值
int CBControl::ListCount() const
{
	if ( IsClassName( TEXT("ComboBox") ) )
	{
		// ComboBox
		return SendMessage(m_hWnd, CB_GETCOUNT, 0, 0);
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		return SendMessage(m_hWnd, LB_GETCOUNT, 0, 0);
	}
	
	
	// 该控件不能用于 ListCount
	return -3;
}




// 返回当前选择的条目编号（编号从1开始），出错返回0
// 若对多选列表框使用，则返回的是具有焦点的项目编号
//   所有项目都没被选择时，返回1（表示第1个项目有焦点）
int CBControl::ListIndex() const
{

	if ( IsClassName( TEXT("ComboBox") ) )
	{
		// ComboBox
		return SendMessage(m_hWnd, CB_GETCURSEL, 0, 0)+1;	// +1 表示转换为编号从1开始
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		return SendMessage(m_hWnd, LB_GETCURSEL, 0, 0)+1;	// +1 表示转换为编号从1开始
	}
	
	// 该控件不能用 ListIndex
	return -3;
}


// 选择一个条目，条目编号从1开始
void CBControl::ListIndexSet( int index ) const
{
	if ( IsClassName( TEXT("ComboBox") ) ) 
	{
		// ComboBox
		SendMessage(m_hWnd, CB_SETCURSEL, index-1, 0);
	}
	else if ( IsClassName( TEXT("ListBox") ) )  
	{
		// ListBox
		SendMessage(m_hWnd, LB_SETCURSEL, index-1, 0);
	}
	
	
	// 该控件不能用于 ListIndexSet
}


// 获得一个条目的内容，index 从1开始
LPTSTR CBControl::List( int index ) const
{
	LRESULT ret;
	if ( IsClassName( TEXT("ComboBox") ) )    
	{
		// ********************************************
		// If you create the combo box with an owner-drawn style but without 
		// the CBS_HASSTRINGS style, the buffer pointed to by the lpszBuffer parameter of
		// the message receives the 32-bit value associated with the item (the item data). 

		// ComboBox
		long length = SendMessage(m_hWnd, CB_GETLBTEXTLEN, index-1, 0);
		if (length == CB_ERR) { mEmptyStr[0]=0; return mEmptyStr; }  // 返回空串

		TCHAR * listText = new TCHAR [length + 1];
		HM.AddPtr(listText);
		HM.ZeroMem(listText, sizeof(TCHAR)+(length + 1));
		ret = SendMessage(m_hWnd, CB_GETLBTEXT, index-1, (LPARAM)listText);
		if (ret == CB_ERR)
			{ mEmptyStr[0]=0; return mEmptyStr; }  // 返回空串
		else
			return listText;
	}
	else if  ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		long length = SendMessage(m_hWnd, LB_GETTEXTLEN, index-1, 0);
		if (length == LB_ERR) { mEmptyStr[0]=0; return mEmptyStr; }  // 返回空串

		TCHAR * listText = new TCHAR [length + 1];
		HM.AddPtr(listText);
		HM.ZeroMem(listText, sizeof(TCHAR)+(length + 1));
		ret = SendMessage(m_hWnd, LB_GETTEXT, index-1, (LPARAM)listText);
		if (ret == LB_ERR)
			{ mEmptyStr[0]=0; return mEmptyStr; }  // 返回空串
		else
			return listText;
	}
	
	
	// 该控件不能用于 List
	return 0;
}


int CBControl::ListItemData( int index ) const
{
	if ( IsClassName( TEXT("ComboBox") ) )    
	{
		return (int)SendMessage(m_hWnd, CB_GETITEMDATA, index-1, 0);
	}
	else if  ( IsClassName( TEXT("ListBox") ) )
	{
		return (int)SendMessage(m_hWnd, LB_GETITEMDATA, index-1, 0);
	}
	
	// 该控件不能使用 ListItemData
	return 0;	
}

void CBControl::ListItemDataSet( int index, int itemData )
{
	if ( IsClassName( TEXT("ComboBox") ) )    
	{
		SendMessage(m_hWnd, CB_SETITEMDATA, index-1, (LPARAM)itemData);
	}
	else if  ( IsClassName( TEXT("ListBox") ) )
	{
		SendMessage(m_hWnd, LB_SETITEMDATA, index-1, (LPARAM)itemData);
	}

	// 该控件不能使用 ListItemData
}


// 返回多选列表中已选条目数
int CBControl::ListSelCount() const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return 0;	// 该函数只能用于 ListBox 控件

	// 单选列表框将返回 0
	return SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0);
}


bool CBControl::ListSeleted( int index ) const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return false;	// 该函数只能用于 ListBox 控件

	if ( SendMessage(m_hWnd, LB_GETSEL, index-1, 0) >0 )	// <0 表示出错(-1)，归于 return false
		return true;
	else
		return false;
}

void CBControl::ListSeletedSet( int index, bool bSel )
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return;	// 该函数只能用于 ListBox 控件

	if (index<1) return;	// 不能SendMessage: -1 到控件，否则将全部选中（取消选中）
	SendMessage(m_hWnd, LB_SETSEL, (BOOL)bSel, index-1);
}


void CBControl::ListSelAll( bool bSel )
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return;	// 该函数只能用于 ListBox 控件

	SendMessage(m_hWnd, LB_SETSEL, (BOOL)bSel, -1);
}



int * CBControl::ListSelItems( int *pSelCount/*=0*/ ) const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) 
	{
		if (pSelCount) *pSelCount = 0;
		return 0;	// 该函数只能用于 ListBox 控件
	}

	int count = ListSelCount();
	if (pSelCount) *pSelCount = count;
	if (count == 0) return 0;

	int *pSels = new int [count+1];		// 结果空间（不使用下标为0的元素）
	HM.AddPtr(pSels);
	HM.ZeroMem(pSels, sizeof(int)*(count+1) );

	int *pSelsTemp = new int [count];	// 临时空间
	SendMessage(m_hWnd, LB_GETSELITEMS, (WPARAM)count, (LPARAM)pSelsTemp);

	// 将临时空间的值拷贝到结果空间（不使用结果空间下标为0的元素）
	HM.CopyMem(pSels+1, pSelsTemp, sizeof(int)*count);

	// 将结果索引号各自 +1，使索引号都从 1 开始
	int i;
	for (i=1; i<=count; i++) pSels[i]+=1;

	delete []pSelsTemp;	// 不删除 pSels，它由 HM 管理

	return pSels;
}


int CBControl::ListTopIndex()
{
	return (int)SendMessage(m_hWnd, LB_SETTOPINDEX, 0, 0)+1;
}

void CBControl::ListTopIndexSet( int idxTop )
{
	SendMessage(m_hWnd, LB_SETTOPINDEX, idxTop-1, 0);
}

// 清除列表的所有内容
void CBControl::ListClear() const
{
	if ( IsClassName( TEXT("ComboBox") ) ) 
	{
		// ComboBox
		SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0);
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0);
	}
	
	// 该控件不能用 ListClear
}


int CBControl::ListItemsHeight() const
{
	if ( IsClassName( TEXT("ComboBox") ) ) 
	{
		// ComboBox
		return (int)SendMessage(m_hWnd, CB_GETITEMHEIGHT, 0, 0 );
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		return (int)SendMessage(m_hWnd, LB_GETITEMHEIGHT, 0, 0 );
	}

	// 该控件不能使用 ListItemsHeight
	return 0;
}

void CBControl::ListItemsHeightSet( int newHeight )
{
	if ( IsClassName( TEXT("ComboBox") ) ) 
	{
		// ComboBox
		SendMessage(m_hWnd, LB_SETITEMHEIGHT, 0, MAKELPARAM(newHeight, 0) );
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		SendMessage(m_hWnd, CB_SETITEMHEIGHT, 0, MAKELPARAM(newHeight, 0) );
	}

	Refresh();
}








// =================== 高级通用功能 ===================



HWND CBControl::hWnd() const
{
	return m_hWnd;	
}

HWND CBControl::hWndParentForm() const
{
	return (HWND)(CBForm::ms_hashCtrls.ItemLong((long)m_hWnd, false));
	// 或 return m_hWndDlg;
}


// 返回一个父窗体的 CBForm 对象的地址指向，用于通过控件访问父窗体
CBForm * CBControl::ParentFormPtr() const
{
	return (CBForm *)(CBForm::ms_hashWnd.Item((long)hWndParentForm(),false));
}

// 获得子窗口控件的默认窗口程序的地址（注实际此地址的函数可能不被使用，因已被子类化）
unsigned long CBControl::PtrWndProcDef() const
{
	return (unsigned long)(CBForm::ms_hashCtrls.Item((long)m_hWnd, false));
}


HFONT CBControl::SetFontMLF( LOGFONT * lpLf )
{
	// 调用基类的虚函数实现功能，函数返回新字体对象句柄
	HFONT hFontNew = CBWndBase::SetFontMLF(lpLf);	

	// 将新建立的字体对象句柄保存到 STRControlProp.hFont
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// 第二个参数为 true，若没有设置过附加属性，现在开辟附加属性的空间
	if (pProp==NULL) return hFontNew; //上条语句第二个参数已为 true，若 pProp 仍为 0 表示出错
	
	// 删除旧字体对象
	if (pProp->hFont) DeleteObject(pProp->hFont);	
	// 用新分配的空间的 hFont 成员记录新字体对象
	pProp->hFont = hFontNew;

	// 返回新字体对象句柄
return hFontNew;
}




