//////////////////////////////////////////////////////////////////////
// BForm.cpp: һЩȫ�ֺ������� DoEvents ������ �� WinMain �����Ķ��壬
//            CBForm ���ʵ��
//   
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include "BForm.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")	// ʹ�������� comctl32.lib ��




//////////////////////////////////////////////////////////////////////////
// ȫ�ֺ��� �� WinMain ����
//////////////////////////////////////////////////////////////////////////

  
// ��װ WinMain �������û��Կɽ� main() ��Ϊ��������
int main();
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char * lpCmdLine, int nShowCmd )
{
	// ��ʼ��ͨ�ÿؼ���
	InitCommonControls();  // ������⹫�к����У����� SHFileOperation �ƺ�Ҳ�ܴﵽͬ����Ч����ִ��SHFileOperation(0);���ɣ�

	// ���� CBApp ���󣬽��йس�����Ϣ�������С��ö�����Ϊ WinMain �ľֲ�����
	//   ����������ȫ�̶��Ǵ��ڵģ���Ϊ WinMain ��������������ͽ�����
	// ʹȫ��ָ����� pApp ָ�� app����ȫ�ֳ����п�ʹ�� (*pApp) ���ʴ˱���
	CBApp app(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	pApp=&app;
	
	// ���� main ������main �������û����Լ���ģ�����Լ����塣���������
	//   ������� main ����
	int retMain=main();

	// ���뱾�̵߳���Ϣѭ�����������û� main() ����ִ�н��������
	// ע���û� main() ������������������δ����
	MessageLoop(0);		// ���� 0 ��ʾ����Ϣѭ����GetMessage �յ� 0 �Ž���

	// WinMain ���أ�����ֵΪ�û� main() �����ķ���ֵ
	return retMain;
}


// ��Ϣѭ������
// bExitStyle=0ʱ��Ϊ����Ϣѭ��������Ϣ�����޴���ʱ�� return
// bExitStyle>1ʱ��ʵ���� +1 ��� ms_iModalLevel ��ֵ
//   ����ģ̬�Ի�����ʾ�����Ϣѭ����ÿ��ʾһ��ģ̬�Ի����½�һ��
//   �µ� MessageLoop���� ms_iModalLevel<iExitStyle�����޴���ʱ return
// bExitStyle=-1ʱ������ DoEvents()��
//   ��ǰ�߳�����Ϣ��PeekMessage ����0��ʱ�� return
// ���� bExitStyle Ϊ���٣��� GetMessage �յ� 0 ���� return 
//   ���� return ǰ�ٴ� PostQuitMessage(0); �Խ��˳���Ϣ����
//   ��ǰ������ MessageLoop ʹǰ������ MessageLoop �����˳�
static void MessageLoop(int iExitStyle/*=0*/)
{
	// ���뱾�̵߳���Ϣѭ��������ñ��߳��е�������Ϣ��Ȼ����Ϣ
	//   �ɷ����������ĸ��ԵĴ��ڹ�����
	// ��ʾģ̬����ʱ���ٴε��ñ������������� iExitStyle Ϊ ms_iModalLevel��>0��
	//   ��ģ̬�������ػ�ж��ʱ���˳��½�����õı������������ϲ���õı����� 
	//   ���һ���� WinMain �����õı�����

	MSG msg;
	int iret=-1;		// ���ܳ�ʼ��Ϊ0���� while ���������жϣ�iret==0 ��ʾ�յ� WM_QUIT ��Ϣ

	while (1) 
	{
		if (iExitStyle>0)		// ����ģ̬���壺ms_iModalLevel<iExitStyle�����޴���ʱ return
		{
			if ( CBForm::ms_iModalLevel<iExitStyle ||  
				CBForm::ms_hashWnd.Count()==0 ) break;
		}
		else if (iExitStyle<0)	// ���� DoEvents����ǰ�߳�����Ϣ�� return
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)==0) break;
		}
		else					// ����Ϣѭ��������Ϣ���޴���ʱ�� return
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)==0  &&  
				CBForm::ms_hashWnd.Count()==0 ) break;
		}
		
		iret=GetMessage(&msg, NULL, 0, 0);	// ��ñ��߳����д��ڵ���Ϣ

		// GetMessage����᷵�� -1���� 
		// while (GetMessage( lpMsg, hWnd, 0, 0)) ... �ǲ���ȡ��
		// ���� 0 ��ʾ���߳��յ��˳��ź� WM_QUIT ��Ϣ���˳���Ϣѭ��
		if (iret==-1 || iret==0) break;	

		// ������ټ�
		// �� TranslateAccelerator ת���ʹ����Ӧ��Ϣ������м��ټ������ټ����Ϊ iret��
		// ����ת��������Ҫ�� IsDialogMessage��TranslateMessage��DispatchMessage
		// ����ת��Ϊ���ټ�����Ϣ TranslateAccelerator ����������
		if (CBForm::ms_hAccelCurrUsed && CBForm::ms_hWndActiForm) 
		{
			if (TranslateAccelerator(CBForm::ms_hWndActiForm, CBForm::ms_hAccelCurrUsed, &msg)) continue;
		}
		
		// ����Ի�����Ϣ���簴 Tab ��ת�ؼ�����ȣ�
		// �� IsDialogMessage ת���ʹ����Ӧ��Ϣ
		// �������Ϣ�ѱ� IsDialogMessage ������Ҫ�� TranslateMessage��DispatchMessage
		// �ǶԻ������Ϣ IsDialogMessage �������� TranslateMessage��DispatchMessage
		
		if (msg.message == WM_KEYDOWN && (msg.wParam==13 || msg.wParam==27))
		{
			// ���»س� �� ���� ESC ������ IsDialogMessage ����
			//   ���� ���»س� �� ���� ESC �ᱻ IsDialogMessage ת��Ϊ WM_COMMAND ��Ϣ
			//   ��wParam==1 �� 2����ʹ����������ؼ������������� �س���ESC
			// ��Ϣת�����ɷ�
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!IsDialogMessage(CBForm::ms_hWndActiForm, &msg))	
			{
				// ��Ϣת�����ɷ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}	// end of while (1)

	// ����������յ������˳���Ϣ ���˳��� while ѭ����iret==0��
	//   �ٴ� PostQuitMessage(0); �Խ��˳���Ϣ����
	//   ��ǰ������ MessageLoop ʹǰ������ MessageLoop �����˳�
	if (iret==0) PostQuitMessage(0);
}


void DoEvents()
{
	MessageLoop(-1);
}


extern void End( int nExitCode/*=0*/ )
{
	PostQuitMessage(nExitCode);
}


//////////////////////////////////////////////////////////////////////////
// CBForm ���ʵ��
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ���� CBForm ��� ��Ԫ����


// ���ô��ڹ��̣����б�����󣨴��壩���ô˺�����Ϊ���ڹ���
// CBForm �����Ԫ����
static BOOL WINAPI CBForm_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// =======================================================================
	// �������в��� PostQuitMessage
	// ��Ϊ��һ���Ի����� PostQuitMessage ����������Ͷ��ر��ˣ�������ֻ��
	//   ��һ���Ի��򱻹ر�
	// =======================================================================
	switch(uMsg)
	{
	case WM_INITDIALOG:
		// ===================================================================
		// lparam ӦΪһ������ĵ�ַ�����˵�ַ�� hwnd �����Ĺ�ϵ�����ϣ��
		if (lParam)
		{
			// =================================================================
			// ============ ���´��崴����������Ϣ��¼�� ms_hashWnd ============
			// =================================================================

			// �ݴ�����ϣ�����Ѵ��ڼ�Ϊ hwnd ����Ŀ������ɾ�����������µ����ݸ���
			if (CBForm::ms_hashWnd.IsKeyExist((long)hWnd)) 
				CBForm::ms_hashWnd.Remove((long)hWnd,false); 
			
			// ���ϣ����������Key=hwnd��Data=�����ַ��
			//   ItemLong=���ټ������ItemLong2=��ģ̬�Ի���Ӱ��� Enabled ״̬��
			CBForm::ms_hashWnd.Add(lParam, (long)hWnd, 0, 0);
			
			// ���ö����е� m_hWnd ��ԱΪ ���ھ��
			((CBForm *)lParam)->m_hWnd = hWnd;

			// ���ö����е� ������Ա
			memset( ((CBForm *)lParam)->m_ClassName, 0, sizeof( ((CBForm *)lParam)->m_ClassName));
			((CBForm *)lParam)->m_atom = GetClassLong(hWnd, GCW_ATOM);
			GetClassName(hWnd, ((CBForm *)lParam)->m_ClassName, 
				sizeof( ((CBForm *)lParam)->m_ClassName ) / sizeof(TCHAR)-1 );
			
			// ���� Form_Load �¼�
			((CBForm *)lParam)->EventsGenerator(WM_INITDIALOG, wParam, lParam);
		}

		return 1; // ���� True, Windows ���Զ������뽹��ŵ���һ���� WS_TABSTOP �Ŀؼ���

		break;

	default:
		
		// ===================================================================
		// ���ø��Զ���� EventsGenerator�����ߴ�����Щ��Ϣ����Ҫʱ�����¼�
		//   ���������� EventsGenerator �ķ���ֵ
		// ===================================================================

		CBForm *pForm;
		pForm=0;
		if (CBForm::ms_hashWnd.IsKeyExist((long)hWnd))
		{
			pForm=(CBForm *)CBForm::ms_hashWnd.Item((long)hWnd, false);
			if (pForm) 
			{ 
				return pForm->EventsGenerator(uMsg, wParam, lParam);
			}
		}
	}

	return 0;
}


