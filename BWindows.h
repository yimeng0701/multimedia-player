//////////////////////////////////////////////////////////////////////////
// Bwindows.h �����˱�д Windows �����������ͷ�ļ���
//            һЩ�����Զ��庯�������������͡���������Ķ���
//   �������
//      CBApp �ࣺ ����Ӧ�ó���ȫ����Ϣ
// 		CBHashLK���������� key �Ĺ�ϣ����
// 		CBHashStrK���ַ����� key �Ĺ�ϣ����
// 		CBRecycledArr��������վ��������
// 		CBRecycledArrInt�����Ͱ������վ��������
// 		CHeapMemory�� ��ȫ�ֶ���ά������ͨ�� new ������ڴ�ָ��
//
// ������ h �ļ�����ʹ��ȫ�ֶ���ָ�룺pApp��->����ó���ȫ����Ϣ����ֻ��
//   ͬʱ���� BForm ��ģ�飨��װ WinMain ����������ܻ����ȷ����Ϣ
//
// ������ h �ļ�����ʹ��ȫ�ֶ���HM �������� new ���ٵ��ڴ�ָ�룩
//    ���� HM.AddPtr(p, bArrNew); ��ӹ���һ���� new ���ٵ��ڴ�ָ��
//    ���� HM.Dispose(); �ͷ�Ŀǰ����������ж�̬�ڴ棨�������ʱҲ���Զ��ͷţ�
//////////////////////////////////////////////////////////////////////////

#pragma once

#define OEMRESOURCE		// to use any of the OBM_ constants
#include <windows.h>
#include <stdlib.h>		// ���� atoi��atof �Ⱥ���
#include <memory.h>		
#include <string.h>
#include <tchar.h>		
#include <math.h>


// EventsGenerator �������ش�ֵ��ʾҪ����Ĭ�ϵĴ��ڳ��򣬷����ٵ���Ĭ�ϵĴ��ڳ���
#define gc_APICEventsGenDefautRet 0xFEEEEEEE  

enum EStandardIcon		// ϵͳͼ��
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
enum EStandardCursor	// ϵͳ���ָ����״��Ϊ��Ӧ API ���� + gc_IDStandCursorIDBase��Ŀ����Ϊ����Դ ID ���֣���Դ ID ��С�� gc_IDStandCursorIDBase��
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

enum EOEMBmp		// ϵͳ OEM λͼ
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

enum EOEMIcon		// ϵͳ OEM ͼ��
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

enum EOEMCursor		// ϵͳ OEM ���
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

enum EColorType			// ϵͳ��ɫ
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

enum EShowWindowCmd		// ������ʾ״̬
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


enum EMsgBoxBtn			// ������Ϣ��İ�ť����
{
	mb_OK = MB_OK,
	mb_OkCancel = MB_OKCANCEL,
	mb_AbortRetryIgnore = MB_ABORTRETRYIGNORE,
	mb_YesNoCancel = MB_YESNOCANCEL,
	mb_YesNo = MB_YESNO,
	mb_RetryCancel = MB_RETRYCANCEL
};

enum EMsgBoxIcon		// ������Ϣ���ͼ��
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


enum EDlgBoxDefBtn		// ������Ϣ��ָ��Ĭ�ϰ�ť
{
		mb_DefButton1 = MB_DEFBUTTON1,
		mb_DefButton2 = MB_DEFBUTTON2,
		mb_DefButton3 = MB_DEFBUTTON3
#if(WINVER >= 0x0400)
		,mb_DefButton4 = MB_DEFBUTTON4
#endif /* WINVER >= 0x0400 */
};

enum EDlgBoxCmdID		//������Ϣ��ָ���û����µİ�ť
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


enum ESysMenu		// ϵͳ�˵�ö������
{
	eSysMenu_Close = SC_CLOSE,
	eSysMenu_Minimize = SC_MINIMIZE,
	eSysMenu_Maxmize = SC_MAXIMIZE,
	eSysMenu_Move = SC_MOVE,
	eSysMenu_Size = SC_SIZE,
	eSysMenu_Restore = SC_RESTORE
};




//////////////////////////////////////////////////////////////////////////
// �����Զ��庯��
//
//////////////////////////////////////////////////////////////////////////

// ������Ϣ��
//   �� title Ϊ NULL�����Զ�ʹ��Ӧ�ó�������Ϊ title
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

// ��һ������ת��Ϊ�˽��Ƶ��ַ������ַ����ռ��Զ�����
LPTSTR Oct(long number);

// ��һ������ת��Ϊʮ�����Ƶ��ַ������ַ����ռ��Զ�����
LPTSTR Hex(long number);


// ��õ�ǰ·���ַ������ַ����ռ��Զ�����
LPTSTR CurDir();

// ��ȡһ���Զ�����Դ���ֽ����ݣ��ռ��ɱ�ģ���Զ����١��Զ�����
//   rSize ��Ϊ0ʱ�����Ӵ˲���ָ��Ŀռ䷵����Դ���ֽ���
unsigned char * LoadResData(UINT idRes, UINT typeRes, unsigned long * rSize=0);
unsigned char * LoadResData(UINT idRes, LPCTSTR typeRes, unsigned long * rSize=0 );
unsigned char * LoadResData(LPCTSTR idRes, UINT typeRes, unsigned long * rSize=0 );
unsigned char * LoadResData(LPCTSTR idRes, LPCTSTR typeRes, unsigned long * rSize=0);

//////////////////////////////////////////////////////////////////////////
// ʱ�� ����
//////////////////////////////////////////////////////////////////////////

// ���ص�ǰϵͳ���ڡ�ʱ���һ���ַ���
//   �� lpDblTime ��Ϊ0��������ǰϵͳ���ڡ�ʱ��ת��Ϊ double 
//     ��Ϊ1601-1-1���������ĺ�������������ָ��� double �ͱ�����
//   �� lpTime ��Ϊ0��������ǰϵͳ���ڡ�ʱ��洢����ָ��Ľṹ��
LPTSTR Now( double *lpDblTime=0, SYSTEMTIME *lpTime=0);

// ���õ�ǰϵͳ���ڡ�ʱ��
BOOL NowSet( SYSTEMTIME stNow );

// ��һ�����ڡ�ʱ��ת��Ϊ double ���أ�Ϊ1601-1-1���������ĺ�������
double DateTimeDbl( SYSTEMTIME stDatetime );

