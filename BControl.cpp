//////////////////////////////////////////////////////////////////////
// BControl.cpp: CBControl ���ʵ��
// ʵ�ֶԻ����е�һ���ؼ��ĸ��ֹ���
//   
//////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <stdio.h>	// �� _stprintf
#include "BForm.h"

// ����ģ��ʹ�õ�ȫ�ֿմ��ַ����ռ䣬�������� TagString Ϊ��ʱ�ķ���ֵ
TCHAR mEmptyStr[2]={0};	


// ���캯��
CBControl::CBControl( HWND hWndDialog/*=0*/, 
					 unsigned short int idResControl/*=0*/, 
					 CBHashLK * ptrHashObjs/*=NULL*/)
{
	// ��Ա��������
	m_ResCtrlID = 0;
	m_hWndDlg = 0;
	m_hWnd = 0;
	m_ptrHashObjs = NULL;
	
	// ��������и����˶Ի������Ϳؼ�ID����������
	if (hWndDialog && idResControl)
		SetResID(hWndDialog, idResControl, ptrHashObjs);
}


// ��������
CBControl::~CBControl()
{
	// ��ֹһ���ͷ�ϵͳ��Դ�������� CBControl �Ƕ�̬�󶨵�
	//   �������ж��ʱ����Ӧ�Ŀؼ���һ����ж��
	;
}


// ����ʹ�ñ���������һ���ؼ�����δ�ڹ��캯�������ã�Ҳ���ñ�����������
// hWndDialog Ϊ�Ի���ľ����idResControl Ϊ�ؼ�����Դid
// �ɹ������Ӵ��ڿؼ������ʧ�ܷ���0
HWND CBControl::SetResID( HWND hWndDialog/*=0*/, 
						  unsigned short int idResControl/*=0*/, 
						  CBHashLK * ptrHashObjs/*=NULL*/ )
{
	HWND hWndCtrl;

	// ��� �����ַ���
	memset(m_ClassName, 0, sizeof(m_ClassName));
	
	// ��ÿؼ� hWnd
	hWndCtrl=GetDlgItem(hWndDialog, (int)idResControl);
	if (hWndCtrl)
	{
		// �������Ϣ��¼����Ա����
		m_ResCtrlID = idResControl;
		m_hWndDlg = hWndDialog;
		m_hWnd = hWndCtrl;
		m_ptrHashObjs = ptrHashObjs;

		// ������Ψһ��ʶ Atom
		m_atom = GetClassLong(hWndCtrl, GCW_ATOM);

		// ��������ַ��� => m_ClassName
		GetClassName(m_hWnd, m_ClassName, sizeof(m_ClassName)/sizeof(TCHAR)-1);

	}

	return hWndCtrl;
}











// ============ ����ͼƬ ============

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
		// ���� Icon ���͵���Դ
		hIco = LoadImage(pApp->hInstance, (LPCTSTR)iconResID, IMAGE_ICON, 0, 0, LR_SHARED);	
	}
	else
	{
		// �����Զ������͵���Դ��typeRes ���͵���Դ��
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

	// ���� SS_CENTERIMAGE �������ʾͼƬʱ���ܻ������⣺
	//   �� hPic Ϊ 0 ��ʾɾ��ͼƬ����������ˢ��ɾ��
	//   ת��Ϊ SS_ICON ���ʱԭλͼ���ܱ���� 
	// ���д˷������Ҫȡ�� SS_CENTERIMAGE ֮���ٻָ�
	if (stl & SS_CENTERIMAGE)
	{
		// ��ʱȡ�� SS_CENTERIMAGE
		stl = stl & ~SS_CENTERIMAGE;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
		bRestoreCENTERIMAGE=true;
	}

	// ����ͼƬ���ͣ������ؼ����
	if ( picType == IMAGE_BITMAP )
	{
		// ���� Static �ؼ�Ϊ SS_BITMAP ���
		stl = stl & ~SS_ICON;
		stl = stl & ~SS_ENHMETAFILE;
		stl = stl | SS_BITMAP;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
	}
	else if ( picType == IMAGE_ICON )
	{
		// ���� Static �ؼ�Ϊ SS_ICON ���
		stl = stl & ~SS_BITMAP;
		stl = stl & ~SS_ENHMETAFILE;
		stl = stl | SS_ICON;
		SetWindowLong(m_hWnd, GWL_STYLE, stl);
	}

	// ������
	if (hPic)	
		m_ptrHashObjs->Add((long)hPic, (long)hPic, (long)picType, 0, 0, 0, 0, false);

	// ����λͼ
	SendMessage(m_hWnd, STM_SETIMAGE, (WPARAM)picType, (LPARAM)hPic);

	// ����Ҫ���ָ� SS_CENTERIMAGE
	if (bRestoreCENTERIMAGE)
		SetWindowLong(m_hWnd, GWL_STYLE, stl | SS_CENTERIMAGE);
}







