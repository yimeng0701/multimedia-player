//////////////////////////////////////////////////////////////////////
// BForm.cpp: 一些全局函数（如 DoEvents 函数） 和 WinMain 函数的定义，
//            CBForm 类的实现
//   
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include "BForm.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")	// 使工程引入 comctl32.lib 库




//////////////////////////////////////////////////////////////////////////
// 全局函数 和 WinMain 函数
//////////////////////////////////////////////////////////////////////////

  
// 封装 WinMain 函数，用户仍可将 main() 作为程序的入口
int main();
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char * lpCmdLine, int nShowCmd )
{
	// 初始化通用控件库
	InitCommonControls();  // 如果任意公有函数中，调用 SHFileOperation 似乎也能达到同样的效果（执行SHFileOperation(0);即可）

	// 定义 CBApp 对象，将有关程序信息保存其中。该对象虽为 WinMain 的局部变量
	//   但程序运行全程都是存在的，因为 WinMain 函数结束，程序就结束了
	// 使全局指针变量 pApp 指向 app，则全局程序中可使用 (*pApp) 访问此变量
	CBApp app(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	pApp=&app;
	
	// 调用 main 函数，main 函数由用户在自己的模块中自己定义。这样程序的
	//   入口仍是 main 函数
	int retMain=main();

	// 进入本线程的消息循环，这是在用户 main() 函数执行结束后的事
	// 注意用户 main() 函数结束，整个程序并未结束
	MessageLoop(0);		// 参数 0 表示主消息循环，GetMessage 收到 0 才结束

	// WinMain 返回，返回值为用户 main() 函数的返回值
	return retMain;
}


// 消息循环函数
// bExitStyle=0时：为主消息循环，无消息、且无窗体时便 return
// bExitStyle>1时：实际是 +1 后的 ms_iModalLevel 的值
//   用于模态对话框显示后的消息循环（每显示一层模态对话框新进一层
//   新的 MessageLoop。若 ms_iModalLevel<iExitStyle，或无窗体时 return
// bExitStyle=-1时：用于 DoEvents()，
//   当前线程无消息（PeekMessage 返回0）时就 return
// 无论 bExitStyle 为多少，在 GetMessage 收到 0 都会 return 
//   并在 return 前再次 PostQuitMessage(0); 以将退出消息传播
//   到前面各层的 MessageLoop 使前面各层的 MessageLoop 都能退出
static void MessageLoop(int iExitStyle/*=0*/)
{
	// 进入本线程的消息循环：将获得本线程中的所有消息，然后将消息
	//   派发到它所属的各自的窗口过程中
	// 显示模态窗体时，再次调用本函数，并设置 iExitStyle 为 ms_iModalLevel（>0）
	//   当模态窗体隐藏或卸载时，退出新进入调用的本函数，返回上层调用的本函数 
	//   最低一层是 WinMain 所调用的本函数

	MSG msg;
	int iret=-1;		// 不能初始化为0，因 while 后有条件判断，iret==0 表示收到 WM_QUIT 消息

	while (1) 
	{
		if (iExitStyle>0)		// 用于模态窗体：ms_iModalLevel<iExitStyle，或无窗体时 return
		{
			if ( CBForm::ms_iModalLevel<iExitStyle ||  
				CBForm::ms_hashWnd.Count()==0 ) break;
		}
		else if (iExitStyle<0)	// 用于 DoEvents：当前线程无消息就 return
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)==0) break;
		}
		else					// 主消息循环：无消息且无窗体时便 return
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)==0  &&  
				CBForm::ms_hashWnd.Count()==0 ) break;
		}
		
		iret=GetMessage(&msg, NULL, 0, 0);	// 获得本线程所有窗口的消息

		// GetMessage出错会返回 -1，故 
		// while (GetMessage( lpMsg, hWnd, 0, 0)) ... 是不可取的
		// 返回 0 表示本线程收到退出信号 WM_QUIT 消息，退出消息循环
		if (iret==-1 || iret==0) break;	

		// 处理加速键
		// 用 TranslateAccelerator 转换和处理对应消息（如果有加速键，加速键句柄为 iret）
		// 如已转换处理，不要再 IsDialogMessage、TranslateMessage、DispatchMessage
		// 不能转换为加速键的消息 TranslateAccelerator 不处理，继续
		if (CBForm::ms_hAccelCurrUsed && CBForm::ms_hWndActiForm) 
		{
			if (TranslateAccelerator(CBForm::ms_hWndActiForm, CBForm::ms_hAccelCurrUsed, &msg)) continue;
		}
		
		// 处理对话框消息（如按 Tab 跳转控件焦点等）
		// 用 IsDialogMessage 转换和处理对应消息
		// 如该条消息已被 IsDialogMessage 处理，不要再 TranslateMessage、DispatchMessage
		// 非对话框的消息 IsDialogMessage 不处理，再 TranslateMessage、DispatchMessage
		
		if (msg.message == WM_KEYDOWN && (msg.wParam==13 || msg.wParam==27))
		{
			// 按下回车 或 按下 ESC 不交由 IsDialogMessage 处理
			//   否则 按下回车 或 按下 ESC 会被 IsDialogMessage 转换为 WM_COMMAND 消息
			//   （wParam==1 和 2），使窗体和其他控件不能正常接收 回车、ESC
			// 消息转换和派发
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!IsDialogMessage(CBForm::ms_hWndActiForm, &msg))	
			{
				// 消息转换和派发
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}	// end of while (1)

	// 如果是由于收到程序退出消息 而退出的 while 循环（iret==0）
	//   再次 PostQuitMessage(0); 以将退出消息传播
	//   到前面各层的 MessageLoop 使前面各层的 MessageLoop 都能退出
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
// CBForm 类的实现
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 定义 CBForm 类的 友元函数


