////////////////////////////////////////////////////////////////////////////
//mdlCDI.h:实现MCI媒体控制：发送MCI命令，从命令返回，及出错信息返回
//
//支持：需要BWindows模块的支持、需要工程增加LINK winmm.lib
////////////////////////////////////////////////////////////////////////////
#include "BWindows.h"

//MciSend:发送一个mci命令，不返回命令所返回的信息字符串
//成功返回true，失败返回false
//失败时，若pszError不为NULL，还从此返回错误提示字符串
//pszError应为一个LPTSTR类型变量的地址，函数将修改此变量的值
//为错误提示字符串的地址（字符串空间由本函数自动开辟，由HM自动管理）
bool MciSend(LPCTSTR szCmd, LPTSTR *pszError=NULL);

//MciSendRetStr: 发送一个mci命令，返回命令所返回的信息字符串（的首地址）
//所返回的字符串空间由本函数自动开辟，由HM自动管理
//失败时返回空字符串，若pszError部位NULL，还从此返回错误提示字符串
//  pazError 应为一个LPTSTR类型变量的地址，函数将修改此变量的值为错误提示
LPTSTR MciSendRetStr(LPCTSTR szCmd, LPTSTR *pszError=NULL);

//GetMciErrorStr: 获得刚刚mci命令失败原因的字符串
//fdwError 为mciSendCommand 或mciSendString 函数所返回的错误代码
LPTSTR MciGetErrorStr(DWORD fdwError);