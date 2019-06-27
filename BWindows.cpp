//////////////////////////////////////////////////////////////////////////
// Bwindows.cpp����д Windows �����õ�һЩ�Զ��庯�������ʵ��
//   �������
//      CBApp �ࣺ ����Ӧ�ó���ȫ����Ϣ
// 		CBHashLK���������� key �Ĺ�ϣ����
// 		CBHashStrK���ַ����� key �Ĺ�ϣ����
// 		CBRecycledArr��������վ��������
// 		CBRecycledArrInt�����Ͱ������վ��������
// 		CHeapMemory�� ��ȫ�ֶ���ά������ͨ�� new ������ڴ�ָ��
//   ͬʱ���� CBHashLKCBHeapMemory ���ȫ�ֶ��� HM �Ķ���
//      CBApp ��ȫ�� pApp ָ��Ķ���
//
//////////////////////////////////////////////////////////////////////////

#include "Bwindows.h"
#include <tchar.h>
#include <stdio.h>	// ʹ�� _vsntprintf ��
#include <memory.h>


// ȫ�� App �����ָ����ָ��Ķ������ڻ�ñ������ĳЩ������Ϣ��
//   �� hInstance �ȡ�����ͬʱ���� BForm ģ�����ʹ���������ȷ��Ϣ
// ����ָ���������Ϊ��������ָ����ָ��������Բ��ܱ��ı�
// ��ȫ�� App ����Ĺ��캯���У��ó�Ա��ʼ��������еĳ���Ա��ֵ
CBApp *pApp=0;	

// ȫ�ֶ���HM�Ķ���
// ��������������� new ���ٵ��ڴ�ָ�룬�ö�������ʱ���Զ� delete ��Щ�ڴ�
//   Bwindows.h ���д�ȫ�ֶ�����������κ�ģ����� Bwindows.h ����ʹ�ô�ȫ�ֶ���
CBHeapMemory HM; 

// ���ÿռ乩�ַ��������ݴ�ʹ�ã�
//��gEmptyTSTR���ڼ���Unicode��Ansi��gEmptySTR������Ansi��gEmptyWSTR ������ Unicode��
// ����ʱ����ָ��Ϊ0�����ݴ��ؿ��ַ������ͷ��ش˿ռ������
// �˿ռ��� BWindows.h ��δ������������ģ�鲻��ʹ��
TCHAR gEmptyTSTR[1];
char gEmptySTR[1];
WCHAR gEmptyWSTR[1];

//////////////////////////////////////////////////////////////////////////
// �����Զ��庯��
//
//////////////////////////////////////////////////////////////////////////


// ������Ϣ��
//   �� title Ϊ NULL�����Զ�ʹ��Ӧ�ó�������Ϊ title
EDlgBoxCmdID MsgBox(LPCTSTR szPrompt, 
					LPCTSTR szTitle,	
					EMsgBoxBtn buttons, 
					EMsgBoxIcon icon, 
					EDlgBoxDefBtn defBtn, 
					bool bSystemModal, 
					bool bHelpButton, 
					bool bRightJustified, 
					bool bRightToLeftReading)
{
	UINT uType;
	if (szTitle == NULL) 
	{
		// ��� title Ϊ NULL���ͻ��Ӧ�ó�������Ϊ title
		TCHAR fName[2048] = {0};
		TCHAR * slash, * dot;

		// ���Ӧ�ó�����
		if (GetModuleFileName(0, fName, sizeof(fName)) == 0)
		{
			// ���Ӧ�ó�����ʧ��
			szTitle=TEXT("Message");		// ����ʹ�� "Message"
		}
		else	// if (GetModuleFileName(0, fName, 2048) == 0)
		{
			// ���Ӧ�ó������ɹ�
			szTitle = fName;	// ������Ϊ����Ӧ�ó���������������ȡ�����ļ�������
								//   �����ȡ�ļ�������ʧ�ܣ���ʹ��������Ӧ�ó�����
			// �������һ�� '\\'
			slash=fName;
			while(*slash) slash++;
			while(*slash != TCHAR('\\') && slash>=fName) slash--;
			if (slash>=fName)
			{
				// ��ȡ�� slash + 1 ��ʼ�����һ�� '.' ֮ǰ�Ĳ���Ϊ�ļ�����
				szTitle = slash + 1;	// �����ý�ȡ�� '\\' ��ĩβ�Ĳ��֣�
										//   �����ٲ��� '.'��������� '.' ʧ�ܣ�
										//    ��ʹ�ô� '\\' ��ĩβ�Ĳ���

				// ���� slash + 1 ������һ�� '.'
				dot = slash+1;
				while(*dot) dot++;
				while(*dot != TCHAR('.') && dot>slash+1) dot--;
				if (*dot == TCHAR('.')) * dot = '\0';		// ���� '.' �ɹ����� '.' ��λ��
															//   ��Ϊ '\0' ʹ�ض��ַ���
			}	// end if (slash)
		}	// end if (GetModuleFileName(0, fName, 2048) == 0)
	}	// end if (title == NULL) 
	
	uType = buttons + icon + defBtn;
	uType += MB_TASKMODAL;				// hWnd == 0 ʱ����� MB_TASKMODAL 
										//	��ʹ��ǰ�߳������Ķ��㴰�ڶ�������
	if (bSystemModal) 
	{
		// hWnd == 0 ʱ���� MB_TASKMODAL�������� MB_SYSTEMMODAL
		uType += MB_TOPMOST;
	}
	if (bHelpButton) uType += MB_HELP;
	if (bRightJustified) uType += MB_RIGHT;
	if (bRightToLeftReading) uType += MB_RTLREADING;
	return (EDlgBoxCmdID)MessageBox(0, szPrompt, szTitle, uType);
}

// ��һ������ת��Ϊ�˽��Ƶ��ַ������ַ����ռ��Զ�����
LPTSTR Oct(long number)
{
	return StrPrintf(TEXT("%o"), number);
}

// ��һ������ת��Ϊʮ�����Ƶ��ַ������ַ����ռ��Զ�����
LPTSTR Hex(long number)
{
	return StrPrintf(TEXT("%X"), number);
}

LPTSTR CurDir()
{
	LPTSTR curDirBuff = new TCHAR[1024];
	HM.AddPtr(curDirBuff);
	GetCurrentDirectory(1024, curDirBuff);
	return curDirBuff;
}


unsigned char * LoadResData( UINT idRes, UINT typeRes, unsigned long * rSize/*=0*/ )
{
	return LoadResData(MAKEINTRESOURCE(idRes), MAKEINTRESOURCE(typeRes), rSize); 
}


unsigned char * LoadResData( UINT idRes, LPCTSTR typeRes, unsigned long * rSize/*=0*/ )
{
	return LoadResData(MAKEINTRESOURCE(idRes), typeRes, rSize); 
}

unsigned char * LoadResData( LPCTSTR idRes, UINT typeRes, unsigned long * rSize/*=0*/ )
{
	return LoadResData(idRes, MAKEINTRESOURCE(typeRes), rSize); 
}

unsigned char * LoadResData( LPCTSTR idRes, LPCTSTR typeRes, unsigned long * rSize/*=0*/ )
{
	HRSRC hRes = FindResource(0, idRes, typeRes);
	if (rSize) *rSize = SizeofResource(0, hRes);
	HGLOBAL hGlob = LoadResource(0, hRes);
	return (unsigned char *)LockResource(hGlob); 
}

//////////////////////////////////////////////////////////////////////////
// ʱ�� ����
//////////////////////////////////////////////////////////////////////////

// ���ص�ǰϵͳ���ڡ�ʱ���һ���ַ���
//   �� lpDblTime ��Ϊ0������������ǰϵͳ���ڡ�ʱ��ת��Ϊ double 
//     ��Ϊ1601-1-1���������ĺ�������������ָ��� double �ͱ�����
//   �� lpTime ��Ϊ0��������ǰϵͳ���ڡ�ʱ��洢����ָ��Ľṹ��
LPTSTR Now( double *lpDblTime/*=0*/, SYSTEMTIME *lpTime/*=0*/ )
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (lpTime) *lpTime = st;
	if (lpDblTime) *lpDblTime = DateTimeDbl(st);

	return StrPrintf(TEXT("%d-%d-%d %02d:%02d:%02d"),	// %02d������2λ������ǰ��0���Ǽӿո���2λ
		st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond);
}

// ���õ�ǰϵͳ���ڡ�ʱ��
BOOL NowSet( SYSTEMTIME stNow )
{
	return SetLocalTime (&stNow);
}


// ��һ�����ڡ�ʱ��ת��Ϊ double ���أ�Ϊ1601-1-1���������ĺ�������
double DateTimeDbl( SYSTEMTIME stDatetime )
{
	FILETIME ft;
	SystemTimeToFileTime(&stDatetime, &ft);
	return ((double)ft.dwLowDateTime  
		+ 4294967296.0 * ft.dwHighDateTime)/1e4;
}


// �����������ڡ�ʱ���ʱ����
double DateDiff( eDataTimeDiffStyle style, 
				 SYSTEMTIME stDatetime1, 
				 SYSTEMTIME stDatetime2 )
{
	double diff=DateTimeDbl(stDatetime2)-DateTimeDbl(stDatetime1);
	switch (style)
	{
	case edtYearDiff:
		diff /= 365;	// ��������ִ������ case �����
	case edtMonthDiff:
		diff /= 12;		// ��������ִ������ case �����
	case edtDayDiff:
		diff /= 24;		// ��������ִ������ case �����
	case edtHourDiff:
		diff /= 60;		// ��������ִ������ case �����
	case edtMinuteDiff:
		diff /= 60;		// ��������ִ������ case �����
	case edtSecondDiff:
		diff /= 1000;	// ��������ִ������ case �����
	case edtMilliseconds:
		break;
	}
	return diff;
}




//////////////////////////////////////////////////////////////////////////
// �Զ����ַ��� ����
//////////////////////////////////////////////////////////////////////////


// �� printf ��ʽ����һ���ַ������ַ����ռ��ɱ�ģ���Զ����١��Զ�����
LPTSTR cdecl StrPrintf( LPCTSTR szFormat, ... )
{
	if (szFormat==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	// ���ٱ������ַ����Ŀռ� lpszResult
	TCHAR * lpszResult = new TCHAR [2048];
	HM.AddPtr(lpszResult);			// �����·�����ڴ�ָ�뱣�浽 HM ���Զ�����
	
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf(lpszResult, 2047, szFormat, pArgList);
	va_end(pArgList);
	
	return lpszResult;
}

// ȡ�ַ�����ǰ length ���ַ�������ַ�����ʹ��Unicodeʱ1��������1�����ȣ�
//   ʹ��ANSIʱ1��������2�����ȣ�
// ��������̬���ٱ������ַ����Ŀռ䣬���Զ�ά���˿ռ䣨��ȫ�� HM ����
//   �����������ַ������׵�ַ
// length�����ַ�������ʱ���������ַ�����length<=0 ʱ���� ָ�� '\0' ���մ�����ָ��
LPTSTR Left(LPCTSTR szStringSrc, int length)  // LPCTSTR ���� const TCHAR *
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	int lenSrc = lstrlen(szStringSrc);
	
	// ��Ҫ��ȡ�Ĳ��ֳ��� length �޶��� 0���ַ�����󳤶�
	if (length < 1) length = 0;
	if (length > lenSrc) length = lenSrc;

	// ���ٱ������ַ����Ŀռ� lpszResult
	TCHAR * lpszResult = new TCHAR [length+1];
	HM.AddPtr(lpszResult);			// �����·�����ڴ�ָ�뱣�浽 HM ���Զ�����

	// ��������
	_tcsncpy(lpszResult, szStringSrc, length);
	*(lpszResult+length)='\0';		// ����ַ��������� '\0'

	return lpszResult;
}



// ȡ�ַ����ĺ� length ���ַ�������ַ�����ʹ��Unicodeʱ1��������1�����ȣ�
//   ʹ��ANSIʱ1��������2�����ȣ�
// ��������̬���ٱ������ַ����Ŀռ䣬���Զ�ά���˿ռ䣨��ȫ�� HM ����
//   �����������ַ������׵�ַ
// length�����ַ�������ʱ���������ַ�����length<=0 ʱ���� ָ�� '\0' ���մ�����ָ��
LPTSTR Right(LPCTSTR szStringSrc, int length)
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	int lenSrc = lstrlen(szStringSrc);
	
	// ��Ҫ��ȡ�Ĳ��ֳ��� length �޶��� 0���ַ�����󳤶�
	if (length < 1) length = 0;
	if (length > lenSrc) length = lenSrc;
	
	// ���ٱ������ַ����Ŀռ� lpszResult
	TCHAR * lpszResult = new TCHAR [length+1];
	HM.AddPtr(lpszResult);	// �����·�����ڴ�ָ�뱣�浽 HM ���Զ�����
	
	// ���ɽ���ַ�������
	lstrcpy(lpszResult, szStringSrc+lenSrc-length);
	return lpszResult;
}



// ȡ�ַ����Ĵӵ� startPos ���ַ��𣬳� length ���ַ���ɵ��ַ�������һ���ַ�λ��Ϊ1��
//   ʹ��Unicodeʱ1��������1�����ȣ�ʹ��ANSIʱ1��������2�����ȣ�
// ��������̬���ٱ������ַ����Ŀռ䣬���Զ�ά���˿ռ䣨��ȫ�� HM ����
//   �����������ַ������׵�ַ
// startPos+length-1 �����ַ�������ʱ���������ַ�����length<=0 ʱ
//	 �� startPos<=0 �� startPos>Դ�ַ������� ʱ����ָ�� '\0' ���մ�����ָ��
LPTSTR Mid(LPCTSTR szStringSrc, int startPos, int length)
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	int lenSrc = lstrlen(szStringSrc);
	
	// ��Ҫ��ȡ����ʼλ�� startPos �޶��� 1���ַ�����󳤶�+1��
	//	 �� startPos Ϊ���ַ�����󳤶�+1��ʱ����ȡ����Ϊ ""
	if (startPos < 1 || startPos>lenSrc) startPos = lenSrc+1;
	
	// ��Ҫ��ȡ�Ĳ��ִ� startPos ��ʼ������Ϊ length �޶��� 
	//	�ַ������ȷ�Χ֮�ڣ����� length��
	if (length < 1) length = 0;
	if (startPos+length-1 > lenSrc) length = lenSrc-startPos+1;
	
	// ���ٱ������ַ����Ŀռ� lpszResult
	TCHAR * lpszResult = new TCHAR [length+1];
	HM.AddPtr(lpszResult);			// ���·�����ڴ�ָ�뱣���� HM
	
	// ��������
	_tcsncpy(lpszResult, szStringSrc+startPos-1, length);
	*(lpszResult+length)='\0';		// ����ַ��������� '\0'
	
	return lpszResult;
}


LPTSTR LTrim( LPCTSTR szStringSrc, bool bDelOtherSpace/*=false*/ )
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�
	
	// ���ٱ������ַ����Ŀռ� lpszResult����������
	TCHAR * lpszResult = new TCHAR [lstrlen(szStringSrc)+1];
	HM.AddPtr(lpszResult);			// ���·�����ڴ�ָ�뱣���� HM
	
	// �����ַ�����ִ��ɾ��
	TCHAR *p=(TCHAR *)szStringSrc;
	while (*p && (*p == TEXT(' ') || (bDelOtherSpace && _istspace(*p))) ) p++;	// ָ��Դ�ַ����ĵ�һ���ǿո�
	_tcscpy(lpszResult, p);	// �� p ��λ�ÿ����ַ���

	// �������ַ���
	return lpszResult;
}

LPTSTR RTrim( LPCTSTR szStringSrc, bool bDelOtherSpace/*=false*/ )
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�
	
	// ���ٱ������ַ����Ŀռ� lpszResult����������
	TCHAR * lpszResult = new TCHAR [lstrlen(szStringSrc)+1];
	HM.AddPtr(lpszResult);			// ���·�����ڴ�ָ�뱣���� HM
	_tcscpy(lpszResult, szStringSrc);
	
	// �����ַ�����ִ��ɾ��
	TCHAR *p=(TCHAR *)lpszResult;
	while (*p) p++; p--;	// ָ�����һ���ַ�
	while (*p==TEXT(' ') || (bDelOtherSpace && _istspace(*p))) p--;
	p++;
	*p=0;

	// �������ַ���
	return lpszResult;

}

LPTSTR Trim( LPCTSTR szStringSrc, bool bDelOtherSpace/*=false*/ )
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�
	
	// ���ٱ������ַ����Ŀռ� lpszResult����������
	TCHAR * lpszResult = new TCHAR [lstrlen(szStringSrc)+1];
	HM.AddPtr(lpszResult);			// ���·�����ڴ�ָ�뱣���� HM
	
	// �����ַ�����ִ��ɾ��ǰ���ո�
	TCHAR *p=(TCHAR *)szStringSrc;
	while (*p && (*p==TEXT(' ') || (bDelOtherSpace && _istspace(*p))) ) p++;	// ָ��Դ�ַ����ĵ�һ���ǿո�
	_tcscpy(lpszResult, p);	// �� p ��λ�ÿ����ַ���
	
	// �����ַ�����ִ��ɾ��β���ո�
	p=(TCHAR *)lpszResult;
	while (*p) p++; p--;	// ָ�����һ���ַ�
	while (*p==TEXT(' ') || (bDelOtherSpace && _istspace(*p))) p--;
	p++;
	*p=0;
	
	// �������ַ���
	return lpszResult;
}