// 公用窗口过程：所有本类对象（窗体）都用此函数作为窗口过程
// CBForm 类的友元函数
static BOOL WINAPI CBForm_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// =======================================================================
	// 本函数中不得 PostQuitMessage
	// 因为在一个对话框中 PostQuitMessage 后，整个程序就都关闭了，而不是只有
	//   这一个对话框被关闭
	// =======================================================================
	switch(uMsg)
	{
	case WM_INITDIALOG:
		// ===================================================================
		// lparam 应为一个对象的地址，将此地址与 hwnd 关联的关系存入哈希表
		if (lParam)
		{
			// =================================================================
			// ============ 有新窗体创建：将其信息记录到 ms_hashWnd ============
			// =================================================================

			// 容错：若哈希表中已存在键为 hwnd 的项目，则先删除它，再用新的内容覆盖
			if (CBForm::ms_hashWnd.IsKeyExist((long)hWnd)) 
				CBForm::ms_hashWnd.Remove((long)hWnd,false); 
			
			// 向哈希表中添加新项（Key=hwnd，Data=对象地址，
			//   ItemLong=加速键句柄，ItemLong2=受模态对话框影响的 Enabled 状态）
			CBForm::ms_hashWnd.Add(lParam, (long)hWnd, 0, 0);
			
			// 设置对象中的 m_hWnd 成员为 窗口句柄
			((CBForm *)lParam)->m_hWnd = hWnd;

			// 设置对象中的 类名成员
			memset( ((CBForm *)lParam)->m_ClassName, 0, sizeof( ((CBForm *)lParam)->m_ClassName));
			((CBForm *)lParam)->m_atom = GetClassLong(hWnd, GCW_ATOM);
			GetClassName(hWnd, ((CBForm *)lParam)->m_ClassName, 
				sizeof( ((CBForm *)lParam)->m_ClassName ) / sizeof(TCHAR)-1 );
			
			// 触发 Form_Load 事件
			((CBForm *)lParam)->EventsGenerator(WM_INITDIALOG, wParam, lParam);
		}

		return 1; // 返回 True, Windows 会自动将输入焦点放到第一个有 WS_TABSTOP 的控件上

		break;

	default:
		
		// ===================================================================
		// 调用各自对象的 EventsGenerator，后者处理这些消息，必要时生成事件
		//   本函数返回 EventsGenerator 的返回值
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


// 公用窗口过程：所有本类对象（窗体）中的子窗口控件，都用此函数作为窗口过程，并所有子窗口控件都被子类处理
// CBForm 类的友元函数
static int CALLBACK CBForm_ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	CBForm *pForm = 0; 
	long r; 

	// 所有消息送到 对应父窗体的.EventsGeneratorCtrl
	if (CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
	{
		// ItemLong = 所位于窗体的 hWnd，可以此为 key 到 ms_hashWnd 中获得窗体
		//   CBForm 对象的地址
		long lHWndForm=CBForm::ms_hashCtrls.ItemLong((long)hWnd, false);
		pForm = (CBForm *)CBForm::ms_hashWnd.Item(lHWndForm, false);
		if (pForm) 
		{ 
			r = pForm->EventsGeneratorCtrl(hWnd, uMsg, wParam, lParam);
			if (r != gc_APICEventsGenDefautRet) return r;
		}
	}


	// 从 CBForm::ms_hashCtrls 中获得本窗口的默认窗口程序的地址，并调用默认窗口程序
	return CallWindowProc ((WNDPROC)(CBForm::ms_hashCtrls.Item((long)hWnd,false)), 
		hWnd, uMsg, wParam, lParam);
}


// 枚举子窗口控件、子类处理所有子窗口控件的回调函数：
// lParam 为控件所属窗体的句柄时（非0），表示加载窗体时的子类处理；
// lParam 为 0 时，表示卸载窗体时的恢复子类处理
// CBForm 类的友元函数
static BOOL CALLBACK EnumChildProcSubClass(HWND hWnd, LPARAM lParam)
{
	if (lParam)
	{
		// ======== 窗体加载时（WM_INITDIALOG）调用的 ========
		// 处理本窗口的子窗口
		EnumChildWindows(hWnd, EnumChildProcSubClass, lParam);

		// 设置所有子窗口具有 WS_CLIPSIBLINGS 风格
		// 否则控件彼此覆盖时，后方控件重绘时会擦掉前方控件
		SetWindowLong( hWnd, GWL_STYLE,
			GetWindowLong(hWnd, GWL_STYLE) | WS_CLIPSIBLINGS ) ;

		// 修正 ComboBox 若高度太小不能拉下下拉框的问题
		// 获得类名字符串 => strClassName
		TCHAR strClassName[128];
		GetClassName(hWnd, strClassName, sizeof(strClassName)/sizeof(TCHAR)-1);
		if (_tcscmp(strClassName, TEXT("ComboBox"))==0)
		{
			CBWndBase ctrl(hWnd, (HWND)lParam);	// 借 CBWndBase 对象修改 ComboBox 高度；lParam 为父窗体句柄
			int heightReq = SendMessage(hWnd, CB_GETITEMHEIGHT, 0, 0) *10; 
			if ( ctrl.Height() < heightReq ) ctrl.HeightSet(heightReq);
		}

		// 将所有子窗口控件子类处理（Control_Proc），并将它们的原窗口程序地址
		//   存入 CBForm 类的静态成员 哈希表 ms_hashCtrls
		// 将 lParam （即父窗体的句柄）存入哈希表元素的 ItemLong
		if (! CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
		{
			// ItemLong2 设为0，表示没有设置过附加属性；只有设置是附加属性时
			//   再动态开辟 STRControlProp 的空间，ItemLong2 才为空间地址（不为0）
			CBForm::ms_hashCtrls.Add((long)GetWindowLong(hWnd,GWL_WNDPROC), (long)hWnd,
				(long)lParam, 0, 0, 0, 0, false);	
			SetWindowLong(hWnd, GWL_WNDPROC, (long)CBForm_ControlProc);
		}

		// 返回非0值，以继续枚举同层其他子窗口控件
		return 1;
	}
	else  // (lParam == 0)
	{
		// ======== 窗体卸载时（WM_DESTROY）调用的 ========
		// 清理所有子窗口控件的相关信息
		//   即 CBForm 类的静态成员 哈希表 CBForm::ms_hashCtrls 中的相关项目
		if (CBForm::ms_hashCtrls.IsKeyExist((long)hWnd))
		{
			// 删除附加属性的空间（如果设置过附加属性的话）
			STRControlProp * pProp = 
				(STRControlProp *)CBForm::ms_hashCtrls.ItemLong2((long)hWnd, false);
			if (pProp) 
			{
				// 删除所指 STRControlProp 的空间
				if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);

				// 删除所指 tagString 的字符串空间
				if (pProp->tagString) delete [](pProp->tagString);

				// 删除字体对象
				if (pProp->hFont) DeleteObject(pProp->hFont);

				// 不必删除光标对象，系统会自动删除
				// pProp->hCursor

				// 删除附加属性的空间
				delete pProp;	
			}

			// 恢复子类处理（原窗口程序地址位于 ms_hashCtrls.Item 中）
			SetWindowLong(hWnd, GWL_WNDPROC, 
				CBForm::ms_hashCtrls.Item((long)hWnd, false));
			
			// 删除 ms_hashCtrls 中的对应项目
			CBForm::ms_hashCtrls.Remove((long)hWnd, false);
		}

		// 处理本窗口的子窗口
		EnumChildWindows(hWnd, EnumChildProcSubClass, lParam);
		
		// 返回非0值，以继续枚举同层其他子窗口控件
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////
// 定义类中的 static 成员和 static 函数

CBHashLK CBForm::ms_hashWnd;
CBHashLK CBForm::ms_hashCtrls;


// 模态对话框 的层次
int CBForm::ms_iModalLevel = 0;


// 加速键句柄和加速键要发送到的目标窗口
HACCEL CBForm::ms_hAccelCurrUsed=NULL;  
HWND CBForm::ms_hWndActiForm=NULL;

// 全局窗口所用鼠标光标索引值：EStandardCursor 枚举值或资源ID
//   0 表示使用各自窗口的鼠标光标，否则使用全局统一光标
long CBForm::ms_CursorGlobalIdx = 0;
//  全局窗口所用鼠标光标句柄（在 ms_CursorGlobalIdx 非0时才有效）
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
		// 加载光标，句柄存入 ms_hCursorGlobal
		// =============================================================
		// LoadCursor 函数即使重复被调用，也不会重复加载资源；系统会判断
		//   如果对应光标已经加载，LoadCursor 直接返回句柄
		// =============================================================
		if ( ms_CursorGlobalIdx > gc_IDStandCursorIDBase)
		{
			// 标准光标
			// ms_CursorGlobalIdx-gc_IDStandCursorIDBase 才是标准光标的ID号
			ms_hCursorGlobal = 
				LoadCursor(NULL, 
				  MAKEINTRESOURCE(ms_CursorGlobalIdx-gc_IDStandCursorIDBase));
		}
		else
		{
			// 资源光标
			// ms_CursorGlobalIdx 就是资源 ID
			if (typeRes==0)
			{
				// 加载 Cursor 类型的资源
				ms_hCursorGlobal =
					LoadCursor(pApp->hInstance, MAKEINTRESOURCE(ms_CursorGlobalIdx));
			}
			else
			{
				// 加载自定义类型的资源（typeRes 类型的资源）
				unsigned long size=0; 
				unsigned char * p= LoadResData(ms_CursorGlobalIdx, typeRes, &size);
				ms_hCursorGlobal = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (ms_CursorGlobalIdx)
	{
		// 不特殊设置光标，使用默认：设置 ms_hCursorGlobal 为 0
		ms_hCursorGlobal = 0;
	}		// end if (ms_CursorGlobalIdx)

	// 先向本程序当前前台窗口发送 WM_SETCURSOR，前台窗口处理 WM_SETCURSOR 以使光标立即生效
	SendMessage(GetActiveWindow(), WM_SETCURSOR, (WPARAM)GetActiveWindow(), 0);
	// 在本程序的所有窗口（包括子窗口）的、不断接收到的 WM_SETCURSOR 消息中会改变鼠标光标
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
		HM.ZeroMem(pProp, sizeof(STRControlProp));	// 新空间清0
		pProp->backColor=-1;	// -1 表示使用默认颜色
		pProp->foreColor=-1;	// -1 表示使用默认颜色
		// 新空间地址记录到 ms_hashCtrls.ItemLong2Set
		if (! ms_hashCtrls.ItemLong2Set((long)hwndCtrl, (long)pProp, false))
		{ delete pProp;  pProp = 0;  }   // 失败容错
	}
	return pProp;
}


//////////////////////////////////////////////////////////////////////////
// 构造和析构

// 构造函数
CBForm::CBForm( unsigned short int idResDialog /*=0*/)
{
	mResDlgID = idResDialog;
	m_hWnd = 0;
	m_ModalShown = false;	// 是否正以模态对话框显示的标志
	m_BackColor = -1;		// 窗口背景色，-1 表示使用默认色（不修改窗口类，而是通过响应 WM_ERASEBKGND 实现）
	m_CursorIdx = 0;		// 窗体当前的鼠标光标索引号，0 表示使用系统默认
	m_hFont = 0;				// 窗体当前的字体对象句柄（为0时表示使用系统默认字体，否则表示用自定义字体）

	// 类公有成员赋初值
	KeyPreview = false;		// 是否优先由窗体处理所有控件的键盘按键消息
}

// 析构函数
CBForm::~CBForm()
{
	UnLoad();
	ClearResource();
}


//////////////////////////////////////////////////////////////////////////
// 公有成员函数

HWND CBForm::hWnd()
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	return m_hWnd;
}


HMENU CBForm::hMenu()
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	return GetMenu(m_hWnd);
}


void CBForm::hMenuSet( HMENU hMenuNew )
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载

	SetMenu(m_hWnd, hMenuNew); 
	if (hMenuNew)
	{	
		ConstructMenuIDItems(GetMenu(m_hWnd), true);
		ConstructMenuIDItems(GetSystemMenu(m_hWnd, 0), false);	// 系统菜单
	}
	else
	{
		ConstructMenuIDItems(NULL, true);
		ConstructMenuIDItems(GetSystemMenu(m_hWnd, 0), false);	// 系统菜单
	}
}