// �����������ڡ�ʱ���ʱ����ʱ���õļ������
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

// �����������ڡ�ʱ���ʱ����
// style Ϊָ��ʱ�����ĵ�λ
double DateDiff(eDataTimeDiffStyle style,
				SYSTEMTIME stDatetime1,
				SYSTEMTIME stDatetime2);


//////////////////////////////////////////////////////////////////////////
// �Զ����ַ��� ����
//////////////////////////////////////////////////////////////////////////

// �� printf ��ʽ����һ���ַ������ַ����ռ��ɱ�ģ���Զ����١��Զ�����
LPTSTR cdecl StrPrintf( LPCTSTR szFormat, ... );

// ȡ�ַ�����ǰ length ���ַ�������ַ������������ַ������׵�ַ
//   ���ַ����ռ��ɱ�ģ���Զ����١��Զ�����
LPTSTR Left(LPCTSTR szStringSrc, int length);		

// ȡ�ַ����ĺ� length ���ַ�������ַ������������ַ������׵�ַ
//   ���ַ����ռ��ɱ�ģ���Զ����١��Զ�����
LPTSTR Right(LPCTSTR szStringSrc, int length);				

// ȡ�ַ����Ĵӵ� startPos ���ַ��𣬳� length ���ַ�������ַ���
//  ����һ���ַ�λ��Ϊ1�����������ַ������׵�ַ
//   ���ַ����ռ��ɱ�ģ���Զ����١��Զ�����
LPTSTR Mid(LPCTSTR szStringSrc, int startPos, int length);	

// ɾ���ַ�����ǰ����β�������пո񣬷���ɾ��������ַ�����
//   ���ַ����ռ��ɱ�ģ���Զ����١��Զ�����
// bDelOtherSpace=trueʱɾ�����пո����ڵ� isspace �������
//   �����ַ���bDelOtherSpace=false ʱֻɾ���ո�
LPTSTR LTrim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);
LPTSTR RTrim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);
LPTSTR Trim(LPCTSTR szStringSrc, bool bDelOtherSpace=false);

// ���ַ����е���ĸȫ��ת��Ϊ��д��UCase����Сд��LCase��
//   ����ת��������ַ��������ַ����ռ��Զ�����
LPTSTR LCase(LPCTSTR szStringSrc);
LPTSTR UCase(LPCTSTR szStringSrc);


enum eBStrCompareMethod		// �ַ����ȽϷ�ʽ
{
	bcmBinaryCompare = 0,	// �����ƱȽϣ����ִ�Сд��
	bcmTextCompare = 1		// �ı��Ƚϣ������ִ�Сд��
};


// �� szSrc �У��ӵ� start ���ַ���ʼ����һ���ַ�λ��Ϊ1����
//	  �����ַ��� szFind �ĵ�һ�γ��ֵ�λ�ã���һ���ַ�λ��Ϊ1����
//    �ҵ�����ֵ>0��û�ҵ�����0
//  ˵����������δ�����κο⺯����strlenҲδ���ã������������Ч��
int InStr(int			start,										
		  LPCTSTR		szSrc, 
		  LPCTSTR		szFind,
	eBStrCompareMethod	compare=bcmBinaryCompare);


// �� szSrc �У��ӵ�һ���ַ���ʼ�������ַ��� szFind ��
//   ��һ�γ��ֵ�λ�ã���һ���ַ�λ��Ϊ1����
//   ����λ�úţ���һ���ַ�λ��Ϊ1�����ҵ�����ֵ>0��û�ҵ�����0
int InStr(LPCTSTR		szSrc,								
		  LPCTSTR		szFind,
    eBStrCompareMethod	compare=bcmBinaryCompare);

// �� szSrc �У��ӵ� start ���ַ���ʼ����һ���ַ�λ��Ϊ1����ĩβ�Ĳ��֣�
//	  �����ַ��� szFind �ĵ�����һ�γ��ֵ�λ�ã���һ���ַ�λ��Ϊ1����
//    �ҵ�����ֵ>0��û�ҵ�����0
//  ˵����������δ�����κο⺯����strlenҲδ���ã������������Ч��
int InStrRev(LPCTSTR		szSrc, 
			 LPCTSTR		szFind, 
			 int			start=1,
			 eBStrCompareMethod	compare=bcmTextCompare);

// ���ָ��ַ��� delimiters ���ָ�һ���ַ����������������ַ���
//	 �� ���ַ��� �ĵ�ַ�� ptrStrings[] ���鷵�أ������������ַ����ĸ���
//   ptrStrings[] �����±��1��ʼ����󵽺�������ֵ
// limit ���Ʒ������ַ�������������Ϊ -1 ʱ�����ƣ������������ַ���
//   ���ַ����ڴ漰 ptrStrings[] �����ڴ涼�ɱ�ģ���Զ����䡢�Զ�����
int Split(LPCTSTR		stringSrc,				
		  TCHAR		** &ptrStrings,				
		  LPCTSTR		delimiters=0,			
		  int		    limit=-1,				
	eBStrCompareMethod  compare=bcmBinaryCompare);	



// �� delimiter ���Ӷ���ַ������������Ӻõ��ַ���
// ����ַ����ĵ�ַ������ stringSrcArray[] ������������
//   �����д��±� arrayIndexStart �� arrayIndexEnd ���ַ���
// delimiter Ϊ 0 ʱ��Ĭ���� "\0" �����ַ������������ַ��� delimiter ����
// bTailDoubleNull ��Ϊ true�����ڽ���ַ���������ټ�һ��'\0'�������������'\0'��
// ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
LPTSTR Join(TCHAR * stringSrcArray[], 
		    const int    arrayIndexEnd, 
		    LPCTSTR delimiter=0, 
		    const int    arrayIndexStart=1, 
		    const bool   bTailDoubleNull=false);

// �滻�ַ���
LPTSTR Replace(LPCTSTR szStringSrc,		// Ҫ���滻���ַ���
			   LPCTSTR szFind,			// Ҫ���滻�������ַ���
			   LPCTSTR szReplaceWith,	// Ҫ�滻Ϊ������
			   int start=1,			// �� szStringSrc �����ַ��������Ŀ�ʼλ��
			   int countLimit=-1,	// ���ַ��������滻�Ĵ������C1 �����������п��ܵ��滻
			   eBStrCompareMethod  compare=bcmBinaryCompare	// �б����ַ���ʱ���õıȽϷ�ʽ
			   );

