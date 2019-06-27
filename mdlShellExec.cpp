//////////////////////////////////////////////////////////////////////
// mdlShellExec.cpp: ���г������ĵ��ĺ���ʵ��
//  
//////////////////////////////////////////////////////////////////////

#include "mdlShellExec.h"
#include <string.h>
#include <memory.h>
#include <tchar.h>


// ֻ�ܱ�ģ��ʹ�õ�ȫ�ֱ���
static TCHAR m_AppPath[MAX_PATH+1];	// ���汾��������Ŀ¼
static CBHeapMemory mHM;			// ���汾ģ���������� new ���ٵ�
									//	�ڴ�ָ�룬�ö�������ʱ���Զ� delete ��Щ�ڴ�


// ��ñ����������·��������ֵ�����'\'��
// bGet83PathFile=true ʱ���8.3���ļ����������ó��ļ���
LPTSTR SEAppPath(bool bGet83PathFile/*=false*/)
{
	// ���Ӧ�ó����ļ���������ȫ·����
	GetModuleFileName(0, m_AppPath, MAX_PATH); 

	// ���8.3��ʽ���ļ���
	if (bGet83PathFile)
	{
		TCHAR temp[MAX_PATH+1];
		GetShortPathName(m_AppPath, temp, MAX_PATH);
		_tcscpy(m_AppPath, temp);  // �ٽ����ļ������� m_AppPath
	}
	
	// ��ȡ·��
	// �������һ�� '\'
	TCHAR * ptrLastSlash=_tcsrchr(m_AppPath,'\\');  
	// ���һ�� '\' ����һ��λ������Ϊ 0
	if (ptrLastSlash) *(ptrLastSlash+1)='\0'; 

	// ����
	return m_AppPath;
}




// ִ��һ�����򡢴�һ���ĵ����һ��URL
// ��bWithParam=true���� sFileOrURL �ĵ�һ���ո�ǰ����������ļ����� _
// ֮�������������򶼿��������ļ���������Ϊ��
//   winShowStyle ��ʾ������ʾ״̬����󻯡���С�������صȣ�
// �÷�������
//   ���г��� SEShellRun("C:\\Windows\\System32\\mspaint.exe");
//   ���ļ��� SEShellRun("C:\\Windows");
//   �Զ���������������ĵ� SEShellRun("E:\\my\\abc.txt");
//   ����ҳ��SEShellRun("http://www.163.com");
//   �����ʼ���SEShellRun("mailto:zhni2011@163.com");
bool SEShellRun( LPCTSTR sFileOrURL, bool bWithParam/*=false*/, EShowWindowCmd winShowStyle/*=SW_ShowNormal*/ )
{
	TCHAR * sFileOrExe = (TCHAR *)sFileOrURL;	// �ⲿ��������ִ�в���
	TCHAR * sParam = 0;				// �ⲿ����Ĳ���
	bool ret;						// ����ֵ

	if (bWithParam)
	{
		// �ֳ� sFileOrURL �еĳ����ļ����Ͳ���
		TCHAR * sFileOrExe = new TCHAR[_tcslen(sFileOrURL)+1];
		_tcscpy(sFileOrExe, sFileOrURL);
		TCHAR * ptrBlank=_tcschr(sFileOrExe, 32);	// �ҵ���һ���ո��λ��
		if (ptrBlank) // �ҵ��˿ո�
		{
			*ptrBlank='\0';
			sParam=(ptrBlank+1);
		}
		ret= (INT)ShellExecute(0, TEXT("open"), sFileOrExe, 
		    sParam, 0, (INT)winShowStyle) > 32;
		delete []sFileOrExe;
	}
	else
	{
		ret= (INT)ShellExecute(0, TEXT("open"), sFileOrExe, 
			sParam, 0, (INT)winShowStyle) > 32;
	}
	
	return ret;
}