// �� szSrc �У��ӵ� start ���ַ���ʼ����һ���ַ�λ��Ϊ1����
//	  �����ַ��� szFind �ĵ�һ�γ��ֵ�λ�ã���һ���ַ�λ��Ϊ1����
//    �ҵ�����ֵ>0��û�ҵ�����0
//  ˵����������δ�����κο⺯����strlenҲδ���ã������������Ч��
int InStr(int			start, 
		  LPCTSTR		szSrc, 
		  LPCTSTR		szFind, 
	eBStrCompareMethod	compare/*=bcmTextCompare*/)
{
	TCHAR * pSrcCompPos = (TCHAR *)(szSrc + start - 1);	// Դ�ַ����п�ʼ�Ƚϵ�λ�ã���ַ��

	if ( szSrc==0 || szFind==0 ) return 0;
	
	// Ҫ�����ҵ��ַ���Ϊ�մ���szSrc ָ�� '\0'�������� 0
	if (*szSrc == 0) return 0;							
	// Ҫ���ҵ�����Ϊ�մ���szFind ָ�� '\0'�������� start
	if (*szFind == 0) return start;							

	// ͨ���ƶ�ָ�� pSrcComp ��Դ�ַ�����ĩβ��ɨ��Դ�ַ���
	while (*pSrcCompPos)
	{
		// ------------------------------------------------------------------------
		// �Ƚ�Դ�ַ����� pSrcCompPos ��ʼ��һ�����ݣ��Ƿ��� stringFind �ַ�����ͬ
		// ------------------------------------------------------------------------
		TCHAR *p1, *p2;	
		p1 = pSrcCompPos;		// Դ�ַ����� pSrcCompPos ��ʼ���ַ�ɨ��
		p2 = (TCHAR *)szFind;	// �����ַ����� szFind ��ʼ���ַ�ɨ��

		// ��� Դ�ַ��� �� �����ַ��� ��δ��ĩβ���ͼ�������ַ��Ƚ�
		while (*p1 && *p2)
		{
			// ���Ҫ�Ƚϵ������ַ���=> c1��c2
			TCHAR c1= *p1, c2= *p2;		// ͨ���� *p1��*p2 ���ݴ�������������Ժ󷴸����� * ���㣬�������Ч��
			if (compare == bcmTextCompare)
			{
				// �������ִ�Сд�Ƚϣ����ڽ� c1��c2 ͳһΪ��д
				if (c1>='a' && c1<='z') c1-=32;
				if (c2>='a' && c2<='z') c2-=32;
			}

			// ��� c1��c2 ����ȣ��������ڲ� while
			if (c1 != c2) break;

			// �Ƚ���һ���ַ�
			p1++;
			p2++;
		}	// end while (*p1 && *p2)

		// �����ڲ� while ѭ�������������
		//	 (1) �����ַ��� ����ĩβ����ʱ���� Դ�ַ��� �Ƿ񵽴�ĩβ����ʾ�Ѿ��ҵ�
		//	 (2) Դ�ַ��� ����ĩβ���������ַ���û�е���ĩβ����ʾû���ҵ�
		//	 (3) Դ�ַ��� �� Ŀ���ַ��� ��û�е���ĩβ��˵������ break �����ģ���ʾû���ҵ�
		// ֻ���ڵ� (1) �����(�Ѿ��ҵ�)ʱ�������أ��������������������Դ�ַ�������һ��λ�ÿ�ʼ����
		if (*p2 == 0)
		{
			// �� (1) ������������ַ��� ����ĩβ
			// �������أ��������
			return pSrcCompPos - szSrc + 1;
		}


		// ------------------------------------------------------------------------
		// ��Դ�ַ���������ƶ�һ���Ƚ�λ��
		// ------------------------------------------------------------------------
		pSrcCompPos++;
	}	// end while (*pSrcCompPos)

	// û���ҵ�
	return 0;
}


int InStr(LPCTSTR szSrc,								// InStr �����ذ�
		  LPCTSTR szFind,
		  eBStrCompareMethod compare/*=bcmBinaryCompare*/)
{
	return InStr(1, szSrc, szFind, compare);
}

// �� szSrc �У��ӵ� start ���ַ���ʼ����һ���ַ�λ��Ϊ1����ĩβ�Ĳ��֣�
//	  �����ַ��� szFind �ĵ�����һ�γ��ֵ�λ�ã���һ���ַ�λ��Ϊ1����
//    �ҵ�����ֵ>0��û�ҵ�����0
//  ˵����������δ�����κο⺯����strlenҲδ���ã������������Ч��
int InStrRev(LPCTSTR		szSrc, 
			 LPCTSTR		szFind, 
			 int			start/*=1*/,
			 eBStrCompareMethod	compare/*=bcmTextCompare*/)
{
	TCHAR * pSrcCompStartPos = (TCHAR *)(szSrc + start - 1);	// Դ�ַ����п�ʼ�Ƚϵ�λ�ã���ַ��
	TCHAR * pSrcCompPos = pSrcCompStartPos; 
	
	if ( szSrc==0 || szFind==0 ) return 0;
	
	// Ҫ�����ҵ��ַ���Ϊ�մ���szSrc ָ�� '\0'�������� 0
	if (*szSrc == 0) return 0;							
	// Ҫ���ҵ�����Ϊ�մ���szFind ָ�� '\0'�������� start
	if (*szFind == 0) return start;	
	
	// �ƶ�ָ�� pSrcComp �����һ���ַ�
	while (*pSrcCompPos) pSrcCompPos++; pSrcCompPos--;

	// ͨ����ǰ�ƶ�ָ�� pSrcComp ��Դ�ַ����� pSrcCompStartPos��ɨ��Դ�ַ���
	while (pSrcCompPos>=pSrcCompStartPos)
	{
		// ------------------------------------------------------------------------
		// �Ƚ�Դ�ַ����� pSrcCompPos ��ʼ��һ�����ݣ��Ƿ��� stringFind �ַ�����ͬ
		// ------------------------------------------------------------------------
		TCHAR *p1, *p2;	
		p1 = pSrcCompPos;		// Դ�ַ����� pSrcCompPos ��ʼ���ַ�ɨ��
		p2 = (TCHAR *)szFind;	// �����ַ����� szFind ��ʼ���ַ�ɨ��
		
		// ��� Դ�ַ��� �� �����ַ��� ��δ��ĩβ���ͼ�������ַ��Ƚ�
		while (*p1 && *p2)
		{
			// ���Ҫ�Ƚϵ������ַ���=> c1��c2
			TCHAR c1= *p1, c2= *p2;		// ͨ���� *p1��*p2 ���ݴ�������������Ժ󷴸����� * ���㣬�������Ч��
			if (compare == bcmTextCompare)
			{
				// �������ִ�Сд�Ƚϣ����ڽ� c1��c2 ͳһΪ��д
				if (c1>='a' && c1<='z') c1-=32;
				if (c2>='a' && c2<='z') c2-=32;
			}
			
			// ��� c1��c2 ����ȣ��������ڲ� while
			if (c1 != c2) break;
			
			// �Ƚ���һ���ַ�
			p1++;
			p2++;
		}	// end while (*p1 && *p2)
		
		// �����ڲ� while ѭ�������������
		//	 (1) �����ַ��� ����ĩβ����ʱ���� Դ�ַ��� �Ƿ񵽴�ĩβ����ʾ�Ѿ��ҵ�
		//	 (2) Դ�ַ��� ����ĩβ���������ַ���û�е���ĩβ����ʾû���ҵ�
		//	 (3) Դ�ַ��� �� Ŀ���ַ��� ��û�е���ĩβ��˵������ break �����ģ���ʾû���ҵ�
		// ֻ���ڵ� (1) �����(�Ѿ��ҵ�)ʱ�������أ��������������������Դ�ַ�������һ��λ�ÿ�ʼ����
		if (*p2 == 0)
		{
			// �� (1) ������������ַ��� ����ĩβ
			// �������أ��������
			return pSrcCompPos - szSrc + 1;
		}
		
		
		// ------------------------------------------------------------------------
		// ��Դ�ַ�������ǰ�ƶ�һ���Ƚ�λ��
		// ------------------------------------------------------------------------
		pSrcCompPos--;
	}	// end while (*pSrcCompPos)
	
	// û���ҵ�
	return 0;
}

// ���ָ��ַ��� delimiters ���ָ�һ���ַ����������������ַ���
//	 �� ���ַ��� �ĵ�ַ�� ptrStrings[] ���鷵�أ������������ַ����ĸ���
//   ptrStrings[] �����±��1��ʼ����󵽺�������ֵ
// limit ���Ʒ������ַ�������������Ϊ -1 ʱ�����ƣ������������ַ���
//   ���ַ����ڴ漰 ptrStrings[] �����ڴ涼�ɱ�ģ���Զ����䡢�Զ�����
int Split(LPCTSTR		stringSrc, 
		  TCHAR		** &ptrStrings,						// һ���ַ�ָ�����飬����ÿ��Ԫ��Ϊһ�����ַ������׵�ַ�����齫�ڱ������б�����ռ䣬�±� [0]��[��������ֵ]��[0]�ռ䲻��
		  LPCTSTR		delimiters/*=0*/,				// �ָ��ַ���
		  int		    limit/*=-1*/,					// limit, ���Ʒ��ص����ַ����������C1��ʾ�������е����ַ���
	eBStrCompareMethod  compare/*=bcmBinaryCompare*/)	// ���֡��ָ��ַ��������ַ����Ƚ��Ƿ����ִ�Сд
{
	const int cPtrArrExpandPer = 20;	// ptrStrings[] ÿ������Ĵ�С
	void **ptrArr=0;
	int iArrUbound=0, iSubstrCount=0;
	
	if (stringSrc == 0) { ptrStrings = 0; return 0; 	}

	// ����ָ������ĳ�ʼ��С
	Redim(ptrArr, cPtrArrExpandPer);
	iArrUbound = cPtrArrExpandPer;

	// �� stringSrc �ַ����в�����һ�� delimiters
	int pos=1, pos2=0;
	pos2 = InStr(pos, stringSrc, delimiters, compare);
	while (pos2)
	{
		// ���� pos �� pos2 �����ַ��������� ptrArr[]
		iSubstrCount++;
		if (iSubstrCount > iArrUbound)
		{
			// �ض���ָ������ ptrArr �ĳ�ʼ��С
			Redim(ptrArr, iArrUbound+cPtrArrExpandPer, iArrUbound, true);
			iArrUbound = iArrUbound+cPtrArrExpandPer;
		}
		ptrArr[iSubstrCount] = (void *)Mid(stringSrc, pos, pos2-pos);
		if (limit>0 && iSubstrCount >= limit) goto FinishSub;

		// pos ����ƶ���������һ�� delimiters
		pos = pos2 + lstrlen(delimiters);
		pos2 = InStr(pos, stringSrc, delimiters, compare);
	}

	// ���һ����
	pos2 = lstrlen(stringSrc) + 1;
	// ���� pos �� pos2 �����ַ��������� ptrArr[]
	iSubstrCount++;
	if (iSubstrCount > iArrUbound)
	{
		// �ض���ָ������ ptrArr �ĳ�ʼ��С
		Redim(ptrArr, iArrUbound+cPtrArrExpandPer, iArrUbound, true);
		iArrUbound = iArrUbound+cPtrArrExpandPer;
	}
	ptrArr[iSubstrCount] = (void *)Mid(stringSrc, pos, pos2-pos);

FinishSub:
	// �ض���ָ������ ptrArr �������С������ ptrStrings �ķ���ֵ
	if (iSubstrCount)
	{
		Redim(ptrArr, iSubstrCount, iArrUbound, true);
		HM.AddPtr(ptrArr);		// ���·�����ڴ�ָ�뱣���� mHM
		ptrStrings = (TCHAR **)ptrArr;
	}
	else
	{
		// �����ַ���
		Erase(ptrArr);
		ptrStrings = 0;
	}

	// �������ַ����ĸ���
	return iSubstrCount;
}