// �����ַ������������Ӻ�ĳ��ַ���
// �������Ӻõ��ַ������׵�ַ���Զ�ά����̬�ռ�
// ÿ�ε��ÿ��������9���ַ���
LPTSTR StrAppend(LPCTSTR str1=0, 
				 LPCTSTR str2=0, 
				 LPCTSTR str3=0, 
				 LPCTSTR str4=0, 
				 LPCTSTR str5=0, 
				 LPCTSTR str6=0, 
				 LPCTSTR str7=0,
				 LPCTSTR str8=0,
				 LPCTSTR str9=0 );


// �� ANSI �� UTF8 �ַ���ת��Ϊ Unicode�����ؽ���ַ����׵�ַ
//   ���� bToUTF8orANSI Ϊ false ʱת�� ANSI��Ϊ true ʱת�� UTF8
//   ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
LPWSTR StrConvUnicode(const char * szAnsi, bool bFromUTF8orANSI=false );   // LPWSTR ���� unsigned short int *

// �� Unicode �ַ���ת��Ϊ ANSI �� UTF8�����ؽ���ַ����׵�ַ
//   ���� bToUTF8orANSI Ϊ false ʱת��Ϊ ANSI��Ϊ true ʱת��Ϊ UTF8
//   ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
char * StrConvFromUnicode(LPCWSTR szUnicode, bool bToUTF8orANSI=false );

// ���ַ���ת��Ϊ double ����ֵ
double Val( LPCWSTR stringVal );	// Unicode �ַ���
double Val( LPCSTR stringVal );		// ANSI �ַ���

// ��������������ת��Ϊ�ַ���
// �����ַ����׵�ַ���ַ����ռ��ɱ������Զ����ٺ͹���
LPTSTR Str(char number);
LPTSTR Str(unsigned short int number);	// TCHAR
LPTSTR Str(int number);
LPTSTR Str(unsigned int number);
LPTSTR Str(unsigned long number);
LPTSTR Str(float number);
LPTSTR Str(double number);
LPTSTR Str(long double number);



//////////////////////////////////////////////////////////////////////////
// �Զ��� ��̬���� ����
//////////////////////////////////////////////////////////////////////////


// �ض��� һ�� REDIMTYPE ����(��int�͡�int *�ͼ�ָ������)������Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�루��Ϊ int *�ͱ�������void **�ͱ���������Ϊ����ָ�룩�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
template <typename REDIMTYPE>
int Redim(  REDIMTYPE *	 &arr, 
			int			  toUBound=-1, 
			int			  uboundCurrent=-1,  
			bool		  preserve=false )	// template ��������Ҫ��ͷ�ļ���
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		REDIMTYPE * ptrNew = new REDIMTYPE[toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(REDIMTYPE)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� REDIMTYPE Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(REDIMTYPE)*ctToCpy); 
		}
		
		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)
	{
		// �����ٿռ䣬ɾ��ԭ�пռ�
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
}


// ɾ����̬����Ŀռ䣬������ָ�� arr Ϊ 0
template <typename REDIMTYPE>
void Erase( REDIMTYPE *  &arr)
{
	if(arr!=0) delete [] arr;
	arr=0;
}



//////////////////////////////////////////////////////////////////////
// CBApp �ࣺ ����Ӧ�ó���ȫ����Ϣ
//
//////////////////////////////////////////////////////////////////////

class CBApp		// Ӧ�ó���ȫ����Ϣ����
{
public:
	// ���캯��
	CBApp(HINSTANCE hInst, 
		HINSTANCE hPrevInst, 
		char * lpCmdLine, 
		int nShowCmd);
	
	// ����Ա��ֻ�ܻ��ֵ�������޸ģ����ڹ��캯���ĳ�ʼ�������޸ĵģ�
	const HINSTANCE hInstance;	// WinMain �������ݹ����� hInstance
	const int CmdShow;			// WinMain �������ݹ����� nShowCmd
	
	// ���Ӧ�ó���ǰ���е�·������� \��
	LPTSTR Path();	

	// ��������в����ַ���������exe�ļ���·�����ַ����ռ��� Windows ά����
	LPTSTR Command() const;

	// �����Ļ��ȡ��߶�
	int ScreenWidth() const;
	int ScreenHeight() const;
private:
	TCHAR m_path[2048];		// Ӧ�ó���ǰ���е�·���ַ���������

};

// ȫ�ֶ������ pApp����ָ�룩����������ָ��Ķ������ڻ�ñ������ȫ�ֳ�����Ϣ��
//   �� hInstance �ȡ������ı��������� BWindows.cpp �У������� 
//   extern �����˱����������а����� h �ļ���ģ�鶼��ʹ�ô�ȫ�ֱ���
extern CBApp *pApp;	



// ===============================================================
// CBHashLK���������� key �Ĺ�ϣ����
// ֧��ÿ��Ԫ������һ�� long �͵����ݺ�һ�� long �͵ĸ�������
//
// �����׳����쳣��
// throw (unsigned char)7;	// �����ڴ�
// throw (unsigned char)5;	// ��Ч�Ĺ��̵��û��������ֵ��������ͼ����Ѵ��ڵ�ͬ����ֵ����Ԫ�أ����ʼ������ڵ�Ԫ��
// throw (unsigned char)9;	// �±�Խ�磺�޷����������ݿռ�
//
// ---- ����ʹ�� Index �ķ�ʽ�������й�ϣ��Ԫ�� ----
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.KeyFromIdx(i)
//     
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.ItemFromIdx(i)
//     
// ע�� Index ���������ݶ�Ӧ��������ɾ�����ţ����ݵ� Index �����ܻ�仯
// ����ͬһʱ�̣�Index ��ͬ��һ�����ݣ�Key,Data,DataLong,DataString����ͬһ��
// ===============================================================

