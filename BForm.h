//////////////////////////////////////////////////////////////////////
// BForm.h: CBForm 类的定义
//
// CBForm 类：实现对话框（模态、非模态），在程序中用作“窗体”
//   实现对话框显示、隐藏、事件处理等操作，但不涉及控件
//
// 程序中包含 BForm 则不需再编写 WinMain 函数，而是仍编写 main 函数
//   main 函数无参数，返回值仍是 int：int main()
// 这时程序中的 main() 函数执行结束，整个程序不会结束
//   整个程序结束的条件是main() 函数执行结束，并且所有窗体
//   全部卸载（本模块的 WinMain 结束）
//
// BForm 包含消息循环，并定义了全局函数 DoEvents(); 
// 可以直接调用 BForm 的静态成员函数：MousePointerGlobal 和 
//   MousePointerSetGlobal 返回或设置 全局鼠标光标形状
//
// 包含本头文件则不必包含 BWindows.h、BControl.h、BMenuItem.h，
//   因为本头文件已经包含了它们
//
//
// 支持：
//     需要 BWindows、BControl、BMenuItem 模块的支持
//////////////////////////////////////////////////////////////////////


// ==== 用法：========================================================
// 在定义本类对象时，必须给出参数 idResDialog，表示对话框资源 ID，例如：
//      CBForm form1(IDD_FORM1);
// 不要将本类对象定义成局部变量，否则窗体将在对应函数结束时被卸载，
//   而应将本类对象定义为全局变量
// 主要用法为：
//   form1.Show();
//   form1.Control(IDCANCEL).EnabledSet(false);
//   form1.Control(IDC_EDIT1).TextSet("abc");
//   form1.EventAdd(IDOK, eCommandButton_Click, cmdOK_Click);
// void cmdOK_Click()
// {
// 	  MsgBox("OK");
// }
//
//
// 加速键用法：
// 随时可用 SetAccelerator(加速键资源ID) 启用一个窗体的加速键功能
//   或取消加速键功能（参数为0）。
// 必须在本窗体为前台时加速键才有效。不同窗体可有不同加速键，
//   哪个窗体为前台，对应窗体的加速键为有效。
// 加速键的事件处理函数，和菜单单击的事件处理函数为同一个：
//   Menu_Click
// ===================================================================


// ==== 技术：========================================================
// 本类不用 DialogBoxParam 创建真正意义上的“模态”对话框，所有 Form 都
//   是非模态的（因 DialogBoxParam 无法处理加速键）
// 要显示“模态”对话框，本类会将所有由本类管理的窗体的 Enabled 都设为
//   false，待模态对话框结束后，再恢复本类管理的所有窗体的 Enabled。
// 这种做法使本类还允许在已经显示“模态”对话框的同时，再显示另一个
//   “非模态”对话框，本类自动将后者置为 Enabled 为 false
//
// 消息循环函数 MessageLoop() 除被 WinMain 调用外，还可被 DoEvents 调用
//   以及显示“模态”对话框时在 ShowWindow 之后被调用：这使在已显示一个
//   “模态”对话框时，在对话框结束后，Show() 方法才会返回
// ===================================================================

#pragma once

#include "BWndBase.h"
#include "BControl.h"
#include "BMenuItem.h"



//////////////////////////////////////////////////////////////////////////
// 全局函数 和 WinMain 函数
//////////////////////////////////////////////////////////////////////////

// 全局 DoEvents 函数：让CPU处理其他事件，处理完毕再返回
//   包含本 h 文件的任何模块都可调用
extern void DoEvents();

// 全局 End 函数：结束本线程的运行，发送 PostQuitMessage(nExitCode)
//   包含本 h 文件的任何模块都可调用
extern void End(int nExitCode=0);


// WinMain 函数不写原型定义，直接在 BForm.cpp 中写函数实现

// 公用窗口过程：所有本类对象（窗体）都用此函数作为窗口过程
static BOOL WINAPI CBForm_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 公用窗口过程：所有本类对象（窗体）中的子窗口控件，都用此函数作为窗口过程，所有子窗口控件都被子类处理
static BOOL WINAPI CBForm_ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 枚举子窗口控件、子类处理（取消子类处理）所有子窗口控件的回调函数：
// lParam 为控件所属窗体的句柄（非0）时，表示加载窗体时的子类处理；
// lParam 为 0 时，表示卸载窗体时的恢复子类处理
static BOOL CALLBACK EnumChildProcSubClass(HWND hwnd, LPARAM lParam);


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
static void MessageLoop(int iExitStyle=0);