// ���ô��ڹ��̣����б�����󣨴��壩�е��Ӵ��ڿؼ������ô˺�����Ϊ���ڹ��̣��������Ӵ��ڿؼ��������ദ��
// CBForm �����Ԫ����
static int CALLBACK CBForm_ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	CBForm *pForm = 0; 
	long r; 

	// ������Ϣ�͵� ��Ӧ�������.EventsGeneratorCtrl
	if (CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
	{
		// ItemLong = ��λ�ڴ���� hWnd�����Դ�Ϊ key �� ms_hashWnd �л�ô���
		//   CBForm ����ĵ�ַ
		long lHWndForm=CBForm::ms_hashCtrls.ItemLong((long)hWnd, false);
		pForm = (CBForm *)CBForm::ms_hashWnd.Item(lHWndForm, false);
		if (pForm) 
		{ 
			r = pForm->EventsGeneratorCtrl(hWnd, uMsg, wParam, lParam);
			if (r != gc_APICEventsGenDefautRet) return r;
		}
	}


	// �� CBForm::ms_hashCtrls �л�ñ����ڵ�Ĭ�ϴ��ڳ���ĵ�ַ��������Ĭ�ϴ��ڳ���
	return CallWindowProc ((WNDPROC)(CBForm::ms_hashCtrls.Item((long)hWnd,false)), 
		hWnd, uMsg, wParam, lParam);
}


// ö���Ӵ��ڿؼ������ദ�������Ӵ��ڿؼ��Ļص�������
// lParam Ϊ�ؼ���������ľ��ʱ����0������ʾ���ش���ʱ�����ദ��
// lParam Ϊ 0 ʱ����ʾж�ش���ʱ�Ļָ����ദ��
// CBForm �����Ԫ����
static BOOL CALLBACK EnumChildProcSubClass(HWND hWnd, LPARAM lParam)
{
	if (lParam)
	{
		// ======== �������ʱ��WM_INITDIALOG�����õ� ========
		// �������ڵ��Ӵ���
		EnumChildWindows(hWnd, EnumChildProcSubClass, lParam);

		// ���������Ӵ��ھ��� WS_CLIPSIBLINGS ���
		// ����ؼ��˴˸���ʱ���󷽿ؼ��ػ�ʱ�����ǰ���ؼ�
		SetWindowLong( hWnd, GWL_STYLE,
			GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPSIBLINGS ) ;

		// ���� ComboBox ���߶�̫С�������������������
		// ��������ַ��� => strClassName
		TCHAR strClassName[128];
		GetClassName(hWnd, strClassName, sizeof(strClassName)/sizeof(TCHAR)-1);
		if (_tcscmp(strClassName, TEXT("ComboBox"))==0)
		{
			CBWndBase ctrl(hWnd, (HWND)lParam);	// �� CBWndBase �����޸� ComboBox �߶ȣ�lParam Ϊ��������
			int heightReq = SendMessage(hWnd, CB_GETITEMHEIGHT, 0, 0) *10; 
			if ( ctrl.Height() < heightReq ) ctrl.HeightSet(heightReq);
		}

		// �������Ӵ��ڿؼ����ദ��Control_Proc�����������ǵ�ԭ���ڳ����ַ
		//   ���� CBForm ��ľ�̬��Ա ��ϣ�� ms_hashCtrls
		// �� lParam ����������ľ���������ϣ��Ԫ�ص� ItemLong
		if (! CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
		{
			// ItemLong2 ��Ϊ0����ʾû�����ù��������ԣ�ֻ�������Ǹ�������ʱ
			//   �ٶ�̬���� STRControlProp �Ŀռ䣬ItemLong2 ��Ϊ�ռ��ַ����Ϊ0��
			CBForm::ms_hashCtrls.Add((long)GetWindowLong(hWnd,GWL_WNDPROC), (long)hWnd,
				(long)lParam, 0, 0, 0, 0, false);	
			SetWindowLong(hWnd, GWL_WNDPROC, (long)CBForm_ControlProc);
		}

		// ���ط�0ֵ���Լ���ö��ͬ�������Ӵ��ڿؼ�
		return 1;
	}
	else  // (lParam == 0)
	{
		// ======== ����ж��ʱ��WM_DESTROY�����õ� ========
		// ���������Ӵ��ڿؼ��������Ϣ
		//   �� CBForm ��ľ�̬��Ա ��ϣ�� CBForm::ms_hashCtrls �е������Ŀ
		if (CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
		{
			// ɾ���������ԵĿռ䣨������ù��������ԵĻ���
			STRControlProp * pProp = 
				(STRControlProp *)CBForm::ms_hashCtrls.ItemLong2((long)hWnd, false);
			if (pProp) 
			{
				// ɾ����ָ STRControlProp �Ŀռ�
				if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);

				// ɾ����ָ tagString ���ַ����ռ�
				if (pProp->tagString) delete [](pProp->tagString);

				// ɾ���������
				if (pProp->hFont) DeleteObject(pProp->hFont);

				// ����ɾ��������ϵͳ���Զ�ɾ��
				// pProp->hCursor

				// ɾ���������ԵĿռ�
				delete pProp;	
			}

			// �ָ����ദ��ԭ���ڳ����ַλ�� ms_hashCtrls.Item �У�
			SetWindowLong(hWnd, GWL_WNDPROC, 
				CBForm::ms_hashCtrls.Item((long)hWnd, false));
			
			// ɾ�� ms_hashCtrls �еĶ�Ӧ��Ŀ
			CBForm::ms_hashCtrls.Remove((long)hWnd, false);
		}

		// �������ڵ��Ӵ���
		EnumChildWindows(hWnd, EnumChildProcSubClass, lParam);
		
		// ���ط�0ֵ���Լ���ö��ͬ�������Ӵ��ڿؼ�
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////
// �������е� static ��Ա�� static ����

CBHashLK CBForm::ms_hashWnd;
CBHashLK CBForm::ms_hashCtrls;


// ģ̬�Ի��� �Ĳ��
int CBForm::ms_iModalLevel = 0;


// ���ټ�����ͼ��ټ�Ҫ���͵���Ŀ�괰��
HACCEL CBForm::ms_hAccelCurrUsed=NULL;  
HWND CBForm::ms_hWndActiForm=NULL;

// ȫ�ִ����������������ֵ��EStandardCursor ö��ֵ����ԴID
//   0 ��ʾʹ�ø��Դ��ڵ�����꣬����ʹ��ȫ��ͳһ���
long CBForm::ms_CursorGlobalIdx = 0;
//  ȫ�ִ������������������ ms_CursorGlobalIdx ��0ʱ����Ч��
HCURSOR CBForm::ms_hCursorGlobal = 0;


long CBForm::MousePointerGlobal()
{
	return ms_CursorGlobalIdx;
}

void CBForm::MousePointerGlobalSet( EStandardCursor cursor )
{
	MousePointerGlobalSet((long)cursor);
}

void CBForm::MousePointerGlobalSet( long idResCursor, LPCTSTR typeRes/*=0*/ )
{
	ms_CursorGlobalIdx = idResCursor;
	
	if (ms_CursorGlobalIdx)
	{
		// ���ع�꣬������� ms_hCursorGlobal
		// =============================================================
		// LoadCursor ������ʹ�ظ������ã�Ҳ�����ظ�������Դ��ϵͳ���ж�
		//   �����Ӧ����Ѿ����أ�LoadCursor ֱ�ӷ��ؾ��
		// =============================================================
		if ( ms_CursorGlobalIdx > gc_IDStandCursorIDBase)
		{
			// ��׼���
			// ms_CursorGlobalIdx-gc_IDStandCursorIDBase ���Ǳ�׼����ID��
			ms_hCursorGlobal = 
				LoadCursor(NULL, 
				  MAKEINTRESOURCE(ms_CursorGlobalIdx-gc_IDStandCursorIDBase));
		}
		else
		{
			// ��Դ���
			// ms_CursorGlobalIdx ������Դ ID
			if (typeRes==0)
			{
				// ���� Cursor ���͵���Դ
				ms_hCursorGlobal =
					LoadCursor(pApp->hInstance, MAKEINTRESOURCE(ms_CursorGlobalIdx));
			}
			else
			{
				// �����Զ������͵���Դ��typeRes ���͵���Դ��
				unsigned long size=0; 
				unsigned char * p= LoadResData(ms_CursorGlobalIdx, typeRes, &size);
				ms_hCursorGlobal = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (ms_CursorGlobalIdx)
	{
		// ���������ù�꣬ʹ��Ĭ�ϣ����� ms_hCursorGlobal Ϊ 0
		ms_hCursorGlobal = 0;
	}		// end if (ms_CursorGlobalIdx)

	// ���򱾳���ǰǰ̨���ڷ��� WM_SETCURSOR��ǰ̨���ڴ��� WM_SETCURSOR ��ʹ���������Ч
	SendMessage(GetActiveWindow(), WM_SETCURSOR, (WPARAM)GetActiveWindow(), 0);
	// �ڱ���������д��ڣ������Ӵ��ڣ��ġ����Ͻ��յ��� WM_SETCURSOR ��Ϣ�л�ı������
}

int CBForm::FormsCount()
{
	return ms_hashWnd.Count();
}

CBForm * CBForm::FormsObj( int index )
{
	return (CBForm *)ms_hashWnd.ItemFromIndex(index, false);
}


STRControlProp * CBForm::PropPtr( HWND hwndCtrl, bool bCreate/*=false*/ )
{
	STRControlProp * pProp = 
		(STRControlProp *)ms_hashCtrls.ItemLong2((long)hwndCtrl, false);
	if (bCreate && pProp==NULL)
	{
		pProp=new STRControlProp;
		HM.ZeroMem(pProp, sizeof(STRControlProp));	// �¿ռ���0
		pProp->backColor=-1;	// -1 ��ʾʹ��Ĭ����ɫ
		pProp->foreColor=-1;	// -1 ��ʾʹ��Ĭ����ɫ
		// �¿ռ��ַ��¼�� ms_hashCtrls.ItemLong2Set
		if (! ms_hashCtrls.ItemLong2Set((long)hwndCtrl, (long)pProp, false))
		{ delete pProp;  pProp = 0;  }   // ʧ���ݴ�
	}
	return pProp;
}


//////////////////////////////////////////////////////////////////////////
// ���������

// ���캯��
CBForm::CBForm( unsigned short int idResDialog /*=0*/)
{
	mResDlgID = idResDialog;
	m_hWnd = 0;
	m_ModalShown = false;	// �Ƿ�����ģ̬�Ի�����ʾ�ı�־
	m_BackColor = -1;		// ���ڱ���ɫ��-1 ��ʾʹ��Ĭ��ɫ�����޸Ĵ����࣬����ͨ����Ӧ WM_ERASEBKGND ʵ�֣�
	m_CursorIdx = 0;		// ���嵱ǰ������������ţ�0 ��ʾʹ��ϵͳĬ��
	m_hFont = 0;				// ���嵱ǰ�������������Ϊ0ʱ��ʾʹ��ϵͳĬ�����壬�����ʾ���Զ������壩

	// �๫�г�Ա����ֵ
	KeyPreview = false;		// �Ƿ������ɴ��崦�����пؼ��ļ��̰�����Ϣ
}

// ��������
CBForm::~CBForm()
{
	UnLoad();
	ClearResource();
}


//////////////////////////////////////////////////////////////////////////
// ���г�Ա����

HWND CBForm::hWnd()
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	return m_hWnd;
}


HMENU CBForm::hMenu()
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	return GetMenu(m_hWnd);
}


void CBForm::hMenuSet( HMENU hMenuNew )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���

	SetMenu(m_hWnd, hMenuNew); 
	if (hMenuNew)
	{	
		ConstructMenuIDItems(GetMenu(m_hWnd), true);
		ConstructMenuIDItems(GetSystemMenu(m_hWnd, 0), false);	// ϵͳ�˵�
	}
	else
	{
		ConstructMenuIDItems(NULL, true);
		ConstructMenuIDItems(GetSystemMenu(m_hWnd, 0), false);	// ϵͳ�˵�
	}
}


HACCEL CBForm::hAccel()
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	return (HACCEL)ms_hashWnd.ItemLong((long)m_hWnd, false);
}


void CBForm::hAccelSet( HACCEL hAccelNew )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ��أ�Ȼ��������ü��ټ�
	
	ms_hashWnd.ItemLongSet((long)m_hWnd, (long)hAccelNew, false);	// ��¼�� ms_hashWnd �� ItemLong
	// �������ǰ̨�������Ǳ�����Ĵ��壬������ CBForm::hWndAccel��CBForm::hAccel
	if (m_hWnd==GetActiveWindow())
	{
		CBForm::ms_hWndActiForm = m_hWnd; 
		CBForm::ms_hAccelCurrUsed = hAccelNew; 
	}
}


bool CBForm::Show( int modal/*=0*/, HWND hwndOwner/*=NULL*/ )
{
	// ����ģ̬��ʽ��ʾ���壬������д��Ϣѭ�������²��ܴ�����ټ�
	//   �ڱ����У����д����Է�ģ̬��ʽ��ʾ
	// ���ڡ�ģ̬����ʵ�֣��Ǳ�����ͨ�������ж��㴰��ȫ�� 
	//   Enabled=False ʵ�ֵ�

	HWND hwnd;
	HWND hWndActiLast = GetActiveWindow(); 

	if (0==m_hWnd) 
	{	
		// ����δ���ضԻ������ڼ���
		// ���д����Է�ģ̬��ʽ CreateDialogParam ����
		hwnd=Load(hwndOwner);   
		if (hwnd==0) return false;
	}
	else 
	{	
		// �Ի����Ѿ����أ�����ʾʱ���ܻ�ı��� Owner
		// ͨ���������ı䴰��� Owner ������Parent��
		//   MSDN ��û�н��ܸı� Owner �ķ�����Ӧ������  
		//   SetWindowLong �� -8 ���� Owner
		if (hwndOwner !=0 && hwndOwner != GetWindow(m_hWnd, GW_OWNER)) 
			SetWindowLong(m_hWnd, -8, (long)hwndOwner); 
	}

	if (modal)
	{
		// ======== ��ʾģ̬�Ի��� ========

		// ����ʾһ��ģ̬�Ի���ʱ��
		//   ������ĳ����ĿǰΪ Disabled �� ms_hashWnd.ItemLong2 ֵΪ0����ά�ָô��ڵĴ�ֵ���䣻
		//   ��������ĳ����ĿǰΪ Enabled�����߸ô��ڴ�ֵ >0�����Ὣ�ô��ڵĴ�ֵ +1
		// ������һ��ģ̬�Ի���ʱ��
		//   ��ĳ���ڴ�ֵΪ0�������κβ��������򽫴�ֵ-1����-1��Ϊ0����ָ�Ϊ Enabled ״̬

		int i; 
		BOOL ena; 
		long level=0; 
		HWND hwndEach=0;
		for (i=1; i<=ms_hashWnd.Count(); i++)
		{
			hwndEach=(HWND)ms_hashWnd.IndexToKey(i);
			if (hwndEach==m_hWnd) 
			{
				// ��ģ̬���壬������������ ItemLong2 Ϊ 0
				ms_hashWnd.ItemLong2FromIndexSet(i, (long)0, false);
				continue;	
			}

			
			// ���ݴ��� i �ĵ�ǰ enabled ״̬�� ItemLong2��level�� �����ж�
			ena = IsWindowEnabled(hwndEach);
			level = ms_hashWnd.ItemLong2FromIndex(i, false); 
			if (ena || level>0)	
			{	
				// ��¼����Ҫ�ѱ����ڻָ���Enabled
				ms_hashWnd.ItemLong2FromIndexSet(i, level+1, false);	
			}
			// else ������ԭΪ Disabled����  ItemLong2��level�� Ϊ0
			//   ����ά�ִ�ֵΪ0�����䣩

			// ������ i ����Ϊ Diabled
			EnableWindow(hwndEach, 0);
		}


		// ���ñ��������ʾģ̬��־
		m_ModalShown = true;
	
		// ģ̬��� ++
		if (ms_iModalLevel<0) ms_iModalLevel=0;		// �ݴ�
		ms_iModalLevel++;

		// ��ʾ��ģ̬������
		ShowWindow(m_hWnd, SW_NORMAL);
		
		// ��������Ҫ��������
		// �ٴε��� MessageLoop�������ò���Ϊ 1
		// ֱ����ģ̬�����屻���ػ�رպ������ MessageLoop �����ŷ���
		MessageLoop(ms_iModalLevel); 
	}
	else
	{
		// ======== ��ʾ��ģ̬�Ի��� ========
		
		if ( ms_iModalLevel>0 )	
		{
			// �������ģ̬�Ի���������ʾ��ԭ���ϲ���������ʾ
			//   ��ģ̬�Ի��򡣵���������ֻҪ���¶Ի�����Ϊ Disabled ����
			// �¶Ի���� ItemLong2 ֵӦΪ ms_iModalLevel
			ms_hashWnd.ItemLong2Set((long)m_hWnd, ms_iModalLevel, false);
			EnableWindow(m_hWnd, 0);

			// ��ʾ�´��ڣ�����Ҫ���´�����Ϊǰ̨��
			ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);	
			SetActiveWindow(hWndActiLast);	// �ָ�ԭ��ǰ̨������Ϊǰ̨
		}
		else
		{
			ShowWindow(m_hWnd, SW_NORMAL);	
		}
		
		// �Է�ģ̬��ʾ�����ñ�־
		m_ModalShown = false;
	}

	return true;
}


