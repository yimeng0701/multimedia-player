////////////////////////////////////////////////////
//BTimer.h:CBT类的定义
//实现定时器
//
//
//Programmed By: WYM
//Last Modified: 2014-12-11
#include <windows.h>

class CBTimer
{
private:

	//===================================================================
	//私有静态成员和成员函数
	//静态成员是指所有该类的对象所公用的“全局”数据或函数
	//（但这种“全局”其他类的对象或类外不能用）
	//====================================================================
		static UINT ms_uIDIncrement;	//为获得定时器ID的增量变量

public:

	//====================================================================
	//类公有成员和成员函数
	//====================================================================
	
	//构造函数
	//ptrFuncCallBack为定时器触发时所要调用的回调函数的地址
	//该函数原型为VOID CALLBACK Timer1_Timer(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);
	//uElapse为定时器触发间隔时间（单位（毫秒））
	//hWndAsso为关联窗口句柄，此可为NULL 表示不与窗口关联
	// 以上参数都既可在此构造函数中给出，也可将来通风成员函数设置
	// 但必须给全 ptrFuncCallBack 和 UElapse,才能启动定时器
	//若在构造函数时指定的 ptrFuncCallBack和UElapse都不为零，则定时器立即被启动
	//否则必须通过相应的成员函数设置各属性，并以后用EnabledSet启动它

	CBTimer(HWND hWndAsso=NULL,UINT uElapse=1000,TIMERPROC ptrFuncCallBack=NULL );
	//析构函数
	~CBTimer();

	//返回或设置定时器的启用/禁用状态
	//若尚未设置好时间间隔和回调函数地址，则无法启用
	//可通过构造函数或IntervalSet和pFuncCallBack成员函数设置
	bool Enabled();
	void EnabledSet(bool value);

	//返回或设置定时器的触发间隔时间（单位：毫秒）
	UINT Interval();
	void IntervalSet(UINT uInterval);
	
	//返回或设置定时器触发时所要调用的回调函数的地址
	//函数原型为
	//
	TIMERPROC pFuncCallBack();
	void pFuncCallBackSet(TIMERPROC ptrFuncCallBack);

	//返回或设置关联窗口句柄，为0表示定时器未关联窗口
	HWND hWndAsso();
	void hWndAssoSet(HWND hwnd);

	//返回定时器的ID（只读）（只在启用时返回值有效；禁用时返回0）
	UINT identifier();

private:

	//==================================================================================
	//类私有成员和成员函数
	//==================================================================================

	UINT m_uTimerID;	//定时器标识（未关联窗口时为API分配的ID，否则为本类分配的ID	）
	HWND m_hwndAsso;	//所关联的窗口句柄（该项可为0 表示定时器未关联窗口）
	UINT m_uElapse;		//定时器触发间隔时间（单位：毫秒）
	TIMERPROC m_ptrFuncCallBack;	//定时器触发时回调函数的地址
	bool m_blEnabled;	            //当前定时器启用/禁用状态
	                                //启用时，将每隔m_uElapse毫秒就会调用回调函数一次
	
};