long CBControl::MousePointer()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->cursorIndex); 
	else
		return 0;	// û�����ù���������
}

void CBControl::MousePointerSet( EStandardCursor cursor )
{
	MousePointerSet( (long)cursor );
}

void CBControl::MousePointerSet( long idResCursor, LPCTSTR typeRes/*=0*/ )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// �ڶ�������Ϊ true����û�����ù��������ԣ����ڿ��ٸ������ԵĿռ�
	if (pProp==NULL) return; //�������ڶ���������Ϊ true���� pProp ��Ϊ 0 ��ʾ����
	
	// ���·���Ŀռ�� cursorIndex ��Ա��¼�������״Index
	pProp->cursorIndex = (long)idResCursor;
	if (pProp->cursorIndex)
	{
		// ���ع�꣬������� pProp->hCursor
		// =============================================================
		// LoadCursor ������ʹ�ظ������ã�Ҳ�����ظ�������Դ��ϵͳ���ж�
		//   �����Ӧ����Ѿ����أ�LoadCursor ֱ�ӷ��ؾ��
		// =============================================================
		if ( pProp->cursorIndex > gc_IDStandCursorIDBase)
		{
			// ��׼���
			// indexCursor-gc_IDStandCursorIDBase ���Ǳ�׼����ID��
			 pProp->hCursor = 
				 LoadCursor(NULL, 
				   MAKEINTRESOURCE(pProp->cursorIndex-gc_IDStandCursorIDBase));
		}
		else
		{
			// ��Դ���
			// pProp->cursorIndex ������Դ ID
			if (typeRes==0)
			{
				// ���� Cursor ���͵���Դ
				pProp->hCursor = LoadCursor(pApp->hInstance, MAKEINTRESOURCE(pProp->cursorIndex));
			}
			else
			{
				// �����Զ������͵���Դ��typeRes ���͵���Դ��
				unsigned long size=0; 
				unsigned char * p= LoadResData(pProp->cursorIndex, typeRes, &size);
				pProp->hCursor = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (pProp->cursorIndex)
	{
		// ���������ù�꣬ʹ��Ĭ�ϣ����� pProp->hCursor Ϊ 0
		pProp->hCursor = 0;
	}		// end if (pProp->cursorIndex)

	// ���Ӵ��ڿؼ����� WM_SETCURSOR����ʹ���������Ч
	SendMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, 0);
	// �ڱ��Ӵ��ڿؼ����Ͻ��յ��� WM_SETCURSOR ��Ϣ�л�ı������
}



COLORREF CBControl::BackColor()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->backColor); 
	else
		return 0;	// û�����ù���������	
}


void CBControl::BackColorSet( COLORREF color )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// �ڶ�������Ϊ true����û�����ù��������ԣ����ڿ��ٸ������ԵĿռ�
	if (pProp==NULL) return; //�������ڶ���������Ϊ true���� pProp ��Ϊ 0 ��ʾ����
	
	// ���·���Ŀռ�� backColor ��Ա��¼�ؼ�����ɫ
	pProp->backColor = color;
	// ͬʱ����ˢ�ӣ������¿ռ�� hBrushBack ��Ա��¼ˢ�Ӿ��
	if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);
	pProp->hBrushBack = CreateSolidBrush(color); 
}

COLORREF CBControl::ForeColor()
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp)
		return (pProp->foreColor); 
	else
		return 0;	// û�����ù���������	
}

void CBControl::ForeColorSet( COLORREF color )
{
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// �ڶ�������Ϊ true����û�����ù��������ԣ����ڿ��ٸ������ԵĿռ�
	if (pProp==NULL) return; //�������ڶ���������Ϊ true���� pProp ��Ϊ 0 ��ʾ����
	
	// ���·���Ŀռ�� foreColor ��Ա��¼�ؼ�����ɫ
	pProp->foreColor = color;
}

// ���� CBControl ���� TextInt��
// Text��TextVal �̳� CBWndBase ����
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

	// û�����ù��������ԣ�����ȫ�� mEmptyStr �ռ�� ""
	mEmptyStr[0]=0; return mEmptyStr; 
}

