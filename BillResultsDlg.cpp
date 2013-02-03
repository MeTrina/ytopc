#include "stdafx.h"
#include "resource.h"

#include "BillResultsDlg.h"
#include "VerificationDlg.h"

LRESULT CBillResultsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	HICON hDlgIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_YTOALL), 
		IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	SendDlgItemMessage(IDC_BILLPIC,STM_SETICON,(WPARAM)hDlgIcon,0);
	DoDataExchange(false);
	list.InsertColumn(0,"ʱ��",LVCFMT_LEFT,108,0);
	list.InsertColumn(1,"�˵�����",LVCFMT_LEFT,300,1);
	for(int i=1;i<=BillContainer->GetBillCount();i++) Combo.AddString(BillContainer->GetBillNumber(i));
	Combo.SetCurSel(0);
	for(int i=1;i<=(BillContainer->GetItemCount(1));i++)
	{
		list.InsertItem(i-1,BillContainer->GetBill(1,i,1),NULL);
		list.SetItemText(i-1,1,BillContainer->GetBill(1,i,2));
	}
	return TRUE;
}
LRESULT CBillResultsDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
LRESULT CBillResultsDlg::OnComboChange(UINT uCode, int nCtrlID, HWND hwndCtrl)
{
	int currentBranch=Combo.GetCurSel()+1;
	list.DeleteAllItems();
	for(int i=1;i<=(BillContainer->GetItemCount(currentBranch));i++)
	{
		list.InsertItem(i-1,BillContainer->GetBill(currentBranch,i,1),NULL);
		list.SetItemText(i-1,1,BillContainer->GetBill(currentBranch,i,2));
	}
	return 0;
}
LRESULT CBillResultsDlg::OnCopy(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int currentBranch=Combo.GetCurSel()+1;
	CString FileContent;
	FileContent=FileContent+"Բͨ�˵���:"+BillContainer->GetBillNumber(currentBranch)+"\r\n\r\n";
	for(int i=1;i<=BillContainer->GetItemCount(currentBranch);i++)
	{
		FileContent=FileContent+BillContainer->GetBill(currentBranch,i,1)+"\t"+BillContainer->GetBill(currentBranch,i,2)+"\r\n";
	}
	if(OpenClipboard())//д��ǰ�������ʱд�룬��Ҫ�ȵ���
	{
		char *pBuf;
		EmptyClipboard();
		HANDLE hClip=GlobalAlloc(GMEM_MOVEABLE,FileContent.GetLength()+1);//�����ڴ�ռ�
		pBuf=(char*)GlobalLock(hClip);
		strcpy(pBuf,FileContent);
		GlobalUnlock(hClip);
		SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();
	}
	return 0;
}
LRESULT CBillResultsDlg::OnSaveText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog m_wndSaveAll(false,NULL,"�˵���ѯ���.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�ı��ĵ�(*.txt)\0*.txt\0All Files(*.*)\0*.*");
	if( IDOK==m_wndSaveAll.DoModal())
	{
			HANDLE hFile=::CreateFile(m_wndSaveAll.m_ofn.lpstrFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			DWORD dwWrites;
			CString FileContent;
			for(int i=1;i<=BillContainer->GetBillCount();i++)
			{
				FileContent=FileContent+"\r\nԲͨ�˵���:"+BillContainer->GetBillNumber(i)+"\r\n\r\n";
				for(int j=1;j<=BillContainer->GetItemCount(i);j++)
				{
					FileContent=FileContent+BillContainer->GetBill(i,j,1)+"\t"+BillContainer->GetBill(i,j,2)+"\r\n";
				}
				FileContent=FileContent+"\r\n";
			}
			::WriteFile(hFile,FileContent,FileContent.GetLength(),&dwWrites,NULL);
			CloseHandle(hFile);
	}
	return 0;
}
LRESULT CBillResultsDlg::OnSavePic(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetWindowPos(HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	CRect rect;
	GetClientRect(&rect);
	HBITMAP hMap =  CopyScreenToBitmap(rect);

	CFileDialog m_wndSavePic(false,NULL,"��ѯ���.bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"λͼ�ļ�(*.bmp)\0*.bmp\0\0");
	if( IDOK==m_wndSavePic.DoModal()) SaveBitmapToFile(hMap,m_wndSavePic.m_ofn.lpstrFile);
	return 0;
}
HBITMAP CBillResultsDlg::CopyScreenToBitmap(LPRECT lpRect) //lpRect ����ѡ������
{
	/* HDC hScrDC, hMemDC;
	// ��Ļ���ڴ��豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������*/
	CClientDC hScrDC(m_hWnd);
	HDC hMemDC;
	HBITMAP hBitmap,hOldBitmap;
	// λͼ���
	int nX,nY,nX2,nY2;
	// ѡ����������
	int nWidth, nHeight;
	// λͼ��Ⱥ͸߶�
	int xScrn,yScrn;
	// ��Ļ�ֱ���
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
	return NULL;
	//Ϊ��Ļ�����豸������

	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
	nX = 0;
	if (nY < 0)
	nY = 0;
	if (nX2 > xScrn)
	nX2 = xScrn;
	if (nY2 > yScrn)
	nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth-128,nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC,0,0, nWidth-128,nHeight,hScrDC, nX, nY, SRCCOPY);
	//�õ���Ļλͼ�ľ��
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	return hBitmap;
}
int CBillResultsDlg::SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���
{      //lpFileName Ϊλͼ�ļ���
 HDC     hDC;         
    //�豸������
 int     iBits;      
 //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
 WORD    wBitCount;   
    //λͼ��ÿ��������ռ�ֽ���
 //�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���
 DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
 BITMAP          Bitmap;        
 //λͼ���Խṹ
 BITMAPFILEHEADER   bmfHdr;        
 //λͼ�ļ�ͷ�ṹ
 BITMAPINFOHEADER   bi;            
 //λͼ��Ϣͷ�ṹ 
 LPBITMAPINFOHEADER lpbi;          
 //ָ��λͼ��Ϣͷ�ṹ
    HANDLE          fh, hDib, hPal;
 HPALETTE     hOldPal=NULL;
 //�����ļ��������ڴ�������ɫ����
 
 //����λͼ�ļ�ÿ��������ռ�ֽ���
 hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
 iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
  GetDeviceCaps(hDC, PLANES);
 DeleteDC(hDC);
 if (iBits <= 1)
  wBitCount = 1;
 else if (iBits <= 4)
  wBitCount = 4;
 else if (iBits <= 8)
  wBitCount = 8;
 else if (iBits <= 24)
  wBitCount = 24;
else
  wBitCount = 32;
 //�����ɫ���С
 if (wBitCount <= 8)
  dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);
 
 //����λͼ��Ϣͷ�ṹ
 GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
 bi.biSize            = sizeof(BITMAPINFOHEADER);
 bi.biWidth           = Bitmap.bmWidth;
 bi.biHeight          = Bitmap.bmHeight;
 bi.biPlanes          = 1;
 bi.biBitCount         = wBitCount;
 bi.biCompression      = BI_RGB;
 bi.biSizeImage        = 0;
 bi.biXPelsPerMeter     = 0;
 bi.biYPelsPerMeter     = 0;
 bi.biClrUsed         = 0;
 bi.biClrImportant      = 0;
 
 dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;
 //Ϊλͼ���ݷ����ڴ�

/*xxxxxxxx����λͼ��С�ֽ�һ��(����һ����������)xxxxxxxxxxxxxxxxxxxx 
//ÿ��ɨ������ռ���ֽ���Ӧ��Ϊ4���������������㷨Ϊ:
 int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
 if((Bitmap.bmWidth*wBitCount) % 32)
  biWidth++; //�����������ļ�1
 biWidth *= 4;//���������õ���Ϊÿ��ɨ���е��ֽ�����
 dwBmBitsSize = biWidth * Bitmap.bmHeight;//�õ���С
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


 hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
 lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
 *lpbi = bi;
 // �����ɫ��   
 hPal = GetStockObject(DEFAULT_PALETTE);
 if (hPal)
 {
  hDC = ::GetDC(NULL);
  hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
  RealizePalette(hDC);
 }
 // ��ȡ�õ�ɫ�����µ�����ֵ
 GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
 //�ָ���ɫ��   
 if (hOldPal)
 {
  SelectPalette(hDC, hOldPal, TRUE);
  RealizePalette(hDC);
  ::ReleaseDC(NULL, hDC);
 }
 //����λͼ�ļ�    
 fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
 if (fh==INVALID_HANDLE_VALUE)
  return FALSE;
 // ����λͼ�ļ�ͷ
 bmfHdr.bfType = 0x4D42;  // "BM"
 dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;  
 bmfHdr.bfSize = dwDIBSize;
 bmfHdr.bfReserved1 = 0;
 bmfHdr.bfReserved2 = 0;
 bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
 // д��λͼ�ļ�ͷ
 WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
 // д��λͼ�ļ���������
  WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
 //���   
 GlobalUnlock(hDib);
 GlobalFree(hDib);
 CloseHandle(fh);
 return TRUE;
}