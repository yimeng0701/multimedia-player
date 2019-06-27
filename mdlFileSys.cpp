//////////////////////////////////////////////////////////////////////
// mdlFileSys.cpp: �ļ�ϵͳ����������ʵ��
// 
// ��ģ��Ҫʹ��ȫ�ֶ��� HM ����̬�ڴ� 
//////////////////////////////////////////////////////////////////////



#include "BWindows.h"
#include "mdlFileSys.h"
#include <memory.h>


//////////////////////////////////////////////////////////////////////////
// �Զ����ļ� ����
//////////////////////////////////////////////////////////////////////////


// �ж�һ���ļ����ļ����Ƿ���ڣ����������ļ�����1�����������ļ��з���2�������ڷ���0
int FMFileExist(LPCTSTR szFile)
{
	bool bIsFolder = false;		// ���Ϊ true��ǿ��˵����Ҫ�жϵ� szFile �Ǹ�Ŀ¼
	int result=0;				// �������ķ���ֵ

	// ��Ҫ�жϵ��ļ����ַ��������� sFile
	TCHAR * sFile = new TCHAR[lstrlen(szFile)+1];
	lstrcpy(sFile, szFile);

	// ��� sFile ���һ���ַ�Ϊ"\"����Ϊ����һ��Ŀ¼��
	//	ȥ��"\"�Ա����Ŀ¼�Ƿ���ڣ���˵�� sFile ���Ǹ�Ŀ¼�����水Ŀ¼�������Ƿ����
	TCHAR * pTailCh = sFile + lstrlen(sFile) - 1;
	if (*pTailCh == '\\') 
	{
		*pTailCh='\0';		// ȥ����β�� '\\'
		bIsFolder=true;		// ˵�� szFile ���Ǹ�Ŀ¼�����水Ŀ¼�������Ƿ����
		pTailCh--;			// pTailCh ��ָ���β
	}
		
	// �жϽ�β�Ƿ�Ϊ ':'����ȥ��'\\'����½�βΪ':'(ԭ�����ڶ����ַ�)��
	// ����ǣ������� FindFirstFile �жϸ�Ŀ¼�Ƿ���ڣ�Ҫ�� GetFileAttributes
	if (*pTailCh == ':')
	{
		int r=GetFileAttributes(sFile);		// ��� GetFileAttributes ʧ�ܣ����� -1
		if (r!=-1 && ((r & FILE_ATTRIBUTE_DIRECTORY)!=0) )
			result = 2;		// ˵��Ŀ¼���ڣ���Ŀ¼��
		else
			result = 0;		// Ŀ¼������
	}
	else		// if (*pTailCh == ':')
	{
		// ʹ�� FindFirstFile ���� sFile �Ƿ����
		WIN32_FIND_DATA fd;
		HANDLE hr=FindFirstFile(sFile, &fd);
		if (hr==INVALID_HANDLE_VALUE)
		{
			result = 0;
		}
		else
		{
			FindClose(hr);	// �رղ��Ҿ��
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				result = 2;
			else
				if (bIsFolder) result = 0; else result = 1;	//ȥ��'\\'Ӧ���Ǹ��ļ���
											// ����ǰ��'\\'(blIsFolder=true)˵���ļ�������
		}
	}	// end if (*pTailCh == ':')


	delete []sFile;
	return result;
}


LPTSTR FMAddBackSlash(LPCTSTR szPath)
{
	int len = lstrlen(szPath);
	TCHAR *pTailCh = (TCHAR *)(szPath + len - 1);
	if (*pTailCh == '\\') 
	{
		// ��ʹ��ԭ�ַ���
		return (TCHAR *)szPath;
	}
	else
	{
		// �����µ��ַ�������� '\\'�����ַ����ڴ��� HM ����
		TCHAR *strResult = new TCHAR[len+2];
		HM.AddPtr(strResult);

		lstrcpy(strResult, szPath);
		*(strResult + len) = '\\';
		*(strResult + len + 1) = '\0';

		return strResult;
	}
}