class CBHashLK
{
private:
	typedef long KeyType;		// key ������
	typedef long DataType;		// ���ݵ�����
	typedef long DataLongType;	// �������ݵ�����
	typedef long DataLong2Type;	// �������ݵ�����
	
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
		int Index;		// mArrTable[] ����� mArrTable[index] Ԫ�أ��Ǳ��汾 MemType ����
						//   ���ڵ� mem[] �е��±꣨index>0������ mem2[] �е��±꣨index<0��
						//   mArrTableCount == memUsedCount + memUsedCount2 ʱ�� index !=0 ʱ ��Ч
						//   �� RefreshArrTable �����ô˳�Ա
	} MemType;
	
	static const int	mcIniMemSize;  			// ��ʼ mem[] �Ĵ�С
	static const int	mcMaxItemCount; 		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
	static const float	mcExpandMaxPort;  		// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
	static const int	mcExpandCountThres; 	// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
	static const int	mcExpandCountThresMax;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandMem2Per; 		// ÿ������ mem2[] �Ĵ�С
	static const int	mcSeqMax; 				// ˳��������ֵ

private:
	MemType * mem;					// ��̬����ָ�룬�����鲻ʹ�� [0] ��Ԫ��
	int memCount, memUsedCount;		// ��̬��������±꣬mem[] �����±�Ϊ [0] �� [memCount]����ϣ������Ԫ�ظ���
	MemType * mem2;					// �ռ��ͻ��Ԫ�صı���ռ䣬˳��ʹ��
	int memCount2, memUsedCount2;	// mem2[] �����±�Ϊ [0] �� [memCount2]������ [0] ���ã���ʹ�ÿռ�Ϊ [1] �� [memUsedCount2]
	int mTravIdxCurr;				// �� NextXXX ���������ĵ�ǰ index����ֵ��ʾ mem[] �е��±꣬��ֵ��ʾ mem2[] �е��±�

	
	// ֧��ͨ�������±� Index ����һ����ϣ�����ݣ�mArrTable ָ��̬���飬
	//   ����Ԫ�ر��棺���й�ϣ������������˳�����ڵ� mem[] �е��±꣨>0��
	//   �� mem2[] �е��±ꡣ
	// ����һ�Σ������й�ϣ������ mem[] �� mem2[] ���±���ڴ����飬
	//   �Ժ����ظ�������ֱ��ͨ�������±� Index ����һ����ϣ�����ݡ�
	//   mArrTableCount != memUsedCount + memUsedCount2 Ϊ��־��
	//   �� !=����־Ҫ����ˢ�±�����ע�����±�����Ԫ��˳����ܻ����š�
	//   ����ϣ�����ݵ� Index ������һֱ�����
	int * mArrTable;
	int mArrTableCount;
	
