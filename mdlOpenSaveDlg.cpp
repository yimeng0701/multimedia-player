//////////////////////////////////////////////////////////////////////
// mdlOpenSaveDlg.cpp: ʵ�ִ򿪡�����Ի���
//
//////////////////////////////////////////////////////////////////////


#include <string.h>
#include <memory.h>
#include "mdlOpenSaveDlg.h"


// �趨�Ի�����ļ����� Filter ("|"�ָ�)
// �����ַ����ռ䣬���µ� Filter �ַ�������˿ռ䣬���� mpStrFilter ָ����
// sFilter Ϊ NULL ʱ��ʹ�á�"�ı��ļ�(*.txt)|*.txt"��
//         Ϊ "" ʱ���� bAddAllFile Ϊ true����ʹ�á������ļ�(*.*)|*.*��
// bAddAllFile �Ƿ��Զ���ĩβ���� "|�����ļ�(*.*)|*.*"
// sAsRegKey���Դ�Ϊ Key �ǼǶԻ���=""���Ǽǣ�
// �ѵǼǵĶԻ�����Ա����ϴδ򿪵��ļ�·�����ϴ�ѡ��� Filter
//   �� OsdOpenDlg/OsdSaveDlg �и��� Key ���ɴ�����Ի���
void OsdSetFilter( LPCTSTR sFilter/*=NULL*/, bool bAddAllFile/*=true*/, LPCTSTR sAsRegKey/*=NULL*/ )
{

	// ��� sFilter Ϊ NULL��������ָ���ַ������� mc_strFilterTxtFile �ĵ�ַ
	if (sFilter==NULL) sFilter=mc_strFilterTxtFile;

	// �����µ��ַ����ռ䣬�Ա㱣���µ� Filter �ַ���
	TCHAR * pft = NULL;
	if (bAddAllFile)
		pft = new TCHAR [_tcslen(sFilter) + 1 + _tcslen(mc_strFilterAllFile)];	// �����ļ��� Filter ���ֵĳ���һ������
	else
		pft = new TCHAR [_tcslen(sFilter) + 1];
	HM.AddPtr(pft);		// �� HM ���� pft ���ַ����ռ�

	// ���µ� Filter �ַ��������¿ռ�
	_tcscpy(pft, sFilter);
	if (*(pft+_tcslen(pft)-1)=='|') *(pft + _tcslen(pft)-1)='\0';	// ȥ������ | ������ӡ������ļ���֮ǰ��
	if (bAddAllFile) _tcscat(pft, mc_strFilterAllFile);
	// ��ʱ�����ټ���ȥ������ | �����������ˡ������ļ�����mc_strFilterAllFile �����û�� |��
	//   ��û��������ļ����ո��Ѿ�����ȥ�������� | 

	// �� mpStrFilter ָ���¿ռ�
	mpStrFilter=pft;	// ��� mpStrFilter ԭ����ָ���Ǹ�ָ������ǰ�ѱ����� HM ���������ع���Ҫ�ͷ��Ǹ�ָ��Ŀռ�
	if (*mpStrFilter == '|') mpStrFilter++;		// �����ͷ��һ�� |��ȥ����ͷ����� |

	// �ǼǶԻ���
	if (sAsRegKey)
	{
		if (mRegedDlgKeys.IsKeyExist(sAsRegKey))
		{
			// �öԻ����ѵǼǣ���ɾ����ǰ�ĵǼǣ��Ա��������ݸ���
			mRegedDlgKeys.Remove(sAsRegKey, false);
		}
		// �Ǽ�
		mRegedDlgKeys.Add(0, sAsRegKey, 1, 0, TEXT(""), mpStrFilter);
	}
}


// �������һ�����õĶԻ�����ļ����� Filter ("|"�ָ�)
// ���ص���һ���ַ�ָ�룬ָ�������ַ������ַ����ڴ�ռ��ɱ�ģ���Զ������������򲻱ظ�Ԥ
// �����������޸�������ָ������ݣ���ģ��� Filter ����Ҳ���ı�
LPTSTR OsdLastFilter()
{
	return mpStrFilter;
}