//////////////////////////////////////////////////////////////////////////
// CBForm 类
//////////////////////////////////////////////////////////////////////////


// 以通知消息（WM_COMMAND 或 WM_NOTIFY）产生的子窗口控件的事件基数
// 1. 以通知消息产生的所有控件事件的枚举值 都 | 此值，对应二进制位为1
// 2. 以其他消息产生的所有窗体事件、控件事件的枚举值 都没有此二进制标志位
//   (在 enum ECBFormCtrlEventsVoid、enum ECBFormCtrlEventsI 等中定义枚举值)
#define c_CBNotifyEventBase 0x20000000

// 菜单（包括系统菜单）或加速键被按下的事件
// 在 m_hashEventNotify 中 Key 值为此值的 Item 为此事件的 事件处理函数的地址
#define c_CBMenuClickEventKey -1


// ======================================================================
// 添加事件方法：
//   添加下面的一项枚举值
//   如果是特殊事件，再在 EventsGenerator 或 EventsGeneratorCtrl 
//   中添加相关代码即可，其他不需修改
// ======================================================================

// 定义所支持的窗体事件、控件事件的枚举值
//   （只包含 事件处理函数无参数无返回值的 事件）
enum ECBFormCtrlEventsVoid		
{
	// 以通知消息产生的控件事件：枚举值 = 通知码 | c_CBNotifyEventBase
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的高 2 位
	//   Key 的低两位为控件ID
	eCommandButton_Click = BN_CLICKED | c_CBNotifyEventBase,		// void cmd1_click();
	eCommandButton_LostFocus = BN_KILLFOCUS | c_CBNotifyEventBase,	// void cmd1_LostFocus();
	eCommandButton_GotFocus = BN_SETFOCUS | c_CBNotifyEventBase,	// void cmd1_GotFocus();
	eStatic_Click = STN_CLICKED | c_CBNotifyEventBase,				// void pic1_Click();
	eStatic_DblClick = STN_DBLCLK | c_CBNotifyEventBase,			// void pic1_DblClick();
	eListBox_SelChange = LBN_SELCHANGE | c_CBNotifyEventBase,		// void List1_SelChange(); 
			// 用 ListIndex 选择时不产生此事件；多选列表时，用户按下箭头键则就会产生该事件（无论选择有无变化）
	eListBox_SelCancel = LBN_SELCANCEL | c_CBNotifyEventBase,		// void List1_SelCancel();	// 用户取消选择的事件


	// 以其他消息产生的事件：枚举值 = WM_XXX
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的低 2 位
	//   （注意不是高2位，高2位为0表示窗体事件，为控件ID表示控件事件）
	eForm_Load = WM_INITDIALOG,					// Form1_Load();
												// 必须在所有对窗体的操作之前 EventAdd 此事件，
												//   否则如先加载窗体后设置此事件，事件处理函数不会被执行
	eForm_Unload = WM_DESTROY, 					// Form1_UnLoad();
	eForm_Resize = WM_SIZE,						// Form1_Resize();	// 在窗体大小调整之后发生
	eForm_Paint = WM_PAINT,						// Form1_Paint();
	eGotFocus = WM_SETFOCUS,					// void Form1OrCtrl1_GotFocus()
	eLostFocus = WM_KILLFOCUS					// void Form1OrCtrl1_LostFocus()
};

// 定义所支持的窗体事件、控件事件的枚举值
//   （只包含 事件处理函数有1个int的参数，无返回值的 事件）
enum ECBFormCtrlEventsI
{
	// 以通知消息产生的控件事件：枚举值 = 通知码 | c_CBNotifyEventBase
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的高 2 位
	//   Key 的低两位为控件ID


	// 以其他消息产生的事件：枚举值 = WM_XXX
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的低 2 位
	//   （注意不是高2位，高2位为0表示窗体事件，为控件ID表示控件事件）
	eForm_ActiDeactivate = WM_ACTIVATE,	// void Form1_ActiDeactivate(int bActiOrDeacti); // bActiOrDeacti=非0为窗体被激活；=0为失活
	eForm_QueryUnload = WM_CLOSE,		// void Form1_QueryUnLoad(int pbCancel);	将 *((int *)pbCancel) = 非0值，将取消卸载
	eMenu_Select = WM_MENUSELECT,		// void Menu_Select(int ptrCBMenuItem);	// 未指向菜单项时（上次指向现在移开），ptrCBMenuItem 为0
	eMenu_InitPopup = WM_INITMENUPOPUP,	// void Menu_InitPopup(int wParam)
};


