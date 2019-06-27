/////////////////////////////////////////////////////////////////////
//BTimer.cpp：CBTimer类的实现
//
//
//Programmed By: WYM
//Last Modified: 2014-12-11
/////////////////////////////////////////////////////////////////////

#include "BTimer.h"

////////////////////////////////////////////////////////////////////
//定义CBTimer类中的static成员

UINT CBTimer::ms_uIDIncrement=0;

///////////////////////////////////////////////////////////////////
//实现CBTimer类中的各成员函数

CBTimer::CBTimer( HWND hWndAsso/*=NULL*/,UINT uElapse/*=1000*/,TIMERPROC ptrFuncCallBack/*=NULL */ )
{
	//清零成员变量
	m_uTimerID=0;
	m_hwndAsso=NULL;
	m_uElapse=0;
	m_ptrFuncCallBack=NULL;
	m_blEnabled=false;

	//通过调用成员函数，使参数所给信息生效
	//由于现在m_blEnabled为false，以下所调用的成员函数
	//都不会启用定时器，而仅设置本对象的成员变量的值
	hWndAssoSet(hWndAsso);	//函数中可能会改变m_uTimerID
	IntervalSet(uElapse);
	pFuncCallBackSet(ptrFuncCallBack);

	//调用成员函数EnabledSet(true)；启用定时器
	//若m_ptrFuncCallBack为0，或者uElapse为0,则EnabledSet不会启用它
	EnabledSet(true);
}

CBTimer::~CBTimer()
{
	//对象析构时，调用EnabledSet(false);删除定时器
	EnabledSet(false);
}

bool CBTimer::Enabled()
{
	return m_blEnabled;	//返回当前定时器启用/禁用状态	
}

void CBTimer::EnabledSet(bool value)
{
	//如果定时器状态与value相同；则直接返回；不重复设置为启用/禁用
	if(m_blEnabled==value) return;
	
	if(value)
	{
		if(m_ptrFuncCallBack!=NULL&& m_uElapse!=0)	//参数有效，启用定时器
		{
			//调用API函数SetTimer创建定时器
			//若此时m_hwndAsso不为0，定时器ID将使用m_uTimerID现有的值
			//SetTimer调用成功时返回的也是该值
			//若此时m_hwndAsso为0，SetTimer将忽略m_uTimerID现有的值
			//SetTimer调用成功时返回API分配的一个定时器ID
			UINT ret=SetTimer(m_hwndAsso,m_uTimerID,m_uElapse,m_ptrFuncCallBack);
			if(ret)	//SetTimer调用成功
			{
				m_uTimerID=ret;
				m_blEnabled=true;	//定时器创建成功，定时器已启动
			}
			else  	//SetTimer调用失败（SetTimer返回值为0）
			{
				//保持m_uTimerID不变，不能将之改为0
				m_blEnabled=false;	//定时器创建失败（SetTimer返回值为0，说明定时器尚未启用）
			}//end of if(ret)	//SetTimer调用成功
		}
		else		//参数无效，未指定回调函数地址或间隔时间为0，不能启用定时器
		{
			m_blEnabled=false;	//未启用定时器
		}//end of if(m_ptrFuncCallBack!=NULL&& m_uElapse!=0)	//参数有效，启用定时器
	}
	else	// if(value) else 禁用定时器
	{
		BOOL bl=KillTimer(m_hwndAsso,m_uTimerID);
		m_blEnabled=false;	//定时器已被禁用
	}	//end of if(value)
}

UINT CBTimer::Interval()
{
	return m_uElapse;	//返回定时器当前的触发时间间隔（毫秒）
}

void CBTimer::IntervalSet( UINT uInterval )
{
	//若时间间隔未变，直接返回
	if(m_uElapse==uInterval) return;

	//设置新的时间间隔
	m_uElapse=uInterval;

	//使新时间间隔生效
	if (m_blEnabled)            //若定时器正在启用
	{
		EnabledSet(false);		//先删除定时器
		EnabledSet(true);		//然后重新启用它
	}
//	else//如果定时器未在启用，则不必做任何事；待主调函数用
		//EnabledSet启用它时，就会用新的时间间隔启用它
}

UINT CBTimer::identifier()
{
	//返回定时器的m_uTimerID（只在启用时返回值有效；禁用时返回0）
	//定时器启用时，返回m_uTimerID,否则返回0
	if(m_blEnabled) return m_uTimerID;else return 0;
}

TIMERPROC CBTimer::pFuncCallBack()
{
	return m_ptrFuncCallBack;	//返回定时器当前的回调函数地址（毫秒）
}

void CBTimer::pFuncCallBackSet( TIMERPROC ptrFuncCallBack )
{
	//若回调函数地址未变，直接返回
	if(m_ptrFuncCallBack==ptrFuncCallBack) return;

	//设置新的回调函数地址
	m_ptrFuncCallBack=ptrFuncCallBack;

	//使新回调函数地址生效
	if (m_blEnabled)            //若定时器正在启用
	{
		EnabledSet(false);		//先删除定时器
		EnabledSet(true);		//然后重新启用它
	}
//	else//如果定时器未在启用，则不必做任何事；待主调函数用
		//EnabledSet启用它时，就会用新的回调函数地址启用它
}

HWND CBTimer::hWndAsso()
{
	return m_hwndAsso;
}

void CBTimer::hWndAssoSet( HWND hwnd )
{
	//若关联窗口未变，直接返回
	if(m_hwndAsso==hwnd) return;

	//关联窗口已改，先删除定时器
	bool blLastEnabled=m_blEnabled;	//此之前的定时器的状态
	if(m_blEnabled) EnabledSet(false);

	//设置新的关联窗口
	m_hwndAsso=hwnd;
	//设定Time ID=>m_uTimerID
	if(m_hwndAsso)
	{
		//如指定了关联窗口句柄不为0，需指定一个唯一的Time ID；
		//  通过ms_uIDIncrement增1确保指定一个没有使用过的ID
		ms_uIDIncrement++;
		m_uTimerID=ms_uIDIncrement;
	}
//else
	//如果关联窗口句柄为0，API不需由我们指定Time ID
	//启动定时器后，m_uTimerID将保存API返回的Time ID

	//使新关联窗口生效
	if(blLastEnabled)		//若此之前定时器正在启用，现在重新启用它
	{
		EnabledSet(true);
	}
//else  //如果定时器未在启用，则不必做任何事；待主调程序用
	    //EnabledSet启用它时，就会以新的关联窗口启用
}