// �� delimiter ���Ӷ���ַ������������Ӻõ��ַ���
// ����ַ����ĵ�ַ������ stringSrcArray[] ������������
//   �����д��±� arrayIndexStart �� arrayIndexEnd ���ַ���
// delimiter Ϊ 0 ʱ��Ĭ���� "\0" �����ַ������������ַ��� delimiter ����
// bTailDoubleNull ��Ϊ true�����ڽ���ַ���������ټ�һ��'\0'�������������'\0'��
// ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
LPTSTR Join(TCHAR * stringSrcArray[], 
		   const int    arrayIndexEnd, 
		   LPCTSTR delimiter/*=0*/, 
		   const int    arrayIndexStart/*=1*/, 
		   const bool   bTailDoubleNull/*=false*/)
{
	if (stringSrcArray==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	// ��ý���ַ������ܳ��� => lenResult
	int lenResult = 0;
	int lenDelimiter;
	int i;

	if (delimiter)
		lenDelimiter = lstrlen(delimiter);
	else
		lenDelimiter = 1;					// �ַ����м���� '\0'��һ���ַ���

	for(i=arrayIndexStart; i<=arrayIndexEnd; i++)
	{
		if (stringSrcArray[i]==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�
		lenResult += lstrlen(stringSrcArray[i]);
		lenResult += lenDelimiter;
	}
	lenResult -= lenDelimiter;				// ���һ���ַ������治�� delimiter
	if (bTailDoubleNull) lenResult += 1;	// ������Ҫ����'\0'���ټ�һ������

	// �������ַ������ڴ�ռ�
	TCHAR * stringResult = new TCHAR[lenResult + 1];	// +1Ϊ���� '\0' �ռ䣨˫'\0'ʱָ���һ��'\0'��
	HM.AddPtr(stringResult);						// ���·�����ڴ�ָ�뱣���� HM

	// �����ַ���
	TCHAR *p = stringResult;
	for(i=arrayIndexStart; i<=arrayIndexEnd; i++)
	{
		// ���� stringSrcArray[i]
		lstrcpy(p, stringSrcArray[i]);
		p += lstrlen(stringSrcArray[i]);

		// ���� �����ַ���
		if (i<arrayIndexEnd)	// �������һ���ַ��������� delimiter
		{
			if (delimiter)
			{
				// ���ַ��� delimiter ��Ϊ�����ַ�
				lstrcpy(p, delimiter);
				p += lstrlen(delimiter);
			}
			else
			{
				// �� '\0' ��Ϊ�����ַ�
				*p = '\0';
				p++;
			}
		}
	}

	// ��ӽ�β�� '\0'
	if (bTailDoubleNull) {*p = '\0'; p++;}
	*p = '\0';

	// ���ؽ���ַ���ָ��
	return stringResult;
}




// �����ַ������������Ӻ�ĳ��ַ���
// �������Ӻõ��ַ������׵�ַ���Զ�ά����̬�ռ�
// ÿ�ε��ÿ��������9���ַ���
LPTSTR StrAppend( LPCTSTR str1/*=0*/, 
				  LPCTSTR str2/*=0*/, 
			  	  LPCTSTR str3/*=0*/, 
				  LPCTSTR str4/*=0*/, 
				  LPCTSTR str5/*=0*/, 
				  LPCTSTR str6/*=0*/, 
				  LPCTSTR str7/*=0*/,
				  LPCTSTR str8/*=0*/,
				  LPCTSTR str9/*=0*/ )
{
	// �����ַ������ܳ��� => resultStrLen
	int resultStrLen=0;
	if (str1) resultStrLen+=_tcslen(str1);
	if (str2) resultStrLen+=_tcslen(str2);
	if (str3) resultStrLen+=_tcslen(str3);
	if (str4) resultStrLen+=_tcslen(str4);
	if (str5) resultStrLen+=_tcslen(str5);
	if (str6) resultStrLen+=_tcslen(str6);
	if (str7) resultStrLen+=_tcslen(str7);
	if (str8) resultStrLen+=_tcslen(str8);
	if (str9) resultStrLen+=_tcslen(str9);
	
	// ���ٽ���ַ����Ŀռ�
	TCHAR * resultStr = new TCHAR[resultStrLen+1];
	HM.AddPtr(resultStr);
	memset(resultStr, 0, resultStrLen+1);
	
	// ���������ַ���
	if (str1) _tcscat(resultStr, str1);
	if (str2) _tcscat(resultStr, str2);
	if (str3) _tcscat(resultStr, str3);
	if (str4) _tcscat(resultStr, str4);
	if (str5) _tcscat(resultStr, str5);
	if (str6) _tcscat(resultStr, str6);
	if (str7) _tcscat(resultStr, str7);
	if (str8) _tcscat(resultStr, str8);
	if (str9) _tcscat(resultStr, str9);
	
	// ���ֵ '\0'
	*(resultStr + resultStrLen)='\0';
	
	// ����
	return resultStr;
}



// �� ANSI �� UTF8 �ַ���ת��Ϊ Unicode�����ؽ���ַ����׵�ַ
//   ���� bToUTF8orANSI Ϊ false ʱת�� ANSI��Ϊ true ʱת�� UTF8
//   ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
LPWSTR StrConvUnicode(const char * szAnsi, bool bFromUTF8orANSI /*=false*/)   // LPWSTR ���� unsigned short int *
{
	if (szAnsi==0) {gEmptyWSTR[0]=0; return gEmptyWSTR;}  // �ݴ�

	UINT codePage;
	WCHAR * wszResult=0;  
	int wLen=0; 
	
	if (bFromUTF8orANSI) codePage=CP_UTF8; else codePage=CP_ACP;

	// ��ý���ַ��������ַ����������� -1 ʹ�����Զ����� szAnsi �ĳ���
	wLen = MultiByteToWideChar(codePage, 0, szAnsi, -1, NULL, 0);
	// �������ַ����Ŀռ�
	wszResult = new WCHAR [wLen+1];
	HM.AddPtr(wszResult);
	// ת��
	MultiByteToWideChar(codePage, 0, szAnsi, -1, wszResult, wLen);
	wszResult[wLen]='\0';

	return wszResult; 
}


// �� Unicode �ַ���ת��Ϊ ANSI �� UTF8�����ؽ���ַ����׵�ַ
//   ���� bToUTF8orANSI Ϊ false ʱת��Ϊ ANSI��Ϊ true ʱת��Ϊ UTF8
//   ����ַ������ڴ��ɱ�ģ���Զ����䡢�Զ�����
char * StrConvFromUnicode(LPCWSTR szUnicode, bool bToUTF8orANSI /*=false*/ )
{
	if (szUnicode==0) {gEmptySTR[0]=0; return gEmptySTR;}  // �ݴ�

	UINT codePage;
	char * szResult=0;
	int aLenBytes=0;

	if (bToUTF8orANSI) codePage=CP_UTF8; else codePage=CP_ACP;

	// ��ý���ַ��������ַ����������� -1 ʹ�����Զ����� szUnicode �ĳ���
	aLenBytes=WideCharToMultiByte(codePage, 0, szUnicode, -1, NULL, 0, NULL, NULL);
	// �������ַ����Ŀռ�
	szResult = new char [aLenBytes];
	HM.AddPtr(szResult);
	// ת������ aLenBytes ������Ҫ�����ֽ������ַ���
	WideCharToMultiByte(codePage, 0, szUnicode, -1, szResult, aLenBytes, NULL, NULL);  

	return szResult;

}

double Val( LPCWSTR stringVal )
{
	if (stringVal==0) {return 0;}  // �ݴ�
	return atof(StrConvFromUnicode(stringVal, false));
}

double Val( LPCSTR stringVal )
{
	if (stringVal==0) {return 0;}  // �ݴ�
	return atof(stringVal);
}



LPTSTR Str(char character)
{
	LPTSTR buff=new TCHAR [10];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%c"), character);
	return buff;
}

LPTSTR Str(unsigned short int number)	// TCHAR
{
	LPTSTR buff=new TCHAR [20];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
#ifdef UNICODE
	*buff=number;		// ���ַ����ķ�ʽ���һ���ַ�
	*(buff+1)='\0';		// ���� TEXT �긳ֵ�ַ�����TCHAR tch=TEXT('��');
#else
	_stprintf(buff, TEXT("%u"), number);
#endif
	return buff;
}

LPTSTR Str(int number)
{
	LPTSTR buff=new TCHAR [20];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%d"), number);
	return buff;
}

LPTSTR Str(unsigned int number)
{
	LPTSTR buff=new TCHAR [20];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%u"), number);
	return buff;
}

LPTSTR Str(unsigned long number)
{
	LPTSTR buff=new TCHAR [20];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%lu"), number);
	return buff;
}

LPTSTR Str(float number)
{
	LPTSTR buff=new TCHAR [40];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%.7g"), number);
	return buff;
}

LPTSTR Str(double number)
{
	LPTSTR buff=new TCHAR [40];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%.15g"), number);
	return buff;
}

LPTSTR Str(long double number)
{
	LPTSTR buff=new TCHAR [40];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_stprintf(buff, TEXT("%.15g"), number);
	return buff;
}

LPTSTR Str( LPCTSTR str )	// ԭ�������󷵻�
{
	if (str==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	LPTSTR buff =0;
	buff = new TCHAR [lstrlen(str)+1];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_tcscpy(buff, str); 

	return buff;
}

LPTSTR LCase( LPCTSTR szStringSrc )
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	// ����һ���ַ����� buff
	LPTSTR buff =0;
	buff = new TCHAR [lstrlen(szStringSrc)+1];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_tcscpy(buff, szStringSrc); 
	
	// ת�� buff �е��ַ���
	CharLower(buff);

	// �������ַ�����buff��
	return buff;
}

LPTSTR UCase( LPCTSTR szStringSrc )
{
	if (szStringSrc==0) {gEmptyTSTR[0]=0; return gEmptyTSTR;}  // �ݴ�

	// ����һ���ַ����� buff
	LPTSTR buff =0;
	buff = new TCHAR [lstrlen(szStringSrc)+1];
	HM.AddPtr(buff);	// �� HM ����̬�ռ�
	_tcscpy(buff, szStringSrc); 
	
	// ת�� buff �е��ַ���
	CharUpper(buff);
	
	// �������ַ�����buff��
	return buff;
}

// �滻�ַ���
LPTSTR Replace( LPCTSTR szStringSrc, 
			    LPCTSTR szFind, 
				LPCTSTR szReplaceWith, 
				int start/*=1*/, 
				int countLimit/*=-1*/, 
				eBStrCompareMethod compare/*=bcmBinaryCompare*/ )
{
	// ======================== ׼������ ========================
	// ��Դ�ַ���ָ��Ϊ0����Դ�ַ���Ϊ ""������ ""
	if (szStringSrc == 0) { gEmptyTSTR[0]=0; return gEmptyTSTR; } 
	if (*szStringSrc == 0) { gEmptyTSTR[0]=0; return gEmptyTSTR;} 
	
	// ��Ҫ�滻Ϊ�����ַ���ָ��Ϊ�գ�����Ҫ�滻Ϊ�����ַ���Ϊ�մ�
	if (szReplaceWith == 0) { gEmptyTSTR[0]=0; szReplaceWith=gEmptyTSTR; }

	// ����һ���ַ����� buff
	//   buff ��������Ϊ Դ�ַ������� + һ�� szReplaceWith �ĳ���
	LPTSTR buff = 0;
	int iFindLen = lstrlen(szFind);
	int iReplLen = lstrlen(szReplaceWith);
	int buffLen = lstrlen(szStringSrc) + iReplLen +1;
	buff = new TCHAR [buffLen];
	// ������� HM.AddPtr(buff); �� HM ����̬�ռ䣬����ܻ�Ҫ����ռ�
	_tcscpy(buff, szStringSrc); 

	// ��Ҫ���ҵ����ַ���ָ��Ϊ�գ������ַ���Ϊ ""������Դ�ַ���
	if (szFind == 0) return buff;
	if (*szFind == 0) return buff;

	// ���ý����� buff �ռ䲻��ʱ������ռ�ʱ�����󲽳� => ibuffLenExpandPer
	int ibuffLenExpandPer = 200;	
	if (iReplLen > ibuffLenExpandPer) ibuffLenExpandPer=iReplLen;
	
	// ======================== ���Һ��滻 ========================
	// ��Դ�ַ����е��ַ���һ������ buff��
	// �������� szFind�������� szFind ������ szReplaceWith
	TCHAR *p=(TCHAR *)(szStringSrc+start-1), *p1, *p2, *p3;
	TCHAR *buffWrite=buff;
	int iReplacedCount=0; 
	while (*p)
	{
		// ��Դ�ַ����� p λ�ò����Ƿ��� szFind ���
		p1=p; p2=(TCHAR *)szFind;
		while (*p1 && *p2)
		{
			// ���Ҫ�Ƚϵ������ַ���=> c1��c2
			TCHAR c1= *p1, c2= *p2;		// ͨ���� *p1��*p2 ���ݴ�������������Ժ󷴸����� * ���㣬�������Ч��
			if (compare == bcmTextCompare)
			{
				// �������ִ�Сд�Ƚϣ����ڽ� c1��c2 ͳһΪ��д
				if (c1>='a' && c1<='z') c1-=32;
				if (c2>='a' && c2<='z') c2-=32;
			}
			
			// ��� c1��c2 ����ȣ��������ڲ� while
			if (c1 != c2) break;
			
			// �Ƚ���һ���ַ�
			p1++;
			p2++;
		}	// end while (*p1 && *p2)

		// �����ڲ� while ѭ�������������
		//	 (1) �����ַ��� ����ĩβ����ʱ���� Դ�ַ��� �Ƿ񵽴�ĩβ����ʾ�Ѿ��ҵ�
		//	 (2) Դ�ַ��� ����ĩβ���������ַ���û�е���ĩβ����ʾû���ҵ�
		//	 (3) Դ�ַ��� �� Ŀ���ַ��� ��û�е���ĩβ��˵������ break �����ģ���ʾû���ҵ�
		// ֻ���ڵ� (1) �����(�Ѿ��ҵ�)ʱ��Ҫ�滻���������������ֱ�ӿ���Դ�ַ����� *p 
		if (*p2 == 0)
		{
			// �� (1) �������ִ���滻��
			//   ������ szReplaceWith ��������Դ�ַ�����
			//   ������ p ��λ��Խ��һ�� szFind �ĳ���
			p3 = (TCHAR *)szReplaceWith;
			while (*p3)
			{  *buffWrite = *p3; buffWrite++; p3++;  }
			p += (iFindLen-1);	// -1 ����Ϊ����� while ���Ҫִ�� p++
			
			// �����滻�������ж��滻��������
			iReplacedCount++;
			if (countLimit>0 && iReplacedCount>=countLimit) break; // ��������� while
		}
		else
		{
			// �� (2)��(3) �������ֱ�ӿ���Դ�ַ���
			*buffWrite = *p; buffWrite++;
		}

		// buff ��ʣ�೤������Ҫ���һ�� szReplaceWith �ĳ���
		if ( buffLen - (buffWrite-buff+1) < iReplLen )
		{
			// ��Ҫ���� buff
			// buff �ĳ�������Ҫ���һ�� szReplaceWith �ĳ���
			int buffSizeLast = buffLen * sizeof(TCHAR);
			buffLen += ibuffLenExpandPer;			// �¿ռ��С���ַ�������
			TCHAR *buff2=new TCHAR [buffLen];		// �����¿ռ�
			HM.CopyMem(buff2, buff, buffSizeLast);	// ���Ͽռ����ݿ������¿ռ�
			buffWrite = buff2 + (buffWrite-buff);	// ʹ buffWrite ָ���¿ռ����ͬλ��
			delete []buff;	// ɾ���Ͽռ�
			buff = buff2;		// ����ִ�� HM.AddPtr(buff); �����¿ռ�
		}

		// Դ�ַ���ָ�� p ָ����һ�ַ�
		p++;
	}	// end while (*p)

	*buffWrite = '\0';

	// ======================== ���ؽ�� ========================
	HM.AddPtr(buff);
	return buff;
}	

BOOL MsgBeep( EMsgBeep soundStyle/* = mb_SoundSpeaker*/ )
{
	return MessageBeep(soundStyle);
}



//////////////////////////////////////////////////////////////////////////
// �Զ��� ��̬���� ����
//////////////////////////////////////////////////////////////////////////

// template <typename REDIMTYPE>
// int Redim( REDIMTYPE *  &arr, int uboundCurrent, int toUBound, bool preserve=false )	// template ��������Ҫ��ͷ�ļ���





//////////////////////////////////////////////////////////////////////////
// CBApp ���ʵ�֣�����Ӧ�ó���ȫ����Ϣ
//
//////////////////////////////////////////////////////////////////////

CBApp::CBApp( HINSTANCE hInst, 
			 HINSTANCE hPrevInst, 
			 char * lpCmdLine, 
			 int nShowCmd ): 
hInstance(hInst), 
CmdShow(nShowCmd)
{
	;
}

LPTSTR CBApp::Command() const
{
	return GetCommandLine();
}

// ���Ӧ�ó���ǰ���е�·��
LPTSTR CBApp::Path()
{
	if ( GetModuleFileName(0, m_path, sizeof(m_path)) )
	{
		// �ҵ����� \\����һλ�ýض�
		TCHAR *p = m_path;
		while (*p) p++; 
		while (*p != '\\') p--;
		p++; *p='\0';
	}
	else
	{
		// ���Ӧ�ó�����ʧ�ܣ����� ""
		m_path[0] = 0;
	}
	return m_path;
}

int CBApp::ScreenWidth() const
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int CBApp::ScreenHeight() const
{
	return GetSystemMetrics(SM_CYSCREEN);
}








//////////////////////////////////////////////////////////////////////
// CBHashLK ���ʵ��: �����ͼ�ֵ�Ĺ�ϣ��
//
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Static ������ֵ
//////////////////////////////////////////////////////////////////////
const int   CBHashLK::mcIniMemSize = 7;					// ��ʼ mem[] �Ĵ�С
const int   CBHashLK::mcMaxItemCount = 100000000;		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
const float CBHashLK::mcExpandMaxPort = 0.75;			// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
const int   CBHashLK::mcExpandCountThres = 10000;		// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
const int   CBHashLK::mcExpandCountThresMax = 10000000;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBHashLK::mcExpandBigPer = 1000000;			// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBHashLK::mcExpandMem2Per = 10;				// ÿ������ mem2[] �Ĵ�С
const int   CBHashLK::mcSeqMax = 5;						// ˳��������ֵ


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBHashLK::CBHashLK(int memSize/*=0*/)
{
	mArrTable=0; mArrTableCount = -1;	// �޻�������

	memUsedCount = 0; 
	mem2 = 0;
	memCount2 = 0;
    memUsedCount2 = 0;
	
	if (memSize)
	{
		// ��ʼ���� memSize ���� mem[] �ռ䣬�� mem2[] �Ŀռ�
		RedimArrMemType(mem, memSize);
		memCount = memSize;
	}
	else
	{
		// ��ʼ���� mcIniMemSize ���� mem[] �ռ䣬�� mem2[] �Ŀռ�
		RedimArrMemType(mem, mcIniMemSize);
		memCount = mcIniMemSize;
	}
}

CBHashLK::~CBHashLK()
{
	Clear();
	// Clear() �����У����¿����˳�ʼ��С�� mem[] �ռ䣬�ٽ���ɾ������
	if (mem) delete[] mem;
	memCount = 0;
}



//////////////////////////////////////////////////////////////////////
// ���з���
//////////////////////////////////////////////////////////////////////

void CBHashLK::AlloMem(int memSize )
{
	/*
	�����ʼ��ʱֻ������ mcIniMemSize ��С�� l_Mem[]���Ժ���ʹ�����Զ��� \
	  �󣻵�������֪���ж�󣬿������ñ����������㹻�������Ժ󲻶� \
	  �Զ������ʱ��ע����ʱҪ��Ԥ�õ�Ԫ�ظ����ඨ��һЩ���������ռ� \
	  ʱ���ռ��ͻ���໹���Զ�����
	�˺���Ҳ�����ڽض� l_Mem[] ����û��ʹ�õĿռ�
      ע��memSize <= memUsedCount ʱ���ܾ����¶��壬��ȷ�����ݲ��ᶪʧ 
	*/

	if (memSize <= memUsedCount || memSize > mcMaxItemCount) return;
	int iPreMemCount;
	iPreMemCount = memCount;

	// ReDim Preserve mem(1 To memSize)
	RedimArrMemType(mem, memSize, memCount, true);
	memCount = memSize;

	if (iPreMemCount <= memCount) ReLocaMem(iPreMemCount); else ReLocaMem(memCount);

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;

	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}

bool CBHashLK::Add( DataType data, KeyType key/*=0*/, DataLongType dataLong/*=0*/, DataLong2Type dataLong2/*=0*/, LPCTSTR dataStr/*=NULL*/, LPCTSTR dataStr2/*=NULL*/, double dataDouble/*=0.0*/, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	
	// ��ϣ���е����ݸ�����಻�ܳ��� mcMaxItemCount
	if (memUsedCount + memUsedCount2 >= mcMaxItemCount)
    {		
		if (raiseErrorIfNotHas)  throw (unsigned char)7;	// �����ڴ�
		return false;
	}
	
	// ��ǰ��ϣ���в�������ͬ�ġ���������
	if (IsKeyExist(key))
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
		return false;
	}
	
	// ͨ������ AlloMemIndex �������һ�����ÿռ���±꣺idx
	idx = AlloMemIndex(key);
	
	if (idx > 0)
	{
		// ��õ��±�ֵΪ����ʱ��ʹ�� mem[] ����Ŀռ�
		mem[idx].Data = data;
		mem[idx].DataLong = dataLong;
		mem[idx].DataLong2 = dataLong2;
		mem[idx].DataDouble = dataDouble;
		mem[idx].Key = key;

		SaveItemString(&(mem[idx].DataStr), dataStr);
		SaveItemString(&(mem[idx].DataStr2), dataStr2);

		mem[idx].Used = true;

		memUsedCount = memUsedCount + 1;
	}
	else if (idx < 0)
	{
		// ��õ��±�ֵΪ����ʱ��ʹ�� mem2[] ����Ŀռ䣬_
		// �ռ��±�Ϊ idx �ľ���ֵ
		mem2[-idx].Data = data;
		mem2[-idx].DataLong = dataLong;
		mem2[-idx].DataLong2 = dataLong2;
		mem2[-idx].DataDouble = dataDouble;
		mem2[-idx].Key = key;

		SaveItemString(&(mem2[-idx].DataStr), dataStr);
		SaveItemString(&(mem2[-idx].DataStr2), dataStr2);

		mem2[-idx].Used = true;

		memUsedCount2 = memUsedCount2 + 1;
	}
	else // idx == 0
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)9;	// �±�Խ�磺�޷����������ݿռ�
	}
	
	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;

	// �������سɹ�
    return true;
}


CBHashLK::DataType CBHashLK::Item( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].Data;
	else if (idx < 0)
		return mem2[-idx].Data;
	else
		return 0;
}


CBHashLK::DataLongType CBHashLK::ItemLong( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataLong;
	else if (idx < 0)
		return mem2[-idx].DataLong;
	else
		return 0;
}


CBHashLK::DataLong2Type CBHashLK::ItemLong2( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataLong2;
	else if (idx < 0)
		return mem2[-idx].DataLong2;
	else
		return 0;
}

double CBHashLK::ItemDouble( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataDouble;
	else if (idx < 0)
		return mem2[-idx].DataDouble;
	else
		return 0;

}


LPTSTR CBHashLK::ItemStr( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataStr;
	else if (idx < 0)
		return mem2[-idx].DataStr;
	else
		return 0;

}

LPTSTR CBHashLK::ItemStr2( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataStr2;
	else if (idx < 0)
		return mem2[-idx].DataStr2;
	else
		return 0;

}


// �ж�һ�� Key �Ƿ��ڵ�ǰ�����д���
bool CBHashLK::IsKeyExist( KeyType key )
{
	int idx;
	idx = GetMemIndexFromKey(key, false);
    return (idx != 0);
}

bool CBHashLK::Remove( KeyType key, bool raiseErrorIfNotHas/*=True*/ )
{
	int idx;
	
	// ���� GetMemIndexFromKey ������á�����Ϊ Key ���������ڿռ���±�
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx == 0)
		return false;
	else if (idx > 0)
	{
		// ��ϣ���С�����Ϊ Key �������� mem[] �����У��±�Ϊ idx
		mem[idx].Used = false;
		mem[idx].Key = 0;
		SaveItemString(&(mem[idx].DataStr), 0);  // �ڶ�������Ϊ0��ɾ�� mem[idx].DataStr ָ��Ŀռ䣬���� mem[idx].DataStr=0;
		SaveItemString(&(mem[idx].DataStr2), 0); 

		memUsedCount = memUsedCount - 1;
	}
	else
	{
		// idx<0 ��ʾ����ϣ���С�����Ϊ Key �������� mem2[] ������ \
		// �±�Ϊ idx �ľ���ֵ
		// ɾ���±�Ϊ��-idx����Ԫ��

		SaveItemString(&(mem[-idx].DataStr), 0); // �ڶ�������Ϊ0��ɾ�� mem[-idx].DataStr ָ��Ŀռ䣬���� mem[-idx].DataStr=0;
		SaveItemString(&(mem[-idx].DataStr2), 0); 

		for(int i=-idx; i<= - 1; i++)
			mem2[i] = mem2[i+1];
		mem2[memUsedCount2].DataStr=0;		// ֱ������Ϊ0����˿ռ��Ѿ����ݸ���һԪ��
		mem2[memUsedCount2].DataStr2=0;		// ֱ������Ϊ0����˿ռ��Ѿ����ݸ���һԪ��
		memUsedCount2 = memUsedCount2 - 1;
	}

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;

	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;

	// �������سɹ�
	return true;
}