// 定义所支持的窗体事件、控件事件的枚举值
//   （只包含 事件处理函数有2个int的参数，无返回值的 事件）
enum ECBFormCtrlEventsII
{
	// 以通知消息产生的控件事件：枚举值 = 通知码 | c_CBNotifyEventBase
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的高 2 位
	//   Key 的低两位为控件ID


	// 以其他消息产生的事件：枚举值 = WM_XXX
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的低 2 位
	//   （注意不是高2位，高2位为0表示窗体事件，为控件ID表示控件事件）
	eKeyPress = WM_CHAR					// void Form1OrCtrl1_KeyPress(int keyAscii, int pbCancel);  // 将 *((int *)pbCancel) = 非0值，将取消
};

// 定义所支持的窗体事件、控件事件的枚举值
//   （只包含 事件处理函数有3个int的参数，无返回值的 事件）
enum ECBFormCtrlEventsIII
{
	
	// 特殊：
	// 菜单和加速键被按下，或者系统菜单被按下，事件函数的地址将存入 m_hashEventNotify 
	//   的 Key 为 c_CBMenuClickEventKey (-1) 的项中
	eMenu_Click = 0x70000000,					// void Menu_Click(int menuID, int bIsFromAcce, int bIsFromSysMenu);


	// 以通知消息产生的控件事件：枚举值 = 通知码 | c_CBNotifyEventBase
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的高 2 位
	//   Key 的低两位为控件ID




	// 以其他消息产生的事件：枚举值 = WM_XXX
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的低 2 位
	//   （注意不是高2位，高2位为0表示窗体事件，为控件ID表示控件事件）
	eKeyDown = WM_KEYDOWN,			// void Form1OrCtrl1_KeyDown(int keyCode, int shift, int pbCancel);   // 将 *((int *)pbCancel) = 非0值，将取消
	eKeyUp = WM_KEYUP,				// void Form1OrCtrl1_KeyUp(int keyCode, int shift, int pbCancel);     // 将 *((int *)pbCancel) = 非0值，将取消

};

// 定义所支持的窗体事件、控件事件的枚举值
//   （只包含 事件处理函数有4个int的参数，无返回值的 事件）
enum ECBFormCtrlEventsIIII
{

	// 以通知消息产生的控件事件：枚举值 = 通知码 | c_CBNotifyEventBase
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的高 2 位
	//   Key 的低两位为控件ID



	// 以其他消息产生的事件：枚举值 = WM_XXX
	//   此枚举值将作为 m_hashEventNotify 的各项 Key 的低 2 位
	//   （注意不是高2位，高2位为0表示窗体事件，为控件ID表示控件事件）

	eMouseDown = WM_LBUTTONDOWN,	// void Form1OrCtrl1_MouseDown(int button, int shift, int x, int y);
									//   button =1,2,4分别表示鼠标左、右、中键被按下；
									//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
									//   x,y 表示鼠标在控件中的坐标（控件坐标系）
									// 左、右、中键被按下的事件枚举值都统一到 WM_LBUTTONDOWN，而不分别设3个值

	eMouseUp = WM_LBUTTONUP,		// void Form1OrCtrl1_MouseUp(int button, int shift, int x, int y);
									//   button =1,2,4分别表示鼠标左、右、中键被按下；
									//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
									//   x,y 表示鼠标在控件中的坐标（控件坐标系）
									// 左、右、中键被抬起的事件枚举值都统一到 WM_LBUTTONUP，而不分别设3个值

	eDblClick = WM_LBUTTONDBLCLK,	// void Form1OrCtrl1_DblClick(int button, int shift, int x, int y);
									//   button =1,2,4分别表示鼠标左、右、中键被按下；
									//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
									//   x,y 表示鼠标在控件中的坐标（控件坐标系）
									// 左、右、中键被双击的事件枚举值都统一到 WM_LBUTTONDBLCLK，而不分别设3个值

	eMouseMove = WM_MOUSEMOVE,		// void Form1OrCtrl1_MouseMove(int button, int shift, int x, int y);
									//   button =1,2,4分别表示鼠标左、右、中键被按下；
									//   shift=1,2,4 分别表示同时 Shift、Ctrl、Alt 键被按下
									//   x,y 表示鼠标在控件中的坐标（控件坐标系）
	
	eFilesDrop = WM_DROPFILES		// void Form1OrCtrl1_eFilesDrop(int ptrArrFiles, int count, int x, int y);
									// 需将 ptrArrFiles 转换类型为 (TCHAR **)ptrArrFiles，然后用 [1]～[count]
									//   获得每个文件的文件名（含全路径）。x,y 为拖放文件时的鼠标光标所在位置

}; 