HACCEL CBForm::hAccel()
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	return (HACCEL)ms_hashWnd.ItemLong((long)m_hWnd, false);
}


void CBForm::hAccelSet( HACCEL hAccelNew )
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载，然后才能设置加速键
	
	ms_hashWnd.ItemLongSet((long)m_hWnd, (long)hAccelNew, false);	// 记录到 ms_hashWnd 的 ItemLong
	// 如果现在前台窗体正是本对象的窗体，就设置 CBForm::hWndAccel、CBForm::hAccel
	if (m_hWnd==GetActiveWindow())
	{
		CBForm::ms_hWndActiForm = m_hWnd; 
		CBForm::ms_hAccelCurrUsed = hAccelNew; 
	}
}


bool CBForm::Show( int modal/*=0*/, HWND hwndOwner/*=NULL*/ )
{
	// 由于模态方式显示窗体，不能自写消息循环，导致不能处理加速键
	//   在本类中，所有窗体以非模态方式显示
	// 对于“模态”的实现，是本函数通过将所有顶层窗体全部 
	//   Enabled=False 实现的

	HWND hwnd;
	HWND hWndActiLast = GetActiveWindow(); 

	if (0==m_hWnd) 
	{	
		// 若尚未加载对话框，现在加载
		// 所有窗体以非模态方式 CreateDialogParam 加载
		hwnd=Load(hwndOwner);   
		if (hwnd==0) return false;
	}
	else 
	{	
		// 对话框已经加载，在显示时可能会改变其 Owner
		// 通过以下语句改变窗体的 Owner （不是Parent）
		//   MSDN 并没有介绍改变 Owner 的方法，应该是用  
		//   SetWindowLong 以 -8 设置 Owner
		if (hwndOwner !=0 && hwndOwner != GetWindow(m_hWnd, GW_OWNER)) 
			SetWindowLong(m_hWnd, -8, (long)hwndOwner); 
	}

	if (modal)
	{
		// ======== 显示模态对话框 ========

		// 在显示一个模态对话框时：
		//   若其他某窗口目前为 Disabled 且 ms_hashWnd.ItemLong2 值为0，则维持该窗口的此值不变；
		//   否则，其他某窗口目前为 Enabled，或者该窗口此值 >0，都会将该窗口的此值 +1
		// 在隐藏一个模态对话框时：
		//   如某窗口此值为0，则不做任何操作；否则将此值-1，如-1后为0，则恢复为 Enabled 状态

		int i; 
		BOOL ena; 
		long level=0; 
		HWND hwndEach=0;
		for (i=1; i<=ms_hashWnd.Count(); i++)
		{
			hwndEach=(HWND)ms_hashWnd.IndexToKey(i);
			if (hwndEach==m_hWnd) 
			{
				// 本模态窗体，不做；但设置 ItemLong2 为 0
				ms_hashWnd.ItemLong2FromIndexSet(i, (long)0, false);
				continue;	
			}

			
			// 根据窗体 i 的当前 enabled 状态和 ItemLong2（level） 做出判断
			ena = IsWindowEnabled(hwndEach);
			level = ms_hashWnd.ItemLong2FromIndex(i, false); 
			if (ena || level>0)	
			{	
				// 记录将来要把本窗口恢复到Enabled
				ms_hashWnd.ItemLong2FromIndexSet(i, level+1, false);	
			}
			// else 若窗口原为 Disabled，且  ItemLong2（level） 为0
			//   则仍维持此值为0（不变）

			// 将窗体 i 设置为 Diabled
			EnableWindow(hwndEach, 0);
		}


		// 设置本对象的显示模态标志
		m_ModalShown = true;
	
		// 模态层次 ++
		if (ms_iModalLevel<0) ms_iModalLevel=0;		// 容错
		ms_iModalLevel++;

		// 显示“模态”窗体
		ShowWindow(m_hWnd, SW_NORMAL);
		
		// 本函数不要立即返回
		// 再次调用 MessageLoop，并设置参数为 1
		// 直到“模态”窗体被隐藏或关闭后下面的 MessageLoop 函数才返回
		MessageLoop(ms_iModalLevel); 
	}
	else
	{
		// ======== 显示非模态对话框 ========
		
		if ( ms_iModalLevel>0 )	
		{
			// 如果已有模态对话框正在显示，原则上不允许再显示
			//   非模态对话框。但本类允许，只要将新对话框设为 Disabled 即可
			// 新对话框的 ItemLong2 值应为 ms_iModalLevel
			ms_hashWnd.ItemLong2Set((long)m_hWnd, ms_iModalLevel, false);
			EnableWindow(m_hWnd, 0);

			// 显示新窗口（但不要将新窗口设为前台）
			ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);	
			SetActiveWindow(hWndActiLast);	// 恢复原来前台窗口仍为前台
		}
		else
		{
			ShowWindow(m_hWnd, SW_NORMAL);	
		}
		
		// 以非模态显示，设置标志
		m_ModalShown = false;
	}

	return true;
}


