//////////////////////////////////////////////////////////////////////////
// Bwindows.h 包含了编写 Windows 程序所必须的头文件，
//            一些常用自定义函数的声明、类型、常数、类的定义
//   类包括：
//      CBApp 类： 管理应用程序全局信息
// 		CBHashLK：长整数型 key 的哈希表类
// 		CBHashStrK：字符串型 key 的哈希表类
// 		CBRecycledArr：带回收站的数组类
// 		CBRecycledArrInt：整型版带回收站的数组类
// 		CHeapMemory： 用全局对象维护所有通过 new 分配的内存指针
//
// 包含本 h 文件即可使用全局对象指针：pApp（->）获得程序全局信息，但只有
//   同时包含 BForm 的模块（封装 WinMain 函数）后才能获得正确的信息
//
// 包含本 h 文件即可使用全局对象：HM （管理以 new 开辟的内存指针）
//    调用 HM.AddPtr(p, bArrNew); 添加管理一个以 new 开辟的内存指针
//    调用 HM.Dispose(); 释放目前所管理的所有动态内存（程序结束时也会自动释放）
//////////////////////////////////////////////////////////////////////////

#pragma once

#define OEMRESOURCE		// to use any of the OBM_ constants
#include <windows.h>
#include <stdlib.h>		// 需用 atoi、atof 等函数
#include <memory.h>		
#include <string.h>
#include <tchar.h>		
#include <math.h>


// EventsGenerator 函数返回此值表示要调用默认的窗口程序，否则不再调用默认的窗口程序
#define gc_APICEventsGenDefautRet 0xFEEEEEEE  

enum EStandardIcon		// 系统图标
{
	IDI_Application = (int)IDI_APPLICATION, 
	IDI_Asterisk = (int)IDI_ASTERISK, 
	IDI_Error = (int)IDI_ERROR, 
	IDI_Exclamation = (int)IDI_EXCLAMATION, 
	IDI_Hand = (int)IDI_HAND, 
	IDI_Information = (int)IDI_INFORMATION, 
	IDI_Question = (int)IDI_QUESTION, 
	IDI_Warning = (int)IDI_WARNING, 
	IDI_Winlogo = (int)IDI_WINLOGO
};

#define gc_IDStandCursorIDBase 0x10000  
enum EStandardCursor	// 系统鼠标指针形状，为对应 API 常量 + gc_IDStandCursorIDBase（目的是为与资源 ID 区分，资源 ID 均小于 gc_IDStandCursorIDBase）
{
	IDC_AppStarting	=	(int)IDC_APPSTARTING + gc_IDStandCursorIDBase, 		// Standard arrow and small hourglass 
	IDC_Arrow	=	(int)IDC_ARROW + gc_IDStandCursorIDBase, 				// Standard arrow 
	IDC_Cross	=	(int)IDC_CROSS + gc_IDStandCursorIDBase, 				// Cross hair 
	IDC_Hand	=	(int)MAKEINTRESOURCE(32649) + gc_IDStandCursorIDBase,	// Windows NT 5.0 and later: Hand 
	IDC_Help	=	(int)IDC_HELP + gc_IDStandCursorIDBase, 				// Arrow and question mark 
	IDC_IBeam	=	(int)IDC_IBEAM + gc_IDStandCursorIDBase, 				// I-beam 
	IDC_Icon	=	(int)IDC_ICON + gc_IDStandCursorIDBase, 				// Obsolete for applications marked version 4.0 or later. 
	IDC_No	=	(int)IDC_NO + gc_IDStandCursorIDBase, 						// Slashed circle 
	IDC_Size	=	(int)IDC_SIZE + gc_IDStandCursorIDBase, 				// Obsolete for applications marked version 4.0 or later. Use IDC_SIZEALL. 
	IDC_SizeAll	=	(int)IDC_SIZEALL + gc_IDStandCursorIDBase, 				// Four-pointed arrow pointing north, south, east, and west 
	IDC_SizeNESW	=	(int)IDC_SIZENESW + gc_IDStandCursorIDBase, 		// Double-pointed arrow pointing northeast and southwest 
	IDC_SizeNS	=	(int)IDC_SIZENS + gc_IDStandCursorIDBase, 				// Double-pointed arrow pointing north and south 
	IDC_SizeNWSE	=	(int)IDC_SIZENWSE + gc_IDStandCursorIDBase, 		// Double-pointed arrow pointing northwest and southeast 
	IDC_SizeWE	=	(int)IDC_SIZEWE + gc_IDStandCursorIDBase, 				// Double-pointed arrow pointing west and east 
	IDC_UpArrow	=	(int)IDC_UPARROW + gc_IDStandCursorIDBase, 				// Vertical arrow 
	IDC_Wait	=	(int)IDC_WAIT + gc_IDStandCursorIDBase 					// Hourglass 
};

enum EOEMBmp		// 系统 OEM 位图
{
	Obm_Btncorners = OBM_BTNCORNERS, 
	Obm_Btsize = OBM_BTSIZE, 
	Obm_Check = OBM_CHECK, 
	Obm_Checkboxes = OBM_CHECKBOXES, 
	Obm_Close = OBM_CLOSE, 
	Obm_Reduce = OBM_REDUCE, 
	Obm_Combo = OBM_COMBO, 
	Obm_Reduced = OBM_REDUCED, 
	Obm_Dnarrow = OBM_DNARROW, 
	Obm_Restore = OBM_RESTORE, 
	Obm_Dnarrowd = OBM_DNARROWD, 
	Obm_Restored = OBM_RESTORED, 
	Obm_Dnarrowi = OBM_DNARROWI, 
	Obm_Rgarrow = OBM_RGARROW, 
	Obm_Lfarrow = OBM_LFARROW, 
	Obm_Rgarrowd = OBM_RGARROWD, 
	Obm_Lfarrowd = OBM_LFARROWD, 
	Obm_Rgarrowi = OBM_RGARROWI, 
	Obm_Lfarrowi = OBM_LFARROWI, 
	Obm_Size = OBM_SIZE, 
	Obm_Mnarrow = OBM_MNARROW, 
	Obm_Uparrow = OBM_UPARROW, 
	Obm_Uparrowd = OBM_UPARROWD, 
// 	Obm_Old_Restore = OBM_OLD_RESTORE,		// Bitmap names that begin with OBM_OLD represent bitmaps used by 16-bit versions of Windows earlier than 3.0. 
// 	Obm_Old_Rgarrow = OBM_OLD_RGARROW, 
// 	Obm_Old_Uparrow = OBM_OLD_UPARROW, 
// 	Obm_Old_Zoom = OBM_OLD_ZOOM, 
// 	Obm_Old_Close = OBM_OLD_CLOSE, 
// 	Obm_Old_Dnarrow = OBM_OLD_DNARROW, 
// 	Obm_Old_Lfarrow = OBM_OLD_LFARROW, 
// 	Obm_Old_Reduce = OBM_OLD_REDUCE, 
	Obm_Uparrowi = OBM_UPARROWI, 
	Obm_Zoom = OBM_ZOOM, 
	Obm_Zoomd = OBM_ZOOMD
};

enum EOEMIcon		// 系统 OEM 图标
{
	Oic_Sample  =	OIC_SAMPLE,	
	Oic_Hand  =	OIC_HAND,	
	Oic_Ques  =	OIC_QUES,	
	Oic_Bang  =	OIC_BANG,	
	Oic_Note  =	OIC_NOTE,

#if(WINVER >= 0x0400)	
	Oic_Winlogo  =	OIC_WINLOGO,
	Oic_Warning  =	OIC_WARNING,	
	Oic_Error  =	OIC_ERROR,	
	Oic_Information  =	OIC_INFORMATION,
#endif 	/* WINVER >= 0x0400 */
};

