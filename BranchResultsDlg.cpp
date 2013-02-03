#include "stdafx.h"
#include "resource.h"

#include "BranchResultsDlg.h"
#include "VerificationDlg.h"

LRESULT CBranchResultsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	DoDataExchange(false);
	for(int i=1;i<=BranchContainer->GetBranchSums();i++) ComboBox.AddString(BranchContainer->GetItem(i,1));
	currentBranch=1;
	SetWindowText("\""+BranchContainer->GetKeyword()+"\""+"��ѯ���");
	for(int i=1;i<=12;i++) 	edits[i]=BranchContainer->GetItem(currentBranch,i);
	ComboBox.SetCurSel(0);
	DoDataExchange(false);
	return TRUE;
}

LRESULT CBranchResultsDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CBranchResultsDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog m_wndSaveAll(false,NULL,"�����ѯ���.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�ı��ĵ�(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
	if( IDOK==m_wndSaveAll.DoModal())
	{
			HANDLE hFile=::CreateFile(m_wndSaveAll.m_ofn.lpstrFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			DWORD dwWrites;
			CString FileContent="\r\nԲͨ����ؼ���:"+BranchContainer->GetKeyword()+"\r\n";
			for(int i=1;i<=(BranchContainer->GetBranchSums());i++)
			{
				FileContent=FileContent+"\r\n���ڵ���\t"+BranchContainer->GetItem(i,1);
				FileContent=FileContent+"\r\n��������\t"+BranchContainer->GetItem(i,2);
				FileContent=FileContent+"\r\n�������\t"+BranchContainer->GetItem(i,3);
				FileContent=FileContent+"\r\n��������\t"+BranchContainer->GetItem(i,4);
				FileContent=FileContent+"\r\nMSN �˺�\t"+BranchContainer->GetItem(i,5);
				FileContent=FileContent+"\r\n��ѯ�绰\t"+BranchContainer->GetItem(i,6);
				FileContent=FileContent+"\r\n���ͷ�Χ\t"+BranchContainer->GetItem(i,7);
				FileContent=FileContent+"\r\n�����ͷ�Χ\t"+BranchContainer->GetItem(i,8);
				FileContent=FileContent+"\r\n����ʱ��\t"+BranchContainer->GetItem(i,9);
				FileContent=FileContent+"\r\n24Сʱ��\t"+BranchContainer->GetItem(i,10);
				FileContent=FileContent+"\r\n�������\t"+BranchContainer->GetItem(i,11);
				FileContent=FileContent+"\r\n������ע\t"+BranchContainer->GetItem(i,11)+"\r\n";
			}
			::WriteFile(hFile,FileContent,FileContent.GetLength(),&dwWrites,NULL);
			CloseHandle(hFile);
	}
	return 0;
}

LRESULT CBranchResultsDlg::OnComboChange(UINT uCode, int nCtrlID, HWND hwndCtrl)
{
	currentBranch=ComboBox.GetCurSel()+1;
	for(int i=1;i<=12;i++) 	edits[i]=BranchContainer->GetItem(currentBranch,i);
	DoDataExchange(false);
	return 0;
}