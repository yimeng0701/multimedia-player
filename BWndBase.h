//////////////////////////////////////////////////////////////////////
// BWndBase.h: CBWndBase ��Ķ���
// �������洰�ڹ��ܣ���Ϊ CBForm��CBControl ��Ļ���
//
// ֧�֣�
//     ��Ҫ BWindows ģ���֧��
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BWindows.h"

// ���峣������Щ����ͷ�ļ�û�У�
#define	WS_EX_LAYERED	0x80000
#define	LWA_COLORKEY	0x1
#define	LWA_ALPHA		0x2

class CBWndBase
{
public:
	// ���캯������������
	CBWndBase( HWND hWndToManage = NULL, HWND hWndParentForm = NULL );	
	~CBWndBase();

	// ��ú����ô��ڵ� ʹ�� ״̬
	void EnabledSet(bool enabledValue) const;	
	bool Enabled() const;	
	
	// ��ú����ô��ڵ� ���� ״̬
	void VisibleSet(bool visibleValue) const;	
	bool Visible() const;
	
	// ���غ����ô����Ƿ��й�����
	bool HScrollBar();
	void HScrollBarSet(bool bValue);
	bool VScrollBar();
	void VScrollBarSet(bool bValue);
	
	// ���غ����� �ؼ��Ƿ���հ� Tab ���ƶ�����
	bool TabStop();
	void TabStopSet(bool bValue);
	
	// ���غ�����Group����
	// �ӵ�һ�� Group=true �Ŀؼ���ʼ ����һ�� Group=True �Ŀ�
    //   ��Ϊֹ��֮������пؼ�Ϊһ��
	bool Group();
	void GroupSet(bool bValue);


	// ================================================
	// λ�á���С
	// ================================================

	// ��ú͸ı䴰�ڴ�С��λ�ã��ƶ����ڡ��ı䴰�ڴ�С��
	void Move(int left=0x7FFFFFFF, int top=0x7FFFFFFF, 
		int width=0x7FFFFFFF, int height=0x7FFFFFFF) const;
	int Left() const;
	int Top() const;
	int Width() const;
	int Height() const;
	int ClientWidth() const;
	int ClientHeight() const;
	void LeftSet(int left) const;
	void TopSet(int top) const;
	void WidthSet(int width) const;
	void HeightSet(int height) const;


	// ================================================
	// �ı�
	// ================================================

	// ���ô����ı����������ذ汾��
	void TextSet(LPCTSTR newText) const;
	void TextSet(char valueChar) const;
	void TextSet(unsigned short int valueInt) const;	// TCHAR
	void TextSet(int valueInt) const;	
	void TextSet(unsigned int valueInt) const; 
	void TextSet(unsigned long valueInt) const;
	void TextSet(float valueSng) const;
	void TextSet(double valueDbl) const;
	void TextSet(long double valueDbl) const;
	
	// ���ô����ı����������ذ汾��
	void TextAdd(LPCTSTR newText) const;
	void TextAdd(char valueChar) const;
	void TextAdd(unsigned short int valueInt) const;
	void TextAdd(int valueInt) const;	
	void TextAdd(unsigned int valueInt) const;
	void TextAdd(unsigned long valueInt) const;
	void TextAdd(float valueSng) const;
	void TextAdd(double valueDbl) const;
	void TextAdd(long double valueDbl) const;
	
	// ��ô����ı��������ַ���
	TCHAR * Text() const;
	// ��ô����ı�ת��Ϊ�� double ����
	double TextVal() const; 



	// ================================================
	// ����
	// ================================================
	
	// ���غ����� ������������������ַ���
	// �ַ����������Զ�����
	LPTSTR FontName();
	void   FontNameSet(LPCTSTR szFontName);

	// ���غ����� ��������Ĵ�С
	float FontSize();
	void  FontSizeSet(float fSize);

	// ���غ����� ���������Ƿ�Ӵ�
	bool FontBold();
	void FontBoldSet(bool value);

	// ���غ����� ���������Ƿ���»���
	bool FontUnderline();
	void FontUnderlineSet(bool value);

	// ���غ����� ���������Ƿ���б
	bool FontItalic();
	void FontItalicSet(bool value);

	// ���غ����� �����������ת�Ƕȣ���λ��1/10 �ȣ�
	float FontRotateDegree();
	void  FontRotateDegreeSet(float fDegree);

	// ���غ����� ����������ַ���
	BYTE FontCharSet();
	void FontCharSetSet(BYTE ucValue);


	// ����Ŀǰ������ʹ�õ��������ľ����0 ��ʾ����ʹ��Ĭ������
	HFONT hFont();
	



	
	// ================================================
	// �߿�
	// ================================================

	// ���غ����ô����Ƿ��б߿�
	bool Border();
	void BorderSet(bool bValue);
	
	// ���غ����� �Ƿ��ǶԻ������͵ı߿�
	// Creates a window that has a border of a style typically used 
	//    with dialog boxes. A window with this style cannot have a 
	//    title bar.
	bool BorderFrameDlg();
	void BorderFrameDlgSet(bool bValue);
	
	// ���غ����� �߿���Ա��϶��Ըı䴰�ڴ�С
	// Creates a window that has a sizing border. 
	//   Same as the WS_SIZEBOX style.
	bool BorderFrameThick();
	void BorderFrameThickSet(bool bValue);

	// ���غ����� �Ƿ�Ϊ��͹��еı߿�
	// Specifies that a window has a border with a raised edge.
	bool BorderRaised();
	void BorderRaisedSet(bool bValue);

	// ���غ����� �Ƿ�Ϊ�а��ݸеı߿�
	// Specifies that a window has a 3D look 
	//   �� that is, a border with a sunken edge.
	bool BorderSunken();
	void BorderSunkenSet(bool bValue);

	// ���غ����� �Ƿ�Ϊ��̬�߿�
	// Creates a window with a three-dimensional border style
	//   intended to be used for items that do not accept user input.
	bool BorderStatic();
	void BorderStaticSet(bool bValue);

	// ���غ����ô����Ƿ��б�����
	bool BorderTitleBar();
	void BorderTitleBarSet(bool bValue);
	
	// ���غ����� �Ƿ�Ϊ�������ߴ�����ʽ��խ��������
	// Creates a tool window, which is a window intended to be 
	//   used as a floating toolbar. A tool window has a title bar 
	//   that is shorter than a normal title bar, and the window 
	//   title is drawn using a smaller font. A tool window does not 
	//   appear in the task bar or in the window that appears when 
	//   the user presses ALT+TAB.
	bool BorderToolWindow();
	void BorderToolWindowSet(bool bValue);



	// ================================================
	// �߼�
	// ================================================

	// ���غ����ÿؼ��Ƿ�͸��
	bool Transparent();
	void TransparentSet(bool bTransparent);
	
	// ���غ����� �����͸���ȣ�0-255��0Ϊ��ȫ͸����255Ϊ��͸����
	//							����Ϊ����ȡ����Ч��
	//            ����δ�����ô���ʽ����-1��ϵͳ��֧�ַ��� -2
	// win2000���Ͽ����ã�winXP���Ͽɷ���
	int Opacity();
	void OpacitySet(int iOpacity);

	// ���غ����� ���塰�ڿա�����ɫ������Ϊ -1 ȡ����Ч��
	// ����-1���޴�Ч��
	// win2000���Ͽ����ã�winXP���Ͽɷ���
	COLORREF TransparencyKey();
	void TransparencyKeySet(COLORREF iTransColor);
	
	// ��ô��ڵ�����������ͨ��ָ��ı������ַ��������ݣ�
	const TCHAR * ClassName() const;	
	
	// �жϴ��ڵ����Ƿ���һ������
	bool IsClassName(LPCTSTR lpTestClassName) const;


	// ���ػ����ô��ڷ��
	// ����ʱ���� bOr > 0�� �������з��������
	//         �� bOr < 0���������з����ȡ�� newStyle
	//         �� bOr == 0�������з���Ϊ newStyle
	unsigned long Style();
	void StyleSet(unsigned long newStyle, int bOr=1);
	
	// ���ػ����ô��ڵ���չ���
	// ����ʱ���� bOr > 0�� ����������չ���������
	//         �� bOr < 0������������չ�����ȡ�� newStyleEx
	//         �� bOr == 0����������չ����Ϊ newStyleEx
	unsigned long StyleEx();
	void StyleExSet(unsigned long newStyleEx, int bOr=1);


	// ================================================
	// ����
	// ================================================

	// ˢ�´�����ʾ
	void Refresh();
	
	// ���ý��㵽������
	void SetFocus();
	
	// ���ô��ڻ�ؼ��� Z-˳�򣬼��Ǹ����������ڿؼ������Ǳ��������ڿؼ�������
	// position=0����λ���������ڿؼ�����ǰ�棻���� λ�������
	void ZOrder(int position=0);


	
	
	// ���и���ճ��
	void Cut();
	void Copy();
	void Paste();

protected:	// ����Ϊ�̳���ı�����Ա
	HWND m_hWnd;		// ���ھ��
	HWND m_hWndDlg;		// �������ļ̳���Ķ����ǿؼ����˱�ʾ�ؼ����ڴ���� hwnd
						// �������ļ̳���Ķ����Ǵ��壬���ô�ֵ������Ϊ0��

	TCHAR m_ClassName[128];	// ���ڿؼ�������
	long m_atom;			// ���ڿؼ���� atom �ţ�Ψһ��ʶһ���࣬ʹ�û��� BWndBase �ĳ�Ա
							// CBControl ��̳�ʱ���� SetResID ���Զ��� GetClassName ����
							// CBForm ��̳�ʱ���� CBForm_DlgProc �� WM_INITDIALOG ������
	
	HFONT GetFontMLF(LOGFONT * lpLf=NULL);		// �����ڵ�ǰ����������Ϣ��ȡ�� lpLf ��ָ��Ľṹ��
												//   ������Ŀǰ�������������
												// �� lpLf Ϊ��ָ�룬ֱ�ӷ���������������ȡ������Ϣ
	virtual HFONT SetFontMLF(LOGFONT * lpLf);	// ���� lpLf ��ָ��Ľṹ����һ�����壬�����ô���ʹ�ø������壻
												//   �����������������ľ��
												// �麯�����̳���������ظú���
												// CBControl �����غ�Ӧ�����غ����н�����������ַ���� STRControlProp.hFont
												// CBForm �����غ�Ӧ�����غ����н�����������ַ���� m_hFont ˽�г�Ա
};