typedef struct _controlProp			// 控件的附加属性 空间将被动态开辟，
{									//   由 ms_hashCtrls.ItemLong2 保存动态空间的地址
	long cursorIndex;				// 控件上的鼠标光标指针索引号
	HCURSOR hCursor;				// 控件上的鼠标光标句柄（在 cursorIndex 非0时才有效）
	COLORREF backColor;				// 控件背景色
	HBRUSH hBrushBack;				// 控件背景色画刷（设置背景色的同时就创建；EnumChildProcSubClass 中的卸载部分再 DeleteObject ）
	COLORREF foreColor;				// 控件前景色
	HFONT hFont;					// 所用字体（为0表示用系统字体；否则表示用自定义字体，最后需 DeleteObject ）

	LPTSTR tagString;				// 附加字符串数据

	
	// ======================================================================
	// 添加新属性方法：
	//	1. 这里添加新成员；
	//	2. CBControl 类中增加获取属性和设置属性的函数，函数中通过调用
	//	   CBForm 的静态函数 PropPtr 分配和获得本结构体数据的动态空间，
	//	   然后使用动态空间中的本成员
	//	   （可参见 BackColor() 和 BackColorSet(COLORREF color)）
	//	3. 实现功能：在 BForm.cpp 的 CBForm_DlgProc 或 CBForm_ControlProc 函数中
	//	   或相关 EventsGenerator 函数中添加处理相关消息的代码，实现功能，代码中
	//	   也使用 CBForm 的静态函数 PropPtr 获得属性设置；
	//	4. 如需要，应在 EnumChildProcSubClass 中的卸载部分卸载相应资源。
	// ======================================================================


} STRControlProp;

// 定义事件处理函数的函数指针类型
typedef void (*ONEventVoid)();					// 无返回值无参数的 事件处理函数的地址
typedef void (*ONEventI)(int);					// 无返回值有 1 个int型参数的 事件处理函数的地址
typedef void (*ONEventII)(int,int);				// 无返回值有 2 个int型参数的 事件处理函数的地址
typedef void (*ONEventIII)(int,int,int);		// 无返回值有 3 个int型参数的 事件处理函数的地址
typedef void (*ONEventIIII)(int,int,int,int);	// 无返回值有 4 个int型参数的 事件处理函数的地址

class CBForm : public CBWndBase		// 管理一个窗体（对话框）的对象
{
private:

	// ======================================================================================
	// 私有静态成员和成员函数
	// ======================================================================================

	// 静态哈希表（所有本类对象共用）：
	//   保存所有通过本类对象管理的、本程序的所有窗体的信息
	// Key=hWnd, Item = (unsigned int)本对象地址即this, 
	// ItemLong = 加速键句柄，ItemLong2 = 受模态对话框影响的本窗口Enabled状态：
	//   0 = 无影响；
	//   >0 = 累加需恢复为 Enabled 层次数
	//   （若窗口原为 Disabled，则显示模态对话框时此值仍被设置为0）
	// 在显示一个模态对话框时：
	//   若其他某窗口目前为 Disabled 且此值为0，则维持该窗口的此值不变；
	//   否则，其他某窗口目前为 Enabled，或者该窗口此值 >0，都会将该窗口的此值 +1
	// 在隐藏一个模态对话框时：
	//   如某窗口此值为0，则不做任何操作；否则将此值-1，如-1后为0，则恢复为 Enabled 状态
	// 在 DialogBoxParam 或 CreateDialogParam 函数中，设置将来传递到 DlgProc 的 
	//   WM_INITDIALOG 消息的 lParam 参数为本对象地址即 this
	//   在 DlgProc 函数中处理 WM_INITDIALOG 消息时将向哈希表中添加一个项目
	static CBHashLK ms_hashWnd;

	// 静态哈希表（所有本类对象共用）：
	//   保存所有子窗口控件信息（位于所有窗体上的控件都统一用此保存）
	// 任何窗体被加载时（WM_INITDIALOG）递归枚举所有子窗口，将之子类化并将
	//   子窗体信息保存于此；任何窗体被卸载时（ClearResource），递归枚举所
	//   有子窗口，从该哈希表中删除这些子窗口信息
	// Key = 控件hwnd，
	// Item = 控件默认窗口程序地址，
	// ItemLong = 所位于窗体的 hWnd，可以此为 key 到 ms_hashWnd 中获得窗体
	//   对应的 CBForm 对象的地址
	// ItemLong2 = 动态分配的一个 STRControlProp 类型的数据的地址，表示控件
	//    的附加属性。只有在设置了控件的某个附加属性时，才会动态开辟一个
	//    STRControlProp 的空间，ItemLong2 才会不为0；
	//    否则 ItemLong2 为 0 表示控件没有被设置过这些属性
	static CBHashLK ms_hashCtrls;			

