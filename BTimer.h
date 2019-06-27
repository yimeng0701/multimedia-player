////////////////////////////////////////////////////
//BTimer.h:CBT��Ķ���
//ʵ�ֶ�ʱ��
//
//
//Programmed By: WYM
//Last Modified: 2014-12-11
#include <windows.h>

class CBTimer
{
private:

	//===================================================================
	//˽�о�̬��Ա�ͳ�Ա����
	//��̬��Ա��ָ���и���Ķ��������õġ�ȫ�֡����ݻ���
	//�������֡�ȫ�֡�������Ķ�������ⲻ���ã�
	//====================================================================
		static UINT ms_uIDIncrement;	//Ϊ��ö�ʱ��ID����������

public:

	//====================================================================
	//�๫�г�Ա�ͳ�Ա����
	//====================================================================
	
	//���캯��
	//ptrFuncCallBackΪ��ʱ������ʱ��Ҫ���õĻص������ĵ�ַ
	//�ú���ԭ��ΪVOID CALLBACK Timer1_Timer(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);
	//uElapseΪ��ʱ���������ʱ�䣨��λ�����룩��
	//hWndAssoΪ�������ھ�����˿�ΪNULL ��ʾ���봰�ڹ���
	// ���ϲ������ȿ��ڴ˹��캯���и�����Ҳ�ɽ���ͨ���Ա��������
	// �������ȫ ptrFuncCallBack �� UElapse,����������ʱ��
	//���ڹ��캯��ʱָ���� ptrFuncCallBack��UElapse����Ϊ�㣬��ʱ������������
	//�������ͨ����Ӧ�ĳ�Ա�������ø����ԣ����Ժ���EnabledSet������

	CBTimer(HWND hWndAsso=NULL,UINT uElapse=1000,TIMERPROC ptrFuncCallBack=NULL );
	//��������
	~CBTimer();

	//���ػ����ö�ʱ��������/����״̬
	//����δ���ú�ʱ�����ͻص�������ַ�����޷�����
	//��ͨ�����캯����IntervalSet��pFuncCallBack��Ա��������
	bool Enabled();
	void EnabledSet(bool value);

	//���ػ����ö�ʱ���Ĵ������ʱ�䣨��λ�����룩
	UINT Interval();
	void IntervalSet(UINT uInterval);
	
	//���ػ����ö�ʱ������ʱ��Ҫ���õĻص������ĵ�ַ
	//����ԭ��Ϊ
	//
	TIMERPROC pFuncCallBack();
	void pFuncCallBackSet(TIMERPROC ptrFuncCallBack);

	//���ػ����ù������ھ����Ϊ0��ʾ��ʱ��δ��������
	HWND hWndAsso();
	void hWndAssoSet(HWND hwnd);

	//���ض�ʱ����ID��ֻ������ֻ������ʱ����ֵ��Ч������ʱ����0��
	UINT identifier();

private:

	//==================================================================================
	//��˽�г�Ա�ͳ�Ա����
	//==================================================================================

	UINT m_uTimerID;	//��ʱ����ʶ��δ��������ʱΪAPI�����ID������Ϊ��������ID	��
	HWND m_hwndAsso;	//�������Ĵ��ھ���������Ϊ0 ��ʾ��ʱ��δ�������ڣ�
	UINT m_uElapse;		//��ʱ���������ʱ�䣨��λ�����룩
	TIMERPROC m_ptrFuncCallBack;	//��ʱ������ʱ�ص������ĵ�ַ
	bool m_blEnabled;	            //��ǰ��ʱ������/����״̬
	                                //����ʱ����ÿ��m_uElapse����ͻ���ûص�����һ��
	
};