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


enum EPlayStatus            //���ֲ���״̬��ö�ٶ���
{
	eToPlayStart = 0,       //׼����ͷ��ʼ����
	ePlaying = 1,           //���ڲ�����
	ePaused = 2             //������ͣ���ٲ���ʱ�ɴ���ͣ������
};
enum EPlayStatus m_playStatus = eToPlayStart;    //��ʾ��ǰ����״̬
TCHAR m_currFile[1024]={0};                  //��ǰ���ļ����ļ���ȫ·��
	                                             //����δ���ļ�,Ϊ"" (*m_currFile == 0)
int m_indexCurrentPlay=0;

//��������
bool OpenMedia (LPCTSTR szFile); //��һ��ý�壨��������ʼ���ţ�
void CloseMedia();               //�ر������Ѵ򿪵�ý��
void UpdateStatus();             //���¿ؼ��ı��Ϳؼ�״̬
//OpenMedia: ��һ��ý�壨��������ʼ���ţ�
bool OpenMedia(LPTSTR szFile)
{
	LPTSTR szError;
	bool r;

	//�Ѵ�ý�岻���ظ��򿪣��ȹر�����ý�壨����еĻ�����Ȼ���ٴ���ý��
	CloseMedia();

	//����ý�壺ƴ���ַ�����open "�ļ���" alias MEDIA��
	//�ļ�����" "��֧�ֺ��ո���ļ���
	r=MciSend(
	  StrAppend(TEXT("open "), TEXT("\""), szFile, TEXT("\""), TEXT(" alias MEDIA")),
	  &szError );
	if (!r)    //����ʧ�ܣ�����������ʾ��������ʾ��MciSend �����Ӳ������ص� szError
	{  MsgBox( StrAppend(szFile, TEXT("\r\n"), szError), TEXT("��ý��ʧ��"),
	      mb_OK, mb_IconExclamation );
	   return false;
	}
	//���õ�ǰ�򿪵�ý���ļ�
	_tcscpy(m_currFile, szFile);

	//����ý��ʱ���ʽ
	MciSend( TEXT("set MEDIA Time Format ms") );

	//���ò��Ž�������Χ��0~���ʱ��
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length"));
	int iLen = Val(szLen);       // ���ַ��� szLen ת��Ϊ������
	                             //ע�ⲻ��дΪ(int)szLen, �����ǽ���ַת��Ϊ����
	int i=(int)Form1.Control(ID_proPlay).hWnd();
	SendMessage(Form1.Control(ID_proPlay).hWnd(), PBM_SETRANGE32, 0, (LPARAM)iLen);

	//���õ�ǰ״̬
	m_playStatus = eToPlayStart;
	UpdateStatus();

	return true;
}

//CloseMedia();   �ر������Ѵ򿪵�ý��
void CloseMedia()
{
	MciSend( TEXT("stop MEDIA") );
	MciSend( TEXT("close MEDIA") );

	//���õ�ǰ״̬
	*m_currFile = TEXT('\0');                 //���õ�ǰ�򿪵�ý���ļ�Ϊ������
	m_playStatus = eToPlayStart;              //���ò���״̬
	UpdateStatus();
}