void CBHashLK::StartTraversal()
{
	// ��ʼ�� NextXXX ... ��������
    mTravIdxCurr = 1;
}


CBHashLK::DataType CBHashLK::NextItem( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� Data
    // �� bRetNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].Data;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].Data;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

CBHashLK::DataLongType CBHashLK::NextItemLong( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataLong;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataLong;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}


CBHashLK::DataLong2Type CBHashLK::NextItemLong2( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataLong2;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataLong2;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

double CBHashLK::NextItemDouble( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataDouble;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataDouble;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

LPTSTR CBHashLK::NextItemStr( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataStr;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataStr;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

LPTSTR CBHashLK::NextItemStr2( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataStr2;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataStr2;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

CBHashLK::KeyType CBHashLK::NextKey( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� String
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].Key;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].Key;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

// �������Ԫ�أ��ض��� mcIniMemSize ���洢�ռ�
void CBHashLK::Clear( void )
{
	// ���	�� Index ���ʸ�Ԫ�صĻ�������
	if (mArrTable) {delete []mArrTable; mArrTable=0;}
	mArrTableCount=-1;

	// ɾ�� mem[] �� mem2[] �е�ÿ��Ԫ�ص� �ַ������� ָ��Ŀռ�
	int i;
	for (i=1; i<=memCount; i++)
	{
		if (mem[i].DataStr) {delete [] mem[i].DataStr; mem[i].DataStr=0; }
		if (mem[i].DataStr2) {delete [] mem[i].DataStr2; mem[i].DataStr2=0; }
	}
	for (i=1; i<=memCount2; i++)
	{
		if (mem2[i].DataStr) {delete [] mem2[i].DataStr; mem2[i].DataStr=0; }
		if (mem2[i].DataStr2) {delete [] mem2[i].DataStr2; mem2[i].DataStr2=0; }
	}

	// ɾ�� mem[] �� mem2[] �Ŀռ�
	delete [] mem; mem=0;
	delete [] mem2; mem2=0;
	memCount = 0; memUsedCount = 0;
	memCount2 = 0; memUsedCount2 = 0;

	// ���¿��ٿռ�
	RedimArrMemType(mem, mcIniMemSize, memCount, false);
	memCount = mcIniMemSize;

	mTravIdxCurr = 0;
}


// ���ع���Ԫ�ظ���
int CBHashLK::Count( void )
{
	return memUsedCount + memUsedCount2;
}




//////////////////////////////////////////////////////////////////////
// ˽�з���
//////////////////////////////////////////////////////////////////////

void CBHashLK::ReLocaMem( int preMemCountTo )
{
	/*
	���·��� mem[], mem2[] �ĸ�Ԫ�صĿռ䣬mem2[] ��ĳЩԪ�ؿ��ܱ� \
	�����ƶ��� mem
	���޸� memUsedCount,memUsedCount2, memCount2, mem2[] ��ֵ
	preMemCountTo ֻ���� mem[1 to preMemCountTo]��preMemCountTo �Ժ��Ԫ�ر���Ϊ \
	δ�ã������ǣ���������ζ����� mem2[] �е�����Ԫ��
	*/
	
	// �� mem[] �е���ʹ��Ԫ�غ� mem2[] �е�����Ԫ���ȷ��� memUsed[] �У� \
	// �� memUsed[] ����Ϊ�㹻��ʵ�� memUsed[] ֻʹ���� lngUsedCount ��Ԫ��
	MemType * memUsed;
	RedimArrMemType(memUsed, preMemCountTo + memUsedCount2);

	int iUsedCount=0;		
	int i;

	// �� mem[] ����ʹ�õ�Ԫ�ش��� memUsed[]
	for (i=1; i<=preMemCountTo; i++)
		if (mem[i].Used)
		{
			iUsedCount = iUsedCount + 1;
			memUsed[iUsedCount] = mem[i];
		}

	// �� mem2[] �е�����Ԫ�ش��� memUsed[]
	for (i=1; i<=memUsedCount2; i++)
	{
		iUsedCount = iUsedCount + 1;
		memUsed[iUsedCount] = mem2[i];
	}


	/*
	��ʱ memUsed[1 To lngUsedCount] ��Ϊ���� mem[] \
	�е���ʹ��Ԫ�� �� mem2[] �е�����Ԫ��
	*/

	// ��� mem��Ҳ��������� Used ��
	RedimArrMemType(mem, memCount, memCount, false); 
	memUsedCount=0;  // memUsedCount ��0���������ƶ���+1

	// ��� mem2��Ҳ��������� Used ��
	RedimArrMemType(mem2, -1, memCount2, false);
	memCount2 = 0;
	memUsedCount2 = 0; 

	// ����� memUsed[1 To lngUsedCount] �е�Ԫ�ذ��������Сӳ���±���� mem[]
	int idx;
	for (i=1; i<=iUsedCount; i++)
	{
		idx = AlloMemIndex(memUsed[i].Key, false);
		if (idx > 0)
		{
			mem[idx] = memUsed[i];
			mem[idx].Used = 1;
			memUsedCount = memUsedCount + 1;
		}
		else
		{
			mem2[-idx] = memUsed[i];
			mem2[-idx].Used = 1;
			memUsedCount2 = memUsedCount2 + 1;
		}
	}

	// ɾ����ʱ�ռ� memUsed
	delete [] memUsed; memUsed=0;

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}


// �ض��� mem[] �����С������ mem[] �Ŀռ�
void CBHashLK::ExpandMem( void )
{
	int iCount, iPreMemCount;

	// �����ϣ���й�����������
	iCount = memUsedCount + memUsedCount2;

	// ȡ�����������������͡���ǰ mem[] �Ŀռ����������ߵĽϴ�ֵ
	if (iCount < memCount) iCount = memCount;

	// ���������ռ�֮ǰ�ġ�ԭ���� mem[] �Ŀռ�����
	iPreMemCount = memCount;

	
	if (iCount<1) iCount=1;		// ���� iCount Ϊ0ʱ���޷�����ռ�
	if (iCount < mcExpandCountThres)
	{
		// ��������������Ƚ��١����������ռ�Ϊԭ����2��
		iCount = iCount * 2;

	}
	else if (iCount < mcExpandCountThresMax)
	{
		// ������������Ѿ����е�ࡱ���������ռ�Ϊԭ����1.5��
		iCount = iCount * 3 / 2;
	}
	else
	{
		// ��������������ܶࡱ�������� mcExpandBigPer ���ռ�
		iCount = iCount + mcExpandBigPer;
	}

	// �ض��������С
	// ReDim Preserve mem(1 To lngCount)
	RedimArrMemType(mem, iCount, memCount, true);
	memCount = iCount;

	// ���������С�����°��������������ݵ���λ�ã�������Ҫ����
	// �����ռ�֮ǰ�ġ�ԭ���� mem[] �Ŀռ�����
	ReLocaMem(iPreMemCount);

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}

int CBHashLK::AlloSeqIdx( int fromIndex, int toIndex )
{
    /*
	 �� mem[] ��һ��ûʹ�õĿռ䣬�� fromIndex ��ʼ�� \
		�� toIndex ����
	 ���� mem[] ��һ��ûʹ��Ԫ�ص��±꣬û�ҵ����� 0
	*/
	int i;
	if (fromIndex <= 0)  fromIndex = 1;
	if (toIndex > memCount) toIndex = memCount;
	
	for (i=fromIndex; i<=toIndex; i++)
		if (! mem[i].Used) return i; 

	return 0;
}

// ������ϣ�������ݴ��� mArrTable()������ mArrTableCount Ϊ���ݸ��������سɹ���ʧ�ܣ�
bool CBHashLK::RefreshArrTable()
{
	int iCount;
	int i,j;
	
	// �����ϣ���й�����������
	iCount = memUsedCount + memUsedCount2;
	
	mArrTableCount=iCount;
	if (mArrTableCount<=0) return false;
	
	if (mArrTable) {delete []mArrTable; mArrTable=0;}
	mArrTable=new int [iCount+1];	// ʹ�����±��1��ʼ
	
	j=1;
	for (i=1; i<=memCount; i++)
	{
		if (mem[i].Used)
		{
			if (j > iCount) return false;
			
			mArrTable[j] = i;		// �� ��ϣ��ı��������ڵ� mem[] ���±� i ���� mArrTable[j]
			mem[i].Index = j;		// �� ��ϣ��ı����� �� Index ��Ա�м�¼ mArrTable ���±� j

			j=j+1;
		}
	}
	
	for (i=1; i<=memUsedCount2; i++)
	{
		if (mem2[i].Used)
		{
			if (j > iCount) return false;
			
			mArrTable[j] = -i;		// �� ��ϣ��ı��������ڵ� mem2[] ���±� i ��ȡ�������� mArrTable[j]
			mem[i].Index = j;		// �� ��ϣ��ı����� �� Index ��Ա�м�¼ mArrTable ���±� j
			
			j=j+1;
		}
	}
	
	return true;
}




int CBHashLK::AlloMemIndex( KeyType key, bool CanExpandMem/*=true */ )
{
	/* 
	  ���� Key ����һ�� mem[] �е�δ�ô洢�ռ䣬���� mem[] �����±�
	  ��� Key �Ǹ�ֵ����ת��Ϊ�����������Ĵ洢�ռ�
		���ظ�ֵ������ mem[] ���ҵ��ռ䣺����ֵ�ľ���ֵΪ mem2[] �� \
		��һ�������±�ռ䣨mem2[]�Զ�Redim�����Դ��� mem2[]
	  ������ȷ������һ����ʹ�õĿռ䣬���������� mem2[] �еĿռ�
	  �����������޸� memUsedCount2 ��ֵ���� redim mem2[]
		CanExpandMem=true ʱ�����������Զ����� mem[]�����򲻻��Զ�����
		
		������
		1. ���� Key Mod memCount + 1���� Index -> idxMod
		2. �������Ԫ���Ѿ�ʹ�ã���Key�Ƿ� < cMaxNumForSquare (sqr(2^31)=46340) \
		�� <����ƽ�� Key��Ȼ�� mod memCount + 1�� \
		�� >=�����ð�λ����λ���㣬����� key �ĸ������֣�Ȼ�� mod memCount + 1
		��������������˲� Index -> idxSq
		3. �� memCount-idxMod+1 -> idxModRev
		4. �� memCount-idxSq+1 -> idxSqRev
		5. �������ҵ��� Index ����ʹ���ˣ��� Count �Ƿ� > \
		mcExpandMaxPort*Count�����ǣ��� CanExpandMem=true�� \
		������ mem[] �Ĵ洢�ռ䣬Ȼ��ݹ鱾���̣��ظ� 1-4 ��
		6. �� idxMod+1,+2,...,+mcSeqMax���� idxMod-1,-2,...,-mcSeqMax
		7. ��û�У����ظ�ֵ������ֵΪ mem2[] ����һ�����ÿռ䣬�Դ��� mem2[]
	*/


	const int cMaxNumForSquare = 46340;
	
	int idxMod=0, idxSq=0;
    int idxModRev=0, idxSqRev=0;
    int iCount=0;
    int keyToCalc=key; // ������ Key����ԶΪ>0����

	keyToCalc = key;
	if (keyToCalc < 0) keyToCalc = 0 - keyToCalc;	// ��� Key �Ǹ�ֵ����ת��Ϊ�����������Ĵ洢�ռ�
	iCount = memUsedCount + memUsedCount2;
    
	if (memCount) 
	{		
		// 1: ���� Key Mod memCount + 1���� Index -> idxMod
		idxMod = keyToCalc % memCount + 1;
		if (! mem[idxMod].Used) return idxMod;
    
		// 2: �� ƽ��Key ���ٳ���ȡ�࣬�� Index -> idxSq
		if (keyToCalc <= cMaxNumForSquare)
		{
			idxSq = (keyToCalc * keyToCalc) % memCount + 1;
		}
		else
		{
			int kBitSum=0;
			kBitSum = (keyToCalc & 0xFFFF0000)>>16;
			kBitSum += (keyToCalc & 0xFF00)>>8;
			kBitSum += (keyToCalc & 0xF0)>>4;
			kBitSum += (keyToCalc & 0xF);
			idxSq = kBitSum % memCount + 1;
		}
		if (! mem[idxSq].Used) return idxSq;

		// 3: ���Ե����� idxMod ���ռ� -> idxModRev
		idxModRev = memCount - idxMod + 1; 
		if (! mem[idxModRev].Used) return idxModRev;
    
		// 4: ���Ե����� idxSq ���ռ� -> idxSqRev
		idxSqRev = memCount - idxSq + 1;
		if (! mem[idxSqRev].Used) return idxSqRev;
	}
    
    // 5: ����ռ�ʹ�ðٷֱȳ�����ֵ�������� mem[] �� �ռ�
    if (CanExpandMem && iCount > mcExpandMaxPort * memCount)
	{
		ExpandMem();  // ���� mem[] �Ŀռ�
		return AlloMemIndex(key, CanExpandMem); // �ݹ飬�ظ�1-4��
	}
        
    
    int lngRetIdx;
    
    // 6: �� idxMod ��ʼ��ǰ������������� mcSeqMax ���ռ�
    int idxMdSta, idxMdEnd; 
    idxMdSta = idxMod - mcSeqMax; idxMdEnd = idxMod + mcSeqMax;
    lngRetIdx = AlloSeqIdx(idxMdSta, idxMod - 1); 
    if (lngRetIdx > 0) return lngRetIdx;
    lngRetIdx = AlloSeqIdx(idxMod + 1, idxMdEnd);
    if (lngRetIdx > 0) return lngRetIdx; 
    
    // 8: ���ظ�ֵ������ֵΪ mem2[] ����һ��Ԫ�أ��Դ��� mem2[]
    if (memUsedCount2 + 1 > memCount2)
    {    
        // ReDim Preserve mem2(1 To mcExpandMem2Per)
		RedimArrMemType(mem2, memCount2 + mcExpandMem2Per, memCount2, true);
		memCount2 = memCount2 + mcExpandMem2Per;
	}

    return -(memUsedCount2 + 1);
}


int CBHashLK::FindSeqIdx( KeyType key, int fromIndex, int toIndex )
{
	
    // �� mem[] �м�ΪKey��Ԫ���±꣬�� fromIndex ��ʼ�� \
	//	�� toIndex ����
	//	���� mem[] ���ҵ������±꣨>0����û�ҵ����� 0

	int i;
	if (fromIndex < 1) fromIndex = 1;
	if (toIndex > memCount) toIndex = memCount;
	
	for (i=fromIndex; i<=toIndex; i++)
		if ((mem[i].Used) && mem[i].Key == key )
			return i;
	
	return 0;
}




int CBHashLK::TraversalGetNextIdx( void )
{
	// �� NextXXX ��������ʱ��������һ����Next���� mem[]�±꣨����ֵ>0���� \
	// �� mem2[] ���±꣨����ֵ<0�������ѱ�������������ֵ=0��
	
	int iRetIdx;
	
	if (mTravIdxCurr > memCount ||
		-mTravIdxCurr > memCount2 ||
		mTravIdxCurr == 0) return 0;
	
	if (mTravIdxCurr > 0)
	{
		//////////// �� mem[] ���� ////////////
		while (! mem[mTravIdxCurr].Used)
		{
			mTravIdxCurr = mTravIdxCurr + 1;
			if (mTravIdxCurr > memCount) break;
		}
		
		if (mTravIdxCurr > memCount)
		{
			//// �ѱ������������� mem2[] �л������ݼ������� mem2[] ////
			if (memCount2 > 0)
			{
				// �����´α��� mem2[] �����ݵ��±�ĸ���
				mTravIdxCurr = -1;
				// ִ������� if mTravIdxCurr < 0 Then 
			}
			else
			{
				// ���ؽ���
				iRetIdx = 0;
				return iRetIdx;
			}
		}
		else
		{
			//// ���� mTravIdxCurr ////
			iRetIdx = mTravIdxCurr;
			// �����´α���ָ�� ָ����һ��λ�ã����� mem[] ����һ���� \
			// ���� mem2[] ����ʼ��
			mTravIdxCurr = mTravIdxCurr + 1;
			if (mTravIdxCurr > memCount) if (memCount2 > 0) mTravIdxCurr = -1;
			return iRetIdx;
		}
	}
	
	if (mTravIdxCurr < 0)
	{
		//////////// �� mem2[] ���� ////////////
		while (! mem2[-mTravIdxCurr].Used)
		{	
			mTravIdxCurr = mTravIdxCurr - 1;
			if (-mTravIdxCurr > memCount2) break;
		}
		
		if (-mTravIdxCurr > memCount2)
		{
			//// �ѱ������� ////
			// ���ؽ���
			iRetIdx = 0; 
		}
		else
		{
			// ���ظ�ֵ�� mTravIdxCurr
			iRetIdx = mTravIdxCurr;
			// ���� mTravIdxCurr ��ָ��
			mTravIdxCurr = mTravIdxCurr - 1;
		}
		return iRetIdx;
	}
	
	return 0;
}




// �ض��� һ�� MemType ���͵����飨������� mem[] �� mem2[]���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�mem �� mem2�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBHashLK::RedimArrMemType( MemType * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		MemType * ptrNew = new MemType [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(MemType)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� MemType Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(MemType)*ctToCpy); 
		}

		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;

		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}


int CBHashLK::GetMemIndexFromKey( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	const int cMaxNumForSquare = 46340;  // sqrt(2^31)=46340
    
    int idxMod=0, idxSq=0;
    int idxModRev=0, idxSqRev=0;
    int keyToCalc=key; // ������ Key����ԶΪ>=0����
    if (keyToCalc < 0) keyToCalc = 0 - keyToCalc;

	if (memCount)
	{
		// 1: ���� Key Mod memCount + 1���� Index -> idxMod
		idxMod = keyToCalc % memCount + 1;
		if (mem[idxMod].Used && mem[idxMod].Key == key) 
			return idxMod;
    
		// 2: �� ƽ��Key���ٳ���ȡ�࣬�� Index -> idxSq
		if (keyToCalc <= cMaxNumForSquare)
		{
			idxSq = (keyToCalc * keyToCalc) % memCount + 1;
		}
		else
		{
			int kBitSum=0;
			kBitSum = (keyToCalc & 0xFFFF0000)>>16;
			kBitSum += (keyToCalc & 0xFF00)>>8;
			kBitSum += (keyToCalc & 0xF0)>>4;
			kBitSum += (keyToCalc & 0xF);
			idxSq = kBitSum % memCount + 1;
		}
		if (mem[idxSq].Used && mem[idxSq].Key == key)
			return idxSq;
    
		// 3: ���Ե����� idxMod ���ռ� -> idxModRev
		idxModRev = memCount - idxMod + 1;
		if (mem[idxModRev].Used && mem[idxModRev].Key == key)
		   return idxModRev;
    
		// 4: ���Ե����� idxSq ���ռ� -> idxSqRev
		idxSqRev = memCount - idxSq + 1;
		if (mem[idxSqRev].Used && mem[idxSqRev].Key == key)
		   return idxSqRev;
	}

    int lngRetIdx=0;
    
    // 6: �� idxMod ��ʼ��ǰ������������� mcSeqMax ���ռ�
    int idxMdSta, idxMdEnd;
    idxMdSta = idxMod - mcSeqMax; idxMdEnd = idxMod + mcSeqMax;
    lngRetIdx = FindSeqIdx(key, idxMdSta, idxMod - 1);
    if (lngRetIdx > 0)  return  lngRetIdx;
    lngRetIdx = FindSeqIdx(key, idxMod + 1, idxMdEnd);
    if (lngRetIdx > 0)  return  lngRetIdx;
    
    // 7: �ٲ鿴 mem2[] �е�Ԫ����û��
    for (int i=1; i<=memUsedCount2; i++)
        if (mem2[i].Used && mem2[i].Key == key) return -i;
	
	if (raiseErrorIfNotHas) throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
	return 0;
}

// �� index ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0
int CBHashLK::GetMemIndexFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	if (mArrTableCount != memUsedCount + memUsedCount2) RefreshArrTable(); // ˢ�����黺��
	if (index<1 || index>mArrTableCount)
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
		return 0;
	}
	
	int idx=mArrTable[index];
	if (idx==0) 
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)7;	// �����ڴ棨mArrTable[index]����Ϊ0��
		return 0;
	}
	else
		return idx;
}