// �򿪡����ļ����Ի���
// bAllowMulSel=false ʱ�����������ļ������ַ����ռ��ɱ�ģ�����������򱻰�ȡ������ ""����NULL����
// bAllowMulSel=true ʱ�����������ļ�·�����ַ����ռ��ɱ�ģ������������� mMulSelFiles
// bRetReadOnly ָ��һ�� bool �ͱ�����ָ�룬*b=true ʱ��ʾ����ֻ����ʽ�򿪡���ѡ�򣬲��ɴ˲������� 
//   �û��Ƿ�ѡ�ˡ���ֻ����ʽ�򿪡���ѡ��*b=false �� =NULL ����ʾ����ֻ����ʽ�򿪡���ѡ��
// ������ sAsRegedKey ��ʾ�ѵǼ�Ϊ sAsRegedKey �� Key �ĶԻ��򣬷�����ʾһ�����µġ��Ի���
LPTSTR OsdOpenDlg( HWND hWnd/*=0*/, LPCTSTR sDlgTitle/*=NULL*/, LPCTSTR sAsRegedKey/*=NULL*/, bool bAllowMulSel/*=false*/, bool *bRetReadOnly/*=false*/ )
{

	OPENFILENAME ofn;
	TCHAR *pstr;
	int i;

	// ��� ofn
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.hInstance = 0;
	ofn.lpstrFileTitle = NULL;  // ����Ҫ�����ļ�����
	ofn.nMaxFileTitle= 0;
	ofn.lpstrDefExt = NULL;		// ����Ҫ�Զ������չ��

	// ���ٻ������������û�ѡ����ļ���
	if (bAllowMulSel)
	{
		// ���ö�ѡ�ļ�ʱ��ʹ�� mcMaxFileLenMul ��С�Ļ�����
		ofn.lpstrFile = new TCHAR[mcMaxFileLenMul];
		memset(ofn.lpstrFile, 0, mcMaxFileLenMul);
		HM.AddPtr(ofn.lpstrFile);	// ��ָ�뱣�浽 HM ����� new ����Ŀռ�
		ofn.nMaxFile = mcMaxFileLenMul;
	}
	else
	{
		// ֻ����ѡ�񵥸��ļ�ʱ��ʹ�� mcMaxFileLenSingle ��С�Ļ�����
		ofn.lpstrFile = new TCHAR[mcMaxFileLenSingle];
		memset(ofn.lpstrFile, 0, mcMaxFileLenSingle);
		HM.AddPtr(ofn.lpstrFile);	// ��ָ�뱣�浽 HM ����� new ����Ŀռ�
		ofn.nMaxFile = mcMaxFileLenSingle;		
	}


	// ===== ���ʹ���ѵǼǵĶԻ��򣬸����ѵǼǵ������ٸ������� =====
	if (sAsRegedKey && mRegedDlgKeys.IsKeyExist(sAsRegedKey))	// ��ָ����ʹ�õǼǶԻ����ֱ������ָ����key��Ч
	{
		TCHAR *ptemp;
		long lngTemp;
		// ���� Filter
		ptemp = mRegedDlgKeys.ItemStr2(sAsRegedKey,false);
		if (ptemp) 
		{
			mpStrFilter = new TCHAR[_tcslen(ptemp)+1];  // ��� mpStrFilter ԭ����ָ���Ǹ�ָ������ǰ�ѱ����� HM ���������ع���Ҫ�ͷ��Ǹ�ָ��Ŀռ�
			HM.AddPtr(mpStrFilter);  					// �� HM ���� Filter �ַ����ռ�
			_tcscpy(mpStrFilter, ptemp);				// ��������ٽ� mpStrFilter ��Ϊ '\0' �ָ������� ofn
		}
		// �����ϴ�ѡ����ļ�Ŀ¼Ϊ����Ĭ��Ŀ¼
		ptemp = mRegedDlgKeys.ItemStr(sAsRegedKey,false);
		if (ptemp) _tcscpy(mFilePath, ptemp);	// ��������ٽ� mFilePath ���� ofn
		// �����ϴ�ѡ��� Filter ��Ϊ����Ĭ��ѡ����
		lngTemp=mRegedDlgKeys.ItemLong(sAsRegedKey,false);
		if (lngTemp>0) ofn.nFilterIndex = (DWORD)lngTemp;
	}

    // ===== ����Ĭ��ȡֵ =====
	if (mpStrFilter)
	{
		// ���� ofn.lpstrFilter������ mpStrFilter �����ݹ���
		pstr = new TCHAR [_tcslen(mpStrFilter)+2];	// +2 ��Ϊ���Ҫ������������'\0'
		HM.AddPtr(pstr);							// ��ָ�뱣�浽 HM ����� new ����Ŀռ�
		_tcscpy(pstr, mpStrFilter);
		// ����ַ��ֵ�� ofn.lpstrFilter
		ofn.lpstrFilter = pstr;
		// ���ԡ�|���ָ��� ofn.lpstrFilter ת��Ϊ�� '\0' �ָ�
		while (*pstr)
		{
			if (*pstr=='|') *pstr='\0';
			pstr++;
		}
		*(pstr+1)='\0';			// ����������������� '\0'
	}
	else
	{
		ofn.lpstrFilter = NULL;
	}
    // ���� lpstrInitialDir
	ofn.lpstrInitialDir = mFilePath;
            

    // ===== ���öԻ������ =====
	ofn.lpstrTitle = sDlgTitle;
    

    // ===== ���� flags =====
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	if (bRetReadOnly==NULL)
		ofn.Flags = ofn.Flags | OFN_HIDEREADONLY;
	else
		if (! *bRetReadOnly) ofn.Flags = ofn.Flags | OFN_HIDEREADONLY;
    if (bAllowMulSel) ofn.Flags = ofn.Flags | OFN_ALLOWMULTISELECT;

    
    // ��ʾ���ļ��Ի���
    i = GetOpenFileName(&ofn);
    if (i==0)
	{
		// ���� GetOpenFileName ���� False ��ԭ��
		i=CommDlgExtendedError();
		if (i==0)
		{
			// �û���ȡ�������ؿ��ַ���
			*ofn.lpstrFile='\0'; if (bRetReadOnly) *bRetReadOnly=false;
			return ofn.lpstrFile; 
		}
		else
		{
			// �������ؿ��ַ���
			*ofn.lpstrFile='\0'; if (bRetReadOnly) *bRetReadOnly=false;
			return ofn.lpstrFile; 
		}
	}
    
    // ================= �������� =================
    // ���ء���ֻ����ʽ�򿪡���ѡ���Ƿ�ѡ��
    if (bRetReadOnly) *bRetReadOnly = (ofn.Flags & OFN_READONLY);

	// ����ģ�鼶����
	_tcscpy(mFilePath, ofn.lpstrFile);
	mLastSelFilterIdx = ofn.nFilterIndex;

    // �����ص��ļ���
	if (bAllowMulSel)
	{
		// ����ѡ���ļ������ӵ� mMulSelFiles
		mMulSelFiles.Clear();
		i=_tcslen(ofn.lpstrFile)+1;	// i ָ����һ�ֶε���ʼλ�ã�������һ�ֶΣ����һ�ֶ�Ϊ·����

		while (*(ofn.lpstrFile+i))
		{
			mMulSelFiles.Add(ofn.lpstrFile+i);	// mMulSelFiles �Ὺ���ڴ�ռ俽���ַ���
			i+=_tcslen(ofn.lpstrFile+i)+1;		// i ָ����һ�ֶε���ʼλ��
		}

		// ��ѡ״̬�û�ֻѡ��һ���ļ�
		if (mMulSelFiles.Count()==0)
		{
			// �ֽ��·�����ֺ��ļ������֣��԰���·���ɺ������ء��ļ����� mMulSelFiles ����
			TCHAR *pSlash=_tcsrchr(ofn.lpstrFile, '\\');		// �������һ�� '\\'
			if (pSlash)
			{
				mMulSelFiles.Add(pSlash+1);	// mMulSelFiles �Ὺ���ڴ�ռ俽���ַ���
				*(pSlash+1)='\0';				// �ض� ofn.lpstrFile��ʹ���Խ�Ϊ·������
			}
			else
			{
				// δ�ҵ����� '\\'���˶γ����Ϊ�ݴ�
				mMulSelFiles.Add(ofn.lpstrFile);
			}
		}		// if (mMulSelFiles.Count()==0)

		// ���ѡ�ļ������ص�·�����Զ�ȷ���� '\'
		if (*(ofn.lpstrFile + _tcslen(ofn.lpstrFile) - 1) != '\\')
		{
			*(ofn.lpstrFile + _tcslen(ofn.lpstrFile) + 1) = '\0';
			*(ofn.lpstrFile + _tcslen(ofn.lpstrFile)) = '\\';
		}
	}  // if (bAllowMulSel)


	// ===== ����óɹ��������ѵǼǵ���Ϣ =====
	if (sAsRegedKey && mRegedDlgKeys.IsKeyExist(sAsRegedKey))	// ��ָ����ʹ�õǼǶԻ����ֱ������ָ����key��Ч
	{
		// ��� ofn.lpstrFile ��·�����֣��Ǽ���Ϣֻ����·������ => pPath
		TCHAR *pPath=new TCHAR[_tcslen(ofn.lpstrFile) + 1];	// �ռ俪�ٵ��㹻�࣬pPath �ռ��Ժ� delete
		TCHAR *pSlh;
		_tcscpy(pPath, ofn.lpstrFile);
		pSlh=_tcsrchr(pPath,'\\');	// �������һ�� '\\'����ѡʱ·��ǰ����ȷ�����Ϊ '\\'��
		if (pSlh) *(pSlh+1)='\0';	// ���� '\\' ����һ��λ�ô���ֹ�ַ���
		mRegedDlgKeys.Remove(sAsRegedKey, false);
		mRegedDlgKeys.Add(0, sAsRegedKey, ofn.nFilterIndex, 0, pPath, mpStrFilter);	// �� pPath��mpStrFilter �����������ַ�������ϣ���У�֮���ϣ����ʹ�� pPath��mpStrFilter ָ����������ָ������ݿɱ���ȫɾ��
		delete []pPath;
	}

	// �����ļ���
	return ofn.lpstrFile;
}