enum EOEMCursor		// 系统 OEM 光标
{
	Ocr_Normal  =	OCR_NORMAL,	
	Ocr_Ibeam  =	OCR_IBEAM,
	Ocr_Wait  =	OCR_WAIT,	
	Ocr_Cross  =	OCR_CROSS,	
	Ocr_Up  =	OCR_UP,	
	Ocr_Size  =	OCR_SIZE,	/* OBSOLETE: use OCR_SIZEALL */
	Ocr_Icon  =	OCR_ICON,	/* OBSOLETE: use OCR_NORMAL */
	Ocr_Sizenwse  =	OCR_SIZENWSE,	
	Ocr_Sizenesw  =	OCR_SIZENESW,	
	Ocr_Sizewe  =	OCR_SIZEWE,	
	Ocr_Sizens  =	OCR_SIZENS,	
	Ocr_Sizeall  =	OCR_SIZEALL,	
	Ocr_Icocur  =	OCR_ICOCUR,	/* OBSOLETE: use OIC_WINLOGO */
	Ocr_No  =	OCR_NO,	

#if(WINVER >= 0x0500)	
	Ocr_Hand  =	OCR_HAND,	
#endif 	/* WINVER >= 0x0500 */

#if(WINVER >= 0x0400)	
	Ocr_Appstarting  =	OCR_APPSTARTING,	
#endif 	/* WINVER >= 0x0400 */
};

enum EColorType			// 系统颜色
{
	COLOR_ActiveBorder	=	COLOR_ACTIVEBORDER,
	COLOR_ActiveCaption	=	COLOR_ACTIVECAPTION,
	COLOR_AppWorkspace	=	COLOR_APPWORKSPACE,
	COLOR_BackGround	=	COLOR_BACKGROUND,
	COLOR_BtnFace	=	COLOR_BTNFACE,
	COLOR_BtnShadow	=	COLOR_BTNSHADOW,
	COLOR_BtnText	=	COLOR_BTNTEXT,
	COLOR_CaptionText	=	COLOR_CAPTIONTEXT,
	COLOR_GrayText	=	COLOR_GRAYTEXT,
	COLOR_Highlight	=	COLOR_HIGHLIGHT,
	COLOR_HighlightText	=	COLOR_HIGHLIGHTTEXT,
	COLOR_InactiveBorder	=	COLOR_INACTIVEBORDER,
	COLOR_InactiveCaption	=	COLOR_INACTIVECAPTION,
	COLOR_Menu	=	COLOR_MENU,
	COLOR_MenuText	=	COLOR_MENUTEXT,
	COLOR_ScrollBar	=	COLOR_SCROLLBAR,
	COLOR_Window	=	COLOR_WINDOW,
	COLOR_WindowFrame	=	COLOR_WINDOWFRAME,
	COLOR_WindowText 	=	COLOR_WINDOWTEXT 
};

enum EShowWindowCmd		// 窗口显示状态
{
	SW_ForceMinimize = SW_FORCEMINIMIZE,	//  Windows NT 5.0 and later: Minimizes a window, even if the thread that owns the window is hung. This flag should only be used when minimizing windows from a different thread. 
	SW_Hide = SW_HIDE,	//  Hides the window and activates another window. 
	SW_Mazimize = SW_MAXIMIZE,	//  Maximizes the specified window. 
	SW_Minimize = SW_MINIMIZE,	//  Minimizes the specified window and activates the next top-level window in the Z order. 
	SW_Restore = SW_RESTORE,	//  Activates and displays the window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window. 
	SW_Show = SW_SHOW,	//  Activates the window and displays it in its current size and position.  
	SW_ShowDefault = SW_SHOWDEFAULT,	//  Sets the show state based on the SW_ flag specified in theSTARTUPINFO structure passed to theCreateProcess function by the program that started the application.  
	SW_ShowMaximized = SW_SHOWMAXIMIZED,	//  Activates the window and displays it as a maximized window. 
	SW_ShowMinimized = SW_SHOWMINIMIZED,	//  Activates the window and displays it as a minimized window. 
	SW_ShowMinNoactive = SW_SHOWMINNOACTIVE,	//  Displays the window as a minimized window. The active window remains active. 
	SW_ShowNA = SW_SHOWNA,	//  Displays the window in its current state. The active window remains active. 
	SW_ShowNoActivate = SW_SHOWNOACTIVATE,	//  Displays a window in its most recent size and position. The active window remains active. 
	SW_ShowNormal = SW_SHOWNORMAL	//  Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time. 
};


enum EMsgBoxBtn			// 用于消息框的按钮类型
{
	mb_OK = MB_OK,
	mb_OkCancel = MB_OKCANCEL,
	mb_AbortRetryIgnore = MB_ABORTRETRYIGNORE,
	mb_YesNoCancel = MB_YESNOCANCEL,
	mb_YesNo = MB_YESNO,
	mb_RetryCancel = MB_RETRYCANCEL
};

enum EMsgBoxIcon		// 用于消息框的图标
{
	mb_IconNone = 0,
	mb_IconError = MB_ICONHAND,
	mb_IconQuestion = MB_ICONQUESTION, 
	mb_IconExclamation = MB_ICONEXCLAMATION, 
	mb_IconInformation = MB_ICONASTERISK 
	#if(WINVER >= 0x0400)
	,mb_UserIcon = MB_USERICON
	#endif /* WINVER >= 0x0400 */
};

enum EMsgBeep
{
	mb_SoundOK = MB_OK,	// SystemDefault
	mb_SoundError = MB_ICONHAND,
	mb_SoundQuestion = MB_ICONQUESTION, 
	mb_SoundExclamation = MB_ICONEXCLAMATION, 
	mb_SoundAsterisk = MB_ICONASTERISK, 
	mb_SoundSpeaker = 0xFFFFFFFF		// Standard beep using the computer speaker
};


enum EDlgBoxDefBtn		// 用于消息框，指定默认按钮
{
		mb_DefButton1 = MB_DEFBUTTON1,
		mb_DefButton2 = MB_DEFBUTTON2,
		mb_DefButton3 = MB_DEFBUTTON3
#if(WINVER >= 0x0400)
		,mb_DefButton4 = MB_DEFBUTTON4
#endif /* WINVER >= 0x0400 */
};

enum EDlgBoxCmdID		//用于消息框，指定用户按下的按钮
{
	NoValue = 0,
	idAbort = IDABORT, 		// Abort button was selected. 
	idCancel = IDCANCEL, 	// Cancel button was selected. 
	idIgnore = IDIGNORE, 	// Ignore button was selected. 
	idNo = IDNO, 			// No button was selected. 
	idOk = IDOK, 			// OK button was selected. 
	idRetry = IDRETRY, 		// Retry button was selected. 
	idYes = IDYES 			// Yes button was selected. 
};


enum ESysMenu		// 系统菜单枚举类型
{
	eSysMenu_Close = SC_CLOSE,
	eSysMenu_Minimize = SC_MINIMIZE,
	eSysMenu_Maxmize = SC_MAXIMIZE,
	eSysMenu_Move = SC_MOVE,
	eSysMenu_Size = SC_SIZE,
	eSysMenu_Restore = SC_RESTORE
};




//////////////////////////////////////////////////////////////////////////
// 常用自定义函数
//
//////////////////////////////////////////////////////////////////////////