// �г� tFolder ������ļ������ļ��У����ļ����У��ַ����ĵ�ַ�ֱ���� retFiles 
//   �� retSubFolders ����ָ�������� ���ڴ�ռ䱾�����ѿ��ٲ��Զ������������򲻱ظ�Ԥ��
//   ����ָ�������±���� 1 ��ʼ��[0] Ԫ�ز���
// bReturnEntirePath �Ƿ񷵻�����·�������� tFolder��������ֻ�����ļ������ļ�����
//  sFilter ָ���ļ�(��)Ҫ���ҵ���������ͨ�������Ϊ NULL ʱĬ��Ϊ"*"����ʾ�����ļ�(��)���г�
// ���������ݹ飬Ҫ��ݹ�ɽ����ص����ļ�����ѭ�����ñ�����
// ��������ֵ��ʾ�Ƿ�ɹ�
bool FMListFilesAPI( LPCTSTR tFolder, 
					 TCHAR ** &retFiles, 
					 TCHAR ** &retSubFolders, 
					 int * retpFilesCount/*=NULL*/, 
					 int * retpSubFoldersCount/*=NULL*/, 
					 bool bReturnEntirePath/*=true*/, 
					 LPCTSTR sFilter/*=NULL*/ )
{
	const int cArrExpandPer = 100;			//����ʽ��������ÿ��������С

	if (tFolder==NULL) return false;		// Ŀ���ļ���δ����
	if (lstrlen(tFolder)==0) return false;	// Ŀ���ļ���Ϊ ""

	// ��ʼ������
	TCHAR *pFolder=FMAddBackSlash(tFolder);	// Ŀ���ļ������ȷ���� '\'
	int iLengthFolder = lstrlen(pFolder);	// iLengthFolder ΪĿ���ļ����ַ����ĳ���
	WIN32_FIND_DATA WFDFileData;
	HANDLE findHandle;

	memset(&WFDFileData, 0, sizeof(WFDFileData));
	// ��ʼ�����ļ�������������浽 findHandle
	if (sFilter)
	{
		// �� Filter��ƴ�� pFolder + sFilter
		TCHAR * pFolderFilter = new TCHAR [lstrlen(pFolder) + lstrlen(sFilter) + 1];
		HM.AddPtr(pFolderFilter);
		lstrcpy(pFolderFilter, pFolder);
		lstrcat(pFolderFilter, sFilter);
		// �� pFolder + sFilter �����ļ�
		findHandle = FindFirstFile(pFolderFilter, &WFDFileData);
	}
	else
	{
		// �� Filter������ pFolder �е������ļ�
		// ƴ�� pFolder + "*"
		TCHAR * pFolderFilter = new TCHAR [lstrlen(pFolder) + 2];
		HM.AddPtr(pFolderFilter);
		lstrcpy(pFolderFilter, pFolder);
		lstrcat(pFolderFilter, TEXT("*"));
		// �� pFolder + "*" �����ļ�
		findHandle = FindFirstFile(pFolderFilter, &WFDFileData);
	}

	// ���� findHandle �жϣ������Ч�򷵻� false
	if (findHandle == INVALID_HANDLE_VALUE)
	{
		// FindFirstFile ����ʧ�ܣ����������Ч
		return false;
	}

	// ��Ч��� findHandle
	// ѭ�����������ļ�(��)����һ���ļ�(��)�������� FindFirstFile �ҵ�
	// �ļ�(��)��Ϣ�� WFDFileData ���ݵĸ���Ա��
	int iFilesCount=0, iSubFoldersCount=0;
	int uboundFilesArr=-1, uboundSubFoldersArr=-1;	// retFiles[] ���顢retSubFolders[] ������Ͻ�
	TCHAR *pEachFile=NULL;							// ���ڱ���ÿ���ļ����У����ַ����Ŀռ��ַ
	int ret=0;										// ���� FindNextFile �����ķ���ֵ
	do
	{
		if (lstrcmp(WFDFileData.cFileName, TEXT("."))!=0 && 
			lstrcmp(WFDFileData.cFileName, TEXT(".."))!=0)	// ������ǰĿ¼���ϲ�Ŀ¼
		{
			// �Ϸ��ļ����ļ�����������֮
			// ��ô��ļ����У����ַ��� => pEachFile
			//   �������¿��ٵ��ַ����Ŀռ�ĵ�ַ��ʱ�� pEachFile ���棬
			//   ֮���ٽ��õ�ַ���� retFiles[] ��retSubFolders[] ����
			if (bReturnEntirePath)
			{
				// ��Ҫ����ȫ·���ļ�����ƴ�ӡ��ļ�·����+���ļ��������� => pEachFile
				pEachFile=new TCHAR[iLengthFolder + lstrlen(WFDFileData.cFileName) + 1];
				HM.AddPtr(pEachFile);
				lstrcpy(pEachFile, pFolder);
				lstrcat(pEachFile, WFDFileData.cFileName);
			}
			else
			{
				// ֻ��Ҫ�����ļ��������ơ��ļ��������� => pEachFile
				// ���븴���ַ��������´� WFDFileData.cFileName �е����ݻ�ʧЧ
				pEachFile=new TCHAR[lstrlen(WFDFileData.cFileName) + 1];
				HM.AddPtr(pEachFile);
				lstrcpy(pEachFile, WFDFileData.cFileName);
			}
			
			// ʹ��λ�Ƚ���ȷ�� pEachFile �Ƿ����һ�����ļ���
			if (WFDFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// �����ļ��У���ӵ� retSubFolders[] ������
				iSubFoldersCount++;
				if (iSubFoldersCount > uboundSubFoldersArr)
				{
					// �ض��� retSubFolders[] �����С
					Redim(retSubFolders, uboundSubFoldersArr+cArrExpandPer, uboundSubFoldersArr, true);
					uboundSubFoldersArr=uboundSubFoldersArr+cArrExpandPer;
				}
				// ��������ݣ��� retSubFolders[iSubFoldersCount] ��¼����¿��ٵĿռ�ĵ�ַ
				retSubFolders[iSubFoldersCount] = pEachFile;
			}
			else
			{
				// ���ļ�����ӵ� retFiles[] ������
				iFilesCount++;
				if (iFilesCount > uboundFilesArr)
				{
					// �ض��� retFiles[] �����С
					Redim(retFiles, uboundFilesArr+cArrExpandPer, uboundFilesArr, true);
					uboundFilesArr=uboundFilesArr+cArrExpandPer;
				}
				// ��������ݣ��� retFiles[iFilesCount] ��¼����¿��ٵĿռ�ĵ�ַ
				retFiles[iFilesCount] = pEachFile;
			}

		}	// ������ǰĿ¼���ϲ�Ŀ¼
		
       
        // �� FindNextFile ������һ���ļ�
		memset(&WFDFileData, 0, sizeof(WFDFileData));
        ret = FindNextFile(findHandle, &WFDFileData);
	} while(ret!= 0 && ret!=ERROR_NO_MORE_FILES);
		
	// �رվ��
	FindClose(findHandle);
		
	// �Ӳ��������ļ������ļ��У�����
	if (retpFilesCount) *retpFilesCount=iFilesCount;
	if (retpSubFoldersCount) *retpSubFoldersCount=iSubFoldersCount;
	
	// ���سɹ�
	return true;
}






