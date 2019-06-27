//////////////////////////////////////////////////////////////////////////////////
//mdlMCI.cpp: ʵ��MCIý����ƣ�����MCI���������ء���������Ϣ����
//
//////////////////////////////////////////////////////////////////////////////////

//���������֮��Ե�ͨ��ģ���.h�ļ�
#include "mdlMCI.h"

bool MciSend(LPCTSTR szCmd, LPTSTR *pszError/*=NULL*/)
{
	DWORD ret =mciSendString( szCmd, NULL,0, NULL);
	if(ret==0)         //===����ͳɹ�======
	{
		return true;
	}
	else               //===�����ʧ��======
	{
		//��pszError���ش�����ʾ��Ϣ
		if(pszError)    //��pszError����ֵΪNULL,�򲻻�ô�����Ϣ
			*pszError=MciGetErrorStr(ret);
		return false;
	}   // end of if(ret==0)
}

LPTSTR MciSendRetStr(LPCTSTR szCmd, LPTSTR *pszError/*=NULL*/)
{
    LPTSTR szResult = NULL;  //����ַ����ĵ�ַ
    TCHAR szBuff[1024];      //���ڽ����ַ������ݵĻ�����
    //��������ʱ�����ַ����������ַ�����Ӧ�Ӵ˻�
    //�����ٿ���������������ַ����ռ��У����ͷŴ�����

    HM.ZeroMem(szBuff, sizeof(szBuff));  //���㻺����
    DWORD ret = mciSendString(
	    szCmd,
	    szBuff,
	    sizeof(szBuff)/sizeof(TCHAR),            //��ʾm_szBuff[]�����ܱ�����ַ���
	    NULL
	    );

    if (ret==0)      //====����ͳɹ�=======
	{
	    szResult = new TCHAR[_tcslen(szBuff)+1];
	    HM.AddPtr(szResult);                  //��NM����̬������ڴ棨��������Զ��ͷţ�
	    _tcscpy(szResult,szBuff);             //��m_szBuff���ַ��������¿ռ� szResult
	    return szResult;                      ////���ؽ���ַ��������׵�ַ��
	}

    else             //=====�����ʧ��=====
	{
	    //======�����ʧ�ܣ����첢���ؿ��ַ���===
	    //��pszError ���ش�����ʾ��Ϣ
	    if(pszError)   //��pszError ����ֵΪNULL���򲻻�ô�����Ϣ
		   *pszError=MciGetErrorStr(ret);
	//���첢���ؿ��ַ���
	szResult = new TCHAR[2];
	HM.AddPtr(szResult);      //��HM����̬������ڴ棨��������Զ��ͷţ�
	*szResult =TEXT('\0');
	return szResult;
	}
}

LPTSTR MciGetErrorStr(DWORD fdwError)
{
	LPTSTR szError = NULL;  //������Ϣ�ַ����ĵ�ַ
	TCHAR m_szBuff[1024];   //���ڽ����ַ������ݵĻ�����
	//��������ʱ�����ַ����������ַ�����Ӧ�Ӵ˻���
	//���ٿ���������������ַ����ռ��У����ͷŴ�����
	HM.ZeroMem(m_szBuff,sizeof(m_szBuff));   //���㻺������

	if( mciGetErrorString(fdwError, m_szBuff, sizeof(m_szBuff)/sizeof(TCHAR)))
	{   //���ش�����Ϣ�ַ���
		szError = new TCHAR[_tcslen(m_szBuff)+1];  //���������Ϣ�ַ���
		HM.AddPtr(szError);          //��HM����̬������ڴ棨��������Զ��ͷţ�
		_tcscpy(szError,m_szBuff);   //��m_szBuff���ַ��������¿ռ�szResult
	}
	else
	{  //������Ϣδ֪����������Ϣ��Ϊ���ַ���
		szError = new TCHAR[2];   //���������ϢΪ���ַ���
		HM.AddPtr(szError);       //��HM����̬������ڴ棨��������Զ��ͷţ�
		*szError =TEXT('\0');
	}
	return szError;



}