// 弹出消息框
//   如 title 为 NULL，就自动使用应用程序名作为 title
EDlgBoxCmdID MsgBox(LPCTSTR			szPrompt, 
				 	LPCTSTR			szTitle = NULL,				
					EMsgBoxBtn		buttons = mb_OK, 
					EMsgBoxIcon		icon = mb_IconNone, 
					EDlgBoxDefBtn	defBtn = mb_DefButton1, 
				    bool			bSystemModal = false, 
				    bool			bHelpButton = false, 
				    bool			bRightJustified = false, 
				    bool			bRightToLeftReading = false);

BOOL MsgBeep(EMsgBeep soundStyle = mb_SoundSpeaker);

// 将一个整数转换为八进制的字符串，字符串空间自动管理
LPTSTR Oct(long number);

// 将一个整数转换为十六进制的字符串，字符串空间自动管理
LPTSTR Hex(long number);


// 获得当前路径字符串，字符串空间自动管理
LPTSTR CurDir();

// 获取一个自定义资源的字节数据（空间由本模块自动开辟、自动管理）
//   rSize 不为0时，将从此参数指向的空间返回资源的字节数
unsigned char * LoadResData(UINT idRes, UINT typeRes, unsigned long * rSize=0);
unsigned char * LoadResData(UINT idRes, LPCTSTR typeRes, unsigned long * rSize=0 );
unsigned char * LoadResData(LPCTSTR idRes, UINT typeRes, unsigned long * rSize=0 );
unsigned char * LoadResData(LPCTSTR idRes, LPCTSTR typeRes, unsigned long * rSize=0);

//////////////////////////////////////////////////////////////////////////
// 时间 函数
//////////////////////////////////////////////////////////////////////////

// 返回当前系统日期、时间的一个字符串
//   若 lpDblTime 不为0，还将当前系统日期、时间转换为 double 
//     （为1601-1-1以来经历的毫秒数）存入它指向的 double 型变量中
//   若 lpTime 不为0，还将当前系统日期、时间存储到它指向的结构中
LPTSTR Now( double *lpDblTime=0, SYSTEMTIME *lpTime=0);

// 设置当前系统日期、时间
BOOL NowSet( SYSTEMTIME stNow );

// 将一个日期、时间转换为 double 返回（为1601-1-1以来经历的毫秒数）
double DateTimeDbl( SYSTEMTIME stDatetime );

// 计算两个日期、时间的时间间隔时所用的间隔类型
enum eDataTimeDiffStyle
{
	edtYearDiff = 1,
	edtMonthDiff,
	edtDayDiff,
	edtHourDiff,
	edtMinuteDiff,
	edtSecondDiff,
	edtMilliseconds
};

// 计算两个日期、时间的时间间隔
// style 为指定时间间隔的单位
double DateDiff(eDataTimeDiffStyle style,
				SYSTEMTIME stDatetime1,
				SYSTEMTIME stDatetime2);


//////////////////////////////////////////////////////////////////////////
// 自定义字符串 函数
//////////////////////////////////////////////////////////////////////////

// 以 printf 方式制作一个字符串（字符串空间由本模块自动开辟、自动管理）
LPTSTR cdecl StrPrintf( LPCTSTR szFormat, ... );

// 取字符串的前 length 个字符组成新字符串，返回新字符串的首地址
//   新字符串空间由本模块自动开辟、自动管理
LPTSTR Left(LPCTSTR szStringSrc, int length);		

// 取字符串的后 length 个字符组成新字符串，返回新字符串的首地址
//   新字符串空间由本模块自动开辟、自动管理
LPTSTR Right(LPCTSTR szStringSrc, int length);				

// 取字符串的从第 startPos 个字符起，长 length 个字符组成新字符串
//  （第一个字符位置为1），返回新字符串的首地址
//   新字符串空间由本模块自动开辟、自动管理
LPTSTR Mid(LPCTSTR szStringSrc, int startPos, int length);	

// 删除字符串的前导、尾部和所有空格，返回删除后的新字符串，
//   新字符串空间由本模块自动开辟、自动管理
// bDelOtherSpace=true时删除所有空格在内的 isspace 返回真的
//   所有字符；bDelOtherSpace=false 时只删除空格
LPTSTR LTrim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);
LPTSTR RTrim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);
LPTSTR Trim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);

// 将字符串中的字母全部转换为大写（UCase）、小写（LCase）
//   返回转换后的新字符串（新字符串空间自动管理）
LPTSTR LCase(LPCTSTR szStringSrc);
LPTSTR UCase(LPCTSTR szStringSrc);


enum eBStrCompareMethod		// 字符串比较方式
{
	bcmBinaryCompare = 0,	// 二进制比较（区分大小写）
	bcmTextCompare = 1		// 文本比较（不区分大小写）
};


// 在 szSrc 中，从第 start 个字符开始（第一个字符位置为1），
//	  查找字符串 szFind 的第一次出现的位置（第一个字符位置为1），
//    找到返回值>0，没找到返回0
//  说明：本函数未调用任何库函数（strlen也未调用），提高了运行效率
int InStr(int			start,										
		  LPCTSTR		szSrc, 
		  LPCTSTR		szFind,
	eBStrCompareMethod	compare=bcmBinaryCompare);


// 在 szSrc 中，从第一个字符开始，查找字符串 szFind 的
//   第一次出现的位置（第一个字符位置为1），
//   返回位置号（第一个字符位置为1）。找到返回值>0，没找到返回0
int InStr(LPCTSTR		szSrc,								
		  LPCTSTR		szFind,
    eBStrCompareMethod	compare=bcmBinaryCompare);

// 在 szSrc 中，从第 start 个字符开始（第一个字符位置为1）到末尾的部分，
//	  查找字符串 szFind 的倒数第一次出现的位置（第一个字符位置为1），
//    找到返回值>0，没找到返回0
//  说明：本函数未调用任何库函数（strlen也未调用），提高了运行效率
int InStrRev(LPCTSTR		szSrc, 
			 LPCTSTR		szFind, 
			 int			start=1,
			 eBStrCompareMethod	compare=bcmTextCompare);

// 按分隔字符串 delimiters ，分割一个字符串，生成若干子字符串
//	 各 子字符串 的地址由 ptrStrings[] 数组返回，函数返回子字符串的个数
//   ptrStrings[] 数组下标从1开始，最大到函数返回值
// limit 限制返回子字符串的最大个数，为 -1 时不限制，将返回所有字符串
//   子字符串内存及 ptrStrings[] 数组内存都由本模块自动分配、自动管理
int Split(LPCTSTR		stringSrc,				
		  TCHAR		** &ptrStrings,				
		  LPCTSTR		delimiters=0,			
		  int		    limit=-1,				
	eBStrCompareMethod  compare=bcmBinaryCompare);	



// 以 delimiter 连接多个字符串，返回连接好的字符串
// 多个字符串的地址由数组 stringSrcArray[] 给出，将连接
//   数组中从下标 arrayIndexStart 到 arrayIndexEnd 的字符串
// delimiter 为 0 时，默认以 "\0" 连接字符串；否则以字符串 delimiter 连接
// bTailDoubleNull 若为 true，则在结果字符串的最后再加一个'\0'（即最后有两个'\0'）
// 结果字符串的内存由本模块自动分配、自动管理
LPTSTR Join(TCHAR * stringSrcArray[], 
		    const int    arrayIndexEnd, 
		    LPCTSTR delimiter=0, 
		    const int    arrayIndexStart=1, 
		    const bool   bTailDoubleNull=false);

// 替换字符串
LPTSTR Replace(LPCTSTR szStringSrc,		// 要被替换的字符串
			   LPCTSTR szFind,			// 要被替换掉的子字符串
			   LPCTSTR szReplaceWith,	// 要替换为的内容
			   int start=1,			// 在 szStringSrc 中子字符串搜索的开始位置
			   int countLimit=-1,	// 子字符串进行替换的次数。–1 表明进行所有可能的替换
			   eBStrCompareMethod  compare=bcmBinaryCompare	// 判别子字符串时所用的比较方式
			   );