// ��һ��ϵͳ�ļ���
bool SEOpenSysFolder( eSESysFolder sysFolder, EShowWindowCmd winShowStyle/*=SW_ShowNormal*/ )
{
	switch(sysFolder)
	{
	case sesfRecycleBin:
		// �� ����վ
		return SEShellRun(
		  TEXT("explorer.exe ::{645FF040-5081-101B-9F08-00AA002F954E}"), 
		  true, winShowStyle);
		break;
	case sesfMyComputer:
		// �� �ҵĵ���
		return SEShellRun(
		  TEXT("explorer.exe ::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"), 
		  true, winShowStyle);
		break;
	case sesfNetworkPlaces:
		// �� �����ھ�
		return SEShellRun(
		  TEXT("explorer.exe ::{208D2C60-3AEA-1069-A2D7-08002B30309D}"), 
		  true, winShowStyle);
		break;
	case sesfMyDocuments:
		// �� �ҵ��ĵ�
		return SEShellRun(TEXT("explorer.exe"), true, winShowStyle);
		break;
	default:
		return 0;
		break;
	}
}


// ��ӡһ���ĵ�
bool SEPrintDocument( LPCTSTR fileToPrint, EShowWindowCmd winShowStyle/*=SW_ShowNormal*/ )
{
	return (INT)ShellExecute(0, TEXT("print"), fileToPrint, 
      0, 0, (INT)winShowStyle) > 32;
}


// ����һ�����򲢵ȴ����������ؽ��̾����ʧ�ܷ���0
// bReturnUntilTerm = trueʱ��ֻ�г�������󣬺����Ż᷵�أ�
//   =False ������������һ�����̾����֮��ɷ�������
// SETestProcessTerminate ���Խ����Ƿ����
//   �� ��ʱ���� SETerminateProcess ��ֹ����
// �� bReturnUntilTerm=false������֮������ SECloseProcessHandle �رս��̾�
//   ������������ֵ������ bReturnUntilTerm=true ʱ�򲻱أ����������Զ��رվ��
HANDLE SERunWaitTerminate( LPCTSTR sExeFileWithParams, bool bReturnUntilTerm /*= false*/, EShowWindowCmd winShowStyle/*=SW_ShowNormal*/ )
{
	long ret;
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;

	memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cb = sizeof(sinfo);
    sinfo.dwFlags = STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = winShowStyle;

    // -------��������
    ret = CreateProcess(0, (LPTSTR)sExeFileWithParams, 0, 0,  true, 
      NORMAL_PRIORITY_CLASS, 0, 0, &sinfo, &pinfo);
    if (ret==0)  // ��������ʧ��
	{
		CloseHandle(pinfo.hThread);
		CloseHandle(pinfo.hProcess);
		return 0;
	}
    
    // �ȴ����̳�ʼ�����
    WaitForInputIdle(pinfo.hProcess, INFINITE);

    // �ر��߳̾��
    CloseHandle(pinfo.hThread);
    
    if(bReturnUntilTerm)
	{
		// �ȴ����̽��������ٷ���
		ret = WaitForSingleObject(pinfo.hProcess, INFINITE);
		if (ret == WAIT_FAILED)
		{
			CloseHandle(pinfo.hThread);
			CloseHandle(pinfo.hProcess);
			return 0;
		}

		// �رվ�� pinfo.hProcess
		CloseHandle(pinfo.hProcess);

		// ����
		return pinfo.hProcess;
	}
	else
	{
		// ������������
		return pinfo.hProcess;
	}
}


// ����һ�������Ƿ��Ѿ�����
// ��û�иý��̻�ʧ��Ҳ���� true��ֻ�н����������вŷ��� false
// bClosehProcessIfTerm = true ��ʾ��������Ѿ������͹رվ�� hProcess
// ������������������е��� SETerminateProcess �رվ��
bool SETestProcessTerminate( HANDLE hProcess, bool bClosehProcessIfTerm /*= true*/ )
{
	DWORD ret = WaitForSingleObject(hProcess, 0);
	switch(ret)
	{
	case WAIT_TIMEOUT:
		return false;
		break;
	case WAIT_FAILED:
		return true;
		break;
	default:
		// �رս��̾�����������ָ���Զ��رյĻ���
		if (bClosehProcessIfTerm) CloseHandle (hProcess);
		return true;
		break;
	}

}


// ��ֹһ������
// bClosehProcess���Ƿ��Զ��رս��̾��
bool SETerminateProcess( HANDLE hProcess, bool bClosehProcess /*= true*/ )
{
	if (TerminateProcess(hProcess, 0))
	{
		if (bClosehProcess) CloseHandle(hProcess);
		return true;
	}
	else
	{
		return false;
	}
}


// �ر�һ�����̾��
void SECloseProcessHandle( HANDLE hProcess )
{
	CloseHandle(hProcess);
}