long CBForm::Hide()
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	return ShowWindow(m_hWnd, SW_HIDE);
}


// ���ضԻ��򵫲�����ʾ������ֻ���Է�ģ̬��ʽ���أ�
// ����Ҫ�Ի��򱻼��غ���ܵ��õ�һЩ������ SetAccelerator �У����Զ����ñ�����
HWND CBForm::Load( HWND hwndOwner/*=NULL*/ )
{
	// һ���ԡ���ģ̬����ʽ���ش��壺���� CreateDialogParam
	// �� dwInitParam ������Ϊ�������ַ��this��
	//   �˽���Ϊ WM_INITDIALOG �� lParam �������� CBForm_DlgProc
	HWND hwnd;
	hwnd=CreateDialogParam(pApp->hInstance, MAKEINTRESOURCE(mResDlgID), hwndOwner, 
	  CBForm_DlgProc, (long)this);	// �������أ����ش��ھ�����ڴ��ڹ��̴��� WM_INITDIALOG ʱ������ m_hWnd
	
	// ���û�� WS_VISIBLE ��ʽ��ʱ���岻���Զ���ʾ
	
	if (hwnd)
	{
		// ��ʼ���˵�
		ConstructMenuIDItems(GetMenu(hwnd),true);	// ���޲˵�������Ϊ0��������˵���ϣ���¼
		ConstructMenuIDItems(GetSystemMenu(hwnd, 0), false);	// ϵͳ�˵�
	}

	return hwnd;
}



// �����Ի���
// ���յ� WM_CLOSE ��Ϣʱ���Զ����ñ�������Ҳ��ֱ�����û�����
void CBForm::UnLoad() const
{
	SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}



// ���ñ����ڽ�������ټ�����Դ ID Ϊ idResAcce
// ��ȡ�����ټ�����������Ϊ 0 ����
void CBForm::SetAccelerator( unsigned short int idResAcce )
{
	HACCEL hAcc=NULL;
	if (idResAcce) hAcc=LoadAccelerators(pApp->hInstance, MAKEINTRESOURCE(idResAcce)); 
	hAccelSet(hAcc);	// ���ù��з��� hAccelSet ����
}

// �������ñ����ڵĲ˵�
void CBForm::SetMenuMain( unsigned short idResMenu )
{
	HMENU hMenu=NULL;
	if (idResMenu)
	{
		hMenu=LoadMenu(pApp->hInstance, MAKEINTRESOURCE(idResMenu));
		hMenuSet(hMenu);	// ���ù��з��� hMenuSet ����
	}
	else
		hMenuSet(NULL);		// ���ù��з��� hMenuSet ����
}


// ����һ���ؼ���CBControl ���󣩣�֮��ɶԸÿؼ����в��������� CBControl ������Է�����
CBControl CBForm::Control( unsigned short int idResControl )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	if (m_Control.SetResID(m_hWnd, idResControl, &m_hashObjs))
		return m_Control;
	else
		return 0;
}