// ʹ�� SHFileOperation �����ļ��Ĺ��ú���
// oper=1: �ƶ��ļ�, oper=2: �����ļ�, oper=3: ɾ���ļ�, oper=4: �������ļ�
// srcFiles Ϊ�� '\0' �ָ���˫ '\0' ��β���ַ���ָ��
static bool ShFilesOper(char	oper, 
				  	 LPCTSTR	srcFiles, 
					 LPCTSTR	destFolder, 
						bool	bShowProgressDlg/*=true*/, 
						bool	bShowConfirmation/*=true*/,
						bool	bShowErrGui/*=true*/,
						bool	bAllowUndo/*=true*/)
{
	SHFILEOPSTRUCT sfo;
	sfo.wFunc = oper;
	sfo.pFrom = srcFiles;
	sfo.pTo = destFolder;
	sfo.fFlags = 0;

	sfo.fAnyOperationsAborted = 0;
	sfo.hNameMappings = 0;
	sfo.hwnd = 0;
	sfo.lpszProgressTitle = 0;

	if (bAllowUndo) sfo.fFlags |= FOF_ALLOWUNDO;
    if (! bShowProgressDlg) sfo.fFlags |= FOF_SILENT;
    if (! bShowConfirmation) sfo.fFlags |= FOF_NOCONFIRMATION;
    if (! bShowErrGui) sfo.fFlags |= FOF_NOERRORUI;

	return (SHFileOperation(&sfo) == 0);
}