long CBForm::Hide()
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	return ShowWindow(m_hWnd, SW_HIDE);
}


// 加载对话框但并不显示出来（只能以非模态方式加载）
// 在需要对话框被加载后才能调用的一些函数如 SetAccelerator 中，可自动调用本函数
HWND CBForm::Load( HWND hwndOwner/*=NULL*/ )
{
	// 一律以“非模态”方式加载窗体：调用 CreateDialogParam
	// 将 dwInitParam 参数设为本对象地址即this，
	//   此将作为 WM_INITDIALOG 的 lParam 参数传给 CBForm_DlgProc
	HWND hwnd;
	hwnd=CreateDialogParam(pApp->hInstance, MAKEINTRESOURCE(mResDlgID), hwndOwner, 
	  CBForm_DlgProc, (long)this);	// 立即返回，返回窗口句柄；在窗口过程处理 WM_INITDIALOG 时会设置 m_hWnd
	
	// 如果没有 WS_VISIBLE 样式此时窗体不会自动显示
	
	if (hwnd)
	{
		// 初始化菜单
		ConstructMenuIDItems(GetMenu(hwnd),true);	// 如无菜单即参数为0，将清除菜单哈希表记录
		ConstructMenuIDItems(GetSystemMenu(hwnd, 0), false);	// 系统菜单
	}

	return hwnd;
}



// 结束对话框
// 接收到 WM_CLOSE 消息时可自动调用本函数，也可直接由用户调用
void CBForm::UnLoad() const
{
	SendMessage(m_hWnd, WM_CLOSE, 0, 0);
}



// 设置本窗口将处理加速键，资源 ID 为 idResAcce
// 若取消加速键，将参数设为 0 即可
void CBForm::SetAccelerator( unsigned short int idResAcce )
{
	HACCEL hAcc=NULL;
	if (idResAcce) hAcc=LoadAccelerators(pApp->hInstance, MAKEINTRESOURCE(idResAcce)); 
	hAccelSet(hAcc);	// 调用公有方法 hAccelSet 设置
}

// 重新设置本窗口的菜单
void CBForm::SetMenuMain( unsigned short idResMenu )
{
	HMENU hMenu=NULL;
	if (idResMenu)
	{
		hMenu=LoadMenu(pApp->hInstance, MAKEINTRESOURCE(idResMenu));
		hMenuSet(hMenu);	// 调用公有方法 hMenuSet 设置
	}
	else
		hMenuSet(NULL);		// 调用公有方法 hMenuSet 设置
}


// 返回一个控件（CBControl 对象），之后可对该控件进行操作（调用 CBControl 类的属性方法）
CBControl CBForm::Control( unsigned short int idResControl )
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	if (m_Control.SetResID(m_hWnd, idResControl, &m_hashObjs))
		return m_Control;
	else
		return 0;
}


// 返回一个菜单项（CBMenuItem 对象），之后可对该菜单项进行操作（调用 CBMenuItem 类的属性方法）
CBMenuItem CBForm::Menu( UINT idResMenuItem )
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载

	// 根据 idMenuItem，在哈希表中查找其所对应的父菜单的 句柄 -> hMenu
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
		// 获得主菜单
		hMenu = GetMenu(m_hWnd);

		if (pos2)
		{
			hMenu = GetSubMenu(hMenu, pos1 - 1);		//-1变为从0开始编号，获得顶层菜单的句柄：例如若pos1==1，hMenuSub为整个“文件”菜单的句柄
		}
		else
		{
			m_MenuItem.SetFromPos(hMenu, pos1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 返回顶级菜单的一个菜单项（位置参数从1开始），例如若pos1==1，返回“文件”菜单项
			return m_MenuItem;
		}
	}
	else if (pos1<0)
	{
		// 获得系统菜单
		hMenu = GetSystemMenu(m_hWnd, 0);
		if (pos2==0)
		{
			// 返回系统菜单的“父菜单项”
			//  （它引出的子菜单句柄为 GetSystemMenu，即它的子菜单的第一项是【还原】）
			m_MenuItem.SetFromPos(0, 1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);		// 1-1 == 0
			return m_MenuItem; 
		}
	}
	else	// pos1 == 0
	{
		// 获得顶级菜单
		hMenu = GetMenu(m_hWnd);
		if (pos2==0)
		{
			// 返回顶层菜单的“父菜单项”
			//  （它引出的子菜单句柄为 GetMenu，即它的子菜单的第一项是【文件】）
			m_MenuItem.SetFromPos(0, 2, m_hWnd, &m_hashMenuIDs, &m_hashObjs);
			return m_MenuItem; 
		}
	}
	

	if (pos3)
	{
		hMenu = GetSubMenu(hMenu, pos2 - 1);		//-1变为从0开始编号
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos2, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 返回一级菜单的一个菜单项（位置参数从1开始），例如若pos1==1、pos2==2，返回“文件”-“打开”的菜单项
		return m_MenuItem;	
	}

	if (pos4)
	{
		hMenu = GetSubMenu(hMenu, pos3 - 1);		//-1变为从0开始编号
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos3, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 位置参数从1开始
		return m_MenuItem;	
	}

	if (pos5)
	{
		hMenu = GetSubMenu(hMenu, pos4 - 1);		//-1变为从0开始编号
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos4, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 位置参数从1开始
		return m_MenuItem;	
	}

	if (pos6)
	{
		hMenu = GetSubMenu(hMenu, pos5 - 1);		//-1变为从0开始编号
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos5, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 位置参数从1开始
		return m_MenuItem;	
	}

	if (pos7)
	{
		hMenu = GetSubMenu(hMenu, pos6 - 1);		//-1变为从0开始编号
	}
	else
	{
		m_MenuItem.SetFromPos(hMenu, pos6, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	// 位置参数从1开始
		return m_MenuItem;	
	}

	m_MenuItem.SetFromPos(hMenu, pos7, m_hWnd, &m_hashMenuIDs, &m_hashObjs);		// 位置参数从1开始
	return m_MenuItem;	

}


CBMenuItem CBForm::Menu( ESysMenu idSysMenu )
{
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载
	
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
		// TrackPopupMenu 只能弹出 popup 式菜单，LoadMenu 得到的不是 popup 式菜单，
		//   应用 GetSubMenu 取其子菜单（这里只取第0项的子菜单）
	DestroyMenu(hMenuPop);
	return ret;
}


void CBForm::IconSet(EStandardIcon icon)
{
	if (m_hWnd==NULL) Load();					// 如果窗口还未加载，现在加载
	HICON hIco = LoadIcon(0, (LPCTSTR)icon);	// 获得的是shared icon，不要 DestroyIcon
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIco);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIco);
}

void CBForm::IconSet(unsigned short iconRes)
{
	if (m_hWnd==NULL) Load();		// 如果窗口还未加载，现在加载
	HICON hIco = LoadIcon(pApp->hInstance, (LPCTSTR)((DWORD)iconRes));	// 获得的是shared icon，不要 DestroyIcon
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIco);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIco);
}


