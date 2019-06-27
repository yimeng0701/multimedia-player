//////////////////////////////////////////////////////////////////////////////////
//mdlMCI.cpp: 实现MCI媒体控制：发送MCI命令、从命令返回、及出错信息返回
//
//////////////////////////////////////////////////////////////////////////////////

//必须包含与之配对的通用模块的.h文件
#include "mdlMCI.h"

bool MciSend(LPCTSTR szCmd, LPTSTR *pszError/*=NULL*/)
{
	DWORD ret =mciSendString( szCmd, NULL,0, NULL);
	if(ret==0)         //===命令发送成功======
	{
		return true;
	}
	else               //===命令发送失败======
	{
		//由pszError返回错误提示信息
		if(pszError)    //若pszError参数值为NULL,则不获得错误信息
			*pszError=MciGetErrorStr(ret);
		return false;
	}   // end of if(ret==0)
}

LPTSTR MciSendRetStr(LPCTSTR szCmd, LPTSTR *pszError/*=NULL*/)
{
    LPTSTR szResult = NULL;  //结果字符串的地址
    TCHAR szBuff[1024];      //用于接受字符串内容的缓冲区
    //仅用于临时接收字符串，接收字符串后，应从此缓
    //冲区再拷贝到各自所需的字符串空间中，而释放此数组

    HM.ZeroMem(szBuff, sizeof(szBuff));  //清零缓冲区
    DWORD ret = mciSendString(
	    szCmd,
	    szBuff,
	    sizeof(szBuff)/sizeof(TCHAR),            //表示m_szBuff[]中所能保存的字符数
	    NULL
	    );

    if (ret==0)      //====命令发送成功=======
	{
	    szResult = new TCHAR[_tcslen(szBuff)+1];
	    HM.AddPtr(szResult);                  //用NM管理动态分配的内存（程序结束自动释放）
	    _tcscpy(szResult,szBuff);             //将m_szBuff的字符串拷入新空间 szResult
	    return szResult;                      ////返回结果字符串（的首地址）
	}

    else             //=====命令发送失败=====
	{
	    //======命令发送失败，制造并返回空字符串===
	    //由pszError 返回错误提示信息
	    if(pszError)   //若pszError 参数值为NULL，则不获得错误信息
		   *pszError=MciGetErrorStr(ret);
	//制造并返回空字符串
	szResult = new TCHAR[2];
	HM.AddPtr(szResult);      //用HM管理动态分配的内存（程序结束自动释放）
	*szResult =TEXT('\0');
	return szResult;
	}
}

LPTSTR MciGetErrorStr(DWORD fdwError)
{
	LPTSTR szError = NULL;  //错误信息字符串的地址
	TCHAR m_szBuff[1024];   //用于接收字符串内容的缓冲区
	//仅用于临时接收字符串，接收字符串后，应从此缓冲
	//区再拷贝到各自所需的字符串空间中，而释放此数组
	HM.ZeroMem(m_szBuff,sizeof(m_szBuff));   //清零缓冲区、

	if( mciGetErrorString(fdwError, m_szBuff, sizeof(m_szBuff)/sizeof(TCHAR)))
	{   //返回错误信息字符串
		szError = new TCHAR[_tcslen(m_szBuff)+1];  //制造错误信息字符串
		HM.AddPtr(szError);          //用HM管理动态分配的内存（程序结束自动释放）
		_tcscpy(szError,m_szBuff);   //将m_szBuff的字符串拷入新空间szResult
	}
	else
	{  //错误信息未知，将错误信息设为空字符串
		szError = new TCHAR[2];   //制造错误信息为空字符串
		HM.AddPtr(szError);       //用HM管理动态分配的内存（程序结束自动释放）
		*szError =TEXT('\0');
	}
	return szError;



}