// ����һ���˵��CBMenuItem ���󣩣�֮��ɶԸò˵�����в��������� CBMenuItem ������Է�����
CBMenuItem CBForm::Menu( UINT idResMenuItem )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���

	// ���� idMenuItem���ڹ�ϣ���в���������Ӧ�ĸ��˵��� ��� -> hMenu
	HMENU hMenu;
	hMenu = (HMENU)m_hashMenuIDs.Item(idResMenuItem, false);

	if (hMenu)
	{
		m_MenuItem.SetFromResID(hMenu, idResMenuItem, m_hWnd, &m_hashMenuIDs, &m_hashObjs); 
		return m_MenuItem;
	}
	else
		return 0;
}

CBMenuItem CBForm::Menu( int pos1, int pos2, int pos3/*=0*/, int pos4/*=0*/, int pos5/*=0*/, int pos6/*=0*/, int pos7/*=0*/ )
{
	HMENU hMenu;

	if (pos1>0)
	{
		// ������˵�
		hMenu = GetMenu(m_hWnd);

		if (pos2)
		{
			hMenu = GetSubMenu(hMenu, pos1 - 1);		//-1��Ϊ��0��ʼ��ţ���ö���˵��ľ����������pos1==1��hMenuSubΪ�������ļ����˵��ľ��
		}
		else
		{
			m_MenuItem.SetFromPos(hMenu, pos1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// ���ض����˵���һ���˵��λ�ò�����1��ʼ����������pos1==1�����ء��ļ����˵���
			return m_MenuItem;
		}
	}
	else if (pos1<0)
	{
		// ���ϵͳ�˵�
		hMenu = GetSystemMenu(m_hWnd, 0);
		if (pos2==0)
		{
			// ����ϵͳ�˵��ġ����˵��
			//  �����������Ӳ˵����Ϊ GetSystemMenu���������Ӳ˵��ĵ�һ���ǡ���ԭ����
			m_MenuItem.SetFromPos(0, 1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);		// 1-1 == 0
			return m_MenuItem; 
		}
	}
	else	// pos1 == 0
	{
		// ��ö����˵�
		hMenu = GetMenu(m_hWnd);
		if (pos2==0)
		{
			// ���ض���˵��ġ����˵��
			//  �����������Ӳ˵����Ϊ GetMenu���������Ӳ˵��ĵ�һ���ǡ��ļ�����
			m_MenuItem.SetFromPos(0, 2, m_hWnd, &m_hashMenuIDs, &m_hashObjs);
			return m_MenuItem; 
		}
	}
	

	if (pos3)
	{
		hMenu = GetSubMenu(hMenu, pos2 - 1);		//-1��Ϊ��0��ʼ���
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos2, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// ����һ���˵���һ���˵��λ�ò�����1��ʼ����������pos1==1��pos2==2�����ء��ļ���-���򿪡��Ĳ˵���
		return m_MenuItem;	
	}

	if (pos4)
	{
		hMenu = GetSubMenu(hMenu, pos3 - 1);		//-1��Ϊ��0��ʼ���
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos3, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// λ�ò�����1��ʼ
		return m_MenuItem;	
	}

	if (pos5)
	{
		hMenu = GetSubMenu(hMenu, pos4 - 1);		//-1��Ϊ��0��ʼ���
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos4, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// λ�ò�����1��ʼ
		return m_MenuItem;	
	}

	if (pos6)
	{
		hMenu = GetSubMenu(hMenu, pos5 - 1);		//-1��Ϊ��0��ʼ���
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos5, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// λ�ò�����1��ʼ
		return m_MenuItem;	
	}

	if (pos7)
	{
		hMenu = GetSubMenu(hMenu, pos6 - 1);		//-1��Ϊ��0��ʼ���
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos6, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// λ�ò�����1��ʼ
		return m_MenuItem;	
	}

	m_MenuItem.SetFromPos(hMenu, pos7, m_hWnd, &m_hashMenuIDs, &m_hashObjs);		// λ�ò�����1��ʼ
	return m_MenuItem;	

}


CBMenuItem CBForm::Menu( ESysMenu idSysMenu )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���
	
	return this->Menu((UINT)idSysMenu);
}

void CBForm::MenuSysRestore() const
{
	GetSystemMenu(m_hWnd, 1);
}


BOOL CBForm::PopupMenu( UINT idResMenu, int x, int y, bool bAllowRightClick/*=true*/ )
{
	HMENU hMenuPop;
	BOOL ret;
	UINT flags=0;
	POINT pt; 
	hMenuPop = LoadMenu(pApp->hInstance, MAKEINTRESOURCE(idResMenu)); 
	if (bAllowRightClick) flags = flags | TPM_RIGHTBUTTON;
	pt.x=x; pt.y=y; 
	ClientToScreen(m_hWnd, &pt);
	ret = TrackPopupMenu(GetSubMenu(hMenuPop, 0), flags, pt.x, pt.y, 0, m_hWnd, NULL);
		// TrackPopupMenu ֻ�ܵ��� popup ʽ�˵���LoadMenu �õ��Ĳ��� popup ʽ�˵���
		//   Ӧ�� GetSubMenu ȡ���Ӳ˵�������ֻȡ��0����Ӳ˵���
	DestroyMenu(hMenuPop);
	return ret;
}


void CBForm::IconSet(EStandardIcon icon)
{
	if (m_hWnd==NULL) Load();					// ������ڻ�δ���أ����ڼ���
	HICON hIco = LoadIcon(0, (LPCTSTR)icon);	// ��õ���shared icon����Ҫ DestroyIcon
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIco);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIco);
}

void CBForm::IconSet(unsigned short iconRes)
{
	if (m_hWnd==NULL) Load();		// ������ڻ�δ���أ����ڼ���
	HICON hIco = LoadIcon(pApp->hInstance, (LPCTSTR)((DWORD)iconRes));	// ��õ���shared icon����Ҫ DestroyIcon
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIco);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIco);
}


COLORREF CBForm::BackColor()
{
	if (-1 == m_BackColor)
	{
		// ��ô�����ı���ɫ
		HBRUSH hBrush;
		LOGBRUSH lb;
		hBrush=(HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND);
		if (hBrush) 
		{
			GetObject((HGDIOBJ)hBrush,sizeof(lb), &lb);
			return lb.lbColor;
		}
		else
		{
			// GetClassLong ʧ�ܣ��򴰿����޻�ˢ������ϵͳ COLOR_BTNFACE ��ɫ
			return GetSysColor(COLOR_BTNFACE);
		}
	}
	else
		// ֱ�ӷ��� m_BackColor
		return m_BackColor;		
}

void CBForm::BackColorSet( EColorType color )
{
	m_BackColor = color;
	InvalidateRect(m_hWnd, NULL, true);
}

void CBForm::BackColorSet( COLORREF color )
{
	m_BackColor = color;
	InvalidateRect(m_hWnd, NULL, true);
}



long CBForm::MousePointer()
{
	return m_CursorIdx;
}

void CBForm::MousePointerSet( EStandardCursor cursor )
{	
	MousePointerSet((long)cursor);
}

void CBForm::MousePointerSet( long idResCursor, LPCTSTR typeRes/*=0*/ )
{
	if (m_hWnd==NULL) Load(); // ������ڻ�δ���أ����ڼ���

	m_CursorIdx = idResCursor; 
	if (m_CursorIdx)
	{
		// ���ع�꣬������� m_hCursor
		// =============================================================
		// LoadCursor ������ʹ�ظ������ã�Ҳ�����ظ�������Դ��ϵͳ���ж�
		//   �����Ӧ����Ѿ����أ�LoadCursor ֱ�ӷ��ؾ��
		// =============================================================
		if ( m_CursorIdx > gc_IDStandCursorIDBase)
		{
			// ��׼���
			// m_CursorIdx-gc_IDStandCursorIDBase ���Ǳ�׼����ID��
			m_hCursor = 
				LoadCursor(NULL, MAKEINTRESOURCE(m_CursorIdx-gc_IDStandCursorIDBase));
		}
		else
		{
			// ��Դ���
			// m_CursorIdx ������Դ ID
			if (typeRes==0)
			{
				// ���� Cursor ���͵���Դ
				m_hCursor = LoadCursor(pApp->hInstance, MAKEINTRESOURCE(m_CursorIdx));
			}
			else
			{
				// �����Զ������͵���Դ��typeRes ���͵���Դ��
				unsigned long size=0; 
				unsigned char * p= LoadResData(m_CursorIdx, typeRes, &size);
				m_hCursor = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (m_CursorIdx)
	{
		// ���������ù�꣬ʹ��Ĭ�ϣ����� m_hCursor Ϊ 0
		m_hCursor = 0; 
	}		// end if (m_CursorIdx)

	// �򱾴��ڷ��� WM_SETCURSOR����ʹ���������Ч
	SendMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, 0);
	// �ڱ����ڲ��Ͻ��յ��� WM_SETCURSOR ��Ϣ�л�ı������
}










// ����һ������Ļ��Ӵ��ڿؼ����¼���������ذ汾��
// ptrFunHandler Ϊһ���¼��������ĵ�ַ
// ����Ǵ����¼���idResControl ����Ӧ��Ϊ0
// ����ǿؼ��¼���idResControl ����Ӧ��Ϊ �ؼ�����ԴID
// ����ǲ˵�������ϵͳ�˵��������ټ��¼�����2������Ϊ eMenu_Click�������� idResControl ����
void CBForm::EventAdd( unsigned short int idResControl, 
					  ECBFormCtrlEventsVoid eventType, 
					  ONEventVoid ptrFunHandler )
{
	EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 0);
}

void CBForm::EventAdd( unsigned short int idResControl, 
					   ECBFormCtrlEventsI eventType, 
					   ONEventI ptrFunHandler )
{
	EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 1);
}

void CBForm::EventAdd( unsigned short int idResControl, 
					  ECBFormCtrlEventsII eventType, 
					  ONEventII ptrFunHandler )
{
	EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 2);
}