COLORREF CBForm::BackColor()
{
	if (-1 == m_BackColor)
	{
		// 获得窗口类的背景色
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
			// GetClassLong 失败，或窗口类无画刷：返回系统 COLOR_BTNFACE 颜色
			return GetSysColor(COLOR_BTNFACE);
		}
	}
	else
		// 直接返回 m_BackColor
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
	if (m_hWnd==NULL) Load(); // 如果窗口还未加载，现在加载

	m_CursorIdx = idResCursor; 
	if (m_CursorIdx)
	{
		// 加载光标，句柄存入 m_hCursor
		// =============================================================
		// LoadCursor 函数即使重复被调用，也不会重复加载资源；系统会判断
		//   如果对应光标已经加载，LoadCursor 直接返回句柄
		// =============================================================
		if ( m_CursorIdx > gc_IDStandCursorIDBase)
		{
			// 标准光标
			// m_CursorIdx-gc_IDStandCursorIDBase 才是标准光标的ID号
			m_hCursor = 
				LoadCursor(NULL, MAKEINTRESOURCE(m_CursorIdx-gc_IDStandCursorIDBase));
		}
		else
		{
			// 资源光标
			// m_CursorIdx 就是资源 ID
			if (typeRes==0)
			{
				// 加载 Cursor 类型的资源
				m_hCursor = LoadCursor(pApp->hInstance, MAKEINTRESOURCE(m_CursorIdx));
			}
			else
			{
				// 加载自定义类型的资源（typeRes 类型的资源）
				unsigned long size=0; 
				unsigned char * p= LoadResData(m_CursorIdx, typeRes, &size);
				m_hCursor = (HCURSOR)CreateIconFromResource(p, size, 0, 0x00030000);
			}
		}
	}	
	else	// if (m_CursorIdx)
	{
		// 不特殊设置光标，使用默认：设置 m_hCursor 为 0
		m_hCursor = 0; 
	}		// end if (m_CursorIdx)

	// 向本窗口发送 WM_SETCURSOR，以使光标立即生效
	SendMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, 0);
	// 在本窗口不断接收到的 WM_SETCURSOR 消息中会改变鼠标光标
}










// 设置一个窗体的或子窗口控件的事件（多个重载版本）
// ptrFunHandler 为一个事件处理函数的地址
// 如果是窗体事件，idResControl 参数应设为0
// 如果是控件事件，idResControl 参数应设为 控件的资源ID
// 如果是菜单（包括系统菜单）、加速键事件，第2个参数为 eMenu_Click，将忽略 idResControl 参数
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
		// ============ 菜单（包括系统菜单）或加速键的事件 ============
		// 忽略 idResControl
		// 将事件处理函数地址存入 m_hashEventNotify.Item(c_CBMenuClickEventKey)，函数需 3 个 int 型的参数
		if (m_hashEventNotify.IsKeyExist(c_CBMenuClickEventKey)) 
			m_hashEventNotify.Remove(c_CBMenuClickEventKey);
		m_hashEventNotify.Add( (long)ptrFunHandler, c_CBMenuClickEventKey, 3, 0, 0, 0, 0, false); 
	}
	else
	{
		// ============ 窗体或控件的事件 ============
		EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 3);
	}

}

void CBForm::EventAdd( unsigned short int idResControl, 
					  ECBFormCtrlEventsIIII eventType, 
					  ONEventIIII ptrFunHandler )
{
	EventAddPrivate(idResControl, (long)eventType, (long)ptrFunHandler, 4);
}