bool FMCopyFiles(LPCTSTR srcOne, 
				 LPCTSTR dest, 
				 bool bShowProgressDlg/*=true*/, 
				 bool bShowConfirmation/*=true*/, 
				 bool bShowErrGui/*=true*/)
{
	// ������ʱ�ռ䣺˫'\0'��β��Դ�ļ��ַ����Ŀռ�
	int iSrcOneLen = lstrlen(srcOne);
	TCHAR * srcDblNULL = new TCHAR[iSrcOneLen + 2];	// +2Ϊ���� '\0' �ռ䣨˫'\0'ʱָ���һ��'\0'��

	// ʹ����ʱ�ռ䣬ʹԴ�ļ���β���������� '\0'
	lstrcpy(srcDblNULL, srcOne);
	*(srcDblNULL + iSrcOneLen + 1)='\0';			// ʹ��β���������� '\0'

	// ���� ShFilesOper ִ�п����ļ�����
	bool bRet;
	bRet = ShFilesOper(2, srcDblNULL, dest, bShowProgressDlg, bShowConfirmation, bShowErrGui, true);

	// ɾ����ʱ�ռ�
	delete []srcDblNULL;

	// ����
	return bRet;
}

bool FMCopyFiles(TCHAR * mulFilesSrcArr[], 
				  int	srcArrIndexEnd, 
		      LPCTSTR	dest, 
				 bool   bShowProgressDlg/*=true*/, 
				 bool   bShowConfirmation/*=true*/, 
				 bool   bShowErrGui/*=true*/, 
				 int    srcArrIndexStart/*=1*/ )
{
	// ����Դ�ļ��ַ���������ɵ�˫'\0'��β���ַ���
	TCHAR * szSrcDblNULL;
	szSrcDblNULL = Join(mulFilesSrcArr, srcArrIndexEnd, NULL, srcArrIndexStart, true);

	// ���� ShFilesOper ִ�п����ļ�����
	return ShFilesOper(2, szSrcDblNULL, dest, bShowProgressDlg, bShowConfirmation, bShowErrGui, true);
}

bool FMMoveFiles(LPCTSTR srcOne, 
				 LPCTSTR dest, 
				 bool bShowProgressDlg/*=true*/, 
				 bool bShowConfirmation/*=true*/, 
				 bool bShowErrGui/*=true*/ )
{
	// ������ʱ�ռ䣺˫'\0'��β��Դ�ļ��ַ����Ŀռ�
	int iSrcOneLen = lstrlen(srcOne);
	TCHAR * srcDblNULL = new TCHAR[iSrcOneLen + 2];	// +2Ϊ���� '\0' �ռ䣨˫'\0'ʱָ���һ��'\0'��
	
	// ʹ����ʱ�ռ䣬ʹԴ�ļ���β���������� '\0'
	lstrcpy(srcDblNULL, srcOne);
	*(srcDblNULL + iSrcOneLen + 1)='\0';			// ʹ��β���������� '\0'
	
	// ���� ShFilesOper ִ���ƶ��ļ�����
	bool bRet;
	bRet = ShFilesOper(1, srcDblNULL, dest, bShowProgressDlg, bShowConfirmation, bShowErrGui, true);
	
	// ɾ����ʱ�ռ�
	delete []srcDblNULL;
	
	// ����
	return bRet;
}

bool FMMoveFiles( TCHAR * mulFilesSrcArr[], 
				 int srcArrIndexEnd, 
				 LPCTSTR dest, 
				 bool bShowProgressDlg/*=true*/, 
				 bool bShowConfirmation/*=true*/, 
				 bool bShowErrGui/*=true*/, 
				 int srcArrIndexStart/*=1*/ )
{
	// ����Դ�ļ��ַ���������ɵ�˫'\0'��β���ַ���
	TCHAR * szSrcDblNULL;
	szSrcDblNULL = Join(mulFilesSrcArr, srcArrIndexEnd, NULL, srcArrIndexStart, true);
	
	// ���� ShFilesOper ִ���ƶ��ļ�����
	return ShFilesOper(1, szSrcDblNULL, dest, bShowProgressDlg, bShowConfirmation, bShowErrGui, true);
}

bool FMDeleteFiles( LPCTSTR srcOne, 
				   bool bMoveToRecycle/*=true*/, 
				   bool bShowProgressDlg/*=true*/, 
				   bool bShowConfirmation/*=true*/, 
				   bool bShowErrGui/*=true*/ )
{
	// ������ʱ�ռ䣺˫'\0'��β��Դ�ļ��ַ����Ŀռ�
	int iSrcOneLen = lstrlen(srcOne);
	TCHAR * srcDblNULL = new TCHAR[iSrcOneLen + 2];	// +2Ϊ���� '\0' �ռ䣨˫'\0'ʱָ���һ��'\0'��
	
	// ʹ����ʱ�ռ䣬ʹԴ�ļ���β���������� '\0'
	lstrcpy(srcDblNULL, srcOne);
	*(srcDblNULL + iSrcOneLen + 1)='\0';			// ʹ��β���������� '\0'
	
	// ���� ShFilesOper ִ��ɾ���ļ�����
	bool bRet;
	bRet = ShFilesOper(3, srcDblNULL, 0, bShowProgressDlg, bShowConfirmation, bShowErrGui, bMoveToRecycle);
	
	// ɾ����ʱ�ռ�
	delete []srcDblNULL;
	
	// ����
	return bRet;	
}

bool FMDeleteFiles(TCHAR * mulFilesSrcArr[], 
				   int srcArrIndexEnd, 
				   bool bMoveToRecycle/*=true*/, 
				   bool bShowProgressDlg/*=true*/, 
				   bool bShowConfirmation/*=true*/, 
				   bool bShowErrGui/*=true*/, 
				   int srcArrIndexStart/*=1*/ )
{
	// ����Դ�ļ��ַ���������ɵ�˫'\0'��β���ַ���
	TCHAR * szSrcDblNULL;
	szSrcDblNULL = Join(mulFilesSrcArr, srcArrIndexEnd, NULL, srcArrIndexStart, true);
	
	// ���� ShFilesOper ִ���ƶ��ļ�����
	return ShFilesOper(3, szSrcDblNULL, 0, bShowProgressDlg, bShowConfirmation, bShowErrGui, bMoveToRecycle);
}

bool FMRenameFile( LPCTSTR srcOne, 
				   LPCTSTR dest, 
				   bool bShowConfirmation/*=true*/, 
				   bool bShowErrGui/*=true*/, 
				   bool bAllowUndo/*=true*/ )
{
	// ������ʱ�ռ䣺˫'\0'��β��Դ�ļ��ַ����Ŀռ�
	int iSrcOneLen = lstrlen(srcOne);
	TCHAR * srcDblNULL = new TCHAR[iSrcOneLen + 2];	// +2Ϊ���� '\0' �ռ䣨˫'\0'ʱָ���һ��'\0'��
	
	// ʹ����ʱ�ռ䣬ʹԴ�ļ���β���������� '\0'
	lstrcpy(srcDblNULL, srcOne);
	*(srcDblNULL + iSrcOneLen + 1)='\0';			// ʹ��β���������� '\0'
	
	// ���� ShFilesOper ִ�и����ļ����Ĳ���
	bool bRet;
	bRet = ShFilesOper(4, srcDblNULL, dest, true, bShowConfirmation, bShowErrGui, bAllowUndo);
	
	// ɾ����ʱ�ռ�
	delete []srcDblNULL;
	
	// ����
	return bRet;	
}





