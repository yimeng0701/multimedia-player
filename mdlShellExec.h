//////////////////////////////////////////////////////////////////////
// mdlShellExec.h: ���г������ĵ�
//  
// ֧�֣���Ҫ BWindows ģ���֧��
//////////////////////////////////////////////////////////////////////

#include "Bwindows.h"


// ϵͳ�ļ���ö������
enum eSESysFolder
{
	sesfMyComputer,		// �ҵĵ���
	sesfMyDocuments,	// �ҵ��ĵ�
	sesfNetworkPlaces,	// �����ھ�
    sesfRecycleBin		// ����վ
};




// ��ñ����������·��������ֵ�����'\'��
// bGet83PathFile=true ʱ���8.3���ļ����������ó��ļ���
LPTSTR SEAppPath(bool bGet83PathFile=false);


// ִ��һ�����򡢴�һ���ĵ����һ��URL
// ��bWithParam=true���� sFileOrURL �ĵ�һ���ո�ǰ����������ļ����� _
// ֮�������������򶼿��������ļ���������Ϊ��
//   winShowStyle ��ʾ������ʾ״̬����󻯡���С�������صȣ�
// �÷�������
//   ���г��� SEShellRun("C:\Windows\System32\mspaint.exe");
//   ���ļ��� SEShellRun("C:\Windows");
//   �Զ���������������ĵ� SEShellRun("E:\my\abc.txt");
//   ����ҳ��SEShellRun("http://www.163.com");
//   �����ʼ���SEShellRun("mailto:abc@def.com");
bool SEShellRun(LPCTSTR sFileOrURL, 
				bool bWithParam=false, 
				EShowWindowCmd winShowStyle=SW_ShowNormal);


// ��һ��ϵͳ�ļ���
bool SEOpenSysFolder(eSESysFolder sysFolder, EShowWindowCmd winShowStyle=SW_ShowNormal);

// ��ӡһ���ĵ�
bool SEPrintDocument(LPCTSTR fileToPrint, EShowWindowCmd winShowStyle=SW_ShowNormal);


// ����һ�����򲢵ȴ����������ؽ��̾����ʧ�ܷ���0
// bReturnUntilTerm = trueʱ��ֻ�г�������󣬺����Ż᷵�أ�
//   =False ������������һ�����̾����֮��ɷ�������
// SETestProcessTerminate ���Խ����Ƿ����
//   �� ��ʱ���� SETerminateProcess ��ֹ����
// �� bReturnUntilTerm=false������֮������ SECloseProcessHandle �رս��̾�
//   ������������ֵ������ bReturnUntilTerm=true ʱ�򲻱أ����������Զ��رվ��
HANDLE SERunWaitTerminate(LPCTSTR sExeFileWithParams,
						  bool bReturnUntilTerm = false, 
						  EShowWindowCmd winShowStyle=SW_ShowNormal);


// ����һ�������Ƿ��Ѿ�����
// ��û�иý��̻�ʧ��Ҳ���� true��ֻ�н����������вŷ��� false
// bClosehProcessIfTerm = true ��ʾ��������Ѿ������͹رվ�� hProcess
// ������������������е��� SETerminateProcess �رվ��
bool SETestProcessTerminate(HANDLE hProcess, bool bClosehProcessIfTerm = true);


// ��ֹһ������
// bClosehProcess���Ƿ��Զ��رս��̾��
bool SETerminateProcess(HANDLE hProcess, bool bClosehProcess = true);


// �ر�һ�����̾��
void SECloseProcessHandle(HANDLE hProcess);
