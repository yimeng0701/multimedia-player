#include "resource.h"
#include "BForm.h"
#include "mdlOpenSaveDlg.h"
#include "mdlMCI.h"
#include <commctrl.h>
#include "BTimer.h"
#include "mdlPathDlg.h"
#include "mdlFileSys.h"
#include "mdlShellExec.h"

CBForm Form1(ID_form1);
CBForm FrmPlayList(ID_frmPlayList);
CBTimer TimerPlay;


enum EPlayStatus            //各种播放状态的枚举定义
{
	eToPlayStart = 0,       //准备从头开始播放
	ePlaying = 1,           //正在播放中
	ePaused = 2             //正在暂停，再播放时可从暂停处继续
};
enum EPlayStatus m_playStatus = eToPlayStart;    //提示当前播放状态
TCHAR m_currFile[1024]={0};                  //当前打开文件的文件名全路径
	                                             //若尚未打开文件,为"" (*m_currFile == 0)
int m_indexCurrentPlay=0;

//函数声明
bool OpenMedia (LPCTSTR szFile); //打开一个媒体（但并不开始播放）
void CloseMedia();               //关闭现在已打开的媒体
void UpdateStatus();             //更新控件文本和控件状态
//OpenMedia: 打开一个媒体（但并不开始播放）
bool OpenMedia(LPTSTR szFile)
{
	LPTSTR szError;
	bool r;

	//已打开媒体不能重复打开，先关闭现有媒体（如果有的话），然后再打开新媒体
	CloseMedia();

	//打开新媒体：拼接字符串“open "文件名" alias MEDIA”
	//文件名加" "以支持含空格的文件名
	r=MciSend(
	  StrAppend(TEXT("open "), TEXT("\""), szFile, TEXT("\""), TEXT(" alias MEDIA")),
	  &szError );
	if (!r)    //若打开失败，弹出错误提示；错误提示是MciSend 函数从参数返回的 szError
	{  MsgBox( StrAppend(szFile, TEXT("\r\n"), szError), TEXT("打开媒体失败"),
	      mb_OK, mb_IconExclamation );
	   return false;
	}
	//设置当前打开的媒体文件
	_tcscpy(m_currFile, szFile);

	//设置媒体时间格式
	MciSend( TEXT("set MEDIA Time Format ms") );

	//设置播放进度条范围：0~最大时长
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length"));
	int iLen = Val(szLen);       // 将字符串 szLen 转换为整数；
	                             //注意不能写为(int)szLen, 后者是将地址转换为整数
	int i=(int)Form1.Control(ID_proPlay).hWnd();
	SendMessage(Form1.Control(ID_proPlay).hWnd(), PBM_SETRANGE32, 0, (LPARAM)iLen);

	//设置当前状态
	m_playStatus = eToPlayStart;
	UpdateStatus();

	return true;
}

//CloseMedia();   关闭现在已打开的媒体
void CloseMedia()
{
	MciSend( TEXT("stop MEDIA") );
	MciSend( TEXT("close MEDIA") );

	//设置当前状态
	*m_currFile = TEXT('\0');                 //设置当前打开的媒体文件为。。。
	m_playStatus = eToPlayStart;              //设置播放状态
	UpdateStatus();
}

