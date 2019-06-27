//////////////////////////////////////////////////////////////////////
// mdlOpenSaveDlg.cpp: ʵ������ļ��жԻ���
//
//////////////////////////////////////////////////////////////////////

#include "mdlPathDlg.h"
#include <tchar.h>
#include <shlobj.h>
#include <memory.h>
#include <string.h>


// ���� Windows ����ļ��жԻ��� ѡ���ļ��У�����ѡ����ļ���·��
//   ���������Զ�ȷ�����صġ��û�ѡ���·�������"\"
//   ���û�����ȡ�������� �մ�""
//   �����ص��ַ����Ŀռ��ɱ�ģ����ȫ�� HM �Զ����١��Զ�����
// sPrompt Ϊ�Ի�������ʾ����ʾ��Ϣ��Ĭ�� NULL ʱ����ʾ��ʾ
// bOnlyShowFileSystem=true ʱֻ��ʾ�ļ�ϵͳ��Ŀ��������ʾ ������塢����վ ��
// sIniSelPath Ϊ��ʼ�Զ�ѡ���Ŀ¼��""Ϊ�Զ�ѡ���ϴε�Ŀ¼��NULL Ϊ���Զ�ѡ
LPTSTR BrowPath( HWND		hWndOwner /*= NULL*/, 
				 LPCTSTR	sPrompt /*= NULL*/, 
				 bool		bOnlyShowFileSystem /*= true*/, 
				 LPCTSTR	sIniSelPath /*= NULL*/, 
				 bool	bBrowseFiles /*= false*/, 
				 bool	bWithEditCtrol /*=false*/ )
{
	BROWSEINFO bi;
	TCHAR * szRetPath = new TCHAR [B_MAX_PATH];	// ���ٷ���·�����ַ����ռ�
	HM.AddPtr(szRetPath);	// �� HM ������·�����ַ����ռ�

	memset(&bi, 0, sizeof(bi));
    bi.hwndOwner = hWndOwner;
	bi.lpszTitle = sPrompt;
	bi.pszDisplayName = szRetPath;
    bi.lpfn = pdlgBrowseCallback;	// ���ûص�����Ϊ pdlgBrowseCallback
    
	// ���� bi.ulFlags
	bi.ulFlags = 0;
	if (bOnlyShowFileSystem) bi.ulFlags |= BIF_RETURNONLYFSDIRS;
	if (bBrowseFiles) bi.ulFlags |= BIF_BROWSEINCLUDEFILES;
	if (bWithEditCtrol) bi.ulFlags |= BIF_EDITBOX;
	
	// �Ի����ʼ��ʾʱ�Զ�ѡ���·��
	if (sIniSelPath == NULL)
		*mSetPathIni='\0';					// pdlgBrowseCallback �ص������н�����ѡ���ʼ�ļ���
	else if (_tcslen(sIniSelPath)==0 && _tcslen(sIniSelPath)<B_MAX_PATH)
		_tcscpy(mSetPathIni, mLastPath);		// pdlgBrowseCallback �ص������н��Զ�ѡ�� mLastPath �ļ���
	else
		_tcscpy(mSetPathIni, sIniSelPath);	// pdlgBrowseCallback �ص������н��Զ�ѡ�� sIniSelPath �ļ���
	
	// ��ʾ����ļ��жԻ���
	LPITEMIDLIST retID= SHBrowseForFolder(&bi);
	if (! retID)
	{
		// ���� SHBrowseForFolder ����ʧ�ܣ����ؿ��ַ���
		*szRetPath='\0';
		return szRetPath;
	}
	else
	{
		// ���� SHBrowseForFolder �����ɹ�����·������ szRetPath[] 
		//   ������ szRetPath ���׵�ַ
		// retID Ϊ��ѡ�ļ��е���Ŀ��ʶ���б�Item Identifier List���ĵ�ַ
		// ͨ�� SHGetPathFromIDList ������ô˱�ʶ���б���������ļ���·����
		SHGetPathFromIDList(retID, szRetPath);

		// ȷ������С�\��
		if (_tcslen(szRetPath))
		{
			TCHAR *pTail=szRetPath+_tcslen(szRetPath)-1;	// szRetPath ���һ���ַ��ĵ�ַ
			if (*pTail!='\\') {*(pTail+1)='\\';*(pTail+2)='\0';}
		}

		// ��¼�� mLastPath
		_tcscpy(mLastPath, szRetPath);
		
		// ����·��
		return szRetPath;
	}
}


// SHBrowseForFolder �Ļص�����
int __stdcall pdlgBrowseCallback( HWND hWnd, unsigned int uMsg, long lParam, long lpData )
{
	if (uMsg == BFFM_INITIALIZED)	// ������Ϣ���У�BFFM_SELCHANGED  BFFM_VALIDATEFAILED
	{
		// �Ի��򴰿ڳ�ʼ�����
		if (mSetPathIni)
			if (_tcslen(mSetPathIni))
			{
				// ���� BFFM_SETSELECTION ��Ϣ���Զ�ѡ���ļ��� mSetPathIni
				// wParamΪ��0��True��ʱ��lParam����Ϊ�ļ���·����mSetPathIni��
				SendMessage(hWnd, BFFM_SETSELECTION, 1, (long)mSetPathIni);
			}
	}

	return 0;
}