// 连接字符串，生成连接后的长字符串
// 返回连接好的字符串的首地址，自动维护动态空间
// 每次调用可最多连接9个字符串
LPTSTR StrAppend(LPCTSTR str1=0, 
				 LPCTSTR str2=0, 
				 LPCTSTR str3=0, 
				 LPCTSTR str4=0, 
				 LPCTSTR str5=0, 
				 LPCTSTR str6=0, 
				 LPCTSTR str7=0,
				 LPCTSTR str8=0,
				 LPCTSTR str9=0 );


// 将 ANSI 或 UTF8 字符串转换为 Unicode，返回结果字符串首地址
//   参数 bToUTF8orANSI 为 false 时转换 ANSI，为 true 时转换 UTF8
//   结果字符串的内存由本模块自动分配、自动管理
LPWSTR StrConvUnicode(const char * szAnsi, bool bFromUTF8orANSI=false );   // LPWSTR 就是 unsigned short int *

// 将 Unicode 字符串转换为 ANSI 或 UTF8，返回结果字符串首地址
//   参数 bToUTF8orANSI 为 false 时转换为 ANSI，为 true 时转换为 UTF8
//   结果字符串的内存由本模块自动分配、自动管理
char * StrConvFromUnicode(LPCWSTR szUnicode, bool bToUTF8orANSI=false );

// 将字符串转换为 double 型数值
double Val( LPCWSTR stringVal );	// Unicode 字符串
double Val( LPCSTR stringVal );		// ANSI 字符串

// 将各种类型数据转换为字符串
// 返回字符串首地址，字符串空间由本函数自动开辟和管理
LPTSTR Str(char number);
LPTSTR Str(unsigned short int number);	// TCHAR
LPTSTR Str(int number);
LPTSTR Str(unsigned int number);
LPTSTR Str(unsigned long number);
LPTSTR Str(float number);
LPTSTR Str(double number);
LPTSTR Str(long double number);



//////////////////////////////////////////////////////////////////////////
// 自定义 动态数组 函数
//////////////////////////////////////////////////////////////////////////


// 重定义 一个 REDIMTYPE 类型(如int型、int *型即指针数组)的数组的大小，新定义空间自动清零
// arr：为数组指针（可为 int *型变量，或void **型变量，后者为数组指针），本函数将修改此指针的指向
// toUBound：为要重定义后数组的上界，定义为：[0] to [toUBound]，为 -1 时不开辟空间，可用于删除原
//	 空间，并 arr 会被设为0
// uboundCurrent：为重定义前数组的上界 [0] to [uboundCurrent]，为 -1 表示尚未开辟过空间为第一次调用
// preserve：保留数组原始数据否则不保留
// 返回新空间上标，即 toUBound
template <typename REDIMTYPE>
int Redim(  REDIMTYPE *	 &arr, 
			int			  toUBound=-1, 
			int			  uboundCurrent=-1,  
			bool		  preserve=false )	// template 函数定义要在头文件中
{
	// 开辟新空间：[0] to [toUBound]
	if (toUBound >= 0)
	{
		REDIMTYPE * ptrNew = new REDIMTYPE[toUBound + 1];		// +1 为使可用下标最大到 toUBound
		// 新空间清零
		memset(ptrNew, 0, sizeof(REDIMTYPE)*(toUBound + 1));
		
		// 将原有空间内容拷贝到新空间
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// 保留原有数据，需要拷贝内存的 REDIMTYPE 元素个数
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// 取 uboundCurrent 和 toUBound 的最小值
			ctToCpy = ctToCpy + 1;								// 必须 +1，因为 uboundCurrent 和 toUBound 都是数组上界
			memcpy(ptrNew, arr, sizeof(REDIMTYPE)*ctToCpy); 
		}
		
		// 删除原有空间
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// 指针指向新空间
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)
	{
		// 不开辟空间，删除原有空间
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
}


// 删除动态数组的空间，并设置指针 arr 为 0
template <typename REDIMTYPE>
void Erase( REDIMTYPE *  &arr)
{
	if(arr!=0) delete [] arr;
	arr=0;
}



//////////////////////////////////////////////////////////////////////
// CBApp 类： 管理应用程序全局信息
//
//////////////////////////////////////////////////////////////////////

class CBApp		// 应用程序全局信息对象
{
public:
	// 构造函数
	CBApp(HINSTANCE hInst, 
		HINSTANCE hPrevInst, 
		char * lpCmdLine, 
		int nShowCmd);
	
	// 常成员（只能获得值，不能修改；是在构造函数的初始化表中修改的）
	const HINSTANCE hInstance;	// WinMain 函数传递过来的 hInstance
	const int CmdShow;			// WinMain 函数传递过来的 nShowCmd
	
	// 获得应用程序当前运行的路径（最后含 \）
	LPTSTR Path();	

	// 获得命令行参数字符串（包含exe文件名路径，字符串空间由 Windows 维护）
	LPTSTR Command() const;

	// 获得屏幕宽度、高度
	int ScreenWidth() const;
	int ScreenHeight() const;
private:
	TCHAR m_path[2048];		// 应用程序当前运行的路径字符串缓冲区

};

// 全局对象变量 pApp（是指针）的声明，所指向的对象用于获得本程序的全局程序信息，
//   如 hInstance 等。真正的变量定义在 BWindows.cpp 中，这里用 
//   extern 声明此变量，则所有包含本 h 文件的模块都可使用此全局变量
extern CBApp *pApp;	



// ===============================================================
// CBHashLK：长整数型 key 的哈希表类
// 支持每个元素中有一个 long 型的数据和一个 long 型的附加数据
//
// 可能抛出的异常：
// throw (unsigned char)7;	// 超出内存
// throw (unsigned char)5;	// 无效的过程调用或参数：键值错误，如试图添加已存在的同样键值的新元素，访问键不存在的元素
// throw (unsigned char)9;	// 下标越界：无法分配新数据空间
//
// ---- 可以使用 Index 的方式遍历所有哈希表元素 ----
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.KeyFromIdx(i)
//     
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.ItemFromIdx(i)
//     
// 注意 Index 并不与数据对应，随着增删和重排，数据的 Index 都可能会变化
// 但在同一时刻，Index 相同的一套数据（Key,Data,DataLong,DataString）是同一套
// ===============================================================

class CBHashLK
{
private:
	typedef long KeyType;		// key 的类型
	typedef long DataType;		// 数据的类型
	typedef long DataLongType;	// 附加数据的类型
	typedef long DataLong2Type;	// 附加数据的类型
	
	typedef struct _MemType
	{
		KeyType Key;
		DataType Data;
		DataLongType DataLong;
		DataLong2Type DataLong2;
		double DataDouble;
		LPTSTR DataStr;
		LPTSTR DataStr2;
		bool Used;
		int Index;		// mArrTable[] 数组的 mArrTable[index] 元素，是保存本 MemType 数据
						//   所在的 mem[] 中的下标（index>0）或在 mem2[] 中的下标（index<0）
						//   mArrTableCount == memUsedCount + memUsedCount2 时且 index !=0 时 有效
						//   在 RefreshArrTable 中设置此成员
	} MemType;
	
