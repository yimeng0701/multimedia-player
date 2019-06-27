//////////////////////////////////////////////////////////////////////
// mdlFileSys.h: �ļ�ϵͳ��������
//  
// ֧�֣���Ҫ BWindows ģ���֧��
//////////////////////////////////////////////////////////////////////



#pragma once

#include "BWindows.h"


// �ж�һ���ļ����ļ��� �� ��Ŀ¼�Ƿ���ڣ�
//   ���������ļ�����1�����������ļ��з���2�������ڷ���0
int FMFileExist(LPCTSTR szFile);	

	
// lpszPath Ϊ·��������������һ���ַ���ָ�룬
//   ȷ�������һ�� '\\'�������ص��ַ������ܻ���ԭ�ַ�����
//   �������ɺ����Լ�������¿��ٵ��ڴ���ַ���
LPTSTR FMAddBackSlash(LPCTSTR szPath);	


// �г� tFolder ������ļ������ļ��У����ļ����У��ַ����ĵ�ַ�ֱ���� retFiles 
// �� retSubFolders ����ָ�������� ���ڴ�ռ䱾�����ѿ��ٲ��Զ������������򲻱ظ�Ԥ��
// bReturnEntirePath �Ƿ񷵻�����·�������� tFolder��������ֻ�����ļ������ļ�����
//  sFilter ָ���ļ�(��)Ҫ���ҵ���������ͨ�������Ϊ NULL ʱĬ��Ϊ"*"����ʾ�����ļ�(��)���г�
// ���������ݹ飬Ҫ��ݹ�ɽ����ص����ļ�����ѭ�����ñ�����
// ��������ֵ��ʾ�Ƿ�ɹ�
bool FMListFilesAPI(LPCTSTR		tFolder, 
					TCHAR   **	&retFiles, 
					TCHAR   **	&retSubFolders, 
					int     *	retpFilesCount=NULL,
					int     *	retpSubFoldersCount=NULL, 
					bool		bReturnEntirePath=true, 
					LPCTSTR		sFilter=NULL);



// �����ļ���һ��ֻ����һ���ļ����ļ���
// ���磺FMCopyFiles("D:\\1.dat","J:\\2.dat");
// ��Ŀ��ָ��ΪĿ¼��FMCopyFiles("D:\\1.dat","J:\\");
// ��Դָ��ΪĿ¼��FMCopyFiles("D:\\�ļ���","J:\\");
bool FMCopyFiles(LPCTSTR srcOne,					// �����ļ����ļ���
				 LPCTSTR dest,
				 bool    bShowProgressDlg=true, 
				 bool    bShowConfirmation=true,
				 bool    bShowErrGui=true);

// �����ļ���һ��ͬʱ��������ļ����ļ��е�һ��ָ��Ŀ¼�����غ�����
bool FMCopyFiles(TCHAR * mulFilesSrcArr[],	// ���Դ�ļ��ַ���ָ�루���飩
				 int     srcArrIndexEnd,		// Դ�ļ��ַ���ָ�루���飩�Ľ����±꣨��һ��Ϊ����Ԫ�ظ�����
				 LPCTSTR dest,				// Ŀ���ļ���
				 bool    bShowProgressDlg=true, 
				 bool    bShowConfirmation=true,
				 bool    bShowErrGui=true,
				 int     srcArrIndexStart=1); // Դ�ļ��ַ���ָ�루���飩����ʼ�±꣨Ĭ�ϲ�ʹ�� [0] �����ݣ�


// �ƶ��ļ���һ��ֻ�ƶ�һ���ļ����ļ���
bool FMMoveFiles(LPCTSTR srcOne,				// �����ļ����ļ���
				 LPCTSTR dest,
				 bool    bShowProgressDlg=true, 
				 bool    bShowConfirmation=true,
				 bool    bShowErrGui=true);

// �����ļ���һ��ͬʱ�ƶ�����ļ����ļ��е�һ��ָ��Ŀ¼�����غ�����
bool FMMoveFiles(TCHAR * mulFilesSrcArr[],		// ���Դ�ļ��ַ���ָ�루���飩
				 int     srcArrIndexEnd,		// Դ�ļ��ַ���ָ�루���飩�Ľ����±꣨��һ��Ϊ����Ԫ�ظ�����
				 LPCTSTR dest,					// Ŀ���ļ���
				 bool    bShowProgressDlg=true, 
				 bool    bShowConfirmation=true,
				 bool    bShowErrGui=true,
				 int     srcArrIndexStart=1);	// Դ�ļ��ַ���ָ�루���飩����ʼ�±꣨Ĭ�ϲ�ʹ�� [0] �����ݣ�

// ɾ���ļ���һ��ֻɾ��һ���ļ����ļ���
bool FMDeleteFiles(LPCTSTR srcOne,				// �����ļ����ļ���
				   bool    bMoveToRecycle=true, 
				   bool    bShowProgressDlg=true, 
				   bool    bShowConfirmation=true,
				   bool    bShowErrGui=true);

// ɾ���ļ���һ��ͬʱɾ������ļ����ļ��У����غ�����
bool FMDeleteFiles(TCHAR * mulFilesSrcArr[],		// ���Դ�ļ��ַ���ָ�루���飩
				   int     srcArrIndexEnd,			// Դ�ļ��ַ���ָ�루���飩�Ľ����±꣨��һ��Ϊ����Ԫ�ظ�����
				   bool    bMoveToRecycle=true, 
				   bool    bShowProgressDlg=true, 
				   bool    bShowConfirmation=true,
				   bool    bShowErrGui=true,
				   int     srcArrIndexStart=1);		// Դ�ļ��ַ���ָ�루���飩����ʼ�±꣨Ĭ�ϲ�ʹ�� [0] �����ݣ�

// ������һ���ļ�
bool FMRenameFile(LPCTSTR srcOne, 
				  LPCTSTR dest,
				  bool    bShowConfirmation=true,
				  bool    bShowErrGui=true, 
				  bool    bAllowUndo=true);


// ʹ�� SHFileOperation �����ļ��Ĺ��ú���
static bool ShFilesOper(char   oper,			// oper=1: �ƶ��ļ�, oper=2: �����ļ�, oper=3: ɾ���ļ�, oper=4: �������ļ�
						LPCTSTR srcFiles, 
						LPCTSTR destFolder, 
						bool    bShowProgressDlg=true, 
						bool    bShowConfirmation=true,
						bool    bShowErrGui=true,
						bool    bAllowUndo=true);