	// 静态成员（所有本类对象共用）：
	//   模态对话框 的层次
	// 此变量值非0时，表示正有 ms_iModalLevel 个（层）模态对话框在显示
	// 每显示一层模态对话框，此变量值+1；每隐藏一个模态对话框，此变量值-1
	static int ms_iModalLevel;

	// 静态成员（所有本类对象共用）：
	//   当前位于前台的窗体的 hWnd，当窗口焦点切换（WM_ACTIVATE）时设置此值
	static HWND ms_hWndActiForm;

	// 静态成员（所有本类对象共用）：
	//   所有窗口中，目前使用的加速键句柄
	// 一个线程的加速键只能有一套，且发到一个窗口；为使多个不同窗体可以使用不同的
	//   加速键，每个窗体对应的本类对象都可记录一套加速键（将 加速键句柄 记录到 
	//   ms_hashWnd.ItemLong）
	// 加速键要发送到的目标窗口为 ms_hWndActiForm，当窗口焦点切换（WM_ACTIVATE）
	//   时再重新设置 ms_hWndActiForm 和 ms_hAccelCurrUsed
	// 在消息循环函数 MessageLoop 中，只从以下 ms_hWndActiForm 和 
	//   ms_hAccelCurrUsed 中派发加速键消息（两者为 0 时表示无加速键）
	static HACCEL ms_hAccelCurrUsed;  

	// 静态成员（所有本类对象共用）：
	//   全局窗口所用鼠标光标索引值：EStandardCursor 枚举值或资源ID，
	//   对应光标句柄为 ms_hCursorGlobal。
	//   0 表示使用各自窗口的鼠标光标，否则使用全局统一光标
	static long ms_CursorGlobalIdx;		

	// 静态成员（所有本类对象共用）：
	//    全局窗口所用鼠标光标句柄（在 ms_CursorGlobalIdx 非0时才有效）
	static HCURSOR ms_hCursorGlobal;	

public:
	
	// ======================================================================================
	// 公有静态成员和成员函数
	// ======================================================================================

	// 静态成员函数（所有本类对象共用）：
	// 返回和设置本程序全局鼠标光标形状的索引值：EStandardCursor 枚举值或资源ID
	// 在本程序中的所有窗口和子窗口控件上鼠标都将具有此鼠标形状
	// 设置为 0 则表示使用各自窗口或子窗口控件自己的光标形状
	static long MousePointerGlobal();
	static void MousePointerGlobalSet(EStandardCursor cursor);		
	static void MousePointerGlobalSet(long idResCursor, LPCTSTR typeRes=0);
	
	// 静态成员函数（所有本类对象共用）：
	// 获得本类所管理的窗体的个数
	static int FormsCount();
	
	// 静态成员函数（所有本类对象共用）：
	// 获得本类所管理的一个窗体的本类对象的地址
	// 可以用此地址 -> 调用本类的属性方法操作窗体
	// 例如：CBForm::FormsObj(1)->Menu(1,0).TextSet(TEXT("OK"));
	static CBForm * FormsObj(int index);

	
	// 静态成员函数（所有本类对象共用）：
	// 获得一个子窗口控件的附加属性 STRControlProp 数据的地址
	// 如没有设置过附加属性，当 bCreate=false 时返回 0，
	//   当 bCreate=true 时新开辟附加属性的空间，返回 新空间地址
	// 原理是管理 ms_hashCtrls.ItemLong2
	static STRControlProp * PropPtr(HWND hwndCtrl, bool bCreate=false);

		

public:

	// ======================================================================================
	// 友元函数
	// ======================================================================================

	// 公用窗口过程：所有本类对象（窗体）都用此函数作为窗口过程
	friend BOOL WINAPI CBForm_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 公用窗口过程：所有本类对象（窗体）中的子窗口控件，都用此函数作为窗口过程，所有子窗口控件都被子类处理
	friend BOOL WINAPI CBForm_ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 枚举子窗口控件、子类处理（取消子类处理）所有子窗口控件的回调函数：
	// lParam 为控件所属窗体的句柄（非0）时，表示加载窗体时的子类处理；
	// lParam 为 0 时，表示卸载窗体时的恢复子类处理
	friend BOOL CALLBACK EnumChildProcSubClass(HWND hwnd, LPARAM lParam);