// 设置一个窗体的或子窗口控件的事件（多个重载版本的共用函数）
void CBForm::EventAddPrivate(unsigned short int idResControl, long eventType, long ptrFunHandler, int nParamsIntCount )
{
	if (eventType & c_CBNotifyEventBase )
	{   
		// ============ 以通知消息产生的控件事件 ============
		// 将事件函数地址存入 m_hashEventNotify
		// key 的高 2 位为 WM_COMMAND 消息或 WM_NOTIFY 消息的通知码
		//  （通知码就是 eventType 的枚举值），低2位为控件ID
		long key = (long)MAKELONG(idResControl, eventType); 
		if (m_hashEventNotify.IsKeyExist(key))
			m_hashEventNotify.Remove(key);
		m_hashEventNotify.Add(ptrFunHandler, key, nParamsIntCount,0,0,0,0,false);
		
		// 调整必要的风格，否则若不是此风格则不能响应该事件
		if (m_hWnd  &&  (eventType | c_CBNotifyEventBase))
		{ 
			// 为子窗口控件添加 BS_NOTIFY 风格
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
		// ============ 以窗口消息产生的事件 ============
		//  （非通知消息的 WM_XXX，包括向窗体和向子窗口控件发送 
		//   WM_XXX 产生的，包括窗体事件和控件事件）
		// 将事件函数地址存入 m_hashEventMessage
		// key 的低 2 位为消息 WM_XXX；
		//       高 2 位为 0 （对窗体事件）或 控件资源ID（对控件事件）
		long key = (long)MAKELONG(eventType, idResControl);
		
		// key=WM_COMMAND消息的wParam（高位为通知码，低位为控件ID），值=用户函数地址
		if ( m_hashEventMessage.IsKeyExist(key) )
			m_hashEventMessage.Remove(key, false);
		m_hashEventMessage.Add(ptrFunHandler, key, nParamsIntCount,0,0,0,0,false);

		// 调整必要的风格，否则若不是此风格则不能响应该事件
		if (eventType == eFilesDrop)	// WM_DROPFILES
		{
			if (idResControl==0)
			{
				// 为窗体添加 接收 WM_DROPFILES 消息
				DragAcceptFiles(m_hWnd, 1);
			}
			else
			{ 
				// 为子窗口控件添加 接收 WM_DROPFILES 消息
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
	case WM_COMMAND:	// 子窗口控件通知消息、或菜单消息、或加速键消息
		if (lParam)
		{
			// ======== 子窗口控件通知消息 ========
			// wparam 高位为通知码，低位为控件ID；整个作为“键”
			// 在 m_hashEventNotify 中查找有无键为 wparam 的项，如果有
			//   表示这个事件需要处理，调用用户函数处理事件
			// 用户函数的地址在 m_hashEventNotify 的对应项的 Data 中保存
			if (m_hashEventNotify.IsKeyExist((long)wParam))
			{
				ONEventVoid ptrFunc0 = NULL; 
				ONEventI ptrFunc1 = NULL;
				ONEventII ptrFunc2 = NULL;
				ONEventIII ptrFunc3 = NULL;
				ONEventIIII ptrFunc4 = NULL;
				switch(m_hashEventNotify.ItemLong((long)wParam, false))
				{
				case 0:		// 0 个参数
					ptrFunc0 =
						(ONEventVoid)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc0) (*ptrFunc0)();								// 加 if 容错判断
					break;
				case 1:		// 1 个 int 型参数
					ptrFunc1 = 
						(ONEventI)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc1) (*ptrFunc1)((int)wParam);						// 加 if 容错判断
					break;
				case 2:		// 2 个 int 型参数
					ptrFunc2 = 
						(ONEventII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc2) (*ptrFunc2)((int)wParam, (int)lParam);		// 加 if 容错判断
					break;
				case 3:		// 3 个 int 型参数
					ptrFunc3 = 
						(ONEventIII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc3) (*ptrFunc3)((int)LOWORD(wParam), (int)HIWORD(wParam), 
						lParam);												// 加 if 容错判断
					break;
				case 4:		// 4 个 int 型参数
					ptrFunc4 = 
						(ONEventIIII)m_hashEventNotify.Item((long)wParam, false);
					if (ptrFunc4) (*ptrFunc4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
						(int)LOWORD(lParam), (int)HIWORD(lParam));				// 加 if 容错判断
					break;

				}	// end of switch
			}	// end of if (m_hashEventNotify.IsKeyExist((long)wparam))
		}	// end of if (lparam)
		else	// if (lparam) else：lparam==NULL
		{  
			// ======== 菜单消息（系统菜单不在此处处理）或加速键消息 ========
			ONEventIII ptrMenuClick;
			ptrMenuClick = (ONEventIII)m_hashEventNotify.Item(c_CBMenuClickEventKey, false);
			if (ptrMenuClick)
			{
				// 参数1为被单击的菜单资源ID；
				// 参数2为是直接选择的菜单(=0)，还是从加速键执行的(=1)
				// 参数3=0 表示是从普通菜单选择的，=1表示是从系统菜单选择的
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

		// 将所有子窗口控件子类处理：参数 lParam 设为本窗体的句柄 m_hWnd
		EnumChildWindows( m_hWnd, EnumChildProcSubClass, (LPARAM)m_hWnd );

		// 调用 Form_Load 事件函数（如果有的话）
		{
			ONEventVoid ptrFuncForm0 = (ONEventVoid)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)();		// 加 if 容错判断
		}
		break;

	case WM_DESTROY:
		// 必须有 WM_DESTROY 处理过程。因父窗体被关闭时，其子窗体的自动关闭过程
		//   不会收到 WM_CLOSE 消息，只会收到 WM_DESTROY 消息

		// 调用 Form_UnLoad 事件函数（如果有的话）
		{
			ONEventVoid ptrFuncForm0 = (ONEventVoid)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)();		// 加 if 容错判断
		}
		
		// 在 ClearResource 中，将枚举子窗口，恢复所有子窗口控件的子类处理
		ClearResource();
		break;

	case WM_CLOSE:
		
		// 调用 Form_QueryLoad 事件函数（如果有的话）
		{
			int bCancel = 0;
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) 
			{
				(*ptrFuncForm0)((int)(&bCancel));		// 加 if 容错判断
				if (bCancel) return bCancel;			// 取消 WM_CLOSE 消息，不卸载窗体
			}
		}

		// 如果前面没有取消（bCancel没有为非0值），继续卸载对话框
		// 不能用 SendMessage(m_hWnd, WM_DESTROY, 0, 0); 
		DestroyWindow(m_hWnd);
		
		// 此时，应该在 CBForm_DlgProc 中收到了 WM_DESTROY 消息，并执行了 ClearResource()
		//   而使 m_hWnd 为 0。如果此时 m_hWnd 仍不为 0，为容错，下面再次执行 ClearResource()
		if (m_hWnd) ClearResource(); 

		break;

	case WM_ACTIVATE:
		
		// 调用 eForm_ActiDeactivate 事件函数（失活时）（如果有的话）
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)(0);		// 加 if 容错判断
		}

		// 切换当前前台窗体
		ms_hWndActiForm=m_hWnd;

		// 切换加速键
		if (LOWORD(wParam)==WA_ACTIVE || LOWORD(wParam)== WA_CLICKACTIVE)
		{
			ms_hAccelCurrUsed=(HACCEL)ms_hashWnd.ItemLong((long)m_hWnd, false); 
		}
		

		// 调用 eForm_ActiDeactivate 事件函数（激活时）（如果有的话）
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			ONEventI ptrFuncForm0 = (ONEventI)m_hashEventMessage.Item((long)uMsg, false);
			if (ptrFuncForm0) (*ptrFuncForm0)(1);		// 加 if 容错判断
		}

		break;

	case WM_SHOWWINDOW:
		// 如果窗口正被隐藏，而且该窗口为模态，恢复所有窗口 Enabled
		if (wParam==0)
			if (m_ModalShown) RestoreModalWinEnabled();
		
		break;
		
	case WM_SETCURSOR:
		if (ms_CursorGlobalIdx)
		{
			// ======== 使用全局鼠标光标 ========
			if (ms_hCursorGlobal)
			{
				SetCursor(ms_hCursorGlobal);
				return 1;	// return 1; 表示不让默认窗口程序处理了
			}
		}
		else if (m_CursorIdx) 
		{
			// ======== 使用为本窗体所设置的鼠标光标 ========
			if (m_hCursor)
			{					
				SetCursor(m_hCursor);
				return 1;	// return 1; 表示不让默认窗口程序处理了
			}
		}
		
		// 其他使用默认，即窗口类的鼠标光标（默认窗口程序自动处理）
		break;
		
	case WM_ERASEBKGND:
		// 设置窗口背景色
		if (m_BackColor != -1)	// -1 表示采用系统默认颜色，不需干预
		{
			RECT rectBK;
			HBRUSH hBrushBK = CreateSolidBrush(m_BackColor);
			GetClientRect(m_hWnd, &rectBK);
			FillRect((HDC)wParam, &rectBK, hBrushBK);
			DeleteObject(hBrushBK);
			return 1;	// 如应用程序自行擦出背景，必须返回 1
		}

		break;
	case WM_CTLCOLORSTATIC: 
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLOREDIT:
		// 设置子窗口控件背景色和前景色（这些消息是发送到父窗口的，不是发送到控件）
		{
			STRControlProp * pProp = CBForm::PropPtr((HWND)lParam); 
			if (pProp)
			{
				if (pProp->foreColor != -1) // 设置前景色
				{
					SetTextColor((HDC)wParam, pProp->foreColor);
					// 为使前景色设置有效，必须返回背景色画刷
					//   如背景色没有设置，这里将之设置为系统颜色
					if (pProp->backColor == -1)
					{
						pProp->backColor = GetSysColor(COLOR_BTNFACE);
						if (pProp->hBrushBack) DeleteObject(pProp->hBrushBack);
						pProp->hBrushBack = CreateSolidBrush(pProp->backColor);
					}
				}

				if (pProp->backColor != -1)
				{
					// 设置背景色
					// 先将文字背景改为透明
					SetBkMode((HDC)wParam, TRANSPARENT);
					// 返回画刷
					if (pProp->hBrushBack) return (long)(pProp->hBrushBack);
				}
			}
		}

		break;

	// ==================================================================
	//	特殊事件的处理
	case WM_LBUTTONDOWN: 	case WM_RBUTTONDOWN:	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:		case WM_RBUTTONUP:		case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:	case WM_MBUTTONDBLCLK:	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
		// 鼠标左、右、中键按下、抬起、双击、鼠标移动
		{
			long key = 0;  int button = 0;
			if (uMsg == 0x200)				// 鼠标移动
			{
				key = WM_MOUSEMOVE;
				if ( wParam & MK_LBUTTON ) button = button | 1;
				if ( wParam & MK_RBUTTON ) button = button | 2;
				if ( wParam & MK_MBUTTON ) button = button | 4;
			}
			else if ((uMsg-0x200) % 3 == 1)		// 按下：0x201、0x204、0x207，(uMsg-0x200) % 3 == 1
			{
				key = WM_LBUTTONDOWN;		// 统一到 WM_LBUTTONDOWN 而不分为3个枚举值
				button = 1 << (((uMsg-0x0200)-1)/3); 
			}
			else if ((uMsg-0x200) % 3 == 2)	// 抬起：0x202、0x205、0x208，(uMsg-0x200) % 3 == 2
			{
				key = WM_LBUTTONUP;			// 统一到 WM_LBUTTONUP 而不分为3个枚举值
				button = 1 << (((uMsg-0x0200)-2)/3);
			}
			else if ((uMsg-0x200) % 3 == 0)	// 双击：0x203、0x206、0x209，(uMsg-0x200) % 3 == 0
			{
				key = WM_LBUTTONDBLCLK;		// 统一到 WM_LBUTTONDBLCLK 而不分为3个枚举值
				button = 1 << ((uMsg-0x0203)/3);
			}
			else
				break;	// 容错

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

	case WM_KEYDOWN: case WM_KEYUP: 		//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
	case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		{
			long key = uMsg;
			if (uMsg>=0x0104) key = (uMsg-0x4);	// 将WM_SYS消息归并到WM_消息
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
				// 参数1为被单击的菜单的ID：SC_CLOSE、SC_MAXIMIZE 等
				// 参数2为是直接选择的菜单(=0)，还是从加速键执行的(=1)
				// 参数3=0 表示是从普通菜单选择的，=1表示是从系统菜单选择的
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
			if (ptrFuncForm1)													// 加 if 容错判断
			{
				if (lParam)
				{
					// 使用模块级 m_MenuItemSel 成员绑定到所选菜单
					if (HIWORD(wParam) & MF_POPUP)
						// 所选菜单项将引出一个弹出式菜单：LOWORD(wParam) 是索引号
						m_MenuItemSel.SetFromPos((HMENU)lParam, LOWORD(wParam)+1, m_hWnd, &m_hashMenuIDs, &m_hashObjs);	//+1 转换为位置号从1开始
					else
						// 所选菜单项是普通命令项：LOWORD(wParam) 是 ResID
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
			if (ptrFuncForm4)													// 加 if 容错判断
			{
				// 获得拖动文件的总个数
				UINT count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
				if (count==0) break;	// 容错

				// 开辟保存字符串指针的空间
				unsigned long * ptrArr = new unsigned long [count+1]; // 不适用 [0] 的空间
				HM.AddPtr(ptrArr);
				HM.ZeroMem(ptrArr, sizeof(unsigned long)*(count+1));

				// 开辟各自字符串的空间，并获得各文件名
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

				// 获得拖放文件时鼠标所在位置
				POINT ptDrag;
				DragQueryPoint((HDROP)wParam, &ptDrag); 
					
				// 释放句柄 hDrop
				DragFinish((HDROP)wParam);

				// 生成事件
				(*ptrFuncForm4)((int)ptrArr, count, (int)ptDrag.x, (int)ptDrag.y);					
			}
		}		

		break;

	default:
		// 按窗体事件处理
		if (m_hashEventMessage.IsKeyExist((long)uMsg))
		{
			ONEventVoid ptrFuncForm0 = NULL;
			ONEventI ptrFuncForm1 = NULL;
			ONEventII ptrFuncForm2 = NULL;
			ONEventIII ptrFuncForm3 = NULL;
			ONEventIIII ptrFuncForm4 = NULL;
			switch(m_hashEventMessage.ItemLong((long)uMsg, false))
			{
			case 0:		// 0 个参数
				ptrFuncForm0 =
					(ONEventVoid)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm0) (*ptrFuncForm0)();							// 加 if 容错判断
				break;
			case 1:		// 1 个 int 型参数
				ptrFuncForm1 = 
					(ONEventI)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm1) (*ptrFuncForm1)((int)wParam);					// 加 if 容错判断
				break;
			case 2:		// 2 个 int 型参数
				ptrFuncForm2 = 
					(ONEventII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm2) (*ptrFuncForm2)((int)wParam, (int)lParam);	// 加 if 容错判断
				break;
			case 3:		// 3 个 int 型参数
				ptrFuncForm3 = 
					(ONEventIII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm3) (*ptrFuncForm3)((int)LOWORD(wParam), (int)HIWORD(wParam),  
					(int)lParam);												// 加 if 容错判断
				break;
			case 4:		// 4 个 int 型参数
				ptrFuncForm4 = 
					(ONEventIIII)m_hashEventMessage.Item((long)(uMsg), false);
				if (ptrFuncForm4) (*ptrFuncForm4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					(int)LOWORD(lParam), (int)HIWORD(lParam));					// 加 if 容错判断
				break;
			}
		}
		break;
	}

	return 0;
}