void CBForm::EventAdd( unsigned short int idResControl, 
					  ECBFormCtrlEventsIII eventType, 
					  ONEventIII ptrFunHandler )
{

	if (eventType == eMenu_Click)
	{
		// ============ �˵�������ϵͳ�˵�������ټ����¼� ============
		// ���� idResControl
		// ���¼���������ַ���� m_hashEventNotify.Item(c_CBMenuClickEventKey)�������� 3 �� int �͵Ĳ���
		if (m_hashEventNotify.IsKeyExist(c_CBMenuClickEventKey)) 
			m_hashEventNotify.Remove(c_CBMenuClickEventKey);
		m_hashEventNotify.Add( (long)ptrFunHandler, c_CBMenuClickEventKey, 3, 0, 0, 0, 0, false); 
	}
	else
	{
		// ============ �����ؼ����¼� ============
		EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 3);
	}

}

void CBForm::EventAdd( unsigned short int idResControl, 
					  ECBFormCtrlEventsIIII eventType, 
					  ONEventIIII ptrFunHandler )
{
	EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 4);
}

// ����һ������Ļ��Ӵ��ڿؼ����¼���������ذ汾�Ĺ��ú�����
void CBForm::EventAddPrivate(unsigned short int idResControl, long eventType, long ptrFunHandler, int nParamsIntCount )
{
	if (eventType & c_CBNotifyEventBase )
	{   
		// ============ ��֪ͨ��Ϣ�����Ŀؼ��¼� ============
		// ���¼�������ַ���� m_hashEventNotify
		// key �ĸ� 2 λΪ WM_COMMAND ��Ϣ�� WM_NOTIFY ��Ϣ��֪ͨ��
		//  ��֪ͨ����� eventType ��ö��ֵ������2λΪ�ؼ�ID
		long key = (long)MAKELONG(idResControl, eventType); 
		if (m_hashEventNotify.IsKeyExist(key))
			m_hashEventNotify.Remove(key);
		m_hashEventNotify.Add(ptrFunHandler, key, nParamsIntCount,0,0,0,0,false);
		
		// ������Ҫ�ķ�񣬷��������Ǵ˷��������Ӧ���¼�
		if (m_hWnd  &&  (eventType | c_CBNotifyEventBase))
		{ 
			// Ϊ�Ӵ��ڿؼ���� BS_NOTIFY ���
			HWND hwndCtrl = GetDlgItem(m_hWnd, (int)idResControl);
			long style=GetWindowLong(hwndCtrl, GWL_STYLE);
			CBControl ctrl;
			ctrl.SetResID(m_hWnd, idResControl, &m_hashObjs);
			if ( ctrl.IsClassName(TEXT("Button")) )
				SetWindowLong(hwndCtrl, GWL_STYLE, style | BS_NOTIFY);
			else if ( ctrl.IsClassName(TEXT("Static")) )
				SetWindowLong(hwndCtrl, GWL_STYLE, style | SS_NOTIFY);
			else if ( ctrl.IsClassName(TEXT("ListBox")) )
				SetWindowLong(hwndCtrl, GWL_STYLE, style | LBS_NOTIFY);
		}

	}
	else	// if (eventType & c_CBNotifyEventBase ) else
	{
		// ============ �Դ�����Ϣ�������¼� ============
		//  ����֪ͨ��Ϣ�� WM_XXX��������������Ӵ��ڿؼ����� 
		//   WM_XXX �����ģ����������¼��Ϳؼ��¼���
		// ���¼�������ַ���� m_hashEventMessage
		// key �ĵ� 2 λΪ��Ϣ WM_XXX��
		//       �� 2 λΪ 0 ���Դ����¼����� �ؼ���ԴID���Կؼ��¼���
		long key = (long)MAKELONG(eventType, idResControl);
		
		// key=WM_COMMAND��Ϣ��wParam����λΪ֪ͨ�룬��λΪ�ؼ�ID����ֵ=�û�������ַ
		if ( m_hashEventMessage.IsKeyExist(key) )
			m_hashEventMessage.Remove(key, false);
		m_hashEventMessage.Add(ptrFunHandler, key, nParamsIntCount,0,0,0,0,false);

		// ������Ҫ�ķ�񣬷��������Ǵ˷��������Ӧ���¼�
		if (eventType == eFilesDrop)	// WM_DROPFILES
		{
			if (idResControl==0)
			{
				// Ϊ������� ���� WM_DROPFILES ��Ϣ
				DragAcceptFiles(m_hWnd, 1);
			}
			else
			{ 
				// Ϊ�Ӵ��ڿؼ���� ���� WM_DROPFILES ��Ϣ
				HWND hwndCtrl = GetDlgItem(m_hWnd, (int)idResControl);
				DragAcceptFiles(hwndCtrl, 1);
			}
		}
	}
}

long CBForm::EventsGenerator( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_COMMAND:	// �Ӵ��ڿؼ�֪ͨ��Ϣ����˵���Ϣ������ټ���Ϣ
		if (lParam)
		{
			// ======== �Ӵ��ڿؼ�֪ͨ��Ϣ ========
			// wparam ��λΪ֪ͨ�룬��λΪ�ؼ�ID��������Ϊ������
			// �� m_hashEventNotify �в������޼�Ϊ wparam ��������
			//   ��ʾ����¼���Ҫ���������û����������¼�
			// �û������ĵ�ַ�� m_hashEventNotify �Ķ�Ӧ��� Data �б���
			if (m_hashEventNotify.IsKeyExist((long)wParam))
			{
				ONEventVoid ptrFunc0 = NULL; 
				ONEventI ptrFunc1 = NULL;
				ONEventII ptrFunc2 = NULL;
				ONEventIII ptrFunc3 = NULL;
				ONEventIIII ptrFunc4 = NULL;
				switch(m_hashEventNotify.ItemLong((long)wParam, false))
				{
				case 0:		// 0 ������
					ptrFunc0 =
						(ONEventVoid)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc0) (*ptrFunc0)();								// �� if �ݴ��ж�
					break;
				case 1:		// 1 �� int �Ͳ���
					ptrFunc1 = 
						(ONEventI)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc1) (*ptrFunc1)((int)wParam);						// �� if �ݴ��ж�
					break;
				case 2:		// 2 �� int �Ͳ���
					ptrFunc2 = 
						(ONEventII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc2) (*ptrFunc2)((int)wParam, (int)lParam);		// �� if �ݴ��ж�
					break;
				case 3:		// 3 �� int �Ͳ���
					ptrFunc3 = 
						(ONEventIII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc3) (*ptrFunc3)((int)LOWORD(wParam), (int)HIWORD(wParam), 
						lParam);												// �� if �ݴ��ж�
					break;
				case 4:		// 4 �� int �Ͳ���
					ptrFunc4 = 
						(ONEventIIII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc4) (*ptrFunc4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
						(int)LOWORD(lParam), (int)HIWORD(lParam));				// �� if �ݴ��ж�
					break;

				}	// end of switch
			}	// end of if (m_hashEventNotify.IsKeyExist((long)wparam))
		}	// end of if (lparam)
		else	// if (lparam) else��lparam==NULL
		{  
			// ======== �˵���Ϣ��ϵͳ�˵����ڴ˴���������ټ���Ϣ ========
			ONEventIII ptrMenuClick;
			ptrMenuClick = (ONEventIII)m_hashEventNotify.Item(c_CBMenuClickEventKey, false);
			if (ptrMenuClick)
			{
				// ����1Ϊ�������Ĳ˵���ԴID��
				// ����2Ϊ��ֱ��ѡ��Ĳ˵�(=0)�����ǴӼ��ټ�ִ�е�(=1)
				// ����3=0 ��ʾ�Ǵ���ͨ�˵�ѡ��ģ�=1��ʾ�Ǵ�ϵͳ�˵�ѡ���
				(*ptrMenuClick)((int)LOWORD(wParam), (int)(HIWORD(wParam)), 0);
			}
		}

		break;

	case WM_NOTIFY:
// 		switch ( ((NMHDR *)lParam)->code )
// 		{
// 		case NM_SETCURSOR:
// 
// 			return 1;
// 			break;
// 		}


		break;

	case WM_INITDIALOG:

		// �������Ӵ��ڿؼ����ദ������ lParam ��Ϊ������ľ�� m_hWnd
		EnumChildWindows( m_hWnd, EnumChildProcSubClass, (LPARAM)m_hWnd );

		// ���� Form_Load �¼�����������еĻ���
		{
			ONEventVoid ptrFuncForm0 = (ONEventVoid)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)();		// �� if �ݴ��ж�
		}
		break;

	case WM_DESTROY:
		// ������ WM_DESTROY ������̡��򸸴��屻�ر�ʱ�����Ӵ�����Զ��رչ���
		//   �����յ� WM_CLOSE ��Ϣ��ֻ���յ� WM_DESTROY ��Ϣ

		// ���� Form_UnLoad �¼�����������еĻ���
		{
			ONEventVoid ptrFuncForm0 = (ONEventVoid)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)();		// �� if �ݴ��ж�
		}
		
		// �� ClearResource �У���ö���Ӵ��ڣ��ָ������Ӵ��ڿؼ������ദ��
		ClearResource();
		break;

	case WM_CLOSE:
		
		// ���� Form_QueryLoad �¼�����������еĻ���
		{
			int bCancel = 0;
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) 
			{
				(*ptrFuncForm0)((int)(&bCancel));		// �� if �ݴ��ж�
				if (bCancel) return bCancel;			// ȡ�� WM_CLOSE ��Ϣ����ж�ش���
			}
		}

		// ���ǰ��û��ȡ����bCancelû��Ϊ��0ֵ��������ж�ضԻ���
		// ������ SendMessage(m_hWnd, WM_DESTROY, 0, 0); 
		DestroyWindow(m_hWnd);
		
		// ��ʱ��Ӧ���� CBForm_DlgProc ���յ��� WM_DESTROY ��Ϣ����ִ���� ClearResource()
		//   ��ʹ m_hWnd Ϊ 0�������ʱ m_hWnd �Բ�Ϊ 0��Ϊ�ݴ������ٴ�ִ�� ClearResource()
		if (m_hWnd) ClearResource(); 

		break;

	case WM_ACTIVATE:
		
		// ���� eForm_ActiDeactivate �¼�������ʧ��ʱ��������еĻ���
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)(0);		// �� if �ݴ��ж�
		}

		// �л���ǰǰ̨����
		ms_hWndActiForm=m_hWnd;

		// �л����ټ�
		if (LOWORD(wParam)==WA_ACTIVE || LOWORD(wParam)== WA_CLICKACTIVE)
		{
			ms_hAccelCurrUsed=(HACCEL)ms_hashWnd.ItemLong((long)m_hWnd, false); 
		}
		

		// ���� eForm_ActiDeactivate �¼�����������ʱ��������еĻ���
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)(1);		// �� if �ݴ��ж�
		}

		break;

	case WM_SHOWWINDOW:
		// ��������������أ����Ҹô���Ϊģ̬���ָ����д��� Enabled
		if (wParam==0)
			if (m_ModalShown) RestoreModalWinEnabled();
		
		break;
		
	case WM_SETCURSOR:
		if (ms_CursorGlobalIdx)
		{
			// ======== ʹ��ȫ������� ========
			if (ms_hCursorGlobal)
			{
				SetCursor(ms_hCursorGlobal);
				return 1;	// return 1; ��ʾ����Ĭ�ϴ��ڳ�������
			}
		}
		else if (m_CursorIdx) 
		{
			// ======== ʹ��Ϊ�����������õ������ ========
			if (m_hCursor)
			{					
				SetCursor(m_hCursor);
				return 1;	// return 1; ��ʾ����Ĭ�ϴ��ڳ�������
			}
		}
		
		// ����ʹ��Ĭ�ϣ��������������꣨Ĭ�ϴ��ڳ����Զ�����
		break;
		
	case WM_ERASEBKGND:
		// ���ô��ڱ���ɫ
		if (m_BackColor != -1)	// -1 ��ʾ����ϵͳĬ����ɫ�������Ԥ
		{
			RECT rectBK;
			HBRUSH hBrushBK = CreateSolidBrush(m_BackColor);
			GetClientRect(m_hWnd, &rectBK);
			FillRect((HDC)wParam, &rectBK, hBrushBK);
			DeleteObject(hBrushBK);
			return 1;	// ��Ӧ�ó������в������������뷵�� 1
		}

		break;
	case WM_CTLCOLORSTATIC: 
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLOREDIT:
		// �����Ӵ��ڿؼ�����ɫ��ǰ��ɫ����Щ��Ϣ�Ƿ��͵������ڵģ����Ƿ��͵��ؼ���
		{
			STRControlProp * pProp = CBForm::PropPtr((HWND)lParam); 
			if (pProp)
			{
				if (pProp->foreColor != -1) // ����ǰ��ɫ
				{
					SetTextColor((HDC)wParam, pProp->foreColor);
					// Ϊʹǰ��ɫ������Ч�����뷵�ر���ɫ��ˢ
					//   �米��ɫû�����ã����ｫ֮����Ϊϵͳ��ɫ
					if (pProp->backColor == -1)
					{
						pProp->backColor = GetSysColor(COLOR_BTNFACE);
						if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);
						pProp->hBrushBack = CreateSolidBrush(pProp->backColor);
					}
				}

				if (pProp->backColor != -1)
				{
					// ���ñ���ɫ
					// �Ƚ����ֱ�����Ϊ͸��
					SetBkMode((HDC)wParam, TRANSPARENT);
					// ���ػ�ˢ
					if (pProp->hBrushBack) return (long)(pProp->hBrushBack);
				}
			}
		}

		break;

	// ==================================================================
	//	�����¼��Ĵ���
	case WM_LBUTTONDOWN: 	case WM_RBUTTONDOWN:	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:		case WM_RBUTTONUP:		case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:	case WM_MBUTTONDBLCLK:	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
		// ������ҡ��м����¡�̧��˫��������ƶ�
		{
			long key = 0;  int button = 0;
			if (uMsg == 0x200)				// ����ƶ�
			{
				key = WM_MOUSEMOVE;
				if ( wParam & MK_LBUTTON ) button = button | 1;
				if ( wParam & MK_RBUTTON ) button = button | 2;
				if ( wParam & MK_MBUTTON ) button = button | 4;
			}
			else if ((uMsg-0x200) % 3 == 1)		// ���£�0x201��0x204��0x207��(uMsg-0x200) % 3 == 1
			{
				key = WM_LBUTTONDOWN;		// ͳһ�� WM_LBUTTONDOWN ������Ϊ3��ö��ֵ
				button = 1 << (((uMsg-0x0200)-1)/3); 
			}
			else if ((uMsg-0x200) % 3 == 2)	// ̧��0x202��0x205��0x208��(uMsg-0x200) % 3 == 2
			{
				key = WM_LBUTTONUP;			// ͳһ�� WM_LBUTTONUP ������Ϊ3��ö��ֵ
				button = 1 << (((uMsg-0x0200)-2)/3);
			}
			else if ((uMsg-0x200) % 3 == 0)	// ˫����0x203��0x206��0x209��(uMsg-0x200) % 3 == 0
			{
				key = WM_LBUTTONDBLCLK;		// ͳһ�� WM_LBUTTONDBLCLK ������Ϊ3��ö��ֵ
				button = 1 << ((uMsg-0x0203)/3);
			}
			else
				break;	// �ݴ�

			if (m_hashEventMessage.IsKeyExist(key))
			{
				ONEventIIII ptrFuncForm4 = NULL;
				ptrFuncForm4 = 
					(ONEventIIII)m_hashEventMessage.Item(key, false);
				if (ptrFuncForm4)
				{
					POINT pt; UINT altKey=0;  
					GetCursorPos(&pt);  ScreenToClient(m_hWnd, &pt);
					if ( GetKeyState(VK_MENU)<0 ) altKey=4;		// Shift=1; Ctrl=2; Alt=4

					(*ptrFuncForm4)(button, 
						altKey | ( ( (wParam & (MK_CONTROL | MK_SHIFT))) /4 ), 
						pt.x, pt.y);

				}	// if (ptrFuncForm4)
			}	// if (m_hashEventMessage.IsKeyExist(key)
		}
		break;

	case WM_CHAR: 
		if (m_hashEventMessage.IsKeyExist(WM_CHAR))
		{
			ONEventII ptrFuncForm2 = NULL;
			int bCancel = 0;
			ptrFuncForm2 = 
				(ONEventII)m_hashEventMessage.Item(WM_CHAR, false);
			if (ptrFuncForm2)
			{
				(*ptrFuncForm2)((int)wParam, (int)(&bCancel)); 
				if (bCancel) return 1;
			}
		}
		break;

	case WM_KEYDOWN: case WM_KEYUP: 		//   shift=1,2,4 �ֱ��ʾͬʱ Shift��Ctrl��Alt ��������
	case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		{
			long key = uMsg;
			if (uMsg>=0x0104) key = (uMsg-0x4);	// ��WM_SYS��Ϣ�鲢��WM_��Ϣ
			if (m_hashEventMessage.IsKeyExist(key))
			{
				int iShift = 0;
				int bCancel = 0;
				if (GetKeyState(VK_SHIFT)<0 ) iShift |= 1;
				if (GetKeyState(VK_CONTROL)<0 ) iShift |= 2;
				if (GetKeyState(VK_MENU)<0 ) iShift |= 4;
				
				ONEventIII ptrFuncForm3 = NULL;
				ptrFuncForm3 = 
					(ONEventIII)m_hashEventMessage.Item(key, false);
				if (ptrFuncForm3)
				{
					(*ptrFuncForm3)((int)wParam, iShift, (int)(&bCancel)); 
					if (bCancel) return 1;
				}

			}
		}

		break;

	case WM_SYSCOMMAND:
		{
			ONEventIII ptrMenuClick;
			ptrMenuClick = (ONEventIII)m_hashEventNotify.Item(c_CBMenuClickEventKey, false);
			if (ptrMenuClick)
			{
				// ����1Ϊ�������Ĳ˵���ID��SC_CLOSE��SC_MAXIMIZE ��
				// ����2Ϊ��ֱ��ѡ��Ĳ˵�(=0)�����ǴӼ��ټ�ִ�е�(=1)
				// ����3=0 ��ʾ�Ǵ���ͨ�˵�ѡ��ģ�=1��ʾ�Ǵ�ϵͳ�˵�ѡ���
				(*ptrMenuClick)((int)wParam, 0, 1);
			}
		}

		break;
	case WM_MENUSELECT:
		if (m_hashEventMessage.IsKeyExist((long)WM_MENUSELECT))
		{
			ONEventI ptrFuncForm1 = NULL;
			
			ptrFuncForm1 = 
				(ONEventI)m_hashEventMessage.Item((long)WM_MENUSELECT, false);
			if (ptrFuncForm1)													// �� if �ݴ��ж�
			{
				if (lParam)
				{
					// ʹ��ģ�鼶 m_MenuItemSel ��Ա�󶨵���ѡ�˵�
					if (HIWORD(wParam) & MF_POPUP)
						// ��ѡ�˵������һ������ʽ�˵���LOWORD(wParam) ��������
						m_MenuItemSel.SetFromPos((HMENU)lParam, LOWORD(wParam)+1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	//+1 ת��Ϊλ�úŴ�1��ʼ
					else
						// ��ѡ�˵�������ͨ�����LOWORD(wParam) �� ResID
						m_MenuItemSel.SetFromResID((HMENU)lParam, LOWORD(wParam), m_hWnd, &m_hashMenuIDs, &m_hashObjs);
					
					(*ptrFuncForm1)((int)&m_MenuItemSel);					
				}
				else
				{
					(*ptrFuncForm1)(NULL);
				}
			}
				break;
		}
		break;

	case WM_DROPFILES:

		if (m_hashEventMessage.IsKeyExist((long)WM_DROPFILES))
		{
			ONEventIIII ptrFuncForm4 = NULL;
			
			ptrFuncForm4 = 
				(ONEventIIII)m_hashEventMessage.Item((long)WM_DROPFILES, false);
			if (ptrFuncForm4)													// �� if �ݴ��ж�
			{
				// ����϶��ļ����ܸ���
				UINT count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
				if (count==0) break;	// �ݴ�

				// ���ٱ����ַ���ָ��Ŀռ�
				unsigned long * ptrArr = new unsigned long [count+1]; // ������ [0] �Ŀռ�
				HM.AddPtr(ptrArr);
				HM.ZeroMem(ptrArr, sizeof(unsigned long)*(count+1));

				// ���ٸ����ַ����Ŀռ䣬����ø��ļ���
				UINT i=0; UINT sizeEach=0; TCHAR * pstr;
				for (i=0; i<count; i++)
				{
					sizeEach = DragQueryFile((HDROP)wParam, i, NULL, 0);
					pstr = new TCHAR [sizeEach+1];
					HM.AddPtr(pstr);
					HM.ZeroMem(pstr, sizeof(TCHAR)*sizeEach);
					ptrArr[i+1] = (unsigned long)pstr;
					DragQueryFile((HDROP)wParam, i, pstr, sizeEach);
				}

				// ����Ϸ��ļ�ʱ�������λ��
				POINT ptDrag;
				DragQueryPoint((HDROP)wParam, &ptDrag); 
					
				// �ͷž�� hDrop
				DragFinish((HDROP)wParam);

				// �����¼�
				(*ptrFuncForm4)((int)ptrArr, count, (int)ptDrag.x, (int)ptDrag.y);					
			}
		}		

		break;

	default:
		// �������¼�����
		if (m_hashEventMessage.IsKeyExist((long)uMsg))
		{
			ONEventVoid ptrFuncForm0 = NULL;
			ONEventI ptrFuncForm1 = NULL;
			ONEventII ptrFuncForm2 = NULL;
			ONEventIII ptrFuncForm3 = NULL;
			ONEventIIII ptrFuncForm4 = NULL;
			switch(m_hashEventMessage.ItemLong((long)uMsg, false))
			{
			case 0:		// 0 ������
				ptrFuncForm0 =
					(ONEventVoid)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm0) (*ptrFuncForm0)();							// �� if �ݴ��ж�
				break;
			case 1:		// 1 �� int �Ͳ���
				ptrFuncForm1 = 
					(ONEventI)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm1) (*ptrFuncForm1)((int)wParam);					// �� if �ݴ��ж�
				break;
			case 2:		// 2 �� int �Ͳ���
				ptrFuncForm2 = 
					(ONEventII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm2) (*ptrFuncForm2)((int)wParam, (int)lParam);	// �� if �ݴ��ж�
				break;
			case 3:		// 3 �� int �Ͳ���
				ptrFuncForm3 = 
					(ONEventIII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm3) (*ptrFuncForm3)((int)LOWORD(wParam), (int)HIWORD(wParam),  
					(int)lParam);												// �� if �ݴ��ж�
				break;
			case 4:		// 4 �� int �Ͳ���
				ptrFuncForm4 = 
					(ONEventIIII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm4) (*ptrFuncForm4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					(int)LOWORD(lParam), (int)HIWORD(lParam));					// �� if �ݴ��ж�
				break;
			}
		}
		break;
	}

	return 0;
}