	// 消息循环函数
	friend void MessageLoop(int iExitStyle/*=0*/);

	// CBControl 类的一些函数
	friend CBForm * CBControl::ParentFormPtr() const;
	friend HWND CBControl::hWndParentForm() const;
	friend unsigned long CBControl::PtrWndProcDef() const;

public:

	// ======================================================================================
	// 类公有成员
	// ======================================================================================
	bool KeyPreview;	// 是否优先由窗体处理所有控件的键盘按键消息


public:

	// ======================================================================================
	// 类公有成员函数
	// ======================================================================================

	// 构造函数。idResDialog 为对话框的资源id，必须给出参数 idResDialog
	CBForm(unsigned short int idResDialog=0);	
	// 析构函数
	~CBForm();

	// 获得本对象目前所管理的窗体的句柄
	HWND hWnd();

	// 获得和设置窗体上主菜单的句柄，若设为 0 表示取消菜单（无菜单条）
	HMENU hMenu();
	void hMenuSet( HMENU hMenuNew );

	// 获得和设置本窗体所用加速键的句柄，若设为 0 表示取消加速键
	HACCEL hAccel();
	void hAccelSet( HACCEL hAccelNew );

	// 显示对话框：
	//   modal=0 非模态，使用主消息循环，本函数立即返回；
	//   modal=1 模态，新进入另一个 MessageLoop(>1) 的消息循环，本函数直到
	//           模态窗体被隐藏或被卸载后才能返回；
	//   hwndOwner 对话框所属窗口的句柄，NULL表无所属窗口（不是父窗口）
	bool Show(int modal=0, HWND hwndOwner=NULL);	
	
	// 隐藏对话框
	long Hide();

	// 加载对话框但并不显示出来（对话框资源中不能包含 WS_VISIBLE 样式，否则还是会显示 ）
	// 某些成员函数的功能在需要对话框被加载后才能有效的如 SetAccelerator 成员函数，
	//   若调用这些函数时窗体尚未加载，这些函数可自动调用本函数先加载窗体
	// 如加载成功，返回窗口句柄；失败返回 0
	HWND Load(HWND hwndOwner=NULL);

	// 关闭对话框（向对话框发送 WM_CLOSE 消息）
	void UnLoad() const;				

	// 设置本窗口将处理加速键 idResAcce
	// 若取消加速键，将参数设为 0 即可
	// 也可通过 hAccelSet 直接以句柄设置
	void SetAccelerator(unsigned short int idResAcce);

	// 重新设置本窗口的菜单
	// 若取消菜单，将参数设为 0 即可
	// 也可通过 hMenuSet 直接以句柄设置
	void SetMenuMain(unsigned short idResMenu);

	// 返回一个控件（CBControl 对象，实际返回绑定具体控件后的 m_Control），
	//   之后可对该控件进行操作（调用 CBControl 类的属性方法）
	CBControl Control(unsigned short int idResControl);	

	// 返回一个菜单项（CBMenuItem 对象，实际返回绑定具体菜单项后的 m_MenuItem），
	//   之后可对该菜单项进行操作（调用 CBMenuItem 类的属性方法）
	// 按资源ID指定菜单项（此重载版不能指定为 引出子菜单 的菜单项因这种菜单项无资源ID）
	// 按位置指定菜单项重载版：位置均从1开始，0表示不用此级（最多7个级别）
	//   pos1>0 表示窗体主菜单顶级位置；
	//   pos1<0 表示系统菜单，此时 pos2 为系统菜单一级菜单项位置
	// 例如：Menu(0, 0) 获得窗体顶级菜单句柄；Menu(-1, 0) 获得系统菜单句柄
	//   Menu(1, 0)  表示【文件】（第2个0为冗余但必须写）; Menu(-1, 1) 表示【还原】
	// Form1.Menu(0, 0).CountSubItems() 获得顶层菜单 菜单项数（第2个0为冗余但必须写）
	// Form1.Menu(-1, 0).CountSubItems() 获得系统菜单 菜单项数（第2个0为冗余但必须写）
	// 要指定为一个 引出子菜单 的菜单项，必须用“按位置指定的重载版”
	CBMenuItem Menu( UINT idResMenuItem );
	CBMenuItem Menu( int pos1, int pos2, 
		int pos3=0, int pos4=0, 
		int pos5=0, int pos6=0, int pos7=0);
	CBMenuItem Menu(ESysMenu idSysMenu);	
	
	// 恢复系统菜单
	void MenuSysRestore() const;

