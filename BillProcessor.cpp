#include "StdAfx.h"
#include "BillProcessor.h"

CBillProcessor::CBillProcessor(CString place)
{
	saveplace=place;
}

CBillProcessor::~CBillProcessor(void)
{
}

bool CBillProcessor::Process()
{	
	//����xml�ļ�
	MSXML2::IXMLDOMDocumentPtr m_plDomDocument;     
	MSXML2::IXMLDOMElementPtr m_pDocRoot;
	HRESULT hr=m_plDomDocument.CreateInstance(__uuidof(MSXML2::DOMDocument30));
	if(FAILED(hr))
	{
		return false;
	}
	variant_t vResult=m_plDomDocument->load(CString(saveplace).AllocSysString());
	
	if (((bool)vResult) == TRUE)
	{
		m_pDocRoot = m_plDomDocument->documentElement;
		AddChildren(m_pDocRoot);
		BillContainer.Finish();
	}
	else
	{
		return false;
	}
	return true;
}

void CBillProcessor::AddChildren(MSXML2::IXMLDOMNodePtr pParent)
{
	//��ӵ�ǰ�ڵ�
	AddChild(pParent);

	//ѭ��������нڵ�
	for (MSXML2::IXMLDOMNodePtr pChild = pParent->firstChild;
		NULL != pChild;
		pChild = pChild->nextSibling)
	{
	//�ݹ�
	AddChildren(pChild);
	}
}
void CBillProcessor::AddChild(MSXML2::IXMLDOMNodePtr pChild)
{
	if (NODE_TEXT == pChild->nodeType)
	{
		BillContainer.Add((char *)pChild->text);
	}
	else
	{
		if(pChild->nodeName==(_bstr_t)"c") BillContainer.NextBill();
		if(pChild->nodeName==(_bstr_t)"d") BillContainer.NextItem();
		if(pChild->nodeName==(_bstr_t)"e") BillContainer.SetFoo(1);
		if(pChild->nodeName==(_bstr_t)"f") BillContainer.SetFoo(2);
	}
}
CBillContainer* CBillProcessor::getResult()
{
	return &BillContainer;
}