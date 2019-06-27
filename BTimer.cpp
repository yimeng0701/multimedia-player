/////////////////////////////////////////////////////////////////////
//BTimer.cpp��CBTimer���ʵ��
//
//
//Programmed By: WYM
//Last Modified: 2014-12-11
/////////////////////////////////////////////////////////////////////

#include "BTimer.h"

////////////////////////////////////////////////////////////////////
//����CBTimer���е�static��Ա

UINT CBTimer::ms_uIDIncrement=0;

///////////////////////////////////////////////////////////////////
//ʵ��CBTimer���еĸ���Ա����

CBTimer::CBTimer( HWND hWndAsso/*=NULL*/,UINT uElapse/*=1000*/,TIMERPROC ptrFuncCallBack/*=NULL */ )
{
	//�����Ա����
	m_uTimerID=0;
	m_hwndAsso=NULL;
	m_uElapse=0;
	m_ptrFuncCallBack=NULL;
	m_blEnabled=false;

	//ͨ�����ó�Ա������ʹ����������Ϣ��Ч
	//��������m_blEnabledΪfalse�����������õĳ�Ա����
	//���������ö�ʱ�����������ñ�����ĳ�Ա������ֵ
	hWndAssoSet(hWndAsso);	//�����п��ܻ�ı�m_uTimerID
	IntervalSet(uElapse);
	pFuncCallBackSet(ptrFuncCallBack);

	//���ó�Ա����EnabledSet(true)�����ö�ʱ��
	//��m_ptrFuncCallBackΪ0������uElapseΪ0,��EnabledSet����������
	EnabledSet(true);
}

CBTimer::~CBTimer()
{
	//��������ʱ������EnabledSet(false);ɾ����ʱ��
	EnabledSet(false);
}

bool CBTimer::Enabled()
{
	return m_blEnabled;	//���ص�ǰ��ʱ������/����״̬	
}

void CBTimer::EnabledSet(bool value)
{
	//�����ʱ��״̬��value��ͬ����ֱ�ӷ��أ����ظ�����Ϊ����/����
	if(m_blEnabled==value) return;
	
	if(value)
	{
		if(m_ptrFuncCallBack!=NULL&& m_uElapse!=0)	//������Ч�����ö�ʱ��
		{
			//����API����SetTimer������ʱ��
			//����ʱm_hwndAsso��Ϊ0����ʱ��ID��ʹ��m_uTimerID���е�ֵ
			//SetTimer���óɹ�ʱ���ص�Ҳ�Ǹ�ֵ
			//����ʱm_hwndAssoΪ0��SetTimer������m_uTimerID���е�ֵ
			//SetTimer���óɹ�ʱ����API�����һ����ʱ��ID
			UINT ret=SetTimer(m_hwndAsso,m_uTimerID,m_uElapse,m_ptrFuncCallBack);
			if(ret)	//SetTimer���óɹ�
			{
				m_uTimerID=ret;
				m_blEnabled=true;	//��ʱ�������ɹ�����ʱ��������
			}
			else  	//SetTimer����ʧ�ܣ�SetTimer����ֵΪ0��
			{
				//����m_uTimerID���䣬���ܽ�֮��Ϊ0
				m_blEnabled=false;	//��ʱ������ʧ�ܣ�SetTimer����ֵΪ0��˵����ʱ����δ���ã�
			}//end of if(ret)	//SetTimer���óɹ�
		}
		else		//������Ч��δָ���ص�������ַ����ʱ��Ϊ0���������ö�ʱ��
		{
			m_blEnabled=false;	//δ���ö�ʱ��
		}//end of if(m_ptrFuncCallBack!=NULL&& m_uElapse!=0)	//������Ч�����ö�ʱ��
	}
	else	// if(value) else ���ö�ʱ��
	{
		BOOL bl=KillTimer(m_hwndAsso,m_uTimerID);
		m_blEnabled=false;	//��ʱ���ѱ�����
	}	//end of if(value)
}

UINT CBTimer::Interval()
{
	return m_uElapse;	//���ض�ʱ����ǰ�Ĵ���ʱ���������룩
}

void CBTimer::IntervalSet( UINT uInterval )
{
	//��ʱ����δ�䣬ֱ�ӷ���
	if(m_uElapse==uInterval) return;

	//�����µ�ʱ����
	m_uElapse=uInterval;

	//ʹ��ʱ������Ч
	if (m_blEnabled)            //����ʱ����������
	{
		EnabledSet(false);		//��ɾ����ʱ��
		EnabledSet(true);		//Ȼ������������
	}
//	else//�����ʱ��δ�����ã��򲻱����κ��£�������������
		//EnabledSet������ʱ���ͻ����µ�ʱ����������
}

UINT CBTimer::identifier()
{
	//���ض�ʱ����m_uTimerID��ֻ������ʱ����ֵ��Ч������ʱ����0��
	//��ʱ������ʱ������m_uTimerID,���򷵻�0
	if(m_blEnabled) return m_uTimerID;else return 0;
}

TIMERPROC CBTimer::pFuncCallBack()
{
	return m_ptrFuncCallBack;	//���ض�ʱ����ǰ�Ļص�������ַ�����룩
}

void CBTimer::pFuncCallBackSet( TIMERPROC ptrFuncCallBack )
{
	//���ص�������ַδ�䣬ֱ�ӷ���
	if(m_ptrFuncCallBack==ptrFuncCallBack) return;

	//�����µĻص�������ַ
	m_ptrFuncCallBack=ptrFuncCallBack;

	//ʹ�»ص�������ַ��Ч
	if (m_blEnabled)            //����ʱ����������
	{
		EnabledSet(false);		//��ɾ����ʱ��
		EnabledSet(true);		//Ȼ������������
	}
//	else//�����ʱ��δ�����ã��򲻱����κ��£�������������
		//EnabledSet������ʱ���ͻ����µĻص�������ַ������
}

HWND CBTimer::hWndAsso()
{
	return m_hwndAsso;
}

void CBTimer::hWndAssoSet( HWND hwnd )
{
	//����������δ�䣬ֱ�ӷ���
	if(m_hwndAsso==hwnd) return;

	//���������Ѹģ���ɾ����ʱ��
	bool blLastEnabled=m_blEnabled;	//��֮ǰ�Ķ�ʱ����״̬
	if(m_blEnabled) EnabledSet(false);

	//�����µĹ�������
	m_hwndAsso=hwnd;
	//�趨Time ID=>m_uTimerID
	if(m_hwndAsso)
	{
		//��ָ���˹������ھ����Ϊ0����ָ��һ��Ψһ��Time ID��
		//  ͨ��ms_uIDIncrement��1ȷ��ָ��һ��û��ʹ�ù���ID
		ms_uIDIncrement++;
		m_uTimerID=ms_uIDIncrement;
	}
//else
	//����������ھ��Ϊ0��API����������ָ��Time ID
	//������ʱ����m_uTimerID������API���ص�Time ID

	//ʹ�¹���������Ч
	if(blLastEnabled)		//����֮ǰ��ʱ���������ã���������������
	{
		EnabledSet(true);
	}
//else  //�����ʱ��δ�����ã��򲻱����κ��£�������������
	    //EnabledSet������ʱ���ͻ����µĹ�����������
}