// �򿪡������ļ����Ի���
// sDefaultFileName ��ʾ�Զ���䵽�Ի����Ĭ���ļ���
// ������ sAsRegedKey ��ʾ�ѵǼ�Ϊ sAsRegedKey �� Key �ĶԻ��򣬷�����ʾһ�����µġ��Ի���
LPTSTR OsdSaveDlg( HWND hWnd/*=0*/, LPCTSTR sDefaultFileName/*=NULL*/, LPCTSTR sDlgTitle/*=NULL*/, LPCTSTR sAsRegedKey/*=NULL*/ )
{
	OPENFILENAME ofn;
	TCHAR *pstr;
	int i;

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = 0;

	ofn.lpstrFile = new TCHAR[mcMaxFileLenSingle];
	memset(ofn.lpstrFile, 0, mcMaxFileLenSingle);
	HM.AddPtr(ofn.lpstrFile);
	if (sDefaultFileName) _tcscpy(ofn.lpstrFile, sDefaultFileName);
	ofn.nMaxFile = mcMaxFileLenSingle;

    ofn.lpstrFileTitle = NULL;	// ����Ҫ�����ļ�����
    ofn.nMaxFileTitle = 0;
    ofn.lpstrDefExt = TEXT("");		// �Զ������չ�������Ϊ NULL�����Զ���ӡ��ļ����Ϳ��е�ѡ��


	// ===== ���ʹ���ѵǼǵĶԻ��򣬸����ѵǼǵ������ٸ������� =====
	if (sAsRegedKey && mRegedDlgKeys.IsKeyExist(sAsRegedKey))	// ��ָ����ʹ�õǼǶԻ����ֱ������ָ����key��Ч
	{
		TCHAR *ptemp;
		long lngTemp;
		// ���� Filter
		ptemp = mRegedDlgKeys.ItemStr2(sAsRegedKey,false);
		if (ptemp) 
		{
			mpStrFilter = new TCHAR[_tcslen(ptemp)+1];  // ��� mpStrFilter ԭ����ָ���Ǹ�ָ������ǰ�ѱ����� HM ���������ع���Ҫ�ͷ��Ǹ�ָ��Ŀռ�
			HM.AddPtr(mpStrFilter);						// �� HM ���� Filter �ַ����ռ�
			_tcscpy(mpStrFilter, ptemp);	// ��������ٽ� mpStrFilter ��Ϊ '\0' �ָ������� ofn
		}
		// �����ϴ�ѡ����ļ�Ŀ¼Ϊ����Ĭ��Ŀ¼
		ptemp = mRegedDlgKeys.ItemStr(sAsRegedKey,false);
		if (ptemp) _tcscpy(mFilePath, ptemp);	// ��������ٽ� mFilePath ���� ofn
		// �����ϴ�ѡ��� Filter ��Ϊ����Ĭ��ѡ����
		lngTemp=mRegedDlgKeys.ItemLong(sAsRegedKey,false);
		if (lngTemp>0) ofn.nFilterIndex = (DWORD)lngTemp;
	}


	
    // ===== ����Ĭ��ȡֵ =====
	if (mpStrFilter)
	{
		// ���� ofn.lpstrFilter������ mpStrFilter �����ݹ���
		pstr = new TCHAR [_tcslen(mpStrFilter)+2];	// +2 ��Ϊ���Ҫ������������'\0'
		HM.AddPtr(pstr);							// ��ָ�뱣�浽 HM ����� new ����Ŀռ�
		_tcscpy(pstr, mpStrFilter);
		// ����ַ��ֵ�� ofn.lpstrFilter
		ofn.lpstrFilter = pstr;
		// ���ԡ�|���ָ��� ofn.lpstrFilter ת��Ϊ�� '\0' �ָ�
		while (*pstr)
		{
			if (*pstr=='|') *pstr='\0';
			pstr++;
		}
		*(pstr+1)='\0';			// ����������������� '\0'
	}
	else
	{
		ofn.lpstrFilter = NULL;
	}

    // ���� lpstrInitialDir
	ofn.lpstrInitialDir = mFilePath;

   
    // ===== ���öԻ������ =====
	ofn.lpstrTitle = sDlgTitle;
    
    // ===== ���� flags =====
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	
	// ��ʾ�����ļ��Ի���
    i = GetSaveFileName(&ofn);
    if (i==0)
	{
		// ���� GetSaveFileName ���� False ��ԭ��
		i=CommDlgExtendedError();
		if (i==0)
		{
			// �û���ȡ�������ؿ��ַ���
			*ofn.lpstrFile='\0'; 
			return ofn.lpstrFile; 
		}
		else
		{
			// �������ؿ��ַ���
			*ofn.lpstrFile='\0'; 
			return ofn.lpstrFile; 
		}
	}

	// ����ģ�鼶����
	_tcscpy(mFilePath, ofn.lpstrFile);
	mLastSelFilterIdx = ofn.nFilterIndex;

	// ===== ����óɹ��������ѵǼǵ���Ϣ =====
	if (sAsRegedKey && mRegedDlgKeys.IsKeyExist(sAsRegedKey))	// ��ָ����ʹ�õǼǶԻ����ֱ������ָ����key��Ч
	{
		// ��� ofn.lpstrFile ��·�����֣��Ǽ���Ϣֻ����·������ => pPath
		TCHAR *pPath=new TCHAR[_tcslen(ofn.lpstrFile) + 1];	// �ռ俪�ٵ��㹻�࣬pPath �ռ��Ժ� delete
		TCHAR *pSlh;
		_tcscpy(pPath, ofn.lpstrFile);
		pSlh=_tcsrchr(pPath,'\\');	// �������һ�� '\\'����ѡʱ·��ǰ����ȷ�����Ϊ '\\'��
		if (pSlh) *(pSlh+1)='\0';	// ���� '\\' ����һ��λ�ô���ֹ�ַ���
		mRegedDlgKeys.Remove(sAsRegedKey, false);
		mRegedDlgKeys.Add(0, sAsRegedKey, ofn.nFilterIndex, 0, pPath, mpStrFilter);	// �� pPath��mpStrFilter �����������ַ�������ϣ���У�֮���ϣ����ʹ�� pPath��mpStrFilter ָ����������ָ������ݿɱ���ȫɾ��
		delete []pPath;
	}

	// ���ضԻ������û�������ļ�����·��
	return ofn.lpstrFile;
}