	static const int	mcIniMemSize;  			// 初始 mem[] 的大小
	static const int	mcMaxItemCount; 		// 最多元素个数（可扩大此值到 long 表示的范围之内）
	static const float	mcExpandMaxPort;  		// 已有元素个数大于 0.75*memCount 时就扩大 mem[] 的空间
	static const int	mcExpandCountThres; 	// 扩大 mem[] 空间时，若 memCount 小于此值则每次扩大到 memCount*2；若 memCount 大于此值则每次扩大到 Count+Count/2
	static const int	mcExpandCountThresMax;	// 扩大 mem[] 空间时，若 memCount 已大于此值，则每次不再扩大到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// 扩大 mem[] 空间时，若 memCount 已大于 mcExpandCountThresMax，则每次不再扩大到到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandMem2Per; 		// 每次扩大 mem2[] 的大小
	static const int	mcSeqMax; 				// 顺序检索最大值

private:
	MemType * mem;					// 动态数组指针，但数组不使用 [0] 的元素
	int memCount, memUsedCount;		// 动态数组最大下标，mem[] 数组下标为 [0] ～ [memCount]。哈希表已用元素个数
	MemType * mem2;					// 空间冲突的元素的保存空间，顺序使用
	int memCount2, memUsedCount2;	// mem2[] 数组下标为 [0] ～ [memCount2]，其中 [0] 不用，已使用空间为 [1] ～ [memUsedCount2]
	int mTravIdxCurr;				// 用 NextXXX 方法遍历的当前 index，正值表示 mem[] 中的下标，负值表示 mem2[] 中的下标

	
	// 支持通过给定下标 Index 访问一个哈希表数据，mArrTable 指向动态数组，
	//   数组元素保存：所有哈希表数据依遍历顺序所在的 mem[] 中的下标（>0）
	//   或 mem2[] 中的下标。
	// 遍历一次，将所有哈希表数据 mem[] 或 mem2[] 的下标存于此数组，
	//   以后不需重复遍历，直接通过给定下标 Index 访问一个哈希表数据。
	//   mArrTableCount != memUsedCount + memUsedCount2 为标志，
	//   如 !=，标志要重新刷新遍历。注意重新遍历后，元素顺序可能会重排。
	//   各哈希表数据的 Index 并不是一直不变的
	int * mArrTable;
	int mArrTableCount;
	
public:
	CBHashLK(int memSize=0);		// memSize=0 则开辟初始 mcIniMemSize 个空间，否则开辟 memSize 个空间，memSize 应比实际数据个数大一些
	~CBHashLK();
	void AlloMem(int memSize);		// 事先可用此函数定义足够大的空间，以减少以后自动扩大空间的次数，提高效率
	bool Add(DataType data, KeyType key=0, DataLongType dataLong=0, DataLong2Type dataLong2=0, LPCTSTR dataStr=0, LPCTSTR dataStr2=0, double dataDouble=0.0, bool raiseErrorIfNotHas=true);	// 添加元素
	bool Remove(KeyType key, bool raiseErrorIfNotHas=true);					// 删除元素