long CBForm::EventsGeneratorCtrl( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	long key = 0;											// ���ڼ�����ϣ��� key
	int idCtrl = GetDlgCtrlID(hWnd);						// ���Կؼ��Ŀؼ� ID
	if (idCtrl == 0) return gc_APICEventsGenDefautRet;		// �޷���ÿؼ� ID�� �������¼�������������
	
	// ����������ϣ�� m_hashEventMessage����Ϊ���� 2 λΪ��Ϣ��WM_XXX������ 2 λΪ�ؼ� ID
	key = MAKELONG(uMsg, idCtrl);

	switch (uMsg)
	{

	case WM_SETCURSOR:
		// ��������꣺���� SetCursor ���õģ�Windows ���Զ���ʾ������Ĺ��
		if (ms_CursorGlobalIdx)
		{
			// ======== ʹ��ȫ������� ms_hCursorGlobal ========
			if (ms_hCursorGlobal)
			{
				SetCursor(ms_hCursorGlobal);
				return 1;	// return 1; ��ʾ����Ĭ�ϴ��ڳ�������
			}
		}
		else
		{
			// �ж��Ƿ�Ϊ���Ӵ��ڿؼ������������
			STRControlProp * pProp = PropPtr(hWnd); 
			if (pProp)
			{
				if (pProp->cursorIndex)
				{
					// ======== ʹ��Ϊ���Ӵ��ڿؼ����õ������ ========
					if (pProp->hCursor)
					{
						SetCursor(pProp->hCursor);
						return 1;	// return 1; ��ʾ����Ĭ�ϴ��ڳ�������
					}
				}
			}
			// ======== ����ʹ��Ĭ�ϣ��������������꣨Ĭ�ϴ��ڳ����Զ����� ========
		}
		
		break;

	// ==================================================================
	//	�����¼��Ĵ���
	case WM_LBUTTONDOWN: 	case WM_RBUTTONDOWN:	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:		case WM_RBUTTONUP:		case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:	case WM_MBUTTONDBLCLK:	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
		// ������ҡ��м����¡�̧��˫��������ƶ�
		{
			long key = 0;  int button = 0;
			if (uMsg == 0x200)				// ����ƶ�
			{
				key = MAKELONG(WM_MOUSEMOVE, idCtrl);
				if ( wParam & MK_LBUTTON ) button = button | 1;
				if ( wParam & MK_MBUTTON ) button = button | 4;
				if ( wParam & MK_RBUTTON ) button = button | 2;
			}
			else if ((uMsg-0x200) % 3 == 1)				// ���£�0x201��0x204��0x207��(uMsg-0x200) % 3 == 1
			{
				key = MAKELONG(WM_LBUTTONDOWN, idCtrl);	// ͳһ�� WM_LBUTTONDOWN ������Ϊ3��ö��ֵ
				button = 1 << ((uMsg-0x0200-1)/3); 
			}
			else if ((uMsg-0x200) % 3 == 2)				// ̧��0x202��0x205��0x208��(uMsg-0x200) % 3 == 2
			{
				key = MAKELONG(WM_LBUTTONUP, idCtrl);	// ͳһ�� WM_LBUTTONUP ������Ϊ3��ö��ֵ
				button = 1 << ((uMsg-0x0200-2)/3); 
			}
			else if ((uMsg-0x200) % 3 == 0)				// ˫����0x203��0x206��0x209��(uMsg-0x200) % 3 == 0
			{
				key = MAKELONG(WM_LBUTTONDBLCLK,idCtrl);// ͳһ�� WM_LBUTTONDBLCLK ������Ϊ3��ö��ֵ
				button = 1 << ((uMsg-0x0200-3)/3);
			}
			else
				break;	// �ݴ�
			
			if (m_hashEventMessage.IsKeyExist(key))
			{
				ONEventIIII ptrFuncForm4 = NULL;
				ptrFuncForm4 = 
					(ONEventIIII)m_hashEventMessage.Item(key, false);
				if (ptrFuncForm4)			// �� if �ݴ��ж�
				{
					POINT pt; UINT altKey=0;  
					GetCursorPos(&pt);  ScreenToClient(hWnd, &pt);
					if (GetKeyState(VK_MENU)<0) altKey=4;		// Shift=1; Ctrl=2; Alt=4

					(*ptrFuncForm4)(button, 
						altKey | ( ( (wParam & (MK_CONTROL | MK_SHIFT))) /4 ), 
						pt.x, pt.y);
					
				}	// if (ptrFuncForm4)
			}	// if (m_hashEventMessage.IsKeyExist(key)
		}
		break;

	case WM_CHAR: 
		// ��������ɴ��崦������¼����ȴ�������ļ����¼�
		if (KeyPreview) EventsGenerator(uMsg, wParam, lParam); 

		// �����ؼ��ļ����¼�
		if (m_hashEventMessage.IsKeyExist(key))
		{
			ONEventII ptrFuncForm2 = NULL;
			int bCancel = 0;
			ptrFuncForm2 = 
				(ONEventII)m_hashEventMessage.Item(key, false);
			if (ptrFuncForm2)
			{
				(*ptrFuncForm2)((int)wParam, (int)(&bCancel)); 
				if (bCancel) return 1;
			}
		}
		break;
		
	case WM_KEYDOWN: case WM_KEYUP: 		//   shift=1,2,4 �ֱ��ʾͬʱ Shift��Ctrl��Alt ��������
	case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		// ��������ɴ��崦������¼����ȴ�������ļ����¼�
		if (KeyPreview) EventsGenerator(uMsg, wParam, lParam); 

		// �����ؼ��ļ����¼�
		if (uMsg>=0x0104) key = MAKELONG((uMsg-0x4), idCtrl);	// ��WM_SYS��Ϣ�鲢��WM_��Ϣ
		if (m_hashEventMessage.IsKeyExist(key))
		{
			int iShift = 0;
			int bCancel = 0;
			if (GetKeyState(VK_SHIFT)<0 ) iShift |= 1;
			if (GetKeyState(VK_CONTROL)<0 ) iShift |= 2;
			if (GetKeyState(VK_MENU)<0 ) iShift |= 4;
			
			ONEventIII ptrFuncForm3 = NULL;
			ptrFuncForm3 = 
				(ONEventIII)m_hashEventMessage.Item(key, false);
			if (ptrFuncForm3)
			{
				(*ptrFuncForm3)((int)wParam, iShift, (int)(&bCancel)); 
				if (bCancel) return 1;
			}
		}

		// ������ MessageLoop �������� IsDialogMessage �Իس��Ĵ���
		//   ʹ�ڰ�ť�ϰ��»س�ʱ���ܴ�����ť�����µ��¼�
		//   �����޲���
		// ����ǰ�ť�ؼ����һس������£���ģ�ⰴť������
		if (wParam == VK_RETURN && uMsg==WM_KEYUP) 
		{
			TCHAR sClassName[255];		
			// ��������ַ��� => m_ClassName
			GetClassName(hWnd, sClassName, sizeof(sClassName)/sizeof(TCHAR)-1);
			if (_tcscmp(sClassName, TEXT("Button"))==0)
				SendMessage(hWnd, BM_CLICK, 0, 0);
		}

		break;

	case WM_DROPFILES:
		
		if (m_hashEventMessage.IsKeyExist(key))
		{
			ONEventIIII ptrFuncForm4 = NULL;
			
			ptrFuncForm4 = 
				(ONEventIIII)m_hashEventMessage.Item(key, false);
			if (ptrFuncForm4)													// �� if �ݴ��ж�
			{
				// ����϶��ļ����ܸ���
				UINT count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
				if (count==0) break;	// �ݴ�
				
				// ���ٱ����ַ���ָ��Ŀռ�
				unsigned long * ptrArr = new unsigned long [count+1]; // ������ [0] �Ŀռ�
				HM.AddPtr(ptrArr);
				HM.ZeroMem(ptrArr, sizeof(unsigned long)*(count+1));
				
				// ���ٸ����ַ����Ŀռ䣬����ø��ļ���
				UINT i=0; UINT sizeEach=0; TCHAR * pstr;
				for (i=0; i<count; i++)
				{
					sizeEach = DragQueryFile((HDROP)wParam, i, NULL, 0);
					pstr = new TCHAR [sizeEach+1];
					HM.AddPtr(pstr);
					HM.ZeroMem(pstr, sizeof(TCHAR)*sizeEach);
					ptrArr[i+1] = (unsigned long)pstr;
					DragQueryFile((HDROP)wParam, i, pstr, sizeEach);
				}
				
				// ����Ϸ��ļ�ʱ�������λ��
				POINT ptDrag;
				DragQueryPoint((HDROP)wParam, &ptDrag); 
				
				// �ͷž�� hDrop
				DragFinish((HDROP)wParam);
				
				// �����¼�
				(*ptrFuncForm4)((int)ptrArr, count, (int)ptDrag.x, (int)ptDrag.y);					
			}
		}		
		
		break;

	default:
		if (m_hashEventMessage.IsKeyExist(key))
		{
			ONEventVoid ptrFunc0 = NULL; 
			ONEventI ptrFunc1 = NULL;
			ONEventII ptrFunc2 = NULL;
			ONEventIII ptrFunc3 = NULL;
			ONEventIIII ptrFunc4 = NULL;
			switch(m_hashEventMessage.ItemLong(key, false))
			{
			case 0:		// 0 ������
				ptrFunc0 =
					(ONEventVoid)m_hashEventMessage.Item(key, false);
				if (ptrFunc0) (*ptrFunc0)();								// �� if �ݴ��ж�
				break;
			case 1:		// 1 �� int �Ͳ���
				ptrFunc1 = 
					(ONEventI)m_hashEventMessage.Item(key, false);
				if (ptrFunc1) (*ptrFunc1)((int)wParam);						// �� if �ݴ��ж�
				break;
			case 2:		// 2 �� int �Ͳ���
				ptrFunc2 = 
					(ONEventII)m_hashEventMessage.Item(key, false);
				if (ptrFunc2) (*ptrFunc2)((int)wParam, (int)lParam);		// �� if �ݴ��ж�
				break;
			case 3:		// 3 �� int �Ͳ���
				ptrFunc3 = 
					(ONEventIII)m_hashEventMessage.Item(key, false);
				if (ptrFunc3) (*ptrFunc3)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					lParam);												// �� if �ݴ��ж�
				break;
			case 4:		// 4 �� int �Ͳ���
				ptrFunc4 = 
					(ONEventIIII)m_hashEventMessage.Item(key, false);
				if (ptrFunc4) (*ptrFunc4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					(int)LOWORD(lParam), (int)HIWORD(lParam));				// �� if �ݴ��ж�
				break;
				
			}	// end of switch
		}	// end of if (m_hashEventMessage.IsKeyExist(key)
	} // end of switch (uMsg)

	return gc_APICEventsGenDefautRet;
}