// ������һ�δ򿪶Ի���ѡ��Ķ�ѡ�ļ������ɱ�����󴴽���ָ̬������ pptrMulFiles
//   �������±�� 1 ��ʼ���ռ� [0] ����
//   ָ��������׵�ַ�ɲ������ء�����������һ�ж�̬�ռ�
//   ��ѡ�ļ��ĸ�����Ҳ�Ƕ�̬����ĸ������ɺ������أ�ÿ������Ԫ�طֱ�ָ��һ���ַ���
//   �ռ��ʾÿ��ѡ����ļ���ֻ���ļ������֣���·����
// ���磺
//	TCHAR **mul;
//	int fileCount, i;
//  fileCount=OSDMulSelFiles(mul);
//  for(i=1; i<=fileCount; i++)
//		cout<<mul[i]<<endl;
int OSDMulSelFiles( TCHAR ** &pptrMulFiles )
{
	int i;

	// �޶�ѡ�ļ������� 0
	if (mMulSelFiles.Count()==0)
	{
		pptrMulFiles=0;
		return 0;
	}

	// ����ָ������
	pptrMulFiles=new TCHAR *[mMulSelFiles.Count()+1];	// �������±�� 1 ��ʼ
	pptrMulFiles[0] = NULL;								// �±�Ϊ [0] �Ŀռ䲻�ã���ֵΪ NULL �԰�ȫ
	HM.AddPtr(pptrMulFiles);

	// Ϊ����ѡ����ļ��ַ������ٴ洢�ռ䣬�����ռ��ַ���� pptrMulFiles[] �ĸ���Ԫ��
	for (i=1; i<=mMulSelFiles.Count(); i++)
	{
		pptrMulFiles[i] = new TCHAR [_tcslen(mMulSelFiles.Item(i))+1];
		HM.AddPtr(pptrMulFiles[i]);
		_tcscpy(pptrMulFiles[i], mMulSelFiles.Item(i));
	}

	// �����ļ�����
	return mMulSelFiles.Count();
}


// �������һ��ѡ����ļ���(��ѡ�ļ�ʱΪ·��)
// ����ʱ���������½�һ���ַ����ռ䣬�����¿ռ��ַ���ÿռ��ɱ����ͷ�
LPTSTR OSDFilePath()
{
	TCHAR *pc=new TCHAR [_tcslen(mFilePath)+1];
	HM.AddPtr(pc);
	_tcscpy(pc, mFilePath);
	return pc;
}

// ���� mFilePath ��ΪĬ���ļ���
void OSDSetInitFilePath( LPCTSTR newInitFilePath )
{
	_tcscpy(mFilePath, newInitFilePath);
}


// ���� ���һ�ζԻ�����û���"�ļ�����"�����б���ѡ��� filter �� ������ mLastSelFilterIdx
int OSDLastSelFilterIdx()
{
	return mLastSelFilterIdx;
}