	// 在 (x,y) 的位置弹出一个快捷菜单
	// idResMenu 为菜单资源 ID，将弹出它所引出的第 0 个子菜单
	// bAllowRightClick 表示是否允许右键单击选择菜单命令（否则只能左键单击选择）
	BOOL PopupMenu(UINT idResMenu, int x, int y, bool bAllowRightClick=true);

	// 设置窗体图标
	void IconSet(EStandardIcon icon);
	void IconSet(unsigned short iconRes);

	// 返回和设置窗体背景色（设为 -1 表示使用默认颜色）
	COLORREF BackColor();
	void BackColorSet(EColorType color);
	void BackColorSet(COLORREF color);

	// 返回和设置本窗体鼠标光标形状的索引值（不修改窗口类，而是通过响应 WM_SETCURSOR 实现）
	//   typeRes 为 0 时，设置 idResCursor 为 Cursor 类型的资源 ID
	//   也可设置其他类型的资源，此时 typeRes 为类型字符串（动画光标时使用）
	// 不影响子窗口控件，如需设置子窗口控件的光标，应调用对应控件的 MousePointerSet 方法
	long MousePointer();
	void MousePointerSet(EStandardCursor cursor);		
	void MousePointerSet(long idResCursor, LPCTSTR typeRes=0);

	

	// 设置一个窗体的或子窗口控件的事件（多个重载版本）
	// ptrFunHandler 为一个事件处理函数的地址
	// idResControl 参数：
	//   如果是窗体事件，idResControl 参数应设为0
	//   如果是控件事件，idResControl 参数应设为 控件的资源ID
	//   如果是菜单（包括系统菜单）、加速键事件，第2个参数为 eMenu_Click，将忽略 idResControl 参数
	// 使用多个重载版本，可借助编译系统检查类型，例如：
	//   属于 ECBFormCtrlEventsVoid 类型的事件，必须用 ONEventVoid 类型的函数
	//   属于 ECBFormCtrlEventsII 类型的事件，必须用 ONEventII 类型的函数
	// 调用 EventAdd 不必提前加载窗体，也不会自动加载窗体
	// 如果事件属于某些通知消息产生的事件，应将相应控件设为包含 Notify 样式；
	//   在调用 EventAdd 时，若是通知消息事件，本函数可自动为控件设置为包含 Notify 样式
	//   但须首先加载窗体，之后的 EventAdd 调用中才可自动设置；如窗体还未加载，无法自动设置
	void EventAdd( unsigned short int idResControl, 
				   ECBFormCtrlEventsVoid eventType, 
				   ONEventVoid ptrFunHandler
		);
	void EventAdd( unsigned short int idResControl, 
				   ECBFormCtrlEventsI eventType, 
				   ONEventI ptrFunHandler
		);
	void EventAdd( unsigned short int idResControl, 
				   ECBFormCtrlEventsII eventType,
				   ONEventII ptrFunHandler
		);	
	void EventAdd( unsigned short int idResControl, 
				   ECBFormCtrlEventsIII eventType,
				   ONEventIII ptrFunHandler
		);	
	void EventAdd( unsigned short int idResControl, 
				   ECBFormCtrlEventsIIII eventType,
				   ONEventIIII ptrFunHandler
		);	


private:

	// ======================================================================================
	// 类私有成员
	// ======================================================================================

	// m_hWnd：对话框（窗体）的句柄，使用基类 BWndBase 的成员，此值将在 CBForm_DlgProc 的 WM_INITDIALOG 消息的处理中被设置

	// TCHAR m_ClassName[128]：窗口控件的类名，使用基类 BWndBase 的成员
	// long m_atom：窗口控件类的 atom 号，唯一标识一种类，使用基类 BWndBase 的成员
	//				这两个值都在 CBForm_DlgProc 的 WM_INITDIALOG 中自动被设置

	unsigned short int mResDlgID;	// 对话框（窗体）的资源id，此值必须在构造函数中给出
	bool m_ModalShown;				// 是否以模态对话框显示的标志，为 true 表示本窗体正以模态对话框显示
	COLORREF m_BackColor;			// 窗体背景色，-1 表示使用默认色（不修改窗口类，而是通过响应 WM_ERASEBKGND 实现）

	long m_CursorIdx;				// 窗体当前的鼠标光标索引号，对应句柄为 m_hCursor。0 表示使用系统默认
	HCURSOR m_hCursor;				// 窗体当前的鼠标光标句柄（在 m_CursorIdx 非0时才有效）

	HFONT m_hFont;					// 窗体当前的字体对象句柄（为0时表示使用系统默认字体，否则表示用自定义字体）
									// 窗体本身的字体对象句柄用此保存；子窗口控件的字体句柄用 STRControlProp.hFont 保存