// �� new �������ַ����ռ䣬�� ptrNewString ָ����ַ����������¿ռ䣻
//   ptrSaveTo ��һ�������ַ�����ַ��ָ������ĵ�ַ����ָ���ָ�����������
//   ���� new ���ٵ����ַ����ռ�ĵ�ַ�������� ��*ptrSaveTo = �¿ռ��ַ��
// �����ͷš�*ptrSaveTo����ָ��Ŀռ�Ĺ���
//   �� ptrSaveTo �����ɱ����� &(mem[i].key) ��ָ���ָ�룻ptrNewString �ɱ������µ� key
//   ����ɡ�mem[i].key=key���Ĳ������������޸� mem[i].key �����ݣ�
//   ��ɾ������ָ��Ŀռ䣬������ָ���¿ռ�
// ��� key Ϊ��ָ�룬���ͷš�*ptrSaveTo����ָ��Ŀռ�
void CBHashLK::SaveItemString( TCHAR ** ptrSaveTo, LPCTSTR ptrNewString )
{
	// ע�� ptrSaveTo �Ǹ�����ָ��
	if (ptrSaveTo==0) return;  // û�б����λ��
	
	// ��� ptrSaveTo ָ���ָ�������Ϊ����ָ�롱����ʾҪ����֮������
	//   ������һ����ǰ���ٵĿռ��ַ��Ӧ��ɾ����ǰ���ٵĿռ�
	if (*ptrSaveTo != 0) {delete [] (*ptrSaveTo); *ptrSaveTo=0; }
	
	if (ptrNewString)
	{
		// �����¿ռ䣬���� ptrNewString ����ַ������¿ռ�
		TCHAR * p = new TCHAR [lstrlen(ptrNewString)+1];
		lstrcpy(p, ptrNewString);
		
		// ʹ *ptrSaveTo ָ���¿ռ�
		*ptrSaveTo = p;
	}
}



// ---------------- �� Index ������������(����Key����KeyΪֻ��) ----------------
// ע������������ɾ��Index ���ܻ�仯��ĳ���ݵ� Index ����������һһ��Ӧ

CBHashLK::DataType CBHashLK::ItemFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Data;
	else if (ii < 0)
		return mem2[-ii].Data;
	else
		return 0;
}

CBHashLK::DataLong2Type CBHashLK::ItemLong2FromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataLong2;
	else if (ii < 0)
		return mem2[-ii].DataLong2;
	else
		return 0;
}

CBHashLK::DataLongType CBHashLK::ItemLongFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataLong;
	else if (ii < 0)
		return mem2[-ii].DataLong;
	else	
		return 0;
}

double CBHashLK::ItemDoubleFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataDouble;
	else if (ii < 0)
		return mem2[-ii].DataDouble;
	else
		return 0;
}

LPTSTR CBHashLK::ItemStrFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataStr;
	else if (ii < 0)
		return mem2[-ii].DataStr;
	else			
		return 0;
}

LPTSTR CBHashLK::ItemStr2FromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataStr2;
	else if (ii < 0)
		return mem2[-ii].DataStr2;
	else			
		return 0;	
}

CBHashLK::KeyType CBHashLK::IndexToKey( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Key;
	else if (ii < 0)
		return mem2[-ii].Key;
	else			
		return 0;	
}


int CBHashLK::KeyToIndex( KeyType key, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	if (mArrTableCount != memUsedCount + memUsedCount2) RefreshArrTable(); // ˢ�����黺��
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Index;
	else if (ii < 0)
		return mem2[-ii].Index;
	else
		return 0;
}


// ---------------- �� Key ������������ ----------------

bool CBHashLK::ItemSet( KeyType key, DataType vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].Data = vNewValue;
	else if (ii < 0)
		mem2[-ii].Data = vNewValue;
	return true;
}

bool CBHashLK::ItemLongSet( KeyType key, DataLongType vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong = vNewValue;
	return true;	
}


bool CBHashLK::ItemLong2Set( KeyType key, DataLong2Type vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong2 = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong2 = vNewValue;
	return true;	
}

bool CBHashLK::ItemDoubleSet( KeyType key, double vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataDouble = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataDouble = vNewValue;
	return true;	
}

bool CBHashLK::ItemStrSet( KeyType key, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr), vNewValue);
	return true;	
}

bool CBHashLK::ItemStr2Set( KeyType key, LPCTSTR vNewValue,bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr2), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr2), vNewValue);
	return true;	
}

// ---------------- �� Index ������������(KeyΪֻ����������Key) ----------------
// ע������������ɾ��Index ���ܻ�仯��ĳ���ݵ� Index ����������һһ��Ӧ

bool CBHashLK::ItemFromIndexSet( int index, DataType vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].Data = vNewValue;
	else if (ii < 0)
		mem2[-ii].Data = vNewValue;
	return true;
}

bool CBHashLK::ItemLongFromIndexSet( int index, DataLongType vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong = vNewValue;
	return true;
}

bool CBHashLK::ItemLong2FromIndexSet( int index, DataLong2Type vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong2 = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong2 = vNewValue;
	return true;	
}

bool CBHashLK::ItemDoubleFromIndexSet( int index, double vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataDouble = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataDouble = vNewValue;
	return true;	
}

bool CBHashLK::ItemStrFromIndexSet( int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr), vNewValue);
	return true;	
}

bool CBHashLK::ItemStr2FromIndexSet( int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr2), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr2), vNewValue);
	return true;
}









//////////////////////////////////////////////////////////////////////
// CBHashStrK ���ʵ��: �ַ����ͼ�ֵ�Ĺ�ϣ��
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Static ������ֵ
//////////////////////////////////////////////////////////////////////
const int   CBHashStrK::mcIniMemSize = 7;				// ��ʼ mem[] �Ĵ�С
const int   CBHashStrK::mcMaxItemCount = 100000000;		// ���Ԫ�ظ������������ֵ�� long ��ʾ�ķ�Χ֮�ڣ�
const float CBHashStrK::mcExpandMaxPort = 0.75;			// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
const int   CBHashStrK::mcExpandCountThres = 10000;		// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
const int   CBHashStrK::mcExpandCountThresMax = 10000000;	// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBHashStrK::mcExpandBigPer = 1000000;			// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBHashStrK::mcExpandMem2Per = 10;				// ÿ������ mem2[] �Ĵ�С
const int   CBHashStrK::mcSeqMax = 5;						// ˳��������ֵ


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBHashStrK::CBHashStrK(int memSize/*=0*/)
{
	mArrTable=0; mArrTableCount = -1;	// �޻�������

	memUsedCount = 0; 
	mem2 = 0;
	memCount2 = 0;
    memUsedCount2 = 0;
	KeyCaseSensitive=false;			// �Ƿ� key Ҫ���ִ�Сд��Ĭ�ϲ��ִ�Сд��

	if (memSize)
	{
		// ��ʼ���� memSize ���� mem[] �ռ䣬�� mem2[] �Ŀռ�
		RedimArrMemType(mem, memSize);
		memCount = memSize;
	}
	else
	{
		// ��ʼ���� mcIniMemSize ���� mem[] �ռ䣬�� mem2[] �Ŀռ�
		RedimArrMemType(mem, mcIniMemSize);
		memCount = mcIniMemSize;
	}
}

CBHashStrK::~CBHashStrK()
{
	Clear();

	// Clear() �����У����¿����˳�ʼ��С�� mem[] �ռ䣬�ٽ���ɾ������
	if (mem) delete[] mem;
	memCount = 0;
}



//////////////////////////////////////////////////////////////////////
// ���з���
//////////////////////////////////////////////////////////////////////

void CBHashStrK::AlloMem(int memSize )
{
	/*
	�����ʼ��ʱֻ������ mcIniMemSize ��С�� l_Mem[]���Ժ���ʹ�����Զ��� \
	  �󣻵�������֪���ж�󣬿������ñ����������㹻�������Ժ󲻶� \
	  �Զ������ʱ��ע����ʱҪ��Ԥ�õ�Ԫ�ظ����ඨ��һЩ���������ռ� \
	  ʱ���ռ��ͻ���໹���Զ�����
	�˺���Ҳ�����ڽض� l_Mem[] ����û��ʹ�õĿռ�
      ע��memSize <= memUsedCount ʱ���ܾ����¶��壬��ȷ�����ݲ��ᶪʧ 
	*/

	if (memSize <= memUsedCount || memSize > mcMaxItemCount) return;
	int iPreMemCount;
	iPreMemCount = memCount;

	// ReDim Preserve mem(1 To memSize)
	RedimArrMemType(mem, memSize, memCount, true);
	memCount = memSize;

	if (iPreMemCount <= memCount) ReLocaMem(iPreMemCount); else ReLocaMem(memCount);

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;

	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}

bool CBHashStrK::Add( int data, LPCTSTR key/*=0*/, long dataLong/*=0*/, long dataLong2/*=0*/, LPCTSTR dataStr/*=NULL*/, LPCTSTR dataStr2/*=NULL*/, double dataDouble/*=0.0*/, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	
	// ��ϣ���е����ݸ�����಻�ܳ��� mcMaxItemCount
	if (memUsedCount + memUsedCount2 >= mcMaxItemCount)
    {		
		if (raiseErrorIfNotHas)  throw (unsigned char)7;	// �����ڴ�
		return false;
	}
	
	// ��ǰ��ϣ���в�������ͬ�ġ���������
	if (IsKeyExist(key))
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
		return false;
	}
	
	// ͨ������ AlloMemIndex �������һ�����ÿռ���±꣺idx
	idx = AlloMemIndex(key);
	
	if (idx > 0)
	{
		// ��õ��±�ֵΪ����ʱ��ʹ�� mem[] ����Ŀռ�
		mem[idx].Data = data;
		mem[idx].DataLong = dataLong;
		mem[idx].DataLong2 = dataLong2;
		mem[idx].DataDouble = dataDouble;

		SaveItemString(&(mem[idx].Key), key);		// ���� key �ַ�������̬����һ���ռ������棬�¿ռ��ַ���浽 mem[idx].Key����� mem[idx].Key ����ֵ��Ϊ 0���ú����������� delete mem[idx].Key ����ָ��Ŀռ�Ĺ���
		SaveItemString(&(mem[idx].DataStr), dataStr);
		SaveItemString(&(mem[idx].DataStr2), dataStr2);
		
		mem[idx].Used = true;

		memUsedCount = memUsedCount + 1;
	}
	else if (idx < 0)
	{
		// ��õ��±�ֵΪ����ʱ��ʹ�� mem2[] ����Ŀռ䣬_
		// �ռ��±�Ϊ idx �ľ���ֵ
		mem2[-idx].Data = data;
		mem2[-idx].DataLong = dataLong;
		mem2[-idx].DataLong2 = dataLong2;
		mem2[-idx].DataDouble = dataDouble;

		SaveItemString(&(mem2[-idx].Key), key);		// ���� key �ַ�������̬����һ���ռ������棬�¿ռ��ַ���浽 mem[idx].Key����� mem[idx].Key ����ֵ��Ϊ 0���ú����������� delete mem[idx].Key ����ָ��Ŀռ�Ĺ���
		SaveItemString(&(mem2[-idx].DataStr), dataStr);
		SaveItemString(&(mem2[-idx].DataStr2), dataStr2);

		mem2[-idx].Used = true;

		memUsedCount2 = memUsedCount2 + 1;
	}
	else // idx == 0
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)9;	// �±�Խ�磺�޷����������ݿռ�
	}
	
	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;

	// �������سɹ�
    return true;
}


int CBHashStrK::Item( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].Data;
	else if (idx < 0)
		return mem2[-idx].Data;
	else
		return 0;

}


long CBHashStrK::ItemLong( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataLong;
	else if (idx < 0)
		return mem2[-idx].DataLong;
	else
		return 0;

}


long CBHashStrK::ItemLong2( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataLong2;
	else if (idx < 0)
		return mem2[-idx].DataLong2;
	else
		return 0;

}

double CBHashStrK::ItemDouble( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataDouble;
	else if (idx < 0)
		return mem2[-idx].DataDouble;
	else
		return 0;

}

LPTSTR CBHashStrK::ItemStr( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataStr;
	else if (idx < 0)
		return mem2[-idx].DataStr;
	else
		return 0;

}

LPTSTR CBHashStrK::ItemStr2( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int idx;
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx > 0)
		return mem[idx].DataStr2;
	else if (idx < 0)
		return mem2[-idx].DataStr2;
	else
		return 0;

}


// �ж�һ�� Key �Ƿ��ڵ�ǰ�����д���
bool CBHashStrK::IsKeyExist( LPCTSTR key )
{
	int idx;
	idx = GetMemIndexFromKey(key, false);
    return (idx != 0);
}

bool CBHashStrK::Remove( LPCTSTR key, bool raiseErrorIfNotHas/*=True*/ )
{
	int idx;
	
	// ���� GetMemIndexFromKey ������á�����Ϊ Key ���������ڿռ���±�
	idx = GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (idx == 0)
		return false;
	else if (idx > 0)
	{
		// ��ϣ���С�����Ϊ Key �������� mem[] �����У��±�Ϊ idx
		mem[idx].Used = false;
		SaveItemString(&(mem[idx].Key), 0);	// �ڶ�������Ϊ0��ɾ�� mem[idx].Key ָ��Ŀռ䣬���� mem[idx].Key=0;
		SaveItemString(&(mem[idx].DataStr), 0); 
		SaveItemString(&(mem[idx].DataStr2), 0); 

		memUsedCount = memUsedCount - 1;
	}
	else
	{
		// idx<0 ��ʾ����ϣ���С�����Ϊ Key �������� mem2[] ������ \
		// �±�Ϊ idx �ľ���ֵ
		// ɾ���±�Ϊ��-idx����Ԫ��
		SaveItemString(&(mem[-idx].Key), 0);	// �ڶ�������Ϊ0��ɾ�� mem[-idx].Key ָ��Ŀռ䣬���� mem[-idx].Key=0;
		SaveItemString(&(mem[-idx].DataStr), 0); 
		SaveItemString(&(mem[-idx].DataStr2), 0); 

		for(int i=-idx; i<= memUsedCount2-1; i++)
			mem2[i] = mem2[i+1];
		mem2[memUsedCount2].Key=0;			// ֱ������Ϊ0����˿ռ��ַ�Ѿ����ݸ���һԪ��
		mem2[memUsedCount2].DataStr=0;		// ֱ������Ϊ0����˿ռ��Ѿ����ݸ���һԪ��
		mem2[memUsedCount2].DataStr2=0;		// ֱ������Ϊ0����˿ռ��Ѿ����ݸ���һԪ��

		memUsedCount2 = memUsedCount2 - 1;
	}

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;

	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;

	// �������سɹ�
	return true;
}

void CBHashStrK::StartTraversal()
{
	// ��ʼ�� NextXXX ... ��������
    mTravIdxCurr = 1;
}


