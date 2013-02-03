#include "stdafx.h"
#include "BranchContainer.h"

//���캯��
CBranchContainer::CBranchContainer()
{
	currentItem=1;
	BranchSums=0;
}

//����˵���¼
void CBranchContainer::Add(CString string)
{
		BranchItem[currentItem][BranchSums]=string;
		currentItem++;
}

//ȡ������������
CString CBranchContainer::GetItem(int cBranch,int cItem)
{
	return BranchItem[cItem][cBranch];
}

int CBranchContainer::GetBranchSums()
{
	return BranchSums;
}

void CBranchContainer::NextBranch()
{
	currentItem=1;
	BranchSums++;
}

CString CBranchContainer::GetKeyword()
{
	return Keyword;
}

void CBranchContainer::SetKeyword(CString key)
{
	Keyword=key;
}