long CBForm::EventsGeneratorCtrl( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	long key = 0;											// 用于检索哈希表的 key
	int idCtrl = GetDlgCtrlID(hWnd);						// 来自控件的控件 ID
	if (idCtrl == 0) return gc_APICEventsGenDefautRet;		// 无法获得控件 ID， 不进行事件处理，立即返回
	
	// 即将检索哈希表 m_hashEventMessage，键为：低 2 位为消息（WM_XXX），高 2 位为控件 ID
	key = MAKELONG(uMsg, idCtrl);

	switch (uMsg)
	{

	case WM_SETCURSOR:
		// 设置鼠标光标：不用 SetCursor 设置的，Windows 将自动显示窗口类的光标
		if (ms_CursorGlobalIdx)
		{
			// ======== 使用全局鼠标光标 ms_hCursorGlobal ========
			if (ms_hCursorGlobal)
			{
				SetCursor(ms_hCursorGlobal);
				return 1;	// return 1; 表示不让默认窗口程序处理了
			}
		}
		else
		{
			// 判断是否为本子窗口控件设置了鼠标光标
			STRControlProp * pProp = PropPtr(hWnd); 
			if (pProp)
			{
				if (pProp->cursorIndex)
				{
					// ======== 使用为本子窗口控件设置的鼠标光标 ========
					if (pProp->hCursor)
					{
						SetCursor(pProp->hCursor);
						return 1;	// return 1; 表示不让默认窗口程序处理了
					}
				}
			}
			// ======== 其他使用默认，即窗口类的鼠标光标（默认窗口程序自动处理） ========
		}
		
		break;

	// ==================================================================
	//	特殊事件的处理
	case WM_LBUTTONDOWN: 	case WM_RBUTTONDOWN:	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:		case WM_RBUTTONUP:		case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:	case WM_MBUTTONDBLCLK:	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
		// 鼠标左、右、中键按下、抬起、双击、鼠标移动
		{
			long key = 0;  int button = 0;
			if (uMsg == 0x200)				// 鼠标移动
			{
				key = MAKELONG(WM_MOUSEMOVE, idCtrl);
				if ( wParam & MK_LBUTTON ) button = button | 1;
				if ( wParam & MK_MBUTTON ) button = button | 4;
				if ( wParam & MK_RBUTTON ) button = button | 2;
			}
			else if ((uMsg-0x200) % 3 == 1)				// 按下：0x201、0x204、0x207，(uMsg-0x200) % 3 == 1
			{
				key = MAKELONG(WM_LBUTTONDOWN, idCtrl);	// 统一到 WM_LBUTTONDOWN 而不分为3个枚举值
				button = 1 << ((uMsg-0x0200-1)/3); 
			}
			else if ((uMsg-0x200) % 3 == 2)				// 抬起：0x202、0x205、0x208，(uMsg-0x200) % 3 == 2
			{
				key = MAKELONG(WM_LBUTTONUP, idCtrl);	// 统一到 WM_LBUTTONUP 而不分为3个枚举值
				button = 1 << ((uMsg-0x0200-2)/3); 
			}
			else if ((uMsg-0x200) % 3 == 0)				// 双击：0x203、0x206、0x209，(uMsg-0x200) % 3 == 0
			{
				key = MAKELONG(WM_LBUTTONDBLCLK,idCtrl);// 统一到 WM_LBUTTONDBLCLK 而不分为3个枚举值
				button = 1 << ((uMsg-0x0200-3)/3);
			}
			else
				break;	// 容错
			
			if (m_hashEventMessage.IsKeyExist(key))
			{
				ONEventIIII ptrFuncForm4 = NULL;
				ptrFuncForm4 = 
					(ONEventIIII)m_hashEventMessage.Item(key, false);
				if (ptrFuncForm4)			// 加 if 容错判断
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
		// 如果优先由窗体处理键盘事件，先触发窗体的键盘事件
		if (KeyPreview) EventsGenerator(uMsg, wParam, lParam); 

		// 触发控件的键盘事件
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
		
	case WM_KEYDOWN: case WM_KEYUP: 		//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
	case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		// 如果优先由窗体处理键盘事件，先触发窗体的键盘事件
		if (KeyPreview) EventsGenerator(uMsg, wParam, lParam); 

		// 触发控件的键盘事件
		if (uMsg>=0x0104) key = MAKELONG((uMsg-0x4), idCtrl);	// 将WM_SYS消息归并到WM_消息
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

		// 由于在 MessageLoop 中屏蔽了 IsDialogMessage 对回车的处理
		//   使在按钮上按下回车时不能触发按钮被按下的事件
		//   这里修补：
		// 如果是按钮控件，且回车被按下，就模拟按钮被单击
		if (wParam == VK_RETURN && uMsg==WM_KEYUP) 
		{
			TCHAR sClassName[255];		
			// 获得类名字符串 => m_ClassName
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
			if (ptrFuncForm4)													// 加 if 容错判断
			{
				// 获得拖动文件的总个数
				UINT count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
				if (count==0) break;	// 容错
				
				// 开辟保存字符串指针的空间
				unsigned long * ptrArr = new unsigned long [count+1]; // 不适用 [0] 的空间
				HM.AddPtr(ptrArr);
				HM.ZeroMem(ptrArr, sizeof(unsigned long)*(count+1));
				
				// 开辟各自字符串的空间，并获得各文件名
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
				
				// 获得拖放文件时鼠标所在位置
				POINT ptDrag;
				DragQueryPoint((HDROP)wParam, &ptDrag); 
				
				// 释放句柄 hDrop
				DragFinish((HDROP)wParam);
				
				// 生成事件
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
			case 0:		// 0 个参数
				ptrFunc0 =
					(ONEventVoid)m_hashEventMessage.Item(key, false);
				if (ptrFunc0) (*ptrFunc0)();								// 加 if 容错判断
				break;
			case 1:		// 1 个 int 型参数
				ptrFunc1 = 
					(ONEventI)m_hashEventMessage.Item(key, false);
				if (ptrFunc1) (*ptrFunc1)((int)wParam);						// 加 if 容错判断
				break;
			case 2:		// 2 个 int 型参数
				ptrFunc2 = 
					(ONEventII)m_hashEventMessage.Item(key, false);
				if (ptrFunc2) (*ptrFunc2)((int)wParam, (int)lParam);		// 加 if 容错判断
				break;
			case 3:		// 3 个 int 型参数
				ptrFunc3 = 
					(ONEventIII)m_hashEventMessage.Item(key, false);
				if (ptrFunc3) (*ptrFunc3)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					lParam);												// 加 if 容错判断
				break;
			case 4:		// 4 个 int 型参数
				ptrFunc4 = 
					(ONEventIIII)m_hashEventMessage.Item(key, false);
				if (ptrFunc4) (*ptrFunc4)((int)LOWORD(wParam), (int)HIWORD(wParam), 
					(int)LOWORD(lParam), (int)HIWORD(lParam));				// 加 if 容错判断
				break;
				
			}	// end of switch
		}	// end of if (m_hashEventMessage.IsKeyExist(key)
	} // end of switch (uMsg)

	return gc_APICEventsGenDefautRet;
}




void CBForm::RestoreModalWinEnabled()
{
	// 在显示一个模态对话框时：
	//   若其他某窗口目前为 Disabled 且此值为0，则维持该窗口的此值不变；
	//   否则，其他某窗口目前为 Enabled，或者该窗口此值 >0，都会将该窗口的此值 +1
	// 在隐藏一个模态对话框时：
	//   如某窗口此值为0，则不做任何操作；否则将此值-1，如-1后为0，则恢复为 Enabled 状态

	int i;  long level=0; 
	for (i=1; i<=ms_hashWnd.Count(); i++)
	{
		level = ms_hashWnd.ItemLong2FromIndex(i, false);
		if (level>0)
		{
			level--;
	
			if (level<=0)
			{
				// 恢复窗口 Enabled
				EnableWindow((HWND)ms_hashWnd.IndexToKey(i), 1);
				level = 0;		// 容错：此时 level 就应为0，
			}
			
			// 将层次记录到 ItemLong2
			ms_hashWnd.ItemLong2FromIndexSet(i, level, false);
		}
		// else level==0 不做任何操作
	}

	// 恢复 Owner 窗口为前台
	SetActiveWindow (GetWindow(m_hWnd, GW_OWNER));

	// 清除标志变量
	m_ModalShown=NULL;

	// 模态层次减1
	ms_iModalLevel--; 
	if (ms_iModalLevel<0) ms_iModalLevel=0;		// 容错
}


void CBForm::ConstructMenuIDItems( HMENU hMenuParent, bool bIsTop/*=false*/ )
{
	int i, iCount;
	UINT idItem;
	
	if (bIsTop) m_hashMenuIDs.Clear();	// 清除已有记录
	if (0==hMenuParent) return;   

	// 获得 hMenuParent 下的菜单项总数
	iCount = GetMenuItemCount(hMenuParent); 
	if (iCount == -1) return;		// GetMenuItemCount 调用失败，或无菜单项
	
	// 逐个处理 hMenuParent 下的所有菜单项
	for(i=0; i<=iCount-1; i++)
	{
		idItem = GetMenuItemID(hMenuParent, i);
		if (idItem == 0xFFFFFFFF)
		{
			// hMenu 下的第 i 项有级联菜单
			// 递归处理此级联菜单下的所有子菜单项
			ConstructMenuIDItems(GetSubMenu(hMenuParent, i));
		}
		else
		{
			// hMenu 下的第 i 项已获得菜单项 ID，保存此 ID 到哈希表
			// 哈希表 key = 菜单项的资源id，data = 该菜单的父菜单的句柄
			m_hashMenuIDs.Add((int)hMenuParent, idItem, 0, 0, 0, 0, 0, false);
		}
		
	}
}




void CBForm::ClearResource()
{
	// 释放所加载的对象
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
			default:	// 包含 IMAGE_BITMAP
				DeleteObject((HGDIOBJ)m_hashObjs.ItemFromIndex(i, false));
				break;
			}
		}
		m_hashObjs.Clear();
	}
	
	// 释放字体对象
	if (m_hFont) DeleteObject(m_hFont);

	// 释放窗体
	if (m_hWnd)
	{
		//==============================================================
		// 在此处不能再调用 DestroyWindow 或发送 WM_DESTROY 消息
		// 因收到 WM_DESTROY 消息时会调用本函数，否则将形成死递归

		// 恢复子窗口控件的子类处理：lParam 设为 0 表示卸载窗口时恢复子类处理子窗口控件
		EnumChildWindows(m_hWnd, EnumChildProcSubClass, 0);

		// 如果前台窗口刚好是本窗口，取消加速键记录和前台窗口记录
		if (CBForm::ms_hWndActiForm == m_hWnd)
		{
			CBForm::ms_hWndActiForm = NULL;
			CBForm::ms_hAccelCurrUsed = NULL;
		}
		
		// 从哈希表中删除该对话框的记录
		ms_hashWnd.Remove((long)m_hWnd, false);
		
		// 如果该窗口为模态，恢复所有窗口 Enabled
		if (m_ModalShown) RestoreModalWinEnabled();
		
		// 清除成员
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
	// 调用基类的虚函数实现功能，函数返回新字体对象句柄
	HFONT hFontNew = CBWndBase::SetFontMLF(lpLf);	
	
	// 将新建立的字体对象句柄保存到 m_hFont 私有成员
	if (m_hFont) DeleteObject(m_hFont);		// 删除旧字体
	m_hFont = hFontNew;						// 用 m_hFont 私有成员记录新字体对象
	
	// 返回新字体对象句柄
	return hFontNew;
}