int CBHashStrK::NextItem( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� Data
    // �� bRetNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].Data;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].Data;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

long CBHashStrK::NextItemLong( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataLong;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataLong;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}



long CBHashStrK::NextItemLong2( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataLong2;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataLong2;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

double CBHashStrK::NextItemDouble( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataDouble;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataDouble;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

LPTSTR CBHashStrK::NextItemStr( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataStr;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataStr;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

LPTSTR CBHashStrK::NextItemStr2( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� DataLong
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].DataStr2;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].DataStr2;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}


// ���ص��ַ���ָ�룬ֱ��ָ�����ڲ��� key ���ݣ����ע�⣺�����޸��˸�ָ��ָ������ݣ�
//   ���������ݵ� key Ҳͬʱ�����仯
LPTSTR CBHashStrK::NextKey( bool &bRetNotValid )
{
	// ���� StartTraversal ���ô˺������� String
    // �� bRetEndNotValid ���� True����˴α����ѽ�������ʱ��������ֵҲ��Ч��
	int idx;
	idx = TraversalGetNextIdx();
	if (idx > 0)
	{
		bRetNotValid = false;
		return mem[idx].Key;
	}
	else if (idx < 0)
	{
		bRetNotValid = false;
		return mem2[-idx].Key;
	}
	else
	{
		bRetNotValid = true;
		return 0;
	}
}

// �������Ԫ�أ��ض��� mcIniMemSize ���洢�ռ�
void CBHashStrK::Clear( void )
{
	// ���	�� Index ���ʸ�Ԫ�صĻ�������
	if (mArrTable) {delete []mArrTable; mArrTable=0;}
	mArrTableCount=-1;

	// ɾ�� mem[] �� mem2[] �е�ÿ��Ԫ�ص� key �� �ַ������� ָ��Ŀռ�
	int i;
	for (i=1; i<=memCount; i++)
	{
		if (mem[i].Key) {delete [] mem[i].Key; mem[i].Key=0; }
		if (mem[i].DataStr) {delete [] mem[i].DataStr; mem[i].DataStr=0; }
		if (mem[i].DataStr2) {delete [] mem[i].DataStr2; mem[i].DataStr2=0; }
	}
	for (i=1; i<=memCount2; i++)
	{
		if (mem2[i].Key) {delete [] mem2[i].Key; mem2[i].Key=0; }
		if (mem2[i].DataStr) {delete [] mem2[i].DataStr; mem2[i].DataStr=0; }
		if (mem2[i].DataStr2) {delete [] mem2[i].DataStr2; mem2[i].DataStr2=0; }
	}

	// ɾ�� mem[] �� mem2[] �Ŀռ�
	delete [] mem; mem=0;
	delete [] mem2; mem2=0;
	memCount = 0; memUsedCount = 0;
	memCount2 = 0; memUsedCount2 = 0;

	// ���¿��ٿռ�
	RedimArrMemType(mem, mcIniMemSize, memCount, false);
	memCount = mcIniMemSize;

	mTravIdxCurr = 0;
}


// ���ع���Ԫ�ظ���
int CBHashStrK::Count( void )
{
	return memUsedCount + memUsedCount2;
}




//////////////////////////////////////////////////////////////////////
// ˽�з���
//////////////////////////////////////////////////////////////////////

void CBHashStrK::ReLocaMem( int preMemCountTo )
{
	/*
	���·��� mem[], mem2[] �ĸ�Ԫ�صĿռ䣬mem2[] ��ĳЩԪ�ؿ��ܱ� \
	�����ƶ��� mem
	���޸� memUsedCount,memUsedCount2, memCount2, mem2[] ��ֵ
	preMemCountTo ֻ���� mem[1 to preMemCountTo]��preMemCountTo �Ժ��Ԫ�ر���Ϊ \
	δ�ã������ǣ���������ζ����� mem2[] �е�����Ԫ��
	*/
	
	// �� mem[] �е���ʹ��Ԫ�غ� mem2[] �е�����Ԫ���ȷ��� memUsed[] �У� \
	// �� memUsed[] ����Ϊ�㹻��ʵ�� memUsed[] ֻʹ���� lngUsedCount ��Ԫ��
	MemType * memUsed;
	RedimArrMemType(memUsed, preMemCountTo + memUsedCount2);

	int iUsedCount=0;		
	int i;

	// �� mem[] ����ʹ�õ�Ԫ�ش��� memUsed[]
	for (i=1; i<=preMemCountTo; i++)
		if (mem[i].Used)
		{
			iUsedCount = iUsedCount + 1;
			memUsed[iUsedCount] = mem[i];
		}

	// �� mem2[] �е�����Ԫ�ش��� memUsed[]
	for (i=1; i<=memUsedCount2; i++)
	{
		iUsedCount = iUsedCount + 1;
		memUsed[iUsedCount] = mem2[i];
	}


	/*
	��ʱ memUsed[1 To lngUsedCount] ��Ϊ���� mem[] \
	�е���ʹ��Ԫ�� �� mem2[] �е�����Ԫ��
	*/

	// ��� mem��Ҳ��������� Used ��
	RedimArrMemType(mem, memCount, memCount, false); 
	memUsedCount=0;  // memUsedCount ��0���������ƶ���+1

	// ��� mem2��Ҳ��������� Used ��
	RedimArrMemType(mem2, -1, memCount2, false);
	memCount2 = 0;
	memUsedCount2 = 0; 

	// ����� memUsed[1 To lngUsedCount] �е�Ԫ�ذ��������Сӳ���±���� mem[]
	int idx;
	for (i=1; i<=iUsedCount; i++)
	{
		idx = AlloMemIndex(memUsed[i].Key, false);
		if (idx > 0)
		{
			mem[idx] = memUsed[i];
			mem[idx].Used = 1;
			memUsedCount = memUsedCount + 1;
		}
		else
		{
			mem2[-idx] = memUsed[i];
			mem2[-idx].Used = 1;
			memUsedCount2 = memUsedCount2 + 1;
		}
	}

	// ɾ����ʱ�ռ� memUsed
	delete [] memUsed; memUsed=0;

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}


// �ض��� mem[] �����С������ mem[] �Ŀռ�
void CBHashStrK::ExpandMem( void )
{
	int iCount, iPreMemCount;

	// �����ϣ���й�����������
	iCount = memUsedCount + memUsedCount2;

	// ȡ�����������������͡���ǰ mem[] �Ŀռ����������ߵĽϴ�ֵ
	if (iCount < memCount) iCount = memCount;

	// ���������ռ�֮ǰ�ġ�ԭ���� mem[] �Ŀռ�����
	iPreMemCount = memCount;

	if (iCount<1) iCount=1;		// ���� iCount Ϊ0ʱ���޷�����ռ�
	if (iCount < mcExpandCountThres)
	{
		// ��������������Ƚ��١����������ռ�Ϊԭ����2��
		iCount = iCount * 2;

	}
	else if (iCount < mcExpandCountThresMax)
	{
		// ������������Ѿ����е�ࡱ���������ռ�Ϊԭ����1.5��
		iCount = iCount * 3 / 2;
	}
	else
	{
		// ��������������ܶࡱ�������� mcExpandBigPer ���ռ�
		iCount = iCount + mcExpandBigPer;
	}

	// �ض��������С
	// ReDim Preserve mem(1 To lngCount)
	RedimArrMemType(mem, iCount, memCount, true);
	memCount = iCount;

	// ���������С�����°��������������ݵ���λ�ã�������Ҫ����
	// �����ռ�֮ǰ�ġ�ԭ���� mem[] �Ŀռ�����
	ReLocaMem(iPreMemCount);

	// ��ϣ�������ָ������
	mTravIdxCurr = 0;
	
	// �� Index ���ʸ�Ԫ�صĻ�����������
    mArrTableCount = -1;
}


// ������ϣ�������ݴ��� mArrTable()������ mArrTableCount Ϊ���ݸ��������سɹ���ʧ�ܣ�
bool CBHashStrK::RefreshArrTable()
{
	int iCount;
	int i,j;
	
	// �����ϣ���й�����������
	iCount = memUsedCount + memUsedCount2;
	
	mArrTableCount=iCount;
	if (mArrTableCount<=0) return false;
	
	if (mArrTable) {delete []mArrTable; mArrTable=0;}
	mArrTable=new int [iCount+1];	// ʹ�����±��1��ʼ
	memset(mArrTable, 0, sizeof(int)*(iCount+1));
	
	j=1;
	for (i=1; i<=memCount; i++)
	{
		if (mem[i].Used)
		{
			if (j > iCount) return false;
			
			mArrTable[j] = i;		// �� ��ϣ��ı��������ڵ� mem[] ���±� i ���� mArrTable[j]
			mem[i].Index = j;		// �� ��ϣ��ı����� �� Index ��Ա�м�¼ mArrTable ���±� j
			
			j=j+1;
		}
	}
	
	for (i=1; i<=memUsedCount2; i++)
	{
		if (mem2[i].Used)
		{
			if (j > iCount) return false;
			
			mArrTable[j] = -i;		// �� ��ϣ��ı��������ڵ� mem2[] ���±� i ��ȡ�������� mArrTable[j]
			mem[i].Index = j;		// �� ��ϣ��ı����� �� Index ��Ա�м�¼ mArrTable ���±� j
			
			j=j+1;
		}
	}
	
	return true;
}


int CBHashStrK::AlloSeqIdx( int fromIndex, int toIndex )
{
    /*
	 �� mem[] ��һ��ûʹ�õĿռ䣬�� fromIndex ��ʼ�� \
		�� toIndex ����
	 ���� mem[] ��һ��ûʹ��Ԫ�ص��±꣬û�ҵ����� 0
	*/
	int i;
	if (fromIndex <= 0)  fromIndex = 1;
	if (toIndex > memCount) toIndex = memCount;
	
	for (i=fromIndex; i<=toIndex; i++)
		if (! mem[i].Used) return i; 

	return 0;
}

int CBHashStrK::AlloMemIndex( LPCTSTR key, bool CanExpandMem/*=true */ )
{
	/* 
	  ���� Key ����һ�� mem[] �е�δ�ô洢�ռ䣬���� mem[] �����±�
		���ظ�ֵ������ mem[] ���ҵ��ռ䣺����ֵ�ľ���ֵΪ mem2[] �� \
		��һ�������±�ռ䣨mem2[]�Զ�Redim�����Դ��� mem2[]
	  ������ȷ������һ����ʹ�õĿռ䣬���������� mem2[] �еĿռ�
	  �����������޸� memUsedCount2 ��ֵ���� redim mem2[]
		CanExpandMem=true ʱ�����������Զ����� mem[]�����򲻻��Զ�����
		
		���������ַ����� key ת��Ϊ���������Ϊ������ȡ����ֵ��Ȼ��
		1. ���� Key Mod memCount + 1���� Index -> idxMod
		2. �������Ԫ���Ѿ�ʹ�ã���Key�Ƿ� < cMaxNumForSquare (sqr(2^31)=46340) \
		�� <����ƽ�� Key��Ȼ�� mod memCount + 1�� \
		�� >=�����ð�λ����λ���㣬����� key �ĸ������֣�Ȼ�� mod memCount + 1
		��������������˲� Index -> idxSq
		3. �� memCount-idxMod+1 -> idxModRev
		4. �� memCount-idxSq+1 -> idxSqRev
		5. �������ҵ��� Index ����ʹ���ˣ��� Count �Ƿ� > \
		mcExpandMaxPort*Count�����ǣ��� CanExpandMem=true�� \
		������ mem[] �Ĵ洢�ռ䣬Ȼ��ݹ鱾���̣��ظ� 1-4 ��
		6. �� idxMod+1,+2,...,+mcSeqMax���� idxMod-1,-2,...,-mcSeqMax
		7. ��û�У����ظ�ֵ������ֵΪ mem2[] ����һ�����ÿռ䣬�Դ��� mem2[]
	*/


	const int cMaxNumForSquare = 46340;
	
	int idxMod=0, idxSq=0;
    int idxModRev=0, idxSqRev=0;
    int iCount=0;
    long keyToCalc = KeyStringToLong(key); // ������ Key����ԶΪ>0����
    if (keyToCalc < 0) keyToCalc = 0 - keyToCalc;	// ��� Key �Ǹ�ֵ����ת��Ϊ�����������Ĵ洢�ռ�
    iCount = memUsedCount + memUsedCount2;
    
	if (memCount) 
	{	
		// 1: ���� Key Mod memCount + 1���� Index -> idxMod
		idxMod = keyToCalc % memCount + 1;
		if (! mem[idxMod].Used) return idxMod;
    
		// 2: �� ƽ��Key ���ٳ���ȡ�࣬�� Index -> idxSq
		if (keyToCalc <= cMaxNumForSquare)
		{
			idxSq = (keyToCalc * keyToCalc) % memCount + 1;
		}
		else
		{
			int kBitSum=0;
			kBitSum = (keyToCalc & 0xFFFF0000)>>16;
			kBitSum += (keyToCalc & 0xFF00)>>8;
			kBitSum += (keyToCalc & 0xF0)>>4;
			kBitSum += (keyToCalc & 0xF);
			idxSq = kBitSum % memCount + 1;
		}
		if (! mem[idxSq].Used) return idxSq;

		// 3: ���Ե����� idxMod ���ռ� -> idxModRev
		idxModRev = memCount - idxMod + 1; 
		if (! mem[idxModRev].Used) return idxModRev;
    
		// 4: ���Ե����� idxSq ���ռ� -> idxSqRev
		idxSqRev = memCount - idxSq + 1;
		if (! mem[idxSqRev].Used) return idxSqRev;
	}
    
    // 5: ����ռ�ʹ�ðٷֱȳ�����ֵ�������� mem[] �� �ռ�
    if (CanExpandMem && iCount > mcExpandMaxPort * memCount)
	{
		ExpandMem();  // ���� mem[] �Ŀռ�
		return AlloMemIndex(key, CanExpandMem); // �ݹ飬�ظ�1-4��
	}
        
    
    int lngRetIdx;
    
    // 6: �� idxMod ��ʼ��ǰ������������� mcSeqMax ���ռ�
    int idxMdSta, idxMdEnd; 
    idxMdSta = idxMod - mcSeqMax; idxMdEnd = idxMod + mcSeqMax;
    lngRetIdx = AlloSeqIdx(idxMdSta, idxMod - 1); 
    if (lngRetIdx > 0) return lngRetIdx;
    lngRetIdx = AlloSeqIdx(idxMod + 1, idxMdEnd);
    if (lngRetIdx > 0) return lngRetIdx; 
    

    // 7: ���ظ�ֵ������ֵΪ mem2[] ����һ��Ԫ�أ��Դ��� mem2[]
    if (memUsedCount2 + 1 > memCount2)
    {    
        // ReDim Preserve mem2(1 To mcExpandMem2Per)
		RedimArrMemType(mem2, memCount2 + mcExpandMem2Per, memCount2, true);
		memCount2 = memCount2 + mcExpandMem2Per;
	}

    return -(memUsedCount2 + 1);
}


int CBHashStrK::FindSeqIdx( LPCTSTR key, int fromIndex, int toIndex )
{
	
    // �� mem[] �м�ΪKey��Ԫ���±꣬�� fromIndex ��ʼ�� \
	//	�� toIndex ����
	//	���� mem[] ���ҵ������±꣨>0����û�ҵ����� 0

	int i;
	if (fromIndex < 1) fromIndex = 1;
	if (toIndex > memCount) toIndex = memCount;

	for (i=fromIndex; i<=toIndex; i++)
		if ((mem[i].Used) && CompareKey(mem[i].Key, key) )
			return i;
	
	return 0;
}




int CBHashStrK::TraversalGetNextIdx( void )
{
	// �� NextXXX ��������ʱ��������һ����Next���� mem[]�±꣨����ֵ>0���� \
	// �� mem2[] ���±꣨����ֵ<0�������ѱ�������������ֵ=0��
	
	int iRetIdx;
	
	if (mTravIdxCurr > memCount ||
		-mTravIdxCurr > memCount2 ||
		mTravIdxCurr == 0) return 0;
	
	if (mTravIdxCurr > 0)
	{
		//////////// �� mem[] ���� ////////////
		while (! mem[mTravIdxCurr].Used)
		{
			mTravIdxCurr = mTravIdxCurr + 1;
			if (mTravIdxCurr > memCount) break;
		}
		
		if (mTravIdxCurr > memCount)
		{
			//// �ѱ������������� mem2[] �л������ݼ������� mem2[] ////
			if (memCount2 > 0)
			{
				// �����´α��� mem2[] �����ݵ��±�ĸ���
				mTravIdxCurr = -1;
				// ִ������� if mTravIdxCurr < 0 Then 
			}
			else
			{
				// ���ؽ���
				iRetIdx = 0;
				return iRetIdx;
			}
		}
		else
		{
			//// ���� mTravIdxCurr ////
			iRetIdx = mTravIdxCurr;
			// �����´α���ָ�� ָ����һ��λ�ã����� mem[] ����һ���� \
			// ���� mem2[] ����ʼ��
			mTravIdxCurr = mTravIdxCurr + 1;
			if (mTravIdxCurr > memCount) if (memCount2 > 0) mTravIdxCurr = -1;
			return iRetIdx;
		}
	}
	
	if (mTravIdxCurr < 0)
	{
		//////////// �� mem2[] ���� ////////////
		while (! mem2[-mTravIdxCurr].Used)
		{	
			mTravIdxCurr = mTravIdxCurr - 1;
			if (-mTravIdxCurr > memCount2) break;
		}
		
		if (-mTravIdxCurr > memCount2)
		{
			//// �ѱ������� ////
			// ���ؽ���
			iRetIdx = 0; 
		}
		else
		{
			// ���ظ�ֵ�� mTravIdxCurr
			iRetIdx = mTravIdxCurr;
			// ���� mTravIdxCurr ��ָ��
			mTravIdxCurr = mTravIdxCurr - 1;
		}
		return iRetIdx;
	}
	
	return 0;
}




// �ض��� һ�� MemType ���͵����飨������� mem[] �� mem2[]���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�mem �� mem2�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBHashStrK::RedimArrMemType( MemType * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		MemType * ptrNew = new MemType [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(MemType)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� MemType Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(MemType)*ctToCpy); 
		}

		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;

		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}


// �� new �������ַ����ռ䣬�� ptrNewString ָ����ַ����������¿ռ䣻
//   ptrSaveTo ��һ�������ַ�����ַ��ָ������ĵ�ַ����ָ���ָ�����������
//   ���� new ���ٵ����ַ����ռ�ĵ�ַ�������� ��*ptrSaveTo = �¿ռ��ַ��
// �����ͷš�*ptrSaveTo����ָ��Ŀռ�Ĺ���
//   �� ptrSaveTo �����ɱ����� &(mem[i].key) ��ָ���ָ�룻ptrNewString �ɱ������µ� key
//   ����ɡ�mem[i].key=key���Ĳ������������޸� mem[i].key �����ݣ�
//   ��ɾ������ָ��Ŀռ䣬������ָ���¿ռ�
// ��� key Ϊ��ָ�룬���ͷš�*ptrSaveTo����ָ��Ŀռ�
void CBHashStrK::SaveItemString( TCHAR ** ptrSaveTo, LPCTSTR ptrNewString )
{
	// ע�� ptrSaveTo �Ǹ�����ָ��
	if (ptrSaveTo==0) return;  // û�б����λ��
	
	// ��� ptrSaveTo ָ���ָ�������Ϊ����ָ�롱����ʾҪ����֮������
	//   ������һ����ǰ���ٵĿռ��ַ��Ӧ��ɾ����ǰ���ٵĿռ�
	if (*ptrSaveTo != 0) {delete [] (*ptrSaveTo); *ptrSaveTo=0; }

	if (ptrNewString)
	{
		// �����¿ռ䣬���� ptrNewString ����ַ������¿ռ�
		TCHAR * p = new TCHAR [_tcslen(ptrNewString)+1];
		_tcscpy(p, ptrNewString);
		
		// ʹ *ptrSaveTo ָ���¿ռ�
		*ptrSaveTo = p;
	}
}