public:
	CBHashLK(int memSize=0);		// memSize=0 �򿪱ٳ�ʼ mcIniMemSize ���ռ䣬���򿪱� memSize ���ռ䣬memSize Ӧ��ʵ�����ݸ�����һЩ
	~CBHashLK();
	void AlloMem(int memSize);		// ���ȿ��ô˺��������㹻��Ŀռ䣬�Լ����Ժ��Զ�����ռ�Ĵ��������Ч��
	bool Add(DataType data, KeyType key=0, DataLongType dataLong=0, DataLong2Type dataLong2=0, LPCTSTR dataStr=0, LPCTSTR dataStr2=0, double dataDouble=0.0, bool raiseErrorIfNotHas=true);	// ���Ԫ��
	bool Remove(KeyType key, bool raiseErrorIfNotHas=true);					// ɾ��Ԫ��

	// ���� key ���Ԫ�ء���������
	DataType Item(KeyType key, bool raiseErrorIfNotHas=true);				
	DataLongType ItemLong(KeyType key, bool raiseErrorIfNotHas=true);
	DataLong2Type ItemLong2(KeyType key, bool raiseErrorIfNotHas=true);
	double ItemDouble(KeyType key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr(KeyType key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2(KeyType key, bool raiseErrorIfNotHas=true);

	// ���� key ����Ԫ�ء���������
	bool ItemSet(KeyType key, DataType vNewValue, bool raiseErrorIfNotHas=true);				
	bool ItemLongSet(KeyType key, DataLongType vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2Set(KeyType key, DataLong2Type vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleSet(KeyType key, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrSet(KeyType key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2Set(KeyType key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);


	// ���� index ���Ԫ�ء���������
	DataType ItemFromIndex(int index, bool raiseErrorIfNotHas=true);			
	DataLongType ItemLongFromIndex(int index, bool raiseErrorIfNotHas=true);
	DataLong2Type ItemLong2FromIndex(int index, bool raiseErrorIfNotHas=true);
	double ItemDoubleFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStrFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2FromIndex(int index, bool raiseErrorIfNotHas=true);

	// ���� index ����Ԫ�ء��������ݣ����������� Key��KeyΪֻ����
	bool ItemFromIndexSet(int index, DataType vNewValue, bool raiseErrorIfNotHas=true);			
	bool ItemLongFromIndexSet(int index, DataLongType vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2FromIndexSet(int index, DataLong2Type vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleFromIndexSet(int index, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrFromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2FromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);

	KeyType IndexToKey(int index, bool raiseErrorIfNotHas=true);
	int KeyToIndex(KeyType key, bool raiseErrorIfNotHas=true);

	bool IsKeyExist(KeyType key);											// �ж�ĳ�� key ��Ԫ���Ƿ����
	void Clear(void);								// �������Ԫ�أ��ض��� mcIniMemSize ���洢�ռ�
	
	void StartTraversal();							// ��ʼһ����������
	DataType NextItem(bool &bRetNotValid);			// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�أ�ֱ�� bRetNotValid ���� true
	DataLongType NextItemLong(bool &bRetNotValid);	// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�صĸ������ݣ�ֱ�� bRetNotValid ���� true
	DataLong2Type NextItemLong2(bool &bRetNotValid);
	double NextItemDouble(bool &bRetNotValid);
	LPTSTR NextItemStr(bool &bRetNotValid);
	LPTSTR NextItemStr2(bool &bRetNotValid);

	KeyType NextKey(bool &bRetNotValid);			// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�ص� key��ֱ�� bRetNotValid ���� true

	int Count(void);								// ���ع���Ԫ�ظ���


private:
	int AlloMemIndex(KeyType key, bool CanExpandMem=true );			// ���� Key ����һ�� mem[] �е�δ�ô洢�ռ䣬���� mem[] �����±�
	int FindMemIndex(KeyType key) const;							// ���� Key ���� mem[] ��Ԫ�أ����� mem[] �����±�
	int FindSeqIdx(KeyType key, int fromIndex, int toIndex);		// �� mem[] �м�Ϊ key ��Ԫ���±꣬�����ҿռ��±�Ϊ�� fromIndex ��ʼ���� toIndex �����Ŀռ�
	void ReLocaMem(int preMemCountTo);								// ���·��� mem[], mem2[] �ĸ�Ԫ�صĵ�ַ��mem2[] ��ĳЩԪ�ؿ��ܱ������ƶ��� mem[]
	void ExpandMem(void);											// �ض��� mem[] �����С������ mem[] �Ŀռ�
	int TraversalGetNextIdx(void);									// �� NextXXX ��������ʱ��������һ����Next���� mem[]�±꣨����ֵ>0������ mem2[] ���±꣨����ֵ<0�������ѱ�������������ֵ=0��
	int AlloSeqIdx(int fromIndex, int toIndex);						// �� mem[] ��һ��ûʹ�õĿռ䣬�����ҿռ��±�Ϊ�� fromIndex ��ʼ���� toIndex �����Ŀռ�
	bool RefreshArrTable();											// ������ϣ���������±���� mArrTable[]������ mArrTableCount Ϊ���ݸ��������سɹ���ʧ�ܣ�

	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);	// �ض��� һ�� MemType ���͵����飨������� lMem[] �� lMem2[]���Ĵ�С���¶���ռ��Զ�����

	int GetMemIndexFromKey(KeyType key, bool raiseErrorIfNotHas=true);	// �� Key ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0
	int GetMemIndexFromIndex(int index, bool raiseErrorIfNotHas=true);	// �� index ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0

	void SaveItemString(TCHAR ** ptrSaveTo, LPCTSTR ptrNewString);			// �� new �������ַ����ռ䣬�� key ָ����ַ����������¿ռ䣻ptrSaveTo ��һ�������ַ�����ַ��ָ������ĵ�ַ����ָ���ָ����������桰�� new ���ٵ����ַ����ռ�ĵ�ַ�������� ��*ptrSaveTo = �¿ռ��ַ��
};






// ===============================================================
// CBHashStrK���ַ����� key �Ĺ�ϣ����
// ֧��ÿ��Ԫ������һ�� long �͵����ݺ�һ�� long �͵ĸ�������
//
// �����׳����쳣��
// throw (unsigned char)7;	// �����ڴ�
// throw (unsigned char)5;	// ��Ч�Ĺ��̵��û��������ֵ��������ͼ����Ѵ��ڵ�ͬ����ֵ����Ԫ�أ����ʼ������ڵ�Ԫ��
// throw (unsigned char)9;	// �±�Խ�磺�޷����������ݿռ�
//
// ---- ����ʹ�� Index �ķ�ʽ�������й�ϣ��Ԫ�� ----
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.KeyFromIdx(i)
//     
//     for (i = 1; i<=hash.Count; i++)
//         cout<<hash.ItemFromIdx(i)
//     
// ע�� Index ���������ݶ�Ӧ��������ɾ�����ţ����ݵ� Index �����ܻ�仯
// ����ͬһʱ�̣�Index ��ͬ��һ�����ݣ�Key,Data,DataLong,DataString����ͬһ��
// ===============================================================

class CBHashStrK
{
private:
	typedef struct _MemType
	{
		LPTSTR Key;			// ָ���������� new ���ٵ�һ���ڴ�����ÿ�� mem[] �ڱ�ɾ��ʱ���� delete [] .key
		int Data;
		long DataLong;
		long DataLong2;
		double DataDouble;
		LPTSTR DataStr;
		LPTSTR DataStr2;
		bool Used;
		int Index;		// mArrTable[] ����� mArrTable[index] Ԫ�أ��Ǳ��汾 MemType ����
						//   ���ڵ� mem[] �е��±꣨index>0������ mem2[] �е��±꣨index<0��
						//   mArrTableCount == memUsedCount + memUsedCount2 ʱ�� index !=0 ʱ ��Ч
						//   �� RefreshArrTable �����ô˳�Ա

	} MemType;
	
	static const int	mcIniMemSize;  			// ��ʼ mem[] �Ĵ�С
	static const int	mcMaxItemCount; 		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
	static const float	mcExpandMaxPort;  		// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
	static const int	mcExpandCountThres; 	// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
	static const int	mcExpandCountThresMax;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandMem2Per; 		// ÿ������ mem2[] �Ĵ�С
	static const int	mcSeqMax; 				// ˳��������ֵ

private:
	MemType * mem;					// ��̬����ָ�룬�����鲻ʹ�� [0] ��Ԫ��
	int memCount, memUsedCount;		// ��̬��������±꣬mem[] �����±�Ϊ [0] �� [memCount]����ϣ������Ԫ�ظ���
	MemType * mem2;					// �ռ��ͻ��Ԫ�صı���ռ䣬˳��ʹ��
	int memCount2, memUsedCount2;	// mem2[] �����±�Ϊ [0] �� [memCount2]������ [0] ���ã���ʹ�ÿռ�Ϊ [1] �� [memUsedCount2]
	int mTravIdxCurr;				// �� NextXXX ���������ĵ�ǰ index����ֵ��ʾ mem[] �е��±꣬��ֵ��ʾ mem2[] �е��±�
	
	// ֧��ͨ�������±� Index ����һ����ϣ�����ݣ�mArrTable ָ��̬���飬
	//   ����Ԫ�ر��棺���й�ϣ������������˳�����ڵ� mem[] �е��±꣨>0��
	//   �� mem2[] �е��±ꡣ
	// ����һ�Σ������й�ϣ������ mem[] �� mem2[] ���±���ڴ����飬
	//   �Ժ����ظ�������ֱ��ͨ�������±� Index ����һ����ϣ�����ݡ�
	//   mArrTableCount != memUsedCount + memUsedCount2 Ϊ��־��
	//   �� !=����־Ҫ����ˢ�±�����ע�����±�����Ԫ��˳����ܻ����š�
	//   ����ϣ�����ݵ� Index ������һֱ�����
	int * mArrTable;
	int mArrTableCount;

public:
	bool KeyCaseSensitive;			// �Ƿ� key Ҫ���ִ�Сд��Ĭ�ϲ��ִ�Сд��

public:
	CBHashStrK(int memSize=0);		// memSize=0 �򿪱ٳ�ʼ mcIniMemSize ���ռ䣬���򿪱� memSize ���ռ䣬memSize Ӧ��ʵ�����ݸ�����һЩ
	~CBHashStrK();
	void AlloMem(int memSize);		// ���ȿ��ô˺��������㹻��Ŀռ䣬�Լ����Ժ��Զ�����ռ�Ĵ��������Ч��
	bool Add(int data, LPCTSTR key=0, long dataLong=0, long dataLong2=0, LPCTSTR dataStr=NULL, LPCTSTR dataStr2=NULL, double dataDouble=0.0, bool raiseErrorIfNotHas=true);	// ���Ԫ��
	bool Remove(LPCTSTR key, bool raiseErrorIfNotHas=true);					// ɾ��Ԫ��
	
	// ���� key ���Ԫ�ء���������
	int Item(LPCTSTR key, bool raiseErrorIfNotHas=true);
	long ItemLong(LPCTSTR key, bool raiseErrorIfNotHas=true);
	long ItemLong2(LPCTSTR key, bool raiseErrorIfNotHas=true);
	double ItemDouble(LPCTSTR key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr(LPCTSTR key, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2(LPCTSTR key, bool raiseErrorIfNotHas=true);

	// ���� key ����Ԫ�ء���������
	bool ItemSet(LPCTSTR key, int vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLongSet(LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2Set(LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleSet(LPCTSTR key, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrSet(LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2Set(LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);


	// ���� index ���Ԫ�ء���������
	int ItemFromIndex(int index, bool raiseErrorIfNotHas=true);	
	long ItemLongFromIndex(int index, bool raiseErrorIfNotHas=true);
	long ItemLong2FromIndex(int index, bool raiseErrorIfNotHas=true);
	double ItemDoubleFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStrFromIndex(int index, bool raiseErrorIfNotHas=true);
	LPTSTR ItemStr2FromIndex(int index, bool raiseErrorIfNotHas=true);

	// ���� index ����Ԫ�ء��������ݣ����������� Key��KeyΪֻ����
	bool ItemFromIndexSet(int index, int vNewValue, bool raiseErrorIfNotHas=true);	
	bool ItemLongFromIndexSet(int index, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemLong2FromIndexSet(int index, long vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemDoubleFromIndexSet(int index, double vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStrFromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);
	bool ItemStr2FromIndexSet(int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas=true);

	LPTSTR IndexToKey(int index, bool raiseErrorIfNotHas=true);
	int KeyToIndex(LPCTSTR key, bool raiseErrorIfNotHas=true);


	bool IsKeyExist(LPCTSTR key);											// �ж�ĳ�� key ��Ԫ���Ƿ����
	void Clear(void);								// �������Ԫ�أ��ض��� mcIniMemSize ���洢�ռ�
	
	void StartTraversal();							// ��ʼһ����������
	int NextItem(bool &bRetNotValid);			// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�أ�ֱ�� bRetNotValid ���� true
	long NextItemLong(bool &bRetNotValid);	// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�صĸ������ݣ�ֱ�� bRetNotValid ���� true
	long NextItemLong2(bool &bRetNotValid);
	double NextItemDouble(bool &bRetNotValid);
	LPTSTR NextItemStr(bool &bRetNotValid);
	LPTSTR NextItemStr2(bool &bRetNotValid);

	LPTSTR NextKey(bool &bRetNotValid);			// �������̿�ʼ�󣬲��ϵ��ô˺��������ÿ��Ԫ�ص� key��ֱ�� bRetNotValid ���� true

	int Count(void);								// ���ع���Ԫ�ظ���


private:
	int AlloMemIndex(LPCTSTR Key, bool CanExpandMem=true );		// ���� Key ����һ�� mem[] �е�δ�ô洢�ռ䣬���� mem[] �����±�
	int FindSeqIdx(LPCTSTR key, int fromIndex, int toIndex);	// �� mem[] �м�Ϊ key ��Ԫ���±꣬�����ҿռ��±�Ϊ�� fromIndex ��ʼ���� toIndex �����Ŀռ�
	void ReLocaMem(int preMemCountTo);							// ���·��� mem[], mem2[] �ĸ�Ԫ�صĵ�ַ��mem2[] ��ĳЩԪ�ؿ��ܱ������ƶ��� mem[]
	void ExpandMem(void);										// �ض��� mem[] �����С������ mem[] �Ŀռ�
	int TraversalGetNextIdx(void);								// �� NextXXX ��������ʱ��������һ����Next���� mem[]�±꣨����ֵ>0������ mem2[] ���±꣨����ֵ<0�������ѱ�������������ֵ=0��
	int AlloSeqIdx(int fromIndex, int toIndex);					// �� mem[] ��һ��ûʹ�õĿռ䣬�����ҿռ��±�Ϊ�� fromIndex ��ʼ���� toIndex �����Ŀռ�
	bool RefreshArrTable();										// ������ϣ���������±���� mArrTable[]������ mArrTableCount Ϊ���ݸ��������سɹ���ʧ�ܣ�

	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);	// �ض��� һ�� MemType ���͵����飨������� lMem[] �� lMem2[]���Ĵ�С���¶���ռ��Զ�����

	int GetMemIndexFromKey(LPCTSTR key, bool raiseErrorIfNotHas=true);	// �� Key ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0
	int GetMemIndexFromIndex(int index, bool raiseErrorIfNotHas=true);	// �� index ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0

	long KeyStringToLong(LPCTSTR key);		// ��һ�� �ַ������͵� key ת��Ϊһ��������
	void SaveItemString(TCHAR ** ptrSaveTo, LPCTSTR ptrNewString);			// �� new �������ַ����ռ䣬�� key ָ����ַ����������¿ռ䣻ptrSaveTo ��һ�������ַ�����ַ��ָ������ĵ�ַ����ָ���ָ����������桰�� new ���ٵ����ַ����ռ�ĵ�ַ�������� ��*ptrSaveTo = �¿ռ��ַ��
	bool CompareKey(LPCTSTR key1, LPCTSTR key2);		// ���� KeyCaseSensitive ���ԣ��Ƚ��� key ���ַ����Ƿ���ȣ���ȷ���True�����ȷ���False
};




//////////////////////////////////////////////////////////////////////
// CBRecycledArr��������վ��������
//
// ����֯һ�����ݣ��������Ϳ�ͬʱΪ��
//    һ���ַ���������������һ������������һ��˫������
// ������ Add��Remove���Զ�ά�� index Ϊ 1��Count ��Χ���Ա�ͨ������
//   �ķ�ʽ���ʸ�Ԫ�أ�Ҳ֧��ͨ�������ķ������ʸ�Ԫ��
//////////////////////////////////////////////////////////////////////


// ���� throw �Ĵ���
// throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
class CBRecycledArr  
{
private:
	typedef struct _MemType
	{
		LPTSTR DataString;	// �ַ�������
		int DataInt;		// ������������1
		int DataInt2;		// ������������2
		float DataFloat;	// ���ӵ���������
		double DataDouble;	// ����˫��������
		bool Used;
	} MemType;
	
	static const int	mcIniMemSize;  			// ��ʼ mem[] �Ĵ�С
	static const int	mcMaxItemCount; 		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
	static const float	mcExpandMaxPort;  		// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
	static const int	mcExpandCountThres; 	// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
	static const int	mcExpandCountThresMax;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcRecyExpandPer;		// ���� recycles[] �ռ�ʱ��ÿ������Ĵ�С
	
private:
	MemType * mem;					// ��̬����ָ�룬�����鲻ʹ�� [0] ��Ԫ��
	int memCount, memUsedCount;		// ��̬��������±꣬mem[] �����±�Ϊ [0] �� [memCount]������[0] �� [memUsedCount]�����п�����ɾ����Ԫ�أ�ɾ����Ԫ���±��� recycles �б��棩
	int * recycles;					// ָ��һ���������飬�����Ԫ�ر��� mem[] ����ɾ����Ԫ�ص��±ꣻ�����鱻ά����С��������
	// recycles[] �����±�Ϊ [0] �� [recyclesCount]������ [0] ���ã��� [1] - [recyUsedCount]������Ϊ���ÿ���ռ�
	int recyCount, recyUsedCount;	// recycles[] ����Ԫ���±����ֵ
	int mTravIdxCurr;				// �� NextXXX ���������ĵ�ǰ index
	
public:
	CBRecycledArr(int initSize = 0);	// initSize = 0 ʱ����ʼ���� mcIniMemSize ��С�Ŀռ䣬���򿪱� initSize ��С�Ŀռ�
	~CBRecycledArr(); 
	
	int Add(LPCTSTR dataString = 0, int dataInt = 0, int dataInt2 = 0, float dataFloat = 0.0, double dataDouble = 0.0);  // ��������ݣ����������ݱ����浽�� mem[] �е��±꣨>0����������0
	bool Remove(int index);				// ɾ��һ�����ݣ�index ΪҪɾ�����ݵ��±ꣻɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
	LPTSTR Item(int index, bool bRaiseErrIfNotHas=false);		// �����±� index������һ���ַ������ݣ�ɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count���ַ������ڴ�ռ��ɱ����Զ������������򲻱ظ�Ԥ�������������޸��˷���ָ���ָ�����ݣ���������ڲ�����Ҳͬʱ�����仯
	int ItemInt(int index, bool bRaiseErrIfNotHas=false);		// �����±� index������һ�����͸������ݣ�ɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
	int ItemInt2(int index, bool bRaiseErrIfNotHas=false);		// �����±� index������һ�����͸�������2��ɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
	float ItemFloat(int index, bool bRaiseErrIfNotHas=false);	// �����±� index������һ�� float �͸������ݣ�ɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
	double ItemDouble(int index, bool bRaiseErrIfNotHas=false);	// �����±� index������һ�� double �͸������ݣ�ɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
	int Count();						// ���ص�ǰӵ�е����ݸ���
	void Clear();						// ɾ����������
	
	void StartTraversal();				// ��ʼһ�α���
	LPTSTR NextItem( bool &bRetNotValid );			// ��������һ�����ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int NextDataInt( bool &bRetNotValid );			// ��������һ�������������ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int NextDataInt2( bool &bRetNotValid );			// ��������һ�������������ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	float NextDataFloat( bool &bRetNotValid );		// ��������һ������ float �����ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	double NextDataDouble( bool &bRetNotValid );	// ��������һ�������� double ���ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	
private:
	void ExpandMem(void);
	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);		// �ض��� һ�� MemType ���͵����飨������� mem[]���Ĵ�С���¶���ռ��Զ�����
	int RedimArrInt( int * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false );				// �ض��� һ�� int ���͵����飨������� recycles���Ĵ�С���¶���ռ��Զ�����
	int FindPosInSortedRecycle(int itemToFind);		// �ö��ֲ��ҷ������� recycles �в���Ԫ�� itemToFind ��λ��
	int UserIndexToMemIndex(int index);				// �����û� index������� mem[] �е��±ꣻɾ�����ݺ󣬺������ݵ��û� index ���Զ�������ʹ�û� index ��Ϊ 1��Count
};




//////////////////////////////////////////////////////////////////////
// CBRecycledArrInt�����Ͱ������վ��������
//
// ����֯һ�����ݣ��������Ϳ�ͬʱΪ��
//    ����������һ������������һ��˫������
// ������ Add��Remove���Զ�ά�� index Ϊ 1��Count ��Χ���Ա�ͨ������
//   �ķ�ʽ���ʸ�Ԫ�أ�Ҳ֧��ͨ�������ķ������ʸ�Ԫ��
//////////////////////////////////////////////////////////////////////

// ���� throw �Ĵ���
// throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
class CBRecycledArrInt  
{
private:
	typedef int DataType;			// ���ݵ�����
	typedef int DataIntType;		// �����������ݵ�����
	typedef float DataFloatType;	// ���ӵ��������ݵ�����
	typedef double DataDoubleType;	// ����˫�������ݵ�����
	
	typedef struct _MemType
	{
		DataType Data;
		DataIntType DataInt;
		DataFloatType DataFloat;
		DataDoubleType DataDouble;
		bool Used;
	} MemType;
	
	static const int	mcIniMemSize;  			// ��ʼ mem[] �Ĵ�С
	static const int	mcMaxItemCount; 		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
	static const float	mcExpandMaxPort;  		// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
	static const int	mcExpandCountThres; 	// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
	static const int	mcExpandCountThresMax;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcExpandBigPer; 		// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
	static const int	mcRecyExpandPer;		// ���� recycles[] �ռ�ʱ��ÿ������Ĵ�С
	
private:
	MemType * mem;					// ��̬����ָ�룬�����鲻ʹ�� [0] ��Ԫ��
	int memCount, memUsedCount;		// ��̬��������±꣬mem[] �����±�Ϊ [0] �� [memCount]������[0] �� [memUsedCount]�����п�����ɾ����Ԫ�أ�ɾ����Ԫ���±��� recycles �б��棩
	int * recycles;					// ָ��һ���������飬�����Ԫ�ر��� mem[] ����ɾ����Ԫ�ص��±�
	// recycles[] �����±�Ϊ [0] �� [recyclesCount]������ [0] ���ã��� [1] - [recyUsedCount]������Ϊ���ÿ���ռ�
	int recyCount, recyUsedCount;	// recycles[] ����Ԫ���±����ֵ
	int mTravIdxCurr;				// �� NextXXX ���������ĵ�ǰ index
	
public:
	CBRecycledArrInt(int initSize = 0);	// initSize = 0 ʱ����ʼ���� mcIniMemSize ��С�Ŀռ䣬���򿪱� initSize ��С�Ŀռ�
	~CBRecycledArrInt(); 
	
	int Add(DataType data, DataIntType dataInt = 0, DataFloatType dataFloat = 0.0, DataDoubleType dataDouble = 0.0);  // ��������ݣ����������ݱ����浽�� mem[] �е��±꣨>0����������0
	bool Remove(int index);				// ɾ��һ�����ݣ�index ΪҪɾ�����ݵ��±ꡣ
	DataType Item(int index, bool bRaiseErrIfNotHas=false);				// �����±� index������һ������
	DataIntType ItemInt(int index, bool bRaiseErrIfNotHas=false);		// �����±� index������һ�����͸�������
	DataFloatType ItemFloat(int index, bool bRaiseErrIfNotHas=false);	// �����±� index������һ�� float �͸�������
	DataDoubleType ItemDouble(int index, bool bRaiseErrIfNotHas=false);	// �����±� index������һ�� double �͸�������
	int Count();						// ���ص�ǰӵ�е����ݸ���
	void Clear();						// ɾ����������
	
	void StartTraversal();				// ��ʼһ�α���
	DataType NextItem( bool &bRetNotValid );				// ��������һ�����ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	DataIntType NextDataInt( bool &bRetNotValid );			// ��������һ�������������ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	DataFloatType NextDataFloat( bool &bRetNotValid );		// ��������һ������ float �����ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	DataDoubleType NextDataDouble( bool &bRetNotValid );	// ��������һ�������� double ���ݣ��� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	
private:
	void ExpandMem(void);
	int RedimArrMemType(MemType * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false);		// �ض��� һ�� MemType ���͵����飨������� mem[]���Ĵ�С���¶���ռ��Զ�����
	int RedimArrInt( int * &arr, int toUBound=-1, int uboundCurrent=-1, bool preserve=false );				// �ض��� һ�� int ���͵����飨������� recycles���Ĵ�С���¶���ռ��Զ�����
	
};




//////////////////////////////////////////////////////////////////////
// CHeapMemory�� ��ȫ�ֶ���ά������ͨ�� new ������ڴ�ָ�룬�ڱ������
//				 ����ʱ���Զ� delete ����Щ�ڴ�
//
//////////////////////////////////////////////////////////////////////

class CBHeapMemory  
{
public:
	CBHeapMemory(int initSize=0);
	virtual ~CBHeapMemory();
	
	// ���һ��Ҫ�ɱ��������ġ�����new����Ŀռ�ĵ�ַ��
	//   �ɹ����ر�����������ĵ�ַ����������������һ����
	// �� bArrayNew=true������ delete ʱ��[]������ delete ʱû��[]
	int AddPtr(void *ptrNew, bool bArrayNew=true);

	// �� new ���� size ���ֽڵĿռ䣬���Զ���0
	//   ���ش˿ռ�ĵ�ַ��������0�������ɱ������Զ���¼�˿ռ�ĵ�ַ
	void * Alloc(int size); 

	// �ͷ� ptr ��ָ���һ���ڴ�ռ�
	//   ptr �������ɱ�����������Ŀռ䣬���򱾺��������ͷ�
	void Free(void *ptr);

	// ���� ptr ��ָ���һ���ڴ�ռ��Ƿ����ɱ��������
	bool IsPtrManaged(void *ptr);

	// ���� ������������Ŀռ����
	int CountPtrs();

	// ���� �������������һ���ռ�ĵ�ַ
	//   index ӦΪ 1��CountPtrs() ֮��
	//   �ɲ��� ptrbArrayNew ���ظÿռ� delete ʱ�Ƿ�Ӧ��[]
	void * PtrEach(int index, bool * ptrbArrayNew=0);

	// ����һ���ڴ�ռ䡣ʵ���ǵ��� memset (������ include <memory.h>) ��
	//   Ϊʹ�������򲻱��� include memory.h������Ҳ�ṩ��������ܽӿ�
	void ZeroMem(void * ptr, unsigned int length);	
	
	// �ڴ濽����ʵ���ǵ��� memcpy (������ include <memory.h>) ��
	//   Ϊʹ�������򲻱��� include memory.h������Ҳ�ṩ��������ܽӿڣ�
	void CopyMem(void * dest, void * source, unsigned int length);	
	
	// ǿ���ͷű���������¼�����е�ַָ��Ŀռ�
	//   ����������ʱҲ���Զ����ñ�����
	void Dispose();									
	
private:

	// �����ɱ��������ġ������� new ��̬����Ŀռ��ַ
	//   key=��ַ��data=��ַ��
	//   dataLong!=0 ʱ��delete ʱ����[]��dataLong=0��delete ʱ��[]
	CBHashLK memHash;
};


// ȫ�ֶ������ HM ����������������������� new ���ٵ��ڴ�ָ�룬�ö���
// ����ʱ���Զ� delete ��Щ�ڴ档�����ı��������� BWindows.cpp �У�����
// �� extern �����˱����������а����� h �ļ���ģ�鶼��ʹ�ô�ȫ�ֱ���

extern CBHeapMemory HM;	