	// 根据 key 获得元素、附加数据
	DataType Item(KeyType key, bool raiseErrorIfNotHas=true);				
	DataLongType ItemLong(KeyType key, bool raiseErrorIfNotHas=true);
	DataLong2Type ItemLong2(KeyType key, bool raiseErrorIfNotHas=true);
	double ItemDouble(KeyType key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr(KeyType key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2(KeyType key, bool raiseErrorIfNotHas=true);

	// 根据 key 设置元素、附加数据
	bool ItemSet(KeyType key, DataType vNewValue, bool raiseErrorIfNotHas=true);				
	bool ItemLongSet(KeyType key, DataLongType vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2Set(KeyType key, DataLong2Type vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleSet(KeyType key, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrSet(KeyType key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2Set(KeyType key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);


	// 根据 index 获得元素、附加数据
	DataType ItemFromIndex(int index, bool raiseErrorIfNotHas=true);			
	DataLongType ItemLongFromIndex(int index, bool raiseErrorIfNotHas=true);
	DataLong2Type ItemLong2FromIndex(int index, bool raiseErrorIfNotHas=true);
	double ItemDoubleFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStrFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2FromIndex(int index, bool raiseErrorIfNotHas=true);

	// 根据 index 设置元素、附加数据（但不能设置 Key，Key为只读）
	bool ItemFromIndexSet(int index, DataType vNewValue, bool raiseErrorIfNotHas=true);			
	bool ItemLongFromIndexSet(int index, DataLongType vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2FromIndexSet(int index, DataLong2Type vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleFromIndexSet(int index, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrFromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2FromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);

	KeyType IndexToKey(int index, bool raiseErrorIfNotHas=true);
	int KeyToIndex(KeyType key, bool raiseErrorIfNotHas=true);

	bool IsKeyExist(KeyType key);											// 判断某个 key 的元素是否存在
	void Clear(void);								// 清除所有元素，重定义 mcIniMemSize 个存储空间
	
	void StartTraversal();							// 开始一个遍历过程
	DataType NextItem(bool &bRetNotValid);			// 遍历过程开始后，不断调用此函数，获得每个元素，直到 bRetNotValid 返回 true
	DataLongType NextItemLong(bool &bRetNotValid);	// 遍历过程开始后，不断调用此函数，获得每个元素的附加数据，直到 bRetNotValid 返回 true
	DataLong2Type NextItemLong2(bool &bRetNotValid);
	double NextItemDouble(bool &bRetNotValid);
	LPTSTR NextItemStr(bool &bRetNotValid);
	LPTSTR NextItemStr2(bool &bRetNotValid);

	KeyType NextKey(bool &bRetNotValid);			// 遍历过程开始后，不断调用此函数，获得每个元素的 key，直到 bRetNotValid 返回 true

	int Count(void);								// 返回共有元素个数


private:
	int AlloMemIndex(KeyType key, bool CanExpandMem=true );			// 根据 Key 分配一个 mem[] 中的未用存储空间，返回 mem[] 数组下标
	int FindMemIndex(KeyType key) const;							// 根据 Key 查找 mem[] 中元素，返回 mem[] 数组下标
	int FindSeqIdx(KeyType key, int fromIndex, int toIndex);		// 找 mem[] 中键为 key 的元素下标，仅查找空间下标为从 fromIndex 开始、到 toIndex 结束的空间
	void ReLocaMem(int preMemCountTo);								// 重新分配 mem[], mem2[] 的各元素的地址，mem2[] 的某些元素可能被重新移动到 mem[]
	void ExpandMem(void);											// 重定义 mem[] 数组大小，扩大 mem[] 的空间
	int TraversalGetNextIdx(void);									// 用 NextXXX 方法遍历时，返回下一个（Next）的 mem[]下标（返回值>0），或 mem2[] 的下标（返回值<0），或已遍历结束（返回值=0）
	int AlloSeqIdx(int fromIndex, int toIndex);						// 找 mem[] 中一个没使用的空间，仅查找空间下标为从 fromIndex 开始、到 toIndex 结束的空间
	bool RefreshArrTable();											// 遍历哈希表，将数据下标存入 mArrTable[]，设置 mArrTableCount 为数据个数（返回成功或失败）

	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);	// 重定义 一个 MemType 类型的数组（如可以是 lMem[] 或 lMem2[]）的大小，新定义空间自动清零

	int GetMemIndexFromKey(KeyType key, bool raiseErrorIfNotHas=true);	// 从 Key 获得数据在 mem[] 中的下标（返回值>0）或在 mem2[] 中的下标（返回值<0），出错返回 0
	int GetMemIndexFromIndex(int index, bool raiseErrorIfNotHas=true);	// 从 index 获得数据在 mem[] 中的下标（返回值>0）或在 mem2[] 中的下标（返回值<0），出错返回 0

	void SaveItemString(TCHAR ** ptrSaveTo, LPCTSTR ptrNewString);			// 用 new 开辟新字符串空间，把 key 指向的字符串拷贝到新空间；ptrSaveTo 是一个保存字符串地址的指针变量的地址，其指向的指针变量将保存“用 new 开辟的新字符串空间的地址”，即让 “*ptrSaveTo = 新空间地址”
};






// ===============================================================
// CBHashStrK：字符串型 key 的哈希表类
// 支持每个元素中有一个 long 型的数据和一个 long 型的附加数据
//
// 可能抛出的异常：
// throw (unsigned char)7;	// 超出内存
// throw (unsigned char)5;	// 无效的过程调用或参数：键值错误，如试图添加已存在的同样键值的新元素，访问键不存在的元素
// throw (unsigned char)9;	// 下标越界：无法分配新数据空间
//
// ---- 可以使用 Index 的方式遍历所有哈希表元素 ----
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.KeyFromIdx(i)
//     
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.ItemFromIdx(i)
//     
// 注意 Index 并不与数据对应，随着增删和重排，数据的 Index 都可能会变化
// 但在同一时刻，Index 相同的一套数据（Key,Data,DataLong,DataString）是同一套
// ===============================================================

class CBHashStrK
{
private:
	typedef struct _MemType
	{
		LPTSTR Key;			// 指向本类管理的用 new 开辟的一块内存区，每个 mem[] 在被删除时，先 delete [] .key
		int Data;
		long DataLong;
		long DataLong2;
		double DataDouble;
		LPTSTR DataStr;
		LPTSTR DataStr2;
		bool Used;
		int Index;		// mArrTable[] 数组的 mArrTable[index] 元素，是保存本 MemType 数据
						//   所在的 mem[] 中的下标（index>0）或在 mem2[] 中的下标（index<0）
						//   mArrTableCount == memUsedCount + memUsedCount2 时且 index !=0 时 有效
						//   在 RefreshArrTable 中设置此成员

	} MemType;
	
	static const int	mcIniMemSize;  			// 初始 mem[] 的大小
	static const int	mcMaxItemCount; 		// 最多元素个数（可扩大此值到 long 表示的范围之内）
	static const float	mcExpandMaxPort;  		// 已有元素个数大于 0.75*memCount 时就扩大 mem[] 的空间
	static const int	mcExpandCountThres; 	// 扩大 mem[] 空间时，若 memCount 小于此值则每次扩大到 memCount*2；若 memCount 大于此值则每次扩大到 Count+Count/2
	static const int	mcExpandCountThresMax;	// 扩大 mem[] 空间时，若 memCount 已大于此值，则每次不再扩大到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// 扩大 mem[] 空间时，若 memCount 已大于 mcExpandCountThresMax，则每次不再扩大到到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandMem2Per; 		// 每次扩大 mem2[] 的大小
	static const int	mcSeqMax; 				// 顺序检索最大值

private:
	MemType * mem;					// 动态数组指针，但数组不使用 [0] 的元素
	int memCount, memUsedCount;		// 动态数组最大下标，mem[] 数组下标为 [0] ～ [memCount]。哈希表已用元素个数
	MemType * mem2;					// 空间冲突的元素的保存空间，顺序使用
	int memCount2, memUsedCount2;	// mem2[] 数组下标为 [0] ～ [memCount2]，其中 [0] 不用，已使用空间为 [1] ～ [memUsedCount2]
	int mTravIdxCurr;				// 用 NextXXX 方法遍历的当前 index，正值表示 mem[] 中的下标，负值表示 mem2[] 中的下标
	
	// 支持通过给定下标 Index 访问一个哈希表数据，mArrTable 指向动态数组，
	//   数组元素保存：所有哈希表数据依遍历顺序所在的 mem[] 中的下标（>0）
	//   或 mem2[] 中的下标。
	// 遍历一次，将所有哈希表数据 mem[] 或 mem2[] 的下标存于此数组，
	//   以后不需重复遍历，直接通过给定下标 Index 访问一个哈希表数据。
	//   mArrTableCount != memUsedCount + memUsedCount2 为标志，
	//   如 !=，标志要重新刷新遍历。注意重新遍历后，元素顺序可能会重排。
	//   各哈希表数据的 Index 并不是一直不变的
	int * mArrTable;
	int mArrTableCount;

public:
	bool KeyCaseSensitive;			// 是否 key 要区分大小写（默认不分大小写）

public:
	CBHashStrK(int memSize=0);		// memSize=0 则开辟初始 mcIniMemSize 个空间，否则开辟 memSize 个空间，memSize 应比实际数据个数大一些
	~CBHashStrK();
	void AlloMem(int memSize);		// 事先可用此函数定义足够大的空间，以减少以后自动扩大空间的次数，提高效率
	bool Add(int data, LPCTSTR key=0, long dataLong=0, long dataLong2=0, LPCTSTR dataStr=NULL, LPCTSTR dataStr2=NULL, double dataDouble=0.0, bool raiseErrorIfNotHas=true);	// 添加元素
	bool Remove(LPCTSTR key, bool raiseErrorIfNotHas=true);					// 删除元素
	
	// 根据 key 获得元素、附加数据
	int Item(LPCTSTR key, bool raiseErrorIfNotHas=true);
	long ItemLong(LPCTSTR key, bool raiseErrorIfNotHas=true);
	long ItemLong2(LPCTSTR key, bool raiseErrorIfNotHas=true);
	double ItemDouble(LPCTSTR key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr(LPCTSTR key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2(LPCTSTR key, bool raiseErrorIfNotHas=true);

	// 根据 key 设置元素、附加数据
	bool ItemSet(LPCTSTR key, int vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLongSet(LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2Set(LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleSet(LPCTSTR key, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrSet(LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2Set(LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);


	// 根据 index 获得元素、附加数据
	int ItemFromIndex(int index, bool raiseErrorIfNotHas=true);	
	long ItemLongFromIndex(int index, bool raiseErrorIfNotHas=true);
	long ItemLong2FromIndex(int index, bool raiseErrorIfNotHas=true);
	double ItemDoubleFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStrFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2FromIndex(int index, bool raiseErrorIfNotHas=true);

	// 根据 index 设置元素、附加数据（但不能设置 Key，Key为只读）
	bool ItemFromIndexSet(int index, int vNewValue, bool raiseErrorIfNotHas=true);	
	bool ItemLongFromIndexSet(int index, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2FromIndexSet(int index, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleFromIndexSet(int index, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrFromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2FromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);

	LPTSTR IndexToKey(int index, bool raiseErrorIfNotHas=true);
	int KeyToIndex(LPCTSTR key, bool raiseErrorIfNotHas=true);


	bool IsKeyExist(LPCTSTR key);											// 判断某个 key 的元素是否存在
	void Clear(void);								// 清除所有元素，重定义 mcIniMemSize 个存储空间
	
	void StartTraversal();							// 开始一个遍历过程
	int NextItem(bool &bRetNotValid);			// 遍历过程开始后，不断调用此函数，获得每个元素，直到 bRetNotValid 返回 true
	long NextItemLong(bool &bRetNotValid);	// 遍历过程开始后，不断调用此函数，获得每个元素的附加数据，直到 bRetNotValid 返回 true
	long NextItemLong2(bool &bRetNotValid);
	double NextItemDouble(bool &bRetNotValid);
	LPTSTR NextItemStr(bool &bRetNotValid);
	LPTSTR NextItemStr2(bool &bRetNotValid);

	LPTSTR NextKey(bool &bRetNotValid);			// 遍历过程开始后，不断调用此函数，获得每个元素的 key，直到 bRetNotValid 返回 true

	int Count(void);								// 返回共有元素个数


private:
	int AlloMemIndex(LPCTSTR Key, bool CanExpandMem=true );		// 根据 Key 分配一个 mem[] 中的未用存储空间，返回 mem[] 数组下标
	int FindSeqIdx(LPCTSTR key, int fromIndex, int toIndex);	// 找 mem[] 中键为 key 的元素下标，仅查找空间下标为从 fromIndex 开始、到 toIndex 结束的空间
	void ReLocaMem(int preMemCountTo);							// 重新分配 mem[], mem2[] 的各元素的地址，mem2[] 的某些元素可能被重新移动到 mem[]
	void ExpandMem(void);										// 重定义 mem[] 数组大小，扩大 mem[] 的空间
	int TraversalGetNextIdx(void);								// 用 NextXXX 方法遍历时，返回下一个（Next）的 mem[]下标（返回值>0），或 mem2[] 的下标（返回值<0），或已遍历结束（返回值=0）
	int AlloSeqIdx(int fromIndex, int toIndex);					// 找 mem[] 中一个没使用的空间，仅查找空间下标为从 fromIndex 开始、到 toIndex 结束的空间
	bool RefreshArrTable();										// 遍历哈希表，将数据下标存入 mArrTable[]，设置 mArrTableCount 为数据个数（返回成功或失败）

	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);	// 重定义 一个 MemType 类型的数组（如可以是 lMem[] 或 lMem2[]）的大小，新定义空间自动清零

	int GetMemIndexFromKey(LPCTSTR key, bool raiseErrorIfNotHas=true);	// 从 Key 获得数据在 mem[] 中的下标（返回值>0）或在 mem2[] 中的下标（返回值<0），出错返回 0
	int GetMemIndexFromIndex(int index, bool raiseErrorIfNotHas=true);	// 从 index 获得数据在 mem[] 中的下标（返回值>0）或在 mem2[] 中的下标（返回值<0），出错返回 0

	long KeyStringToLong(LPCTSTR key);		// 将一个 字符串类型的 key 转换为一个长整数
	void SaveItemString(TCHAR ** ptrSaveTo, LPCTSTR ptrNewString);			// 用 new 开辟新字符串空间，把 key 指向的字符串拷贝到新空间；ptrSaveTo 是一个保存字符串地址的指针变量的地址，其指向的指针变量将保存“用 new 开辟的新字符串空间的地址”，即让 “*ptrSaveTo = 新空间地址”
	bool CompareKey(LPCTSTR key1, LPCTSTR key2);		// 根据 KeyCaseSensitive 属性，比较两 key 的字符串是否相等：相等返回True，不等返回False
};




//////////////////////////////////////////////////////////////////////
// CBRecycledArr：带回收站的数组类
//
// 可组织一批数据，数据类型可同时为：
//    一个字符串、两个整数、一个单精度数、一个双精度数
// 可随意 Add、Remove，自动维护 index 为 1～Count 范围，以便通过数组
//   的方式访问各元素；也支持通过遍历的方法访问各元素
//////////////////////////////////////////////////////////////////////


// 可能 throw 的错误：
// throw (unsigned char)5;			// 无效的过程调用或参数
class CBRecycledArr  
{
private:
	typedef struct _MemType
	{
		LPTSTR DataString;	// 字符串数据
		int DataInt;		// 附加整型数据1
		int DataInt2;		// 附加整型数据2
		float DataFloat;	// 附加单精度数据
		double DataDouble;	// 附加双精度数据
		bool Used;
	} MemType;
	
	static const int	mcIniMemSize;  			// 初始 mem[] 的大小
	static const int	mcMaxItemCount; 		// 最多元素个数（可扩大此值到 long 表示的范围之内）
	static const float	mcExpandMaxPort;  		// 已有元素个数大于 0.75*memCount 时就扩大 mem[] 的空间
	static const int	mcExpandCountThres; 	// 扩大 mem[] 空间时，若 memCount 小于此值则每次扩大到 memCount*2；若 memCount 大于此值则每次扩大到 Count+Count/2
	static const int	mcExpandCountThresMax;	// 扩大 mem[] 空间时，若 memCount 已大于此值，则每次不再扩大到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// 扩大 mem[] 空间时，若 memCount 已大于 mcExpandCountThresMax，则每次不再扩大到到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcRecyExpandPer;		// 扩大 recycles[] 空间时，每次扩大的大小
	
private:
	MemType * mem;					// 动态数组指针，但数组不使用 [0] 的元素
	int memCount, memUsedCount;		// 动态数组最大下标，mem[] 数组下标为 [0] ～ [memCount]。已用[0] ～ [memUsedCount]（其中可能有删除的元素，删除的元素下标在 recycles 中保存）
	int * recycles;					// 指向一个整型数组，数组各元素保存 mem[] 中已删除的元素的下标；该数组被维护从小到大排序
	// recycles[] 数组下标为 [0] ～ [recyclesCount]，其中 [0] 不用；用 [1] - [recyUsedCount]，其余为可用空余空间
	int recyCount, recyUsedCount;	// recycles[] 数组元素下标最大值
	int mTravIdxCurr;				// 用 NextXXX 方法遍历的当前 index
	
public:
	CBRecycledArr(int initSize = 0);	// initSize = 0 时，初始开辟 mcIniMemSize 大小的空间，否则开辟 initSize 大小的空间
	~CBRecycledArr(); 
	
	int Add(LPCTSTR dataString = 0, int dataInt = 0, int dataInt2 = 0, float dataFloat = 0.0, double dataDouble = 0.0);  // 添加新数据，返回新数据被保存到的 mem[] 中的下标（>0），出错返回0
	bool Remove(int index);				// 删除一个数据，index 为要删除数据的下标；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count
	LPTSTR Item(int index, bool bRaiseErrIfNotHas=false);		// 根据下标 index，返回一个字符串数据；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count；字符串的内存空间由本类自动管理，主调程序不必干预；如主调程序修改了返回指针的指向内容，则本类对象内部内容也同时发生变化
	int ItemInt(int index, bool bRaiseErrIfNotHas=false);		// 根据下标 index，返回一个整型附加数据；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count
	int ItemInt2(int index, bool bRaiseErrIfNotHas=false);		// 根据下标 index，返回一个整型附加数据2；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count
	float ItemFloat(int index, bool bRaiseErrIfNotHas=false);	// 根据下标 index，返回一个 float 型附加数据；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count
	double ItemDouble(int index, bool bRaiseErrIfNotHas=false);	// 根据下标 index，返回一个 double 型附加数据；删除数据后，后续数据 index 会自动调整；使 index 总为 1～Count
	int Count();						// 返回当前拥有的数据个数
	void Clear();						// 删除所有数据
	
	void StartTraversal();				// 开始一次遍历
	LPTSTR NextItem( bool &bRetNotValid );			// 遍历的下一个数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	int NextDataInt( bool &bRetNotValid );			// 遍历的下一个附加整型数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	int NextDataInt2( bool &bRetNotValid );			// 遍历的下一个附加整型数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	float NextDataFloat( bool &bRetNotValid );		// 遍历的下一个附加 float 型数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	double NextDataDouble( bool &bRetNotValid );	// 遍历的下一个附加整 double 数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	
private:
	void ExpandMem(void);
	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);		// 重定义 一个 MemType 类型的数组（如可以是 mem[]）的大小，新定义空间自动清零
	int RedimArrInt( int * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false );				// 重定义 一个 int 类型的数组（如可以是 recycles）的大小，新定义空间自动清零
	int FindPosInSortedRecycle(int itemToFind);		// 用二分查找法在数组 recycles 中查找元素 itemToFind 的位置
	int UserIndexToMemIndex(int index);				// 根据用户 index，获得在 mem[] 中的下标；删除数据后，后续数据的用户 index 会自动调整；使用户 index 总为 1～Count
};




//////////////////////////////////////////////////////////////////////
// CBRecycledArrInt：整型版带回收站的数组类
//
// 可组织一批数据，数据类型可同时为：
//    两个整数、一个单精度数、一个双精度数
// 可随意 Add、Remove，自动维护 index 为 1～Count 范围，以便通过数组
//   的方式访问各元素；也支持通过遍历的方法访问各元素
//////////////////////////////////////////////////////////////////////

// 可能 throw 的错误：
// throw (unsigned char)5;			// 无效的过程调用或参数
class CBRecycledArrInt  
{
private:
	typedef int DataType;			// 数据的类型
	typedef int DataIntType;		// 附加整型数据的类型
	typedef float DataFloatType;	// 附加单精度数据的类型
	typedef double DataDoubleType;	// 附加双精度数据的类型
	
	typedef struct _MemType
	{
		DataType Data;
		DataIntType DataInt;
		DataFloatType DataFloat;
		DataDoubleType DataDouble;
		bool Used;
	} MemType;
	
	static const int	mcIniMemSize;  			// 初始 mem[] 的大小
	static const int	mcMaxItemCount; 		// 最多元素个数（可扩大此值到 long 表示的范围之内）
	static const float	mcExpandMaxPort;  		// 已有元素个数大于 0.75*memCount 时就扩大 mem[] 的空间
	static const int	mcExpandCountThres; 	// 扩大 mem[] 空间时，若 memCount 小于此值则每次扩大到 memCount*2；若 memCount 大于此值则每次扩大到 Count+Count/2
	static const int	mcExpandCountThresMax;	// 扩大 mem[] 空间时，若 memCount 已大于此值，则每次不再扩大到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// 扩大 mem[] 空间时，若 memCount 已大于 mcExpandCountThresMax，则每次不再扩大到到 Count+Count/2，而只扩大到 Count+mcExpandBigPer
	static const int	mcRecyExpandPer;		// 扩大 recycles[] 空间时，每次扩大的大小
	
private:
	MemType * mem;					// 动态数组指针，但数组不使用 [0] 的元素
	int memCount, memUsedCount;		// 动态数组最大下标，mem[] 数组下标为 [0] ～ [memCount]。已用[0] ～ [memUsedCount]（其中可能有删除的元素，删除的元素下标在 recycles 中保存）
	int * recycles;					// 指向一个整型数组，数组各元素保存 mem[] 中已删除的元素的下标
	// recycles[] 数组下标为 [0] ～ [recyclesCount]，其中 [0] 不用；用 [1] - [recyUsedCount]，其余为可用空余空间
	int recyCount, recyUsedCount;	// recycles[] 数组元素下标最大值
	int mTravIdxCurr;				// 用 NextXXX 方法遍历的当前 index
	
public:
	CBRecycledArrInt(int initSize = 0);	// initSize = 0 时，初始开辟 mcIniMemSize 大小的空间，否则开辟 initSize 大小的空间
	~CBRecycledArrInt(); 
	
	int Add(DataType data, DataIntType dataInt = 0, DataFloatType dataFloat = 0.0, DataDoubleType dataDouble = 0.0);  // 添加新数据，返回新数据被保存到的 mem[] 中的下标（>0），出错返回0
	bool Remove(int index);				// 删除一个数据，index 为要删除数据的下标。
	DataType Item(int index, bool bRaiseErrIfNotHas=false);				// 根据下标 index，返回一个数据
	DataIntType ItemInt(int index, bool bRaiseErrIfNotHas=false);		// 根据下标 index，返回一个整型附加数据
	DataFloatType ItemFloat(int index, bool bRaiseErrIfNotHas=false);	// 根据下标 index，返回一个 float 型附加数据
	DataDoubleType ItemDouble(int index, bool bRaiseErrIfNotHas=false);	// 根据下标 index，返回一个 double 型附加数据
	int Count();						// 返回当前拥有的数据个数
	void Clear();						// 删除所有数据
	
	void StartTraversal();				// 开始一次遍历
	DataType NextItem( bool &bRetNotValid );				// 遍历的下一个数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	DataIntType NextDataInt( bool &bRetNotValid );			// 遍历的下一个附加整型数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	DataFloatType NextDataFloat( bool &bRetNotValid );		// 遍历的下一个附加 float 型数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	DataDoubleType NextDataDouble( bool &bRetNotValid );	// 遍历的下一个附加整 double 数据，若 bRetEndNotValid 返回 True，表此次遍历已结束（此时函数返回值也无效）
	
private:
	void ExpandMem(void);
	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);		// 重定义 一个 MemType 类型的数组（如可以是 mem[]）的大小，新定义空间自动清零
	int RedimArrInt( int * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false );				// 重定义 一个 int 类型的数组（如可以是 recycles）的大小，新定义空间自动清零
	
};




//////////////////////////////////////////////////////////////////////
// CHeapMemory： 用全局对象维护所有通过 new 分配的内存指针，在本类对象
//				 析构时会自动 delete 此这些内存
//
//////////////////////////////////////////////////////////////////////

class CBHeapMemory  
{
public:
	CBHeapMemory(int initSize=0);
	virtual ~CBHeapMemory();
	
	// 添加一个要由本对象管理的、已用new分配的空间的地址，
	//   成功返回本对象所管理的地址个数（含新增的这一个）
	// 若 bArrayNew=true，将在 delete 时有[]；否则 delete 时没有[]
	int AddPtr(void *ptrNew, bool bArrayNew=true);

	// 用 new 分配 size 个字节的空间，并自动清0
	//   返回此空间的地址（出错返回0），并由本对象自动记录此空间的地址
	void * Alloc(int size); 

	// 释放 ptr 所指向的一段内存空间
	//   ptr 必须是由本对象所管理的空间，否则本函数不会释放
	void Free(void *ptr);

	// 返回 ptr 所指向的一段内存空间是否正由本对象管理
	bool IsPtrManaged(void *ptr);

	// 返回 本对象正管理的空间个数
	int CountPtrs();

	// 返回 本对象正管理的一个空间的地址
	//   index 应为 1～CountPtrs() 之间
	//   由参数 ptrbArrayNew 返回该空间 delete 时是否应有[]
	void * PtrEach(int index, bool * ptrbArrayNew=0);

	// 清零一块内存空间。实际是调用 memset (本类已 include <memory.h>) ，
	//   为使主调程序不必再 include memory.h，本类也提供了这个功能接口
	void ZeroMem(void * ptr, unsigned int length);	
	
	// 内存拷贝。实际是调用 memcpy (本类已 include <memory.h>) ，
	//   为使主调程序不必再 include memory.h，本类也提供了这个功能接口）
	void CopyMem(void * dest, void * source, unsigned int length);	
	
	// 强制释放本对象所记录的所有地址指向的空间
	//   本对象析构时也会自动调用本函数
	void Dispose();									
	
private:

	// 保存由本对象管理的、所有用 new 动态分配的空间地址
	//   key=地址；data=地址；
	//   dataLong!=0 时，delete 时会有[]；dataLong=0，delete 时无[]
	CBHashLK memHash;
};


// 全局对象变量 HM 的声明，管理程序中所有以 new 开辟的内存指针，该对象
// 析构时可自动 delete 这些内存。真正的变量定义在 BWindows.cpp 中，这里
// 用 extern 声明此变量，则所有包含本 h 文件的模块都可使用此全局变量

extern CBHeapMemory HM;	