	CBControl m_Control;			// 用于访问和控制窗体上的一个控件，可以动态绑定到不同控件
	CBMenuItem m_MenuItem;			// 用于访问和控制窗体上的一个菜单项，可以动态绑定到不同菜单项（包括系统菜单项）
	CBMenuItem m_MenuItemSel;		// 用于 Menu_Select 时绑定一个所选菜单项，可以动态绑定到不同菜单项
	
	CBHashLK m_hashMenuIDs;			// 哈希表：key = 菜单项的资源id，data = 该菜单的父菜单的句柄

	// 以通知消息（WM_COMMAND 或 WM_NOTIFY）产生的 子窗口控件事件的 事件处理函数地址列表
	//   key = WM_COMMAND 消息的 wParam（高2位为通知码，低2位为控件ID），
	//    或 = WM_NOTIFY 消息的 NMHDR 中的 .idCtrl 和 .code （转换为2字节）的拼接，
	//         使 key 的高2位为 .code，低2位为 idCtrl 
	//    或 = -1( 即 c_CBMenuClickEventKey )，表示 菜单（包括系统菜单）或加速键被按下
	//          的事件，对应此 key 的 Item 为这种事件的 事件处理函数的地址
	//   Item = 用户事件处理函数的地址，即触发事件要调用的事件处理函数的地址
	//   ItemLong = 0 ～ 4 分别表示 事件处理函数需要0个 ～ 4个 int型的参数
	CBHashLK m_hashEventNotify;	


	// 以窗口消息（非通知消息，但包含向窗体发送的 WM_XXX 和向子窗口控件发送
	//   的 WM_XXX）产生的 窗体事件、或 子窗口控件的事件的 事件处理函数地址列表
	//  key = 低 2 位为发送到窗体的消息（WM_XXX），高 2 位为0
	//   或 = 低 2 位为发送到子窗口控件的消息（WM_XXX），高 2 位为控件 ID
	//  （注与 m_hashEventNotify 的高、低位分配不同，这里用低位表示消息，使窗体事件
	//     直接可用所收到的消息作为 key 到哈希表中查找，而不必将消息 <<16 才能获得 key
	//  Item = 事件处理函数地址
	//  ItemLong = 0 ～ 4 分别表示事件处理函数需要0个 ～ 4个 int型的参数
	CBHashLK m_hashEventMessage;

	// 保存本窗体或本窗体上的控件、菜单中所加载的位图、光标、图标等对象信息，
	//   在本类对象析构时被自动释放
	// Key = 对象句柄；Data = 对象句柄；ItemLong = IMAGE_BITMAP/IMAGE_CURSOR/IMAGE_ICON
	CBHashLK m_hashObjs;
private:

	// ======================================================================================
	// 类私有成员函数
	// ======================================================================================

	// 设置一个窗体的或子窗口控件的事件（EventAdd的多个重载版本的共用操作）
	void EventAddPrivate(unsigned short int idResControl, 
		long eventType, 
		long ptrFunHandler,
		int nParamsIntCount		// 0 ～ 4 分别表示事件处理函数需要0个 ～ 4个 int型的参数
		);
	
	// 生成事件的程序：处理窗体消息，由窗体（对话框）过程 CBForm_DlgProc 调用
	long EventsGenerator(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 生成事件的程序：处理控件消息，由控件被子类处理后的控件窗口过程
	//   CBForm_ControlProc 调用
	long EventsGeneratorCtrl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	// 当模态对话框被隐藏或被关闭时，恢复其他窗口的 Enabled
	void RestoreModalWinEnabled();


	// 获取一个菜单下的所有菜单项的 id，并将 id 和其直属父菜单句柄的
	//   对应关系保存到哈希表 m_hashMenuIDs
	// 本函数还递归调用使其子菜单下的菜单项也被处理
	// bIsTop==true 时，将先清除哈希表 m_hashMenuIDs，应仅在第一次调用 
	//   时才将此参数设为 true，递归调用时应将此参数设为 false
	void ConstructMenuIDItems(HMENU hMenuParent, bool bIsTop=false);	

	// 窗口被销毁后需调用此函数释放资源
	void ClearResource();


	virtual HFONT SetFontMLF(LOGFONT * lpLf);	// 根据 lpLf 所指向的结构创建一种字体，并设置窗口使用该种字体；
												//   函数返回新字体对象的句柄
												// 这里为重载 CBWndBase 基类的虚函数
												// CBForm 类重载后，应在重载函数中将新字体对象地址存入 m_hFont 私有成员
};