void CBControl::TagStringSet( LPCTSTR tagString )
{
	if (tagString==NULL) // ���ǿ�ָ�����Ϊ���ַ���
		{ mEmptyStr[0]=0; tagString=mEmptyStr;	} 
	
	// ����ǿ��ַ������һ�δ���ù��������ԣ����ֻ�δ���ü���
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd); 
	if (pProp==NULL && *tagString==0) return;

	// ���ø������ԣ���ȡ�ռ䣨�總�����ԵĿռ���δ�������ڿ��٣�
	pProp = CBForm::PropPtr(m_hWnd, true);
	// �ڶ�������Ϊ true����û�����ù��������ԣ����ڿ��ٸ������ԵĿռ�
	if (pProp==NULL) return; //�������ڶ���������Ϊ true���� pProp ��Ϊ 0 ��ʾ����
	
	if (pProp->tagString) delete [](pProp->tagString);	// ɾ��ԭ�ַ����Ŀռ䣬����еĻ�
	pProp->tagString = new TCHAR[lstrlen(tagString)+1];
	_tcscpy(pProp->tagString, tagString);
}




// =================== ��ť��ؼ��Ĺ��� ===================
// ���ص�ѡ��ѡ��ť�Ƿ񱻹�ѡ�ˣ�0=δѡ��1=��ѡ��2=��̬
unsigned int CBControl::ValueChecked() const
{
	return IsDlgButtonChecked(m_hWndDlg, m_ResCtrlID);
}

// ���õ�ѡ��ѡ��ť�Ĺ�ѡ״̬��0=δѡ��1=��ѡ��2=��̬
// Ҫ����Ϊ��2=��̬������ѡ�������� tri-STATE ���
void CBControl::ValueCheckedSet( UINT uCheck ) const
{
	
	CheckDlgButton(m_hWndDlg, m_ResCtrlID, uCheck);
}


// =================== ��Ͽ��б����ؼ��Ĺ��� ===================

// ���һ����Ŀ��������ӵ���Ŀ��ţ���1��ʼ���������� 0
// index=0 ʱ����ĩβ��ӣ�������ָ���� index λ����ӣ����߲����Զ�����
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
	

	// �ÿؼ��������� AddItem
	return -3;
}


// ɾ��һ����Ŀ����Ŵ�1��ʼ������ɾ������б�����Ŀ����������0
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
	
	// �ÿؼ��������� RemoveItem
	return -3;
}


// ���ع�����Ŀ����������<0��ֵ
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
	
	
	// �ÿؼ��������� ListCount
	return -3;
}




// ���ص�ǰѡ�����Ŀ��ţ���Ŵ�1��ʼ����������0
// ���Զ�ѡ�б��ʹ�ã��򷵻ص��Ǿ��н������Ŀ���
//   ������Ŀ��û��ѡ��ʱ������1����ʾ��1����Ŀ�н��㣩
int CBControl::ListIndex() const
{

	if ( IsClassName( TEXT("ComboBox") ) )
	{
		// ComboBox
		return SendMessage(m_hWnd, CB_GETCURSEL, 0, 0)+1;	// +1 ��ʾת��Ϊ��Ŵ�1��ʼ
	}
	else if ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		return SendMessage(m_hWnd, LB_GETCURSEL, 0, 0)+1;	// +1 ��ʾת��Ϊ��Ŵ�1��ʼ
	}
	
	// �ÿؼ������� ListIndex
	return -3;
}


// ѡ��һ����Ŀ����Ŀ��Ŵ�1��ʼ
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
	
	
	// �ÿؼ��������� ListIndexSet
}


// ���һ����Ŀ�����ݣ�index ��1��ʼ
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
		if (length == CB_ERR) { mEmptyStr[0]=0; return mEmptyStr; }  // ���ؿմ�

		TCHAR * listText = new TCHAR [length + 1];
		HM.AddPtr(listText);
		HM.ZeroMem(listText, sizeof(TCHAR)+(length + 1));
		ret = SendMessage(m_hWnd, CB_GETLBTEXT, index-1, (LPARAM)listText);
		if (ret == CB_ERR)
			{ mEmptyStr[0]=0; return mEmptyStr; }  // ���ؿմ�
		else
			return listText;
	}
	else if  ( IsClassName( TEXT("ListBox") ) )
	{
		// ListBox
		long length = SendMessage(m_hWnd, LB_GETTEXTLEN, index-1, 0);
		if (length == LB_ERR) { mEmptyStr[0]=0; return mEmptyStr; }  // ���ؿմ�

		TCHAR * listText = new TCHAR [length + 1];
		HM.AddPtr(listText);
		HM.ZeroMem(listText, sizeof(TCHAR)+(length + 1));
		ret = SendMessage(m_hWnd, LB_GETTEXT, index-1, (LPARAM)listText);
		if (ret == LB_ERR)
			{ mEmptyStr[0]=0; return mEmptyStr; }  // ���ؿմ�
		else
			return listText;
	}
	
	
	// �ÿؼ��������� List
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
	
	// �ÿؼ�����ʹ�� ListItemData
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

	// �ÿؼ�����ʹ�� ListItemData
}