void CBForm::RestoreModalWinEnabled()
{
	// ����ʾһ��ģ̬�Ի���ʱ��
	//   ������ĳ����ĿǰΪ Disabled �Ҵ�ֵΪ0����ά�ָô��ڵĴ�ֵ���䣻
	//   ��������ĳ����ĿǰΪ Enabled�����߸ô��ڴ�ֵ >0�����Ὣ�ô��ڵĴ�ֵ +1
	// ������һ��ģ̬�Ի���ʱ��
	//   ��ĳ���ڴ�ֵΪ0�������κβ��������򽫴�ֵ-1����-1��Ϊ0����ָ�Ϊ Enabled ״̬

	int i;  long level=0; 
	for (i=1; i<=ms_hashWnd.Count(); i++)
	{
		level = ms_hashWnd.ItemLong2FromIndex(i, false);
		if (level>0)
		{
			level--;
	
			if (level<=0)
			{
				// �ָ����� Enabled
				EnableWindow((HWND)ms_hashWnd.IndexToKey(i), 1);
				level = 0;		// �ݴ���ʱ level ��ӦΪ0��
			}
			
			// ����μ�¼�� ItemLong2
			ms_hashWnd.ItemLong2FromIndexSet(i, level, false);
		}
		// else level==0 �����κβ���
	}

	// �ָ� Owner ����Ϊǰ̨
	SetActiveWindow (GetWindow(m_hWnd, GW_OWNER));

	// �����־����
	m_ModalShown=NULL;

	// ģ̬��μ�1
	ms_iModalLevel--; 
	if (ms_iModalLevel<0) ms_iModalLevel=0;		// �ݴ�
}


void CBForm::ConstructMenuIDItems( HMENU hMenuParent, bool bIsTop/*=false*/ )
{
	int i, iCount;
	UINT idItem;
	
	if (bIsTop) m_hashMenuIDs.Clear();	// ������м�¼
	if (0==hMenuParent) return;   

	// ��� hMenuParent �µĲ˵�������
	iCount = GetMenuItemCount(hMenuParent); 
	if (iCount == -1) return;		// GetMenuItemCount ����ʧ�ܣ����޲˵���
	
	// ������� hMenuParent �µ����в˵���
	for(i=0; i<=iCount-1; i++)
	{
		idItem = GetMenuItemID(hMenuParent, i);
		if (idItem == 0xFFFFFFFF)
		{
			// hMenu �µĵ� i ���м����˵�
			// �ݹ鴦��˼����˵��µ������Ӳ˵���
			ConstructMenuIDItems(GetSubMenu(hMenuParent, i));
		}
		else
		{
			// hMenu �µĵ� i ���ѻ�ò˵��� ID������� ID ����ϣ��
			// ��ϣ�� key = �˵������Դid��data = �ò˵��ĸ��˵��ľ��
			m_hashMenuIDs.Add((int)hMenuParent, idItem, 0, 0, 0, 0, 0, false);
		}
		
	}
}




void CBForm::ClearResource()
{
	// �ͷ������صĶ���
	if (m_hashObjs.Count())
	{
		int i;
		for (i=1; i<=m_hashObjs.Count(); i++)
		{
			switch(m_hashObjs.ItemLongFromIndex(i,false))
			{
			case IMAGE_ICON:
				DestroyIcon((HICON)m_hashObjs.ItemFromIndex(i, false));
				break;
			case IMAGE_CURSOR:
				DestroyCursor((HCURSOR)m_hashObjs.ItemFromIndex(i, false));
				break;
			default:	// ���� IMAGE_BITMAP
				DeleteObject((HGDIOBJ)m_hashObjs.ItemFromIndex(i, false));
				break;
			}
		}
		m_hashObjs.Clear();
	}
	
	// �ͷ��������
	if (m_hFont) DeleteObject(m_hFont);

	// �ͷŴ���
	if (m_hWnd)
	{
		//==============================================================
		// �ڴ˴������ٵ��� DestroyWindow ���� WM_DESTROY ��Ϣ
		// ���յ� WM_DESTROY ��Ϣʱ����ñ������������γ����ݹ�

		// �ָ��Ӵ��ڿؼ������ദ��lParam ��Ϊ 0 ��ʾж�ش���ʱ�ָ����ദ���Ӵ��ڿؼ�
		EnumChildWindows(m_hWnd, EnumChildProcSubClass, 0);

		// ���ǰ̨���ڸպ��Ǳ����ڣ�ȡ�����ټ���¼��ǰ̨���ڼ�¼
		if (CBForm::ms_hWndActiForm == m_hWnd)
		{
			CBForm::ms_hWndActiForm = NULL;
			CBForm::ms_hAccelCurrUsed = NULL;
		}
		
		// �ӹ�ϣ����ɾ���öԻ���ļ�¼
		ms_hashWnd.Remove((long)m_hWnd, false);
		
		// ����ô���Ϊģ̬���ָ����д��� Enabled
		if (m_ModalShown) RestoreModalWinEnabled();
		
		// �����Ա
		m_hashMenuIDs.Clear();
		m_BackColor = -1;
		m_CursorIdx = 0; 
		m_hCursor = 0;
		m_hWnd=0;
		// m_hashEventNotify.Clear();
		// m_hashEventMessage.Clear();
		
	}
}

HFONT CBForm::SetFontMLF( LOGFONT * lpLf )
{
	// ���û�����麯��ʵ�ֹ��ܣ��������������������
	HFONT hFontNew = CBWndBase::SetFontMLF(lpLf);	
	
	// ���½�����������������浽 m_hFont ˽�г�Ա
	if (m_hFont) DeleteObject(m_hFont);		// ɾ��������
	m_hFont = hFontNew;						// �� m_hFont ˽�г�Ա��¼���������
	
	// ���������������
	return hFontNew;
}