//UpdateStatus(); ���¿ؼ��ı��Ϳؼ�״̬
void UpdateStatus()
{
	if (*m_currFile == 0)
	{
		Form1. Control(ID_lblName).TextSet(TEXT("δ��ý��"));

		//���ò��ţ�ֹͣ��ť��ʹ��״̬
		Form1.Control(ID_cmdPlay).EnabledSet(false);
        Form1.Control(ID_cmdStop).EnabledSet(false);
		Form1.Menu(ID_mnuControlPlay).EnabledSet(false);
        Form1.Menu(ID_mnuControlStop).EnabledSet(false);

	}
	else
	{ 
		//ȡm_currFile �ַ������ļ�����������ʾ�� ID_lblName
		TCHAR *p = m_currFile;
		while (*p) p++;              //��pָ��m_currFile��ĩβ \0
		while (*p!=TEXT('\\')) p--;  //��pָ��m_currFile��Z���һ����\��
		p++;                         //��pָ��m_currFile��Z���һ����\����
		                             //��һ���ַ����ļ������ĵ�һ���ַ�
		Form1.Control(ID_lblName).TextSet(p);

		//���ò��š�ֹͣ��ť��ʹ��״̬
        Form1.Control(ID_cmdPlay).EnabledSet(true);
        Form1.Control(ID_cmdStop).EnabledSet(true);
        Form1.Menu(ID_mnuControlPlay).EnabledSet(true);
        Form1.Menu(ID_mnuControlStop).EnabledSet(true);
	}
	
	//������һ�ף���һ�װ�ť��ʹ��״̬
	int iCountList = Form1.Control(ID_lstPlayList).ListCount() ;
    int iSelList = Form1.Control(ID_lstPlayList).ListIndex();
    Form1.Control(ID_cmdPrev).EnabledSet(iCountList>1 && m_indexCurrentPlay>1);
    Form1.Control(ID_cmdNext).EnabledSet(iCountList>1 && m_indexCurrentPlay<iCountList);

	//����m_playStatus ��״̬���ÿؼ��ı��Ͷ�ʱ��״̬
	switch(m_playStatus)
	{
	case eToPlayStart:
		Form1.Control(ID_cmdPlay).TextSet(TEXT("����(&P)"));
		Form1.Menu(ID_mnuControlPlay).TextSet(TEXT("����(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("������"));
		TimerPlay.EnabledSet(false);        //ֹͣ��ʱ��
		break;
	case ePlaying:
        Form1.Control(ID_cmdPlay).TextSet(TEXT("��ͣ(&P)"));
        Form1.Menu(ID_mnuControlPlay).TextSet(TEXT("��ͣ(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("���ڲ���..."));
        TimerPlay.EnabledSet(true);        //������ʱ��
		break;
	case ePaused:
        Form1.Control(ID_cmdPlay).TextSet(TEXT("����(&P)"));
        Form1.Control(ID_lblStatus).TextSet(TEXT("��ͣ..."));
        TimerPlay.EnabledSet(false);        //ֹͣ��ʱ��
		break;
	}

	//���õ�ǰ��Ŀ��/������Ŀ
	int iCount = Form1.Control(ID_lstPlayList).ListCount();
	if (iCount==0)
		Form1.Control(ID_lblNum).TextSet( TEXT("û����Ŀ") );
	else
        Form1.Control(ID_lblNum).TextSet(
		    StrAppend( Str(m_indexCurrentPlay), TEXT("/"), Str(iCount), TEXT(" "))
			);
}


void cmdOpen_Click()
{
	OsdSetFilter(TEXT("ý���ʽ|*.wav;*.mid;*.rmi;*.au;*.snd;*.aif;*.mp3;*.avi;*.mov;*.mpg;*.mpeg;*.dat;*.cda"));
	LPTSTR file = OsdOpenDlg(Form1.hWnd(),TEXT("��ѡ��Ҫ���ŵ��ļ�"));  //�á��򿪡��Ի�������ļ�
	if (*file)      // ��� *file != 0˵���û�ѡ�����ļ������ǰ���ȡ����
	{
		Form1.Control(ID_lstPlayList).ListClear();     //����б�
        Form1.Control(ID_lstPlayList).AddItem(file);   //ʹ�б���ֻ���Ӵ���


        m_indexCurrentPlay = 1;         //���ڲ����б��еĵ�һ���ļ�
		OpenMedia(file);     //�򿪴��ļ�ý��
		MciSend( TEXT("play MEDIA") );   //���Ͳ�������
		m_playStatus = ePlaying;         //��ǰ״̬��Ϊ�����ڲ��š�

		UpdateStatus();      //���¿ؼ��ı��Ϳؼ�״̬
	}
}

void cmdPlay_Click()
{
	switch(m_playStatus)
	{
	case eToPlayStart:     //eToPlayStart��ePaused��ִ��ePaused�Ĳ���
	case ePaused:
		MciSend( TEXT("play MEDIA") );  //���Ͳ�������
        m_playStatus = ePlaying;        //��ǰ״̬��Ϊ�����ڲ��š�
		break;
	case ePlaying:
	    MciSend( TEXT("pause MEDIA") );  //���Ͳ�������
        m_playStatus = ePaused;      //��ǰ״̬��Ϊ�����ڲ��š�
		break;
	}
	UpdateStatus();       //���¿ؼ��ı����ı�״̬
}

void cmdStop_Click()
{
        MciSend( TEXT("seek MEDIA to start") );  //��ý�岥��λ����Ϊ��ͷλ��
        m_playStatus = eToPlayStart;
		UpdateStatus();       //���¿ؼ��ı����ı�״̬
}

void cmdPrev_Click()
{
    int iCount = Form1.Control(ID_lstPlayList).ListCount();
	m_indexCurrentPlay--;
	if (m_indexCurrentPlay<1) m_indexCurrentPlay=iCount;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //���ļ�ý��
    MciSend( TEXT("play MEDIA") );  //���Ͳ�������
    m_playStatus = ePlaying;        //��ǰ״̬��Ϊ�����ڲ��š�

	UpdateStatus();                  //���¿ռ��ı��Ϳؼ�״̬
}

void cmdNext_Click()
{
    int iCount = Form1.Control(ID_lstPlayList).ListCount();
	m_indexCurrentPlay++;
	if (m_indexCurrentPlay>iCount) m_indexCurrentPlay=1;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //���ļ�ý��
    MciSend( TEXT("play MEDIA") );  //���Ͳ�������
    m_playStatus = ePlaying;        //��ǰ״̬��Ϊ�����ڲ��š�

	UpdateStatus();                  //���¿ռ��ı��Ϳؼ�״̬
}

void cmdList_Click()
{
		FrmPlayList.Show(1, Form1.hWnd());
}
//�����ס�����˳�ǰ�ر�ý��
void Form1_QueryUnload(int pbCancel)
{
	CloseMedia();       //�ر����д򿪵�ý�壨����еĻ���
}

VOID CALLBACK TimerPlay_Timer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//��õ�ǰ����λ��
	LPTSTR szCurrPos = MciSendRetStr(TEXT("status MEDIA position") );
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length") );
	int iPos = Val(szCurrPos);     //��ǰλ��
	int iLen = Val(szLen);         //�ܳ���

	//���ݵ�ǰλ�����ý�������ʾ
	SendMessage(Form1.Control(ID_proPlay).hWnd(), PBM_SETPOS, (WPARAM)iPos, 0);

	//��ʾ�ı��Ľ�����ʾ
	Form1.Control(ID_lblPos).TextSet(
		StrAppend(Str(iPos), TEXT("/"), Str(iLen))
		);  //****iPos,iLen Ϊ���뵥λ

	if (iPos >= iLen)
	{
		//���Ž���
		if (Form1.Control(ID_lstPlayList).ListCount()<2 )
		{
			//�����б���ֻ��һ��
		    MciSend( TEXT("seek MEDIA to start") );     //��ý�岥��λ����Ϊ��ͷλ��
		    m_playStatus = eToPlayStart;
		}
		else
		{
			//�����б����ж���1��
			//�����б��е���һ��
			cmdNext_Click();
		}
		UpdateStatus();                              //���¿ؼ��ı��Ϳؼ�״̬
	}
}

void lstPlayList_DblClick(int button, int shift, int x, int y)
{
	//���б���˫��������ѡ����
	int iSel= Form1.Control(ID_lstPlayList).ListIndex();
	if (iSel<=0) return;

	m_indexCurrentPlay = iSel;

	OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay)); //���ļ�ý��
	MciSend( TEXT("play MEDIA") ); //���Ͳ�������
	m_playStatus = ePlaying;        //��ǰ״̬��Ϊ�����ڲ��š�

	UpdateStatus();             //���¿ؼ��ı��Ϳؼ�״̬
}

void Form1_Load()
{
	//����Form1.Control(ID_lblName)�����
	//����Ҫ����ʹ�øÿؼ��Ķ����Ա�������Ƚ��ؼ�
	//  ��ֵ�� lblName, �������� lblName �����Ч��
	CBControl lblName =Form1.Control(ID_lblName);
	lblName.BackColorSet(0);           //����ɫΪ��ɫ
    lblName.ForeColorSet(0xff00);      //ǰ��ɫΪ��ɫ
    lblName.FontNameSet(TEXT("���Ŀ���"));     //����
    lblName.FontBoldSet(true);                 //�Ӵ�
    lblName.FontSizeSet(25);                   //�ֺ�

	// ���� Form1.Control(ID_lblNum) �����
    CBControl lblNum = Form1.Control(ID_lblNum);
    lblNum.BackColorSet(0);           //����ɫΪ��ɫ
    lblNum.ForeColorSet(0xff00);      //ǰ��ɫΪ��ɫ
    lblNum.FontNameSet(TEXT("���Ŀ���"));     //����
    lblNum.FontBoldSet(true);                 //�Ӵ�
    lblNum.FontSizeSet(25);                   //�ֺ�

	//���ö�ʱ��
	TimerPlay.hWndAssoSet(Form1.hWnd());
	TimerPlay.IntervalSet(200);                 //ÿ200�������һ��
	TimerPlay.pFuncCallBackSet(TimerPlay_Timer);           //���ô�������
	TimerPlay.EnabledSet(false);                //������Ϊfalse, ����ʱ������

	// ���¿ؼ��ı�״̬
	UpdateStatus();
}



void ProPlay_MouseDown(int button, int shift, int x, int y)
{
	LPTSTR szLen = MciSendRetStr(TEXT("status MEDIA length") );
	int iLen = Val(szLen);        //�ܳ���
	int iPos;
	//���ݵ��λ�ã�x���꣩����Ӧ��ת����λ�ã�iPos
	iPos = (double)x/Form1.Control(ID_proPlay).ClientWidth() * iLen;

	// ��ת�� iPos λ�ÿ�ʼ����
	MciSend(TEXT ("stop MEDIA"));        //��ֹͣý��
	MciSend( StrAppend(TEXT("seek MEDIA to "), Str(iPos)) );         //������ iPos ��λ��
    MciSend(TEXT ("play MEDIA"));        //���¿�ʼ����
}


////////////////////////////////////////////////////////////////////////////////////////
// FrmPlayList �������
////////////////////////////////////////////////////////////////////////////////////////
void FrmPlayList_Load()
{   //��Form1 ��ID_lstPlayList �����ݶ����� FrmPlayList�� ID_lstSet ��
	int i;
	for (i=1; i<=Form1.Control(ID_lstPlayList).ListCount(); i++)
		FrmPlayList.Control(ID_lstListSet).AddItem(
		    Form1.Control(ID_lstPlayList).List(i)   );
}

void cmdBrowse_Click()
{
	//ѡ��һ��Ŀ¼=> szPath
	LPTSTR szPath;
	szPath = BrowPath(FrmPlayList.hWnd(), TEXT("��ѡ��ý���ļ������ļ��У�"));
	if (*szPath == 0) return;   //�û���ȡ��

	// �г���Ŀ¼�µ��ļ�: ����BWindows ͨ��ģ���е� FMListFilesAPI
	TCHAR **files, **folders;
	int iFiles, iFolders;
	int i;
	FMListFilesAPI(szPath, files, folders, &iFiles, &iFolders);

	// ����Ŀ¼�µ��ļ����� FrmPlayList �Ĵ���� ID_lstFiles ��
	// ���ļ����ǣ� files[0], files[1], files[2], ..., files[iFiles]
	for (i=1; i<=iFiles; i++)
		FrmPlayList.Control(ID_lstFiles). AddItem( files[i] );
}
void cmdRightAll_Click()
{
	// ��FrmPlayList �Ĵ���� ID_lstFiles �е�������Ŀȫ������
	//    ���ű� ������ FrmPlayList ����� ID_lstListSet �У�
	int i;
	for (i=1; i<=FrmPlayList.Control(ID_lstFiles).ListCount(); i++)
        FrmPlayList.Control(ID_lstListSet).AddItem(
            FrmPlayList.Control(ID_lstFiles).List(i)
			);
}

void cmdRight_Click()
{ 
    // ��FrmPlayList �Ĵ���� ID_lstFiles �е�ѡ����Ŀ����
	//  ���ű� ������ FrmPlayList ����� ID_lstListSet �У�

	//ID_listFiles �е���Ŀ�����ѡ�� �� ListSelItems �����������ѡ����ı��
	int i;
	int iSelCount;
	int *iSelArr = FrmPlayList.Control(ID_lstFiles).ListSelItems(&iSelCount);
	         // iSelArr �ɵ������ã����б���������ѡ����ı�� 

	for (i=1; i<=iSelCount; i++)
        FrmPlayList.Control(ID_lstListSet).AddItem(
            FrmPlayList.Control(ID_lstFiles).List(iSelArr[i])
			);
}


void cmdLeftAll_Click()
{
	// ��� FrmPlayList ����� ID_LstListSet (���ű�) �е�������Ŀ
    FrmPlayList.Control(ID_lstListSet).ListClear();
}

void cmdLeft_Click()
{
    // �� FrmPlayList ����� ID_LstListSet (���ű�) �е�ѡ����Ŀɾ��

	// ID_LstListSet �е���Ŀ�����ѡ�� ��ListSelItems �����������ѡ����ı��
    int i;
	int iSelCount;
	int * iSelArr = FrmPlayList.Control(ID_lstListSet).ListSelItems(&iSelCount);
	         // iSelArr �ɵ������ã����б���������ѡ����ı�� 

	for (i=iSelCount; i>=1; i--)   //����ɾ������ǰ��ɾ����Ӱ�������ı��
        FrmPlayList.Control(ID_lstListSet).RemoveItem(iSelArr[i]);
}

void lstFiles_DblClick(int button, int shift, int x, int y)
{
	// ˫�� lstFiles ִ�� cmdRight_Click()
	// ������λ�ڴ˺����ڱ��ļ���ͷ�Ӻ������� void cmdLeft_Click();
	cmdRight_Click();
}

void lstListSet_DblClick(int button, int shift, int x, int y)
{
    // ˫�� lstListSet ִ�� cmdLeft_Click()
	// ������λ�ڴ˺����ڱ��ļ���ͷ�Ӻ������� void cmdLeft_Click();
	cmdLeft_Click();
}

void cmdOK_Click()
{
	//�� FrmPlayList �е� ID_lstListSet ����Ŀ������ Form1 ��ID_lstPlayList
	int i;
	int iCount = FrmPlayList.Control(ID_lstListSet).ListCount();
	Form1.Control(ID_lstPlayList).ListClear();
	for (i=1; i<=iCount; i++)
        Form1.Control(ID_lstPlayList).AddItem(
		    FrmPlayList.Control(ID_lstListSet).List(i)
			);

	//����δ���ļ����Ҳ����б�������Ŀ�����ڴ��б��еĵ�һ��
	if (*m_currFile ==0 && iCount>0)
	{
		m_indexCurrentPlay =1;
		OpenMedia(Form1.Control(ID_lstPlayList).List(m_indexCurrentPlay));  //���ļ�ý��
		MciSend( TEXT("play MEDIA") );    //���Ͳ�������
		m_playStatus = ePlaying;          //��ǰ״̬��Ϊ�����ڲ��š�
	}

	//���¿ռ��ı��Ϳؼ�״̬
	UpdateStatus();

	//ж�� FrmPlayList
	FrmPlayList.UnLoad();
}

void cmdCancel_Click()
{
	FrmPlayList.UnLoad();
}


////////////////////////////////////////////�˵�����////////////////////////////////////////////////////////

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
			MsgBox(StrAppend(TEXT("�޷��򿪹�����: "), szError),
			    TEXT("�޷��򿪹�����"), mb_OK, mb_IconExclamation);
		break;
     case ID_mnuFileCloseDoor:
		if ( ! MciSend(TEXT("set cdaudio door closed"), &szError) )
			MsgBox(StrAppend(TEXT("�޷��򿪹�����: "), szError),
			    TEXT("�޷��򿪹�����"), mb_OK, mb_IconExclamation);
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
		 SEShellRun(TEXT("sndvol32.exe"));         //ע��Ҫ�����ļ� #include "mdlShellExec.h"
		 break;
	 
	 case ID_mnuHelpAbout:
		MsgBox(TEXT("�ҵ�ý�岥���� �汾3.0\n���ߣ�������   ����ҽѧ����2��\n\n��Ȩ����(C) 2015"),
			TEXT("�����ҵ�ý�岥����"), mb_OK, mb_IconInformation);
		 break;
	 case ID_mnuHelpTopic: 
		 LPTSTR szFile;
		 //ƴ�ӳ�������Ŀ¼�µ� cale.chm�ļ�
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
	
	//ע�����¶��� FrmPlayList ������¼�������
	//���� FrmPlayList.EventAdd, ����Form1.EventAdd
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

	UpdateStatus();             //���¿ؼ��ı��Ϳؼ�״̬
}
	       