// ���ض�ѡ�б�����ѡ��Ŀ��
int CBControl::ListSelCount() const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return 0;	// �ú���ֻ������ ListBox �ؼ�

	// ��ѡ�б�򽫷��� 0
	return SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0);
}


bool CBControl::ListSeleted( int index ) const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return false;	// �ú���ֻ������ ListBox �ؼ�

	if ( SendMessage(m_hWnd, LB_GETSEL, index-1, 0) >0 )	// <0 ��ʾ����(-1)������ return false
		return true;
	else
		return false;
}

void CBControl::ListSeletedSet( int index, bool bSel )
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return;	// �ú���ֻ������ ListBox �ؼ�

	if (index<1) return;	// ����SendMessage: -1 ���ؼ�������ȫ��ѡ�У�ȡ��ѡ�У�
	SendMessage(m_hWnd, LB_SETSEL, (BOOL)bSel, index-1);
}


void CBControl::ListSelAll( bool bSel )
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) return;	// �ú���ֻ������ ListBox �ؼ�

	SendMessage(m_hWnd, LB_SETSEL, (BOOL)bSel, -1);
}



int * CBControl::ListSelItems( int *pSelCount/*=0*/ ) const
{
	if  ( ! IsClassName( TEXT("ListBox") ) ) 
	{
		if (pSelCount) *pSelCount = 0;
		return 0;	// �ú���ֻ������ ListBox �ؼ�
	}

	int count = ListSelCount();
	if (pSelCount) *pSelCount = count;
	if (count == 0) return 0;

	int *pSels = new int [count+1];		// ����ռ䣨��ʹ���±�Ϊ0��Ԫ�أ�
	HM.AddPtr(pSels);
	HM.ZeroMem(pSels, sizeof(int)*(count+1) );

	int *pSelsTemp = new int [count];	// ��ʱ�ռ�
	SendMessage(m_hWnd, LB_GETSELITEMS, (WPARAM)count, (LPARAM)pSelsTemp);

	// ����ʱ�ռ��ֵ����������ռ䣨��ʹ�ý���ռ��±�Ϊ0��Ԫ�أ�
	HM.CopyMem(pSels+1, pSelsTemp, sizeof(int)*count);

	// ����������Ÿ��� +1��ʹ�����Ŷ��� 1 ��ʼ
	int i;
	for (i=1; i<=count; i++) pSels[i]+=1;

	delete []pSelsTemp;	// ��ɾ�� pSels������ HM ����

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

// ����б����������
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
	
	// �ÿؼ������� ListClear
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

	// �ÿؼ�����ʹ�� ListItemsHeight
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








// =================== �߼�ͨ�ù��� ===================



HWND CBControl::hWnd() const
{
	return m_hWnd;	
}

HWND CBControl::hWndParentForm() const
{
	return (HWND)(CBForm::ms_hashCtrls.ItemLong((long)m_hWnd, false));
	// �� return m_hWndDlg;
}


// ����һ��������� CBForm ����ĵ�ַָ������ͨ���ؼ����ʸ�����
CBForm * CBControl::ParentFormPtr() const
{
	return (CBForm *)(CBForm::ms_hashWnd.Item((long)hWndParentForm(),false));
}

// ����Ӵ��ڿؼ���Ĭ�ϴ��ڳ���ĵ�ַ��עʵ�ʴ˵�ַ�ĺ������ܲ���ʹ�ã����ѱ����໯��
unsigned long CBControl::PtrWndProcDef() const
{
	return (unsigned long)(CBForm::ms_hashCtrls.Item((long)m_hWnd, false));
}


HFONT CBControl::SetFontMLF( LOGFONT * lpLf )
{
	// ���û�����麯��ʵ�ֹ��ܣ��������������������
	HFONT hFontNew = CBWndBase::SetFontMLF(lpLf);	

	// ���½�����������������浽 STRControlProp.hFont
	STRControlProp * pProp = CBForm::PropPtr(m_hWnd, true);
	// �ڶ�������Ϊ true����û�����ù��������ԣ����ڿ��ٸ������ԵĿռ�
	if (pProp==NULL) return hFontNew; //�������ڶ���������Ϊ true���� pProp ��Ϊ 0 ��ʾ����
	
	// ɾ�����������
	if (pProp->hFont) DeleteObject(pProp->hFont);	
	// ���·���Ŀռ�� hFont ��Ա��¼���������
	pProp->hFont = hFontNew;

	// ���������������
return hFontNew;
}