// �� Key ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0
int CBHashStrK::GetMemIndexFromKey( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	const long cMaxNumForSquare = 46340;  // sqrt(2^31)=46340
    
    int idxMod=0, idxSq=0;
    int idxModRev=0, idxSqRev=0;
    long keyToCalc = KeyStringToLong(key); // ������ Key����ԶΪ>=0����
    if (keyToCalc < 0) keyToCalc = 0 - keyToCalc;

	if (memCount)
	{
		// 1: ���� Key Mod memCount + 1���� Index -> idxMod
		idxMod = keyToCalc % memCount + 1;
		if (mem[idxMod].Used && CompareKey(mem[idxMod].Key, key)) 
			return idxMod;
    
		// 2: �� ƽ��Key���ٳ���ȡ�࣬�� Index -> idxSq
		if (keyToCalc <= cMaxNumForSquare)
		{
			idxSq = (keyToCalc * keyToCalc) % memCount + 1;
		}
		else
		{
			int kBitSum=0;
			kBitSum = (keyToCalc & 0xFFFF0000)>>16;
			kBitSum += (keyToCalc & 0xFF00)>>8;
			kBitSum += (keyToCalc & 0xF0)>>4;
			kBitSum += (keyToCalc & 0xF);
			idxSq = kBitSum % memCount + 1;
		}
		if (mem[idxSq].Used && CompareKey(mem[idxSq].Key, key))
			return idxSq;
    
		// 3: ���Ե����� idxMod ���ռ� -> idxModRev
		idxModRev = memCount - idxMod + 1;
		if (mem[idxModRev].Used && CompareKey(mem[idxModRev].Key, key))
		   return idxModRev;
    
		// 4: ���Ե����� idxSq ���ռ� -> idxSqRev
		idxSqRev = memCount - idxSq + 1;
		if (mem[idxSqRev].Used && CompareKey(mem[idxSqRev].Key, key))
		   return idxSqRev;
	}

    int lngRetIdx;
    
    // 6: �� idxMod ��ʼ��ǰ������������� mcSeqMax ���ռ�
    int idxMdSta, idxMdEnd;
    idxMdSta = idxMod - mcSeqMax; idxMdEnd = idxMod + mcSeqMax;
    lngRetIdx = FindSeqIdx(key, idxMdSta, idxMod - 1);
    if (lngRetIdx > 0)  return  lngRetIdx;
    lngRetIdx = FindSeqIdx(key, idxMod + 1, idxMdEnd);
    if (lngRetIdx > 0)  return  lngRetIdx;

    // 7: �ٲ鿴 mem2[] �е�Ԫ����û��
    for (int i=1; i<=memUsedCount2; i++)
        if (mem2[i].Used && CompareKey(mem2[i].Key, key)) return -i;
	
	if (raiseErrorIfNotHas) throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
	return 0;
}

// �� index ��������� mem[] �е��±꣨����ֵ>0������ mem2[] �е��±꣨����ֵ<0���������� 0
int CBHashStrK::GetMemIndexFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	if (mArrTableCount != memUsedCount + memUsedCount2) RefreshArrTable(); // ˢ�����黺��
	if (index<1 || index>mArrTableCount)
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)5;	// ��Ч�Ĺ��̵��û����
		return 0;
	}
	
	int idx=mArrTable[index];
	if (idx==0) 
	{
		if (raiseErrorIfNotHas)  throw (unsigned char)7;	// �����ڴ棨mArrTable[index]����Ϊ0��
		return 0;
	}
	else
		return idx;
}


// ��һ�� �ַ������͵� key ת��Ϊһ��������
// �����ǣ�ȡǰ16���ַ�����16���ַ�������32���ַ���ASCII�������ͷ���
long CBHashStrK::KeyStringToLong( LPCTSTR key )
{
	// �����ǣ�ȡǰ16���ַ�����16���ַ�������32���ַ���ASCII�������ͷ���
    const int c_CalcCharNumHead = 16;
    const int c_CalcCharNumTail = 16;

	long lResult=0;
	int iHeadToPos=0;	// ʵ��Ҫȡǰ�����ַ�
	int iTailToPos=0;	// ʵ��Ҫȡ�󼸸��ַ�
	int i;

	TCHAR * keyLoc = new TCHAR[_tcslen(key)+1];		// ������ key
	_tcscpy(keyLoc, key);							// ���� key �ַ������¿��ٵĿռ����������� key ����Ҫ������ KeyCaseSensitive ����ת����Сд
	if (! KeyCaseSensitive)  _tcsupr(keyLoc);		// ��������ִ�Сд���� key ת��Ϊ ��д
	
	// ��ʼ���ۼ� ASCII ��Ľ��
    lResult = 0;

	// �ۼ�ǰ c_CalcCharNumHead ���ַ��� ASCII �루���� c_CalcCharNumHead �������ַ�������Ϊֹ��
	iHeadToPos = c_CalcCharNumHead;
	if ((int)_tcslen(keyLoc) < iHeadToPos) iHeadToPos = _tcslen(keyLoc);
	for(i=0; i<iHeadToPos; i++)
		lResult += *(keyLoc+i);

	// �ۼӺ� c_CalcCharNumTail ���ַ��� ASCII �루���� c_CalcCharNumTail �������ַ�������Ϊֹ��
	iTailToPos = c_CalcCharNumTail;
	if ((int)_tcslen(keyLoc) - iHeadToPos < iTailToPos) iTailToPos = _tcslen(keyLoc) - iHeadToPos;
	for(i = (int)_tcslen(keyLoc) - iTailToPos; i<(int)_tcslen(keyLoc); i++)
		lResult += *(keyLoc+i);

	// �ͷſռ�
	delete []keyLoc;


	// ���ؽ�����ۼӵ� ASCII ��
	return lResult;
}


// ���� lKeyCaseSensitive ���ԣ��Ƚ��� key ���ַ����Ƿ����
// ��ȷ���True�����ȷ���False
bool CBHashStrK::CompareKey(LPCTSTR key1, LPCTSTR key2 )
{
	if (key1==0 || key2==0) return false;
	if (KeyCaseSensitive)
		return lstrcmp(key1, key2)==0;
	else
	{
		return lstrcmpi(key1, key2)==0;
	}
}


// ---------------- �� Index ������������(����Key����KeyΪֻ��) ----------------
// ע������������ɾ��Index ���ܻ�仯��ĳ���ݵ� Index ����������һһ��Ӧ

int CBHashStrK::ItemFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Data;
	else if (ii < 0)
		return mem2[-ii].Data;
	else
		return 0;
}

long CBHashStrK::ItemLongFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataLong;
	else if (ii < 0)
		return mem2[-ii].DataLong;
	else
		return 0;
}

long CBHashStrK::ItemLong2FromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataLong2;
	else if (ii < 0)
		return mem2[-ii].DataLong2;
	else
		return 0;		
}

double CBHashStrK::ItemDoubleFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataDouble;
	else if (ii < 0)
		return mem2[-ii].DataDouble;
	else
		return 0;
}

LPTSTR CBHashStrK::ItemStrFromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataStr;
	else if (ii < 0)
		return mem2[-ii].DataStr;
	else
		return 0;	
}

LPTSTR CBHashStrK::ItemStr2FromIndex( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].DataStr2;
	else if (ii < 0)
		return mem2[-ii].DataStr2;
	else
		return 0;
}

LPTSTR CBHashStrK::IndexToKey( int index, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Key;
	else if (ii < 0)
		return mem2[-ii].Key;
	else
		return 0;		
}


int CBHashStrK::KeyToIndex( LPCTSTR key, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	if (mArrTableCount != memUsedCount + memUsedCount2) RefreshArrTable(); // ˢ�����黺��
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii > 0)
		return mem[ii].Index;
	else if (ii < 0)
		return mem2[-ii].Index;
	else
		return 0;		
}



// ---------------- �� Key ������������ ----------------

bool CBHashStrK::ItemSet( LPCTSTR key, int vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].Data = vNewValue;
	else if (ii < 0)
		mem2[-ii].Data = vNewValue;
	return true;	
}

bool CBHashStrK::ItemLong2Set( LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong2 = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong2 = vNewValue;
	return true;	
}

bool CBHashStrK::ItemLongSet( LPCTSTR key, long vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong = vNewValue;
	return true;	
}

bool CBHashStrK::ItemDoubleSet( LPCTSTR key, double vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataDouble = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataDouble = vNewValue;
	return true;	
}

bool CBHashStrK::ItemStrSet( LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr), vNewValue);
	return true;	
}

bool CBHashStrK::ItemStr2Set( LPCTSTR key, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromKey(key, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr2), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr2), vNewValue);
	return true;	
}


// ---------------- �� Index ������������(KeyΪֻ����������Key) ----------------
// ע������������ɾ��Index ���ܻ�仯��ĳ���ݵ� Index ����������һһ��Ӧ

bool CBHashStrK::ItemFromIndexSet( int index, int vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].Data = vNewValue;
	else if (ii < 0)
		mem2[-ii].Data = vNewValue;
	return true;	
}

bool CBHashStrK::ItemLongFromIndexSet( int index, long vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong = vNewValue;
	return true;	
}

bool CBHashStrK::ItemLong2FromIndexSet( int index, long vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataLong2 = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataLong2 = vNewValue;
	return true;	
}

bool CBHashStrK::ItemDoubleFromIndexSet( int index, double vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		mem[ii].DataDouble = vNewValue;
	else if (ii < 0)
		mem2[-ii].DataDouble = vNewValue;
	return true;	
}

bool CBHashStrK::ItemStrFromIndexSet( int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr), vNewValue);
	return true;	
}

bool CBHashStrK::ItemStr2FromIndexSet( int index, LPCTSTR vNewValue, bool raiseErrorIfNotHas/*=true*/ )
{
	int ii;
	ii=GetMemIndexFromIndex(index, raiseErrorIfNotHas);
	if (ii == 0)
		return false;
	else if (ii > 0)
		SaveItemString(&(mem[ii].DataStr2), vNewValue);
	else if (ii < 0)
		SaveItemString(&(mem2[-ii].DataStr2), vNewValue);
	return true;
}




//////////////////////////////////////////////////////////////////////
// CBRecycledArr ���ʵ�֣�������վ������
//
//////////////////////////////////////////////////////////////////////

// ===================================================================================
// �ַ������ݣ�
//   �� Add ʱ�� new �ռ䣬���� mem[] �Ŀռ�ʱ���� mem[] Ԫ�ص��ַ���ָ�벻 = new
//   �� Remove ʱ��delete �ռ䣬Ȼ�󱣴浽������վ��������վ�е������ַ���ָ�붼ΪNULL
//   ������ʱ��delete �ռ�
// ===================================================================================

//////////////////////////////////////////////////////////////////////
// Static ������ֵ
//////////////////////////////////////////////////////////////////////
const int   CBRecycledArr::mcIniMemSize = 7;				// ��ʼ mem[] �Ĵ�С
const int   CBRecycledArr::mcMaxItemCount = 100000000;		// ���Ԫ�ظ����������ڴ�ԭ��2G�ڴ�֮�ڣ�sizeof(MemType)*��ֵ���ó���2G��
const float CBRecycledArr::mcExpandMaxPort = 0.75;			// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
const int   CBRecycledArr::mcExpandCountThres = 10000;		// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
const int   CBRecycledArr::mcExpandCountThresMax = 10000000;// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBRecycledArr::mcExpandBigPer = 1000000;		// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int	CBRecycledArr::mcRecyExpandPer = 100;			// ���� recycles[] �ռ�ʱ��ÿ������Ĵ�С
		

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBRecycledArr::CBRecycledArr(int initSize/*=0*/)
{
	// ��ʼ����Ա������ֵ
	mem = 0;
	memCount = 0; memUsedCount = 0;
	recycles = 0;
	recyCount = 0; recyUsedCount = 0;
	mTravIdxCurr = 0;

	// ���ٳ�ʼ�ռ�
	if (initSize)
	{
		// ���� initSize ��С�Ŀռ�
		RedimArrMemType(mem, initSize);
		memCount = initSize;
	}
	else
	{
		// ���� mcIniMemSize ��С�Ŀռ�
		RedimArrMemType(mem, mcIniMemSize);
		memCount = mcIniMemSize;
	}
}

CBRecycledArr::~CBRecycledArr()
{
	Clear();
	// Clear() �������Զ������� mcIniMemSize ��С�Ŀռ䣬����ɾ��������
	delete []mem; memCount=0;
} 





//////////////////////////////////////////////////////////////////////
// ���к���
//////////////////////////////////////////////////////////////////////

// ��������ݣ����������ݱ����浽�� mem[] �е��±꣨>0����������0
int CBRecycledArr::Add( LPCTSTR dataString /*= NULL*/, int dataInt /*= 0*/, int dataInt2 /*= 0*/, float dataFloat /*= 0.0*/, double dataDouble /*= 0.0*/ )
{
	int idx = 0;  // Ҫ�������ݴ��� mem[idx]

	// ======== Ϊ�����ݷ����¿ռ䣬��ÿռ��±� idx ========
	if (recycles!=0 && recyUsedCount>0)
	{
		// ����վ���пռ䣬ʹ�û���վ�е����һ���ռ䣻����վʣ�ಿ����ά���Ŵ�С��������
		idx = recycles [recyUsedCount];
		recyUsedCount--;
	}
	else
	{
		// ����վ��û�пռ䣬ʹ�� mem[idx] �е���һ�����ÿռ�
		if (memUsedCount+1 <= memCount)
		{
			memUsedCount++;
			idx = memUsedCount;
		}
		else
		{
			// Redim preserve mem[1 to memCount + һ��] 
			ExpandMem();
			// ����ռ���ٴ��ж�
			if (memUsedCount+1 <= memCount) 
			{
				memUsedCount++;
				idx = memUsedCount;
			}
			else
			{
				// ����ռ�󻹲��� memUsedCount <= memCount���޷�����ռ䣬���س���
				return 0;		
			}
		}
	}

	// ======== ���������� ========
	if (dataString)
	{
		mem[idx].DataString = new TCHAR [_tcslen(dataString) + 1];
		_tcscpy(mem[idx].DataString, dataString);
	}
	else
	{
		mem[idx].DataString = NULL;
	}
	mem[idx].DataInt = dataInt;
	mem[idx].DataInt2 = dataInt2;
	mem[idx].DataFloat = dataFloat;
	mem[idx].DataDouble = dataDouble;
	mem[idx].Used = true;

	// ���������ݱ����浽�� mem[] �е��±�
	return idx;
}


bool CBRecycledArr::Remove( int index )
{
	int idx, i;

	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount) return false;	// index ���Ϸ�
	if (! mem[index].Used) return false;			// �����ѱ�ɾ��

	// ======== ��������ֵΪ�� ========
	if (mem[index].DataString)	// ɾ�������ݵ��ַ�����ռ�õĿռ䣨����еĻ���
	{
		delete [] mem[index].DataString;
		mem[index].DataString = NULL;
	}
	mem[index].DataInt = 0;
	mem[index].DataInt2 = 0;
	mem[index].DataFloat = 0.0;
	mem[index].DataDouble = 0.0;
	mem[index].Used = false;

	// ======== �ڻ���վ�м�¼�ÿռ��ѱ�ɾ�� ========
	// �ڻ���վ�б����¼ index��ά������վ recycles[] �����С��������

	if (recycles==0 || recyUsedCount + 1 > recyCount)
	{
		// �������վ�еĿռ�
		RedimArrInt(recycles, recyCount+mcRecyExpandPer, recyCount, true);
		recyCount = recyCount+mcRecyExpandPer;
	}
	
	// ʹ�ö��ֲ����ҵ� index Ӧ�����浽�� recycles[] �±�ĸ���
	idx=FindPosInSortedRecycle(index);	
	
	// ���� + 1
	recyUsedCount++;

	// ����������
	if (idx<0)
	{
		// ���������index Ӧ�����浽�� recycles[-idx]
		for(i=recyUsedCount; i>-idx; i++)
			recycles[i]=recycles[i-1];
		recycles[-idx] = index;
	}
	else
	{
		// �������������վ���Ѿ�����ֵΪ index �����Ԫ�أ�����ֲ��ҳ���
		// �ݴ���ʹ�� recycles[idx] ���� index
		recycles[idx] = index;
	}

	// ���سɹ�
	return true;
}


LPTSTR CBRecycledArr::Item( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;	
	}

	return mem[index].DataString;
}


int CBRecycledArr::ItemInt( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;	
	}

	return mem[index].DataInt;
}

int CBRecycledArr::ItemInt2( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;	
	}
	
	return mem[index].DataInt2;
}

float CBRecycledArr::ItemFloat( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;	
	}
	
	return mem[index].DataFloat;
}

double CBRecycledArr::ItemDouble( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	index=UserIndexToMemIndex(index);
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;	
	}
	
	return mem[index].DataDouble;
}



void CBRecycledArr::Clear()
{
	// ��ɾ�������ַ����Ŀռ�
	int i;
	for(i=1; i<=memCount; i++)
		if (mem[i].DataString)
		{
			delete [] mem[i].DataString;
			mem[i].DataString = NULL;
		}

	// ɾ�� mem ����Ŀռ�� ����վ ����Ŀռ�
	if (mem) delete []mem;
	if (recycles) delete []recycles;

	mem = 0; memCount = 0; memUsedCount = 0;
	recycles = 0; recyCount = 0; recyUsedCount = 0;
	mTravIdxCurr = 0;

	// ���� mcIniMemSize ��С�Ŀռ�
	RedimArrMemType(mem, mcIniMemSize);
	memCount = mcIniMemSize;
}



void CBRecycledArr::StartTraversal()
{
	mTravIdxCurr = 1;
}


LPTSTR CBRecycledArr::NextItem( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataString;}
	}

	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0;
}


int CBRecycledArr::NextDataInt( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataInt;}
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0;
}

int CBRecycledArr::NextDataInt2( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataInt2;}
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0;
}

