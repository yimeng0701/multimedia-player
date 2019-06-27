//////////////////////////////////////////////////////////////////////
// mdlOpenSaveDlg.h: ʵ�ִ򿪡�����Ի����ģ�麯��ԭ�ͼ�������������
// ֧�֣�
//     ��Ҫ BWindows ģ���֧�֣����õ� CBHashStrK��CBRecycledArr��
//////////////////////////////////////////////////////////////////////


#include "BWindows.h"

// ========================================================================
// ��������
// ========================================================================

#define mc_strFilterAllFile TEXT("|�����ļ�(*.*)|*.*")		// Ĭ�������ļ��� Filter ���֣���Ҫֱ����ӵ� Filter �Ľ�β���֣���ͷ������"|"
#define mc_strFilterTxtFile TEXT("�ı��ļ�(*.txt)|*.txt")	// �ı��ļ��� Filter ���֣���Ҫֱ��ʹ�õ� Filter �Ŀ�ͷ���֣���ͷ������"|"
#define mcMaxFileLenSingle 2048		// Ԥ����������С  ��ѡ�ļ�ʱ�ô˴�С�Ļ������洢�����ļ�����·��������ʱҲ�ô˴洢�ļ�����·��
#define mcMaxFileLenMul 63488		// Ԥ����������С  �򿪶Ի���Ķ�ѡ�ļ�ʱ�ô˴�С�Ļ������洢�����ļ����ַ���




// ========================================================================
// ģ�鼶���������Ͷ��壬��Щȫ�ֱ���ֻ�ܱ���Դ�����ļ�ʹ�ã����ܱ�����Դ�����ļ�ʹ��
// ========================================================================

static CBRecycledArr mMulSelFiles;			// ����Ի��������ѡ�����һ�ζ�ѡ��ÿ���ļ������ڴ�
static TCHAR mFilePath[mcMaxFileLenSingle];	// ������һ��ѡ����ļ���(��ѡ�ļ�ʱΪ·��)��Ҳ�ɸ�����ֵ��Ϊ�Ի���ѡ���Ĭ���ļ�
static int mLastSelFilterIdx=0;				// ������һ�ζԻ�����û���"�ļ�����"�����б���ѡ��� filter �� ������
static TCHAR * mpStrFilter=NULL;				// �Ի�����ļ����� Filter �ַ���("|"�ָ�)��

// ��ϣ�����ѵǼǵĶԻ�����Ϣ
// Key=�ǼǶԻ���� Key (�ַ���)��ItemStr=�öԻ���� Filter��ItemStr2=�ϴ�ѡ����ļ�
//   ItemLong=�ϴ�ѡ��� Filter ��������
static CBHashStrK mRegedDlgKeys;				



// ========================================================================
// ��������
// ========================================================================


// �趨�Ի�����ļ����� Filter ("|"�ָ�); �����ַ����ռ䣬���µ� Filter �ַ�������˿ռ䣬���� mpStrFilter ָ����
void OsdSetFilter(LPCTSTR sFilter=NULL, bool bAddAllFile=true, LPCTSTR sAsRegKey=NULL);

// �򿪡����ļ����Ի���
// bAllowMulSel=false ʱ�����������ļ������ַ����ռ��ɱ�ģ�����������򱻰�ȡ������ ""����NULL����
// bAllowMulSel=true ʱ�����������ļ�·�����ַ����ռ��ɱ�ģ������������� mMulSelFiles
// bRetReadOnlyָ��һ�� bool �ͱ�����ָ�룬*b=true ʱ��ʾ����ֻ����ʽ�򿪡���ѡ�򣬲��ɴ˲������� 
//   �û��Ƿ�ѡ�ˡ���ֻ����ʽ�򿪡���ѡ��*b=false �� =NULL ����ʾ����ֻ����ʽ�򿪡���ѡ��
// ������ sAsRegedKey ��ʾ�ѵǼ�Ϊ sAsRegedKey �� Key �ĶԻ��򣬷�����ʾһ�����µġ��Ի���
LPTSTR OsdOpenDlg(HWND hWnd=0, LPCTSTR sDlgTitle=NULL, LPCTSTR sAsRegedKey=NULL, bool bAllowMulSel=false, bool *bRetReadOnly=NULL);

// �򿪡������ļ����Ի���
// sDefaultFileName ��ʾ�Զ���䵽�Ի����Ĭ���ļ���
// ������ sAsRegedKey ��ʾ�ѵǼ�Ϊ sAsRegedKey �� Key �ĶԻ��򣬷�����ʾһ�����µġ��Ի���
LPTSTR OsdSaveDlg(HWND hWnd=0, LPCTSTR sDefaultFileName=NULL, LPCTSTR sDlgTitle=NULL, LPCTSTR sAsRegedKey=NULL);


// ������һ�δ򿪶Ի���ѡ��Ķ�ѡ�ļ������ɱ�����󴴽���ָ̬������ pptrMulFiles
//   �������±�� 1 ��ʼ���ռ� [0] ����
//   ָ��������׵�ַ�ɲ������ء�����������һ�ж�̬�ռ�
//   ��ѡ�ļ��ĸ�����Ҳ�Ƕ�̬����ĸ������ɺ������أ�ÿ������Ԫ�طֱ�ָ��һ���ַ���
//   �ռ��ʾÿ��ѡ����ļ���ֻ���ļ������֣���·����
// ���磺
//	char **mul;
//	int fileCount, i;
//  fileCount=OSDMulSelFiles(mul);
//  for(i=1; i<=fileCount; i++)
//		cout<<mul[i]<<endl;
int OSDMulSelFiles(char ** &pptrMulFiles);

// �������һ�����õĶԻ�����ļ����� Filter ("|"�ָ�)
LPTSTR OsdLastFilter();

// �������һ��ѡ����ļ���(��ѡ�ļ�ʱΪ·��)
// ����ʱ���������½�һ���ַ����ռ䣬�����¿ռ��ַ���ÿռ��ɱ����ͷ�
LPTSTR OSDFilePath();

// ���� mFilePath ��ΪĬ���ļ���
void OSDSetInitFilePath(LPCTSTR newInitFilePath);

// ���� ���һ�ζԻ�����û���"�ļ�����"�����б���ѡ��� filter �� ������ mLastSelFilterIdx
int OSDLastSelFilterIdx();