//UpdateStatus(); 更新控件文本和控件状态
void UpdateStatus()
{
	if (*m_currFile == 0)
	{
		Form1. Control(ID_lblName).TextSet(TEXT("未打开媒体"));

		//设置播放，停止按钮的使能状态
		Form1.Control(ID_cmdPlay).EnabledSet(false);
        Form1.Control(ID_cmdStop).EnabledSet(false);
		Form1.Menu(ID_mnuControlPlay).EnabledSet(false);
        Form1.Menu(ID_mnuControlStop).EnabledSet(false);

	}
	else
	{ 
		//取m_currFile 字符串的文件主名部分显示到 ID_lblName
		TCHAR *p = m_currFile;
		while (*p) p++;              //让p指向m_currFile的末尾 \0
		while (*p!=TEXT('\\')) p--;  //让p指向m_currFile的Z最后一个“\”
		p++;                         //让p指向m_currFile的Z最后一个“\”的
		                             //下一个字符即文件主名的第一个字符
		Form1.Control(ID_lblName).TextSet(p);

		//设置播放、停止按钮的使能状态
        Form1.Control(ID_cmdPlay).EnabledSet(true);
        Form1.Control(ID_cmdStop).EnabledSet(true);
        Form1.Menu(ID_mnuControlPlay).EnabledSet(true);
        Form1.Menu(ID_mnuControlStop).EnabledSet(true);
	}
	
	//设置上一首，下一首按钮的使能状态
	int iCountList = Form1.Control(ID_lstPlayList).ListCount() ;
    int iSelList = Form1.Control(ID_lstPlayList).ListIndex();
    Form1.Control(ID_cmdPrev).EnabledSet(iCountList>1 && m_indexCurrentPlay>1);
    Form1.Control(ID_cmdNext).EnabledSet(iCountList>1 && m_indexCurrentPlay<iCountList);

	//根据m_playStatus 的状态设置控件文本和定时器状态
	switch(m_playStatus)
	{
	case eToPlayStart:
		Form1.Control(ID_cmdPlay).TextSet(TEXT("播放(&P)"));
		Form1.Menu(ID_mnuControlPlay).TextSet(TEXT("播放(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("就绪。"));
		TimerPlay.EnabledSet(false);        //停止定时器
		break;
	case ePlaying:
        Form1.Control(ID_cmdPlay).TextSet(TEXT("暂停(&P)"));
        Form1.Menu(ID_mnuControlPlay).TextSet(TEXT("暂停(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("正在播放..."));
        TimerPlay.EnabledSet(true);        //启动定时器
		break;
	case ePaused:
        Form1.Control(ID_cmdPlay).TextSet(TEXT("播放(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("暂停..."));
        TimerPlay.EnabledSet(false);        //停止定时器
		break;
	}

	//设置当前曲目号/共有曲目
	int iCount = Form1.Control(ID_lstPlayList).ListCount();
	if (iCount==0)
		Form1.Control(ID_lblNum).TextSet( TEXT("没有曲目") );
	else
        Form1.Control(ID_lblNum).TextSet(
		    StrAppend( Str(m_indexCurrentPlay), TEXT("/"), Str(iCount), TEXT(" "))
			);
}


void cmdOpen_Click()
{
	OsdSetFilter(TEXT("媒体格式|*.wav;*.mid;*.rmi;*.au;*.snd;*.aif;*.mp3;*.avi;*.mov;*.mpg;*.mpeg;*.dat;*.cda"));
	LPTSTR file = OsdOpenDlg(Form1.hWnd(),TEXT("请选择要播放的文件"));  //用“打开”对话框浏览文件
	if (*file)      // 如果 *file != 0说明用户选择了文件（不是按的取消）
	{
		Form1.Control(ID_lstPlayList).ListClear();     //清空列表
        Form1.Control(ID_lstPlayList).AddItem(file);   //使列表中只增加此项


        m_indexCurrentPlay = 1;         //正在播放列表中的第一个文件
		OpenMedia(file);     //打开此文件媒体
		MciSend( TEXT("play MEDIA") );   //发送播放命令
		m_playStatus = ePlaying;         //当前状态变为“正在播放”

		UpdateStatus();      //更新控件文本和控件状态
	}
}

void cmdPlay_Click()
{
	switch(m_playStatus)
	{
	case eToPlayStart:     //eToPlayStart和ePaused都执行ePaused的部分
	case ePaused:
		MciSend( TEXT("play MEDIA") );  //发送播放命令
        m_playStatus = ePlaying;        //当前状态变为“正在播放”
		break;
	case ePlaying:
	    MciSend( TEXT("pause MEDIA") );  //发送播放命令
        m_playStatus = ePaused;      //当前状态变为“正在播放”
		break;
	}
	UpdateStatus();       //更新控件文本和文本状态
}

void cmdStop_Click()
{
        MciSend( TEXT("seek MEDIA to start") );  //将媒体播放位置置为开头位置
        m_playStatus = eToPlayStart;
		UpdateStatus();       //更新控件文本和文本状态
}

void cmdPrev_Click()
{
    int iCount = Form1.Control(ID_lstPlayList).ListCount();
	m_indexCurrentPlay--;
	if (m_indexCurrentPlay<1) m_indexCurrentPlay=iCount;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //打开文件媒体
    MciSend( TEXT("play MEDIA") );  //发送播放命令
    m_playStatus = ePlaying;        //当前状态变为“正在播放”

	UpdateStatus();                  //更新空间文本和控件状态
}

void cmdNext_Click()
{
    int iCount = Form1.Control(ID_lstPlayList).ListCount();
	m_indexCurrentPlay++;
	if (m_indexCurrentPlay>iCount) m_indexCurrentPlay=1;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //打开文件媒体
    MciSend( TEXT("play MEDIA") );  //发送播放命令
    m_playStatus = ePlaying;        //当前状态变为“正在播放”

	UpdateStatus();                  //更新空间文本和控件状态
}

void cmdList_Click()
{
		FrmPlayList.Show(1, Form1.hWnd());
}
//必须记住：在退出前关闭媒体
void Form1_QueryUnload(int pbCancel)
{
	CloseMedia();       //关闭现有打开的媒体（如果有的话）
}

VOID CALLBACK TimerPlay_Timer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//获得当前播放位置
	LPTSTR szCurrPos = MciSendRetStr(TEXT("status MEDIA position") );
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length") );
	int iPos = Val(szCurrPos);     //当前位置
	int iLen = Val(szLen);         //总长度

	//根据当前位置设置进度条提示
	SendMessage(Form1.Control(ID_proPlay).hWnd(), PBM_SETPOS, (WPARAM)iPos, 0);

	//显示文本的进度提示
	Form1.Control(ID_lblPos).TextSet(
		StrAppend(Str(iPos), TEXT("/"), Str(iLen))
		);  //****iPos,iLen 为毫秒单位

	if (iPos >= iLen)
	{
		//播放结束
		if (Form1.Control(ID_lstPlayList).ListCount()<2 )
		{
			//播放列表中只有一项
		    MciSend( TEXT("seek MEDIA to start") );     //将媒体播放位置置为开头位置
		    m_playStatus = eToPlayStart;
		}
		else
		{
			//播放列表中有多于1项
			//播放列表中的下一首
			cmdNext_Click();
		}
		UpdateStatus();                              //更新控件文本和控件状态
	}
}

void lstPlayList_DblClick(int button, int shift, int x, int y)
{
	//在列表中双击，播放选定项
	int iSel= Form1.Control(ID_lstPlayList).ListIndex();
	if (iSel<=0) return;

	m_indexCurrentPlay = iSel;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay)); //打开文件媒体
	MciSend( TEXT("play MEDIA") ); //发送播放命令
	m_playStatus = ePlaying;        //当前状态变为“正在播放”

	UpdateStatus();             //更新控件文本和控件状态
}

void Form1_Load()
{
	//设置Form1.Control(ID_lblName)的外观
	//由于要连续使用该控件的多个成员函数，先将控件
	//  赋值到 lblName, 后连续用 lblName 以提高效率
	CBControl lblName =Form1.Control(ID_lblName);
	lblName.BackColorSet(0);           //背景色为黑色
    lblName.ForeColorSet(0xff00);      //前景色为绿色
    lblName.FontNameSet(TEXT("华文楷体"));     //字体
    lblName.FontBoldSet(true);                 //加粗
    lblName.FontSizeSet(25);                   //字号

	// 设置 Form1.Control(ID_lblNum) 的外观
    CBControl lblNum = Form1.Control(ID_lblNum);
    lblNum.BackColorSet(0);           //背景色为黑色
    lblNum.ForeColorSet(0xff00);      //前景色为绿色
    lblNum.FontNameSet(TEXT("华文楷体"));     //字体
    lblNum.FontBoldSet(true);                 //加粗
    lblNum.FontSizeSet(25);                   //字号

	//设置定时器
	TimerPlay.hWndAssoSet(Form1.hWnd());
	TimerPlay.IntervalSet(200);                 //每200毫秒更新一次
	TimerPlay.pFuncCallBackSet(TimerPlay_Timer);           //设置触发函数
	TimerPlay.EnabledSet(false);                //先设置为false, 播放时再启动

	// 更新控件文本状态
	UpdateStatus();
}



void ProPlay_MouseDown(int button, int shift, int x, int y)
{
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length") );
	int iLen = Val(szLen);        //总长度
	int iPos;
	//根据点击位置（x坐标）计算应跳转到的位置：iPos
	iPos = (double)x/Form1.Control(ID_proPlay).ClientWidth() * iLen;

	// 跳转到 iPos 位置开始播放
	MciSend(TEXT ("stop MEDIA"));        //先停止媒体
	MciSend( StrAppend(TEXT("seek MEDIA to "), Str(iPos)) );         //搜索到 iPos 的位置
    MciSend(TEXT ("play MEDIA"));        //重新开始播放
}


////////////////////////////////////////////////////////////////////////////////////////
// FrmPlayList 窗体代码
////////////////////////////////////////////////////////////////////////////////////////
void FrmPlayList_Load()
{   //将Form1 的ID_lstPlayList 的内容都送入 FrmPlayList的 ID_lstSet 中
	int i;
	for (i=1; i<=Form1.Control(ID_lstPlayList).ListCount(); i++)
		FrmPlayList.Control(ID_lstListSet).AddItem(
		    Form1.Control(ID_lstPlayList).List(i)   );
}

void cmdBrowse_Click()
{
	//选择一个目录=> szPath
	LPTSTR szPath;
	szPath = BrowPath(FrmPlayList.hWnd(), TEXT("请选择媒体文件所在文件夹："));
	if (*szPath == 0) return;   //用户按取消

	// 列出该目录下的文件: 调用BWindows 通用模块中的 FMListFilesAPI
	TCHAR **files, **folders;
	int iFiles, iFolders;
	int i;
	FMListFilesAPI(szPath, files, folders, &iFiles, &iFolders);

	// 将该目录下的文件列在 FrmPlayList 的窗体的 ID_lstFiles 中
	// 各文件名是： files[0], files[1], files[2], ..., files[iFiles]
	for (i=1; i<=iFiles; i++)
		FrmPlayList.Control(ID_lstFiles). AddItem( files[i] );
}
void cmdRightAll_Click()
{
	// 将FrmPlayList 的窗体的 ID_lstFiles 中的所有项目全部加入
	//    播放表 （加入 FrmPlayList 窗体的 ID_lstListSet 中）
	int i;
	for (i=1; i<=FrmPlayList.Control(ID_lstFiles).ListCount(); i++)
        FrmPlayList.Control(ID_lstListSet).AddItem(
            FrmPlayList.Control(ID_lstFiles).List(i)
			);
}

void cmdRight_Click()
{ 
    // 将FrmPlayList 的窗体的 ID_lstFiles 中的选定项目加入
	//  播放表 （加入 FrmPlayList 窗体的 ID_lstListSet 中）

	//ID_listFiles 中的项目允许多选， 用 ListSelItems 函数获得所有选定项的编号
	int i;
	int iSelCount;
	int *iSelArr = FrmPlayList.Control(ID_lstFiles).ListSelItems(&iSelCount);
	         // iSelArr 可当数组用，其中保存了所有选定项的编号 

	for (i=1; i<=iSelCount; i++)
        FrmPlayList.Control(ID_lstListSet).AddItem(
            FrmPlayList.Control(ID_lstFiles).List(iSelArr[i])
			);
}


void cmdLeftAll_Click()
{
	// 清除 FrmPlayList 窗体的 ID_LstListSet (播放表) 中的所有项目
    FrmPlayList.Control(ID_lstListSet).ListClear();
}

void cmdLeft_Click()
{
    // 将 FrmPlayList 窗体的 ID_LstListSet (播放表) 中的选定项目删除

	// ID_LstListSet 中的项目允许多选， 用ListSelItems 函数获得所有选定项的编号
    int i;
	int iSelCount;
	int * iSelArr = FrmPlayList.Control(ID_lstListSet).ListSelItems(&iSelCount);
	         // iSelArr 可当数组用，其中保存了所有选定项的编号 

	for (i=iSelCount; i>=1; i--)   //逆序删除，因前者删除将影响后面项的编号
        FrmPlayList.Control(ID_lstListSet).RemoveItem(iSelArr[i]);
}

void lstFiles_DblClick(int button, int shift, int x, int y)
{
	// 双击 lstFiles 执行 cmdRight_Click()
	// 若函数位于此后，需在本文件开头加函数声明 void cmdLeft_Click();
	cmdRight_Click();
}

void lstListSet_DblClick(int button, int shift, int x, int y)
{
    // 双击 lstListSet 执行 cmdLeft_Click()
	// 若函数位于此后，需在本文件开头加函数声明 void cmdLeft_Click();
	cmdLeft_Click();
}

void cmdOK_Click()
{
	//将 FrmPlayList 中的 ID_lstListSet 的项目都送入 Form1 的ID_lstPlayList
	int i;
	int iCount = FrmPlayList.Control(ID_lstListSet).ListCount();
	Form1.Control(ID_lstPlayList).ListClear();
	for (i=1; i<=iCount; i++)
        Form1.Control(ID_lstPlayList).AddItem(
		    FrmPlayList.Control(ID_lstListSet).List(i)
			);

	//若尚未打开文件，且播放列表中有项目，现在打开列表中的第一项
	if (*m_currFile ==0 && iCount>0)
	{
		m_indexCurrentPlay =1;
		OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //打开文件媒体
		MciSend( TEXT("play MEDIA") );    //发送播放命令
		m_playStatus = ePlaying;          //当前状态变为“正在播放”
	}

	//更新空间文本和控件状态
	UpdateStatus();

	//卸载 FrmPlayList
	FrmPlayList.UnLoad();
}

void cmdCancel_Click()
{
	FrmPlayList.UnLoad();
}


////////////////////////////////////////////菜单功能////////////////////////////////////////////////////////

void Menu_Click(int menuID, int bIsFromAcce, int bIsFromSysMenu)
{
	LPTSTR szError;
	switch(menuID)
	{
	case ID_mnuFileOpen:
		cmdOpen_Click();
		break;
	case ID_mnuFilePlayList:
		cmdList_Click();
		break;
	case ID_mnuFileOpenDoor:
		if ( ! MciSend(TEXT("set cdaudio door open"), &szError) )
			MsgBox(StrAppend(TEXT("无法打开光驱门: "), szError),
			    TEXT("无法打开光驱门"), mb_OK, mb_IconExclamation);
		break;
     case ID_mnuFileCloseDoor:
		if ( ! MciSend(TEXT("set cdaudio door closed"), &szError) )
			MsgBox(StrAppend(TEXT("无法打开光驱门: "), szError),
			    TEXT("无法打开光驱门"), mb_OK, mb_IconExclamation);
		break;
	 case ID_mnuFileExit:
		 Form1.UnLoad();
		 break;
	 case ID_mnuControlPlay:
		 cmdPlay_Click();
		 break;
	 case ID_mnuControlStop:
		 cmdStop_Click();
		 break;
     case ID_mnuControlPrev:
		 cmdPrev_Click();
		 break;
	 case ID_mnuControlNext:
		 cmdNext_Click();
		 break;
	 case ID_mnuControlVioceL:
		 Form1.Menu(ID_mnuControlVioceL).CheckedSet(
			 ! Form1.Menu(ID_mnuControlVioceL).Checked() );
		 if (Form1.Menu(ID_mnuControlVioceL).Checked() )
			 MciSend(TEXT("set MEDIA audio left on"));
		 else
             MciSend(TEXT("set MEDIA audio left off"));
		 break;
     case ID_mnuControlVioceR:
		 Form1.Menu(ID_mnuControlVioceR).CheckedSet(
			 ! Form1.Menu(ID_mnuControlVioceR).Checked() );
		 if (Form1.Menu(ID_mnuControlVioceR).Checked() )
			 MciSend(TEXT("set MEDIA audio right on"));
		 else
             MciSend(TEXT("set MEDIA audio right off"));
		 break;
	 
	 case ID_mnuControlVoice:
		 SEShellRun(TEXT("sndvol32.exe"));         //注意要包含文件 #include "mdlShellExec.h"
		 break;
	 
	 case ID_mnuHelpAbout:
		MsgBox(TEXT("我的媒体播放器 版本3.0\n作者：吴伊萌   生物医学工程2班\n\n版权所有(C) 2015"),
			TEXT("关于我的媒体播放器"), mb_OK, mb_IconInformation);
		 break;
	 case ID_mnuHelpTopic: 
		 LPTSTR szFile;
		 //拼接程序运行目录下的 cale.chm文件
		 szFile =StrAppend(pApp->Path(),TEXT("calc.chm") );
         SEShellRun(szFile);
		 break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

main()
{
	Form1.EventAdd(0, eForm_Load, Form1_Load);
    Form1.EventAdd(0, eForm_QueryUnload, Form1_QueryUnload);
    Form1.EventAdd(ID_cmdOpen, eCommandButton_Click, cmdOpen_Click);
    Form1.EventAdd(ID_cmdPlay, eCommandButton_Click, cmdPlay_Click);
    Form1.EventAdd(ID_cmdStop, eCommandButton_Click, cmdStop_Click);
    Form1.EventAdd(ID_cmdPrev, eCommandButton_Click, cmdPrev_Click);
    Form1.EventAdd(ID_cmdNext, eCommandButton_Click, cmdNext_Click);
    Form1.EventAdd(ID_cmdList, eCommandButton_Click, cmdList_Click);
	Form1.EventAdd(ID_proPlay, eMouseDown, ProPlay_MouseDown);
    Form1.EventAdd(ID_lstPlayList, eDblClick, lstPlayList_DblClick);
    Form1.EventAdd(0, eMenu_Click, Menu_Click);
	
	//注意以下都市 FrmPlayList 窗体的事件关联，
	//都是 FrmPlayList.EventAdd, 不是Form1.EventAdd
    FrmPlayList.EventAdd(0, eForm_Load, FrmPlayList_Load);
    FrmPlayList.EventAdd(ID_cmdBrowse, eCommandButton_Click, cmdBrowse_Click);
    FrmPlayList.EventAdd(ID_cmdRightAll, eCommandButton_Click, cmdRightAll_Click);
    FrmPlayList.EventAdd(ID_cmdRight, eCommandButton_Click, cmdRight_Click);
    FrmPlayList.EventAdd(ID_cmdLeftAll, eCommandButton_Click, cmdLeftAll_Click);
    FrmPlayList.EventAdd(ID_cmdLeft, eCommandButton_Click, cmdLeft_Click);
    FrmPlayList.EventAdd(ID_cmdOK, eCommandButton_Click, cmdOK_Click);
    FrmPlayList.EventAdd(ID_cmdCancel, eCommandButton_Click, cmdCancel_Click);
    FrmPlayList.EventAdd(ID_lstFiles, eDblClick, lstFiles_DblClick);
    FrmPlayList.EventAdd(ID_lstListSet, eDblClick, lstListSet_DblClick);
	
    Form1.IconSet(IDI_ICON1);
	Form1.Show();

	UpdateStatus();             //更新控件文本和控件状态
}
	       