float CBRecycledArr::NextDataFloat( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataFloat;}
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0.0;
}

double CBRecycledArr::NextDataDouble( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataDouble; }
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0.0;
}


// ���ص�ǰӵ�е����ݸ���
int CBRecycledArr::Count()
{
	int ct = memUsedCount-recyUsedCount;
	if (ct<0) ct = 0;
	return ct;
}



//////////////////////////////////////////////////////////////////////
// ˽�к���
//////////////////////////////////////////////////////////////////////

void CBRecycledArr::ExpandMem( void )
{
	int toCount = 0;
	toCount=memCount;

	if (toCount < 1) toCount = 1;	// ���� toCount Ϊ0�޷�����ռ�
	if (toCount < mcExpandCountThres)
	{
		// ��������������Ƚ��١����������ռ�Ϊԭ����2��
		toCount = toCount * 2;
	}
	else if (toCount < mcExpandCountThresMax)
	{
		// ������������Ѿ����е�ࡱ���������ռ�Ϊԭ����1.5��
		toCount = toCount * 3 / 2;
	}
	else
	{
		// ��������������ܶࡱ�������� mcExpandBigPer ���ռ�
		toCount = toCount + mcExpandBigPer;
	}
	
	// �ض��������С
	// ReDim Preserve mem(1 To toCount)
	RedimArrMemType(mem, toCount, memCount, true);
	memCount = toCount;
}




// �ض��� һ�� MemType ���͵����飨������� recycles���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�recycles�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBRecycledArr::RedimArrMemType( MemType * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		MemType * ptrNew = new MemType [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(MemType)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� MemType Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(MemType)*ctToCpy); 
		}
		
		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}


// �ض��� һ�� int ���͵����飨������� mem[]���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�mem�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBRecycledArr::RedimArrInt( int * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		int * ptrNew = new int [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(int)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� int Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(int)*ctToCpy); 
		}
		
		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}



// �ö��ֲ��ҷ������� recycles �в���Ԫ�� itemToFind ��λ��
// ���� recycles �Ѱ���С�������������±�Ҫ��1��ʼ, recyUsedCount ͬʱΪ�±��Ͻ�
// ��������ֵ��
//   1. ���ҵ�Ԫ���������д��ڣ����ش��ڵ��±꣨>0����
//   2. �粻����Ҫ�ҵ�Ԫ�أ��������ظ�����<0���������ֵΪά�����������Ҫ�������λ��
//      (��Ҫ�����λ�������һ��Ԫ��֮�󣬺������صĸ����ľ���ֵΪԭ����±�+1)
//   3. ������ 0
int CBRecycledArr::FindPosInSortedRecycle( int itemToFind )
{
	int iStart=1, iEnd=recyUsedCount;
	int i;
	
	if (recyUsedCount<1) return -1;	// ���黹û��Ԫ�أ�����-1����ʾ��һ������Ҫ�����뵽�±�Ϊ [1] ��λ�ã������±��1��ʼ��
	while(iStart<=iEnd)
	{
		i = iStart+(iEnd-iStart)/2;
		if (itemToFind > recycles[i])
			iStart = i + 1;
		else if(itemToFind < recycles[i])
			iEnd = i - 1;
		else
			return i;		// �ҵ�Ԫ�أ��±�Ϊ i
	}
	
	// û���ҵ�������Ҫ�����λ�ã�ֱ�ӷ��� iEnd+1 �ĸ�������
	return -(iEnd+1);
}


// �����û� index������� mem[] �е��±ꣻɾ�����ݺ󣬺������� index ���Զ�������ʹ index ��Ϊ 1��Count
// ������ 0
int CBRecycledArr::UserIndexToMemIndex( int index )
{
	// ����� ����վ �м�¼����Щ�±��У��� index С���м��� => i
	int i=FindPosInSortedRecycle(index);
	if (i<0) 
		i=-i-1;
	// else ֱ�ӷ��� index+i
	return index+i;
}








//////////////////////////////////////////////////////////////////////
// CBRecycledArrInt ���ʵ�֣����Ͱ������վ������
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Static ������ֵ
//////////////////////////////////////////////////////////////////////
const int   CBRecycledArrInt::mcIniMemSize = 7;					// ��ʼ mem[] �Ĵ�С
const int   CBRecycledArrInt::mcMaxItemCount = 100000000;		// ���Ԫ�ظ����������ڴ�ԭ��2G�ڴ�֮�ڣ�sizeof(MemType)*��ֵ���ó���2G��
const float CBRecycledArrInt::mcExpandMaxPort = 0.75;			// ����Ԫ�ظ������� 0.75*memCount ʱ������ mem[] �Ŀռ�
const int   CBRecycledArrInt::mcExpandCountThres = 10000;		// ���� mem[] �ռ�ʱ���� memCount С�ڴ�ֵ��ÿ������ memCount*2���� memCount ���ڴ�ֵ��ÿ������ Count+Count/2
const int   CBRecycledArrInt::mcExpandCountThresMax = 10000000;// ���� mem[] �ռ�ʱ���� memCount �Ѵ��ڴ�ֵ����ÿ�β������� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int   CBRecycledArrInt::mcExpandBigPer = 1000000;			// ���� mem[] �ռ�ʱ���� memCount �Ѵ��� mcExpandCountThresMax����ÿ�β������󵽵� Count+Count/2����ֻ���� Count+mcExpandBigPer
const int	CBRecycledArrInt::mcRecyExpandPer = 100;			// ���� recycles[] �ռ�ʱ��ÿ������Ĵ�С
		

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBRecycledArrInt::CBRecycledArrInt(int initSize/*=0*/)
{
	// ��ʼ����Ա������ֵ
	mem = 0;
	memCount = 0; memUsedCount = 0;
	recycles = 0;
	recyCount = 0; recyUsedCount = 0;
	mTravIdxCurr = 0;

	// ���ٳ�ʼ�ռ�
	if (initSize)
	{
		// ���� initSize ��С�Ŀռ�
		RedimArrMemType(mem, initSize);
		memCount = initSize;
	}
	else
	{
		// ���� mcIniMemSize ��С�Ŀռ�
		RedimArrMemType(mem, mcIniMemSize);
		memCount = mcIniMemSize;
	}
	
}

CBRecycledArrInt::~CBRecycledArrInt()
{
	if (mem) delete []mem;
	mem = 0; memCount = 0; memUsedCount = 0;
	if (recycles) delete []recycles;
	recycles = 0; recyCount = 0; recyUsedCount = 0;

	mTravIdxCurr = 0;
} 





//////////////////////////////////////////////////////////////////////
// ���к���
//////////////////////////////////////////////////////////////////////

// ��������ݣ����������ݱ����浽�� mem[] �е��±꣨>0����������0
int CBRecycledArrInt::Add( DataType data, DataIntType dataInt /*= 0*/, DataFloatType dataFloat /*= 0.0*/, DataDoubleType dataDouble /*= 0.0*/ )
{
	int idx = 0;  // Ҫ�������ݴ��� mem[idx]

	// Ϊ�����ݷ����¿ռ䣬��ÿռ��±� idx
	if (recycles!=0 && recyUsedCount>0)
	{
		// ����վ���пռ䣬ʹ�û���վ�е����һ���ռ�
		idx = recycles [recyUsedCount];
		recyUsedCount--;
	}
	else
	{
		// ����վ��û�пռ䣬ʹ�� mem[idx] �е���һ�����ÿռ�
		if (memUsedCount+1 <= memCount)
		{
			memUsedCount++;
			idx = memUsedCount;
		}
		else
		{
			// Redim preserve mem[1 to memCount + һ��] 
			ExpandMem();
			// ����ռ���ٴ��ж�
			if (memUsedCount+1 <= memCount) 
			{
				memUsedCount++;
				idx = memUsedCount;
			}
			else
			{
				// ����ռ�󻹲��� memUsedCount <= memCount���޷�����ռ䣬���س���
				return 0;		
			}
		}
	}

	// ����������
	mem[idx].Data = data;
	mem[idx].DataInt = dataInt;
	mem[idx].DataFloat = dataFloat;
	mem[idx].DataDouble = dataDouble;
	mem[idx].Used = true;

	// ���������ݱ����浽�� mem[] �е��±�
	return idx;
}


bool CBRecycledArrInt::Remove( int index )
{
	if (index<1 || index>memCount) return false;	// index ���Ϸ�
	if (! mem[index].Used) return false;			// �����ѱ�ɾ��

	// ��������ֵΪ��
	mem[index].Data = 0;
	mem[index].DataInt = 0;
	mem[index].DataFloat = 0.0;
	mem[index].DataDouble = 0.0;
	mem[index].Used = false;

	// �ڻ���վ�м�¼�ÿռ��ѱ�ɾ��
	if (recycles==0 || recyUsedCount + 1 > recyCount)
	{
		// �������վ�еĿռ�
		RedimArrInt(recycles, recyCount+mcRecyExpandPer, recyCount, true);
		recyCount = recyCount+mcRecyExpandPer;
	}
	recyUsedCount++;
	recycles[recyUsedCount] = index;

	// ���سɹ�
	return true;
}


CBRecycledArrInt::DataType CBRecycledArrInt::Item( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;	
	}

	return mem[index].Data;
}

CBRecycledArrInt::DataIntType CBRecycledArrInt::ItemInt( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0;	
	}

	return mem[index].DataInt;
}

CBRecycledArrInt::DataFloatType CBRecycledArrInt::ItemFloat( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;	
	}
	
	return mem[index].DataFloat;
}

CBRecycledArrInt::DataDoubleType CBRecycledArrInt::ItemDouble( int index, bool bRaiseErrIfNotHas/*=false*/ )
{
	if (index<1 || index>memCount)	// index ���Ϸ�
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;
	}
	if (! mem[index].Used) 				// �����ѱ�ɾ��
	{
		if (bRaiseErrIfNotHas) throw (unsigned char)5;			// ��Ч�Ĺ��̵��û����
		return 0.0;	
	}
	
	return mem[index].DataDouble;
}



void CBRecycledArrInt::Clear()
{
	if (mem) delete []mem;
	if (recycles) delete []recycles;

	mem = 0; memCount = 0; memUsedCount = 0;
	recycles = 0; recyCount = 0; recyUsedCount = 0;
	mTravIdxCurr = 0;

	// ���� mcIniMemSize ��С�Ŀռ�
	RedimArrMemType(mem, mcIniMemSize);
	memCount = mcIniMemSize;
}



void CBRecycledArrInt::StartTraversal()
{
	mTravIdxCurr = 1;
}


CBRecycledArrInt::DataType CBRecycledArrInt::NextItem( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].Data;}
	}

	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0;
}


CBRecycledArrInt::DataIntType CBRecycledArrInt::NextDataInt( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataInt;}
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0;
}

CBRecycledArrInt::DataFloatType CBRecycledArrInt::NextDataFloat( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataFloat;}
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0.0;
}

CBRecycledArrInt::DataDoubleType CBRecycledArrInt::NextDataDouble( bool &bRetNotValid )
{
	bRetNotValid = true;	// Ԥ�践�����ݳɹ�
	for (; mTravIdxCurr<=memCount; mTravIdxCurr++)
	{
		if (mem[mTravIdxCurr].Used) {mTravIdxCurr++; return mem[mTravIdxCurr-1].DataDouble; }
	}
	
	// �������ݽ�������ʱ��������ֵ��Ч
	bRetNotValid = false;	
	return 0.0;
}


// ���ص�ǰӵ�е����ݸ���
int CBRecycledArrInt::Count()
{
	int ct = memUsedCount-recyUsedCount;
	if (ct<0) ct = 0;
	return ct;
}



//////////////////////////////////////////////////////////////////////
// ˽�к���
//////////////////////////////////////////////////////////////////////

void CBRecycledArrInt::ExpandMem( void )
{
	int toCount = 0;
	if (memCount < mcExpandCountThres)
	{
		// ��������������Ƚ��١����������ռ�Ϊԭ����2��
		toCount = memCount * 2;
	}
	else if (memCount < mcExpandCountThresMax)
	{
		// ������������Ѿ����е�ࡱ���������ռ�Ϊԭ����1.5��
		toCount = memCount * 3 / 2;
	}
	else
	{
		// ��������������ܶࡱ�������� mcExpandBigPer ���ռ�
		toCount = memCount + mcExpandBigPer;
	}
	
	// �ض��������С
	// ReDim Preserve mem(1 To toCount)
	RedimArrMemType(mem, toCount, memCount, true);
	memCount = toCount;
}




// �ض��� һ�� MemType ���͵����飨������� recycles���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�recycles�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBRecycledArrInt::RedimArrMemType( MemType * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		MemType * ptrNew = new MemType [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(MemType)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� MemType Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(MemType)*ctToCpy); 
		}
		
		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}


// �ض��� һ�� int ���͵����飨������� mem[]���Ĵ�С���¶���ռ��Զ�����
// arr��Ϊ����ָ�룬�ɴ��ݣ�mem�����������޸Ĵ�ָ���ָ��
// toUBound��ΪҪ�ض����������Ͻ磬����Ϊ��[0] to [toUBound]��Ϊ -1 ʱ�����ٿռ䣬������ɾ��ԭ
//	 �ռ䣬�� arr �ᱻ��Ϊ0
// uboundCurrent��Ϊ�ض���ǰ������Ͻ� [0] to [uboundCurrent]��Ϊ -1 ��ʾ��δ���ٹ��ռ�Ϊ��һ�ε���
// preserve����������ԭʼ���ݷ��򲻱���
// �����¿ռ��ϱ꣬�� toUBound
int CBRecycledArrInt::RedimArrInt( int * &arr, int toUBound/*=-1*/, int uboundCurrent/*=-1*/, bool preserve/*=false*/ )
{
	// �����¿ռ䣺[0] to [toUBound]
	if (toUBound >= 0)
	{
		int * ptrNew = new int [toUBound + 1];		// +1 Ϊʹ�����±���� toUBound
		// �¿ռ�����
		memset(ptrNew, 0, sizeof(int)*(toUBound + 1));
		
		// ��ԭ�пռ����ݿ������¿ռ�
		if (preserve && arr!=0 && uboundCurrent>=0)
		{
			int ctToCpy;										// ����ԭ�����ݣ���Ҫ�����ڴ�� int Ԫ�ظ���
			ctToCpy = uboundCurrent;
			if (uboundCurrent>toUBound) ctToCpy = toUBound;		// ȡ uboundCurrent �� toUBound ����Сֵ
			ctToCpy = ctToCpy + 1;								// ���� +1����Ϊ uboundCurrent �� toUBound ���������Ͻ�
			memcpy(ptrNew, arr, sizeof(int)*ctToCpy); 
		}
		
		// ɾ��ԭ�пռ�
		if (arr!=0 && uboundCurrent>=0) delete [] arr;
		
		// ָ��ָ���¿ռ�
		arr = ptrNew;
		return toUBound;
	}
	else		// if (toUBound < 0)�������ٿռ䣬ɾ��ԭ�пռ�
	{
		if(arr!=0 && uboundCurrent>=0) delete [] arr;
		arr = 0;
		return 0;
	}
	
}






//////////////////////////////////////////////////////////////////////
// CHeapMemory ���ʵ�֣���ȫ�ֶ���ά������ͨ�� new ������ڴ�ָ��
//
//////////////////////////////////////////////////////////////////////



CBHeapMemory::CBHeapMemory(int initSize/*=0*/)
{
	// ���ٳ�ʼ�ռ�
	if (initSize) initSize=1000;
	memHash.AlloMem(initSize);
}

CBHeapMemory::~CBHeapMemory()
{
	Dispose();
}


int CBHeapMemory::AddPtr( void *ptrNew, bool bArrayNew/*=true*/ )
{
	memHash.Add((long)ptrNew, (long)ptrNew, bArrayNew, 0, 0, 0, 0, false);
	return memHash.Count();
}


// �� new ���� size ���ֽڵĿռ䣬���Զ���0
void * CBHeapMemory::Alloc( int size )
{
	char * ptr=new char[size];
	memset(ptr, 0, size);
	AddPtr(ptr, true);
	return (void *)ptr;
}

// �ͷ� ptr ��ָ���һ���ڴ�ռ�
//   ptr �������ɱ�����������Ŀռ䣬���򱾺��������ͷ�
void CBHeapMemory::Free( void *ptr )
{
	if ( ptr && memHash.IsKeyExist((long)ptr) )
	{
		if ( memHash.Item((long)ptr,false) == (long)ptr )	// У�飬�� !=���� delete
		{
			if (memHash.ItemLong((long)ptr,false) )
				delete []ptr;
			else
				delete ptr;
		}	
		memHash.Remove((long)ptr, false);
	}
}


bool CBHeapMemory::IsPtrManaged( void *ptr )
{
	return memHash.IsKeyExist((long)ptr);
}

int CBHeapMemory::CountPtrs()
{
	return memHash.Count();
}

void * CBHeapMemory::PtrEach( int index, bool * ptrbArrayNew/*=0*/ )
{
	if ( ptrbArrayNew )
	{
		if ( memHash.ItemLongFromIndex(index,false) )
			*ptrbArrayNew=true; 
		else
			*ptrbArrayNew=false;
	}
	return (void *)memHash.ItemFromIndex(index, false);
}


void CBHeapMemory::Dispose()
{
	// ɾ�������м�¼�����пռ�
	int i=0; 
	void * ptr;
	for (i=1; i<=memHash.Count(); i++)
	{
		ptr = (void *)memHash.ItemFromIndex(i,false);
		if ( ptr )
		{
			if ( memHash.ItemLongFromIndex(i,false) )
				delete [] ptr; 
			else
				delete ptr;  
			memHash.ItemFromIndexSet(i, 0, false);
		}
	}
	
	// ��� memHash
	memHash.Clear();
}


// ����һ���ڴ�ռ䣨ʵ���ǵ��� memset (������ include <memory.h>) ��
// Ϊʹ�������򲻱��� include <memory.h>������Ҳ�ṩ��������ܽӿڣ�
void CBHeapMemory::ZeroMem( void * ptr, unsigned int length )
{
	memset(ptr, 0, length);
}


// �ڴ濽����ʵ���ǵ��� memcpy (������ include <memory.h>) ��
// Ϊʹ�������򲻱��� include <memory.h>������Ҳ�ṩ��������ܽӿڣ�
void CBHeapMemory::CopyMem( void * dest, void * source, unsigned int length )
{
	memcpy(dest, source, length);	
}
















