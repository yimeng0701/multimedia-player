//////////////////////////////////////////////////////////////////////
// BControl.h: CBControl ��Ķ���
// ʵ�ֶԻ����е�һ���ؼ��ĸ��ֹ���
// ���������ȫ�� HM ����̬������ڴ�
//
// ֧�֣�
//     ��Ҫ BWindows��BForm��BMenuItem ģ���֧��
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BWndBase.h"
// BControl.cpp ������ #include "BForm.h"���� BControl.cpp
//   �е� CBControl ���ʵ���У����õ� CBForm ��

class CBForm;	// �������

class CBControl : public CBWndBase
{
public:
	// ���캯����hWndDialog Ϊ�Ի���ľ����idResControl Ϊ�ؼ�����Դid
	// ptrHashObjs Ϊ�ؼ����� Form ����CBForm�ࣩ��ά����λͼ����ꡢͼ��ȶ����
	//   �����ϣ��ĵ�ַ�����ڼ��ز˵����λͼ����Դʱ�������������
	CBControl(HWND hWndDialog=0, unsigned short int idResControl=0, CBHashLK * ptrHashObjs=NULL);		
	
	// ��������
	~CBControl();

	// ����ʹ�ñ���������һ���ؼ�����δ�ڹ��캯�������ã�Ҳ���ñ�����������
	// hWndDialog Ϊ�Ի���ľ����idResControl Ϊ�ؼ�����Դid
	// �ɹ������Ӵ��ڿؼ������ʧ�ܷ���0
	HWND SetResID(HWND hWndDialog=0, unsigned short int idResControl=0, CBHashLK * ptrHashObjs=NULL);	


	// ���þ�̬�ؼ���ͼƬ���������ذ汾��
	void PictureSet(UINT bmpResID) const;
	void PictureSet(LPCTSTR bmpFile) const;

	// PictureSetIcon �ɼ���ͼ�꣬Ҳ�ɼ��ض������
	//   typeRes ��Ϊ0ʱ��ָ���Զ�����Դ�����ͣ�Ϊ0��ʾIcon���ͣ�
	// ==== ���ض����������ַ�����=====
	// �� Form1.Control(ID_pic1).PictureSetIcon(TEXT("APPLE.ANI")); 
	//	// ���ض�������ļ�
	// �� Form1.Control(ID_pic1).PictureSetIcon(IDR_MYANI1, TEXT("MYANI"));   
	//	// ����Դ�ļ��м��� "MYANI" ���͵�һ����Դ��ani�ļ����ȱ�����Ϊ����Դ��
	void PictureSetIcon(EStandardIcon iconStd) const;
	void PictureSetIcon(UINT iconResID, LPCTSTR typeRes=0) const;
	void PictureSetIcon(LPCTSTR iconFile) const;
	
	// ֱ���Ծ������ͼƬ��
	// picType ΪͼƬ���ͣ�0,1,2,3 �ֱ��ʾ λͼ��ͼ�ꡢ��ꡢenhͼԪ
	void PictureSet(HANDLE hPic, UINT picType) const;	


	// ======== �������� ========
	// �ø��ӵ� STRControlProp ���洢
	// CBForm::ms_hashCtrls.ItemLong2 Ϊ��̬���ٵ� STRControlProp �ռ�ĵ�ַ

	// ���غ����ÿؼ��������״������ֵ�����޸Ĵ����࣬ͨ�� WM_SETCURSOR ʵ�֣�
	//   typeRes Ϊ 0 ʱ������ idResCursor Ϊ Cursor ���͵���Դ ID
	//   Ҳ�������������͵���Դ����ʱ typeRes Ϊ�����ַ������������ʱʹ�ã�
	long MousePointer();
	void MousePointerSet(EStandardCursor cursor);		
	void MousePointerSet(long idResCursor, LPCTSTR typeRes=0);

	// ���غ����ÿؼ�����ɫ
	COLORREF BackColor();
	void BackColorSet(COLORREF color);

	// ���غ����ÿؼ�ǰ��ɫ
	COLORREF ForeColor();
	void ForeColorSet(COLORREF color);

	// ��ÿؼ��ı��������صĲ����ַ���������ת��Ϊ������
	// retSuccess Ϊһ��ָ�룬ָ�����ݱ�ʾ�����Ƿ�ִ�гɹ�
	// ���� CBControl ���� TextInt��
	// Text��TextVal �̳� CBWndBase ����
	int TextInt(BOOL * retSuccess=0);

	// �����ַ�������
	LPTSTR TagString();
	void TagStringSet(LPCTSTR tagString);


	// ======== Static ��ؼ����� ========


	// ======== ��ť��ؼ����� ========

	// ���غ����õ�ѡ��ѡ��ť�Ƿ񱻹�ѡ�ˣ�0=δѡ��1=��ѡ��2=��̬
	unsigned int ValueChecked() const;			
	void ValueCheckedSet( UINT uCheck ) const;	


	// ======== ��Ͽ��б����ؼ����� ========

