////////////////////////////////////////////////////////////////////////////
//mdlCDI.h:ʵ��MCIý����ƣ�����MCI���������أ���������Ϣ����
//
//֧�֣���ҪBWindowsģ���֧�֡���Ҫ��������LINK winmm.lib
////////////////////////////////////////////////////////////////////////////
#include "BWindows.h"

//MciSend:����һ��mci������������������ص���Ϣ�ַ���
//�ɹ�����true��ʧ�ܷ���false
//ʧ��ʱ����pszError��ΪNULL�����Ӵ˷��ش�����ʾ�ַ���
//pszErrorӦΪһ��LPTSTR���ͱ����ĵ�ַ���������޸Ĵ˱�����ֵ
//Ϊ������ʾ�ַ����ĵ�ַ���ַ����ռ��ɱ������Զ����٣���HM�Զ�����
bool MciSend(LPCTSTR szCmd, LPTSTR *pszError=NULL);

//MciSendRetStr: ����һ��mci����������������ص���Ϣ�ַ��������׵�ַ��
//�����ص��ַ����ռ��ɱ������Զ����٣���HM�Զ�����
//ʧ��ʱ���ؿ��ַ�������pszError��λNULL�����Ӵ˷��ش�����ʾ�ַ���
//  pazError ӦΪһ��LPTSTR���ͱ����ĵ�ַ���������޸Ĵ˱�����ֵΪ������ʾ
LPTSTR MciSendRetStr(LPCTSTR szCmd, LPTSTR *pszError=NULL);

//GetMciErrorStr: ��øո�mci����ʧ��ԭ����ַ���
//fdwError ΪmciSendCommand ��mciSendString ���������صĴ������
LPTSTR MciGetErrorStr(DWORD fdwError);