	// ���һ����Ŀ��������ӵ���Ŀ��ţ���1��ʼ���������� 0
	// index=0 ʱ����ĩβ��ӣ�������ָ���� index λ����ӣ����߲����Զ�����
	int AddItem(LPCTSTR item, int index = 0) const;	

	// ɾ��һ����Ŀ����Ŵ�1��ʼ������ɾ������б�����Ŀ����������0
	int RemoveItem(int index) const;

	// ���ع�����Ŀ����������<0��ֵ
	int ListCount() const;

	// ���ص�ǰѡ�����Ŀ��ţ���Ŵ�1��ʼ����������0
	// ���Զ�ѡ�б��ʹ�ã��򷵻ص��Ǿ��н������Ŀ���
	//   ������Ŀ��û��ѡ��ʱ������1����ʾ��1����Ŀ�н��㣩
	int ListIndex() const;

	// ѡ��һ����Ŀ����Ŀ��Ŵ�1��ʼ
	// �ú����Զ�ѡ�б��ʹ����Ч
	void ListIndexSet(int index) const;

	// ���һ����Ŀ�����ݣ�index ��1��ʼ
	LPTSTR List(int index) const;

	// ��ȡ������ �б���и���Ŀ�ĸ����������ݣ�index ��1��ʼ
	int ListItemData(int index) const;
	void ListItemDataSet(int index, int itemData);

	// ��ȡ������ ��ѡ�б��һ����Ŀ�Ƿ�ѡ�е�״̬��index ��1��ʼ
	bool ListSeleted(int index) const;
	void ListSeletedSet(int index, bool bSel);
	
	// ȫѡ/ȡ��ѡ�� ��ѡ�б��������Ŀ
	void ListSelAll(bool bSel);

	// ���غ������б��е�һ���ɼ���������ţ��� 1 ��ʼ��
	int ListTopIndex();
	void ListTopIndexSet(int idxTop);

	// ���ض�ѡ�б�����ѡ��Ŀ��
	int ListSelCount() const;

	// ���һ���б��������ѡ����Ŀ�������ţ�����һ�� int ������
	//   ����Ŀ�����Ŵ� 1 ��ʼ
	// �����±�� 1 ��ʼ���ɱ������Զ���������ռ�
	// �������������׵�ַ��û��ѡ���������� 0
	// ����Ԫ�ظ������ص����� pSelCount ��ָ�ռ䣬pSelCount Ϊ0ʱ������
	//   ���� ListSelCount ���ѡ���Ԫ�ظ���
	// �Ե�ѡ�б��������ֻ��һ��Ԫ��
	int * ListSelItems(int *pSelCount=0) const;

	// ����б����������
	void ListClear() const;

	// ��ȡ�������б���߶�
	int ListItemsHeight() const;
	void ListItemsHeightSet(int newHeight);


	// ======== �߼�ͨ�ù��� ========
	HWND hWnd() const;					// ����Ӵ��ڿؼ��Ĵ��ھ��
	HWND hWndParentForm() const;		// �������������Ĵ��ھ��
	// ����һ��������� CBForm ����ĵ�ַָ������ͨ���ؼ����ʸ�����
	// ���磺Form1.Control(ID_cmd1).ParentFormPtr()->BackColorSet(...);
	CBForm * ParentFormPtr() const;		
	// ����Ӵ��ڿؼ���Ĭ�ϴ��ڳ���ĵ�ַ��עʵ�ʴ˵�ַ�ĺ������ܲ���ʹ�ã����ѱ����໯��
	unsigned long PtrWndProcDef() const;			
	
private:
	// HWND m_hWnd���Ӵ��ڿؼ��ľ����ʹ�û��� BWndBase �ĳ�Ա����ֵ�� SetResID �����б�����
	// HWND m_hWndDlg�� �ؼ����ڴ���� hwnd��ʹ�û��� BWndBase �ĳ�Ա����ֵ�� SetResID �����б�����

	// TCHAR m_ClassName[128]�����ڿؼ���������ʹ�û��� BWndBase �ĳ�Ա
	// long m_atom�����ڿؼ���� atom �ţ�Ψһ��ʶһ���࣬ʹ�û��� BWndBase �ĳ�Ա
	//				������ֵ���� SetResID ���Զ�������

	unsigned short int m_ResCtrlID;	// �ؼ�����Դid����ֵ�� SetResID �����б�����
	CBHashLK * m_ptrHashObjs;		// �Ӵ��ڿؼ����� Form ����CBForm�ࣩ��ά����λͼ����ꡢͼ��ȶ���ľ����ϣ��ĵ�ַ����Form ��������ʱ���Զ��ͷţ��˵�����ص���Դ���أ���λͼ��Ӧ�������ص���Դ�����������
	
	virtual HFONT SetFontMLF(LOGFONT * lpLf);	// ���� lpLf ��ָ��Ľṹ����һ�����壬�����ô���ʹ�ø������壻
												//   �����������������ľ��
												// ����Ϊ���� CBWndBase ������麯��
												// CBControl �����غ�Ҫ�����غ����н�����������ַ���� STRControlProp.hFont
};