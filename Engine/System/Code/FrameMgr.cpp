#include "..\..\Header\FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)


CFrameMgr::CFrameMgr()
{
}


CFrameMgr::~CFrameMgr()
{
	Free();
}

_bool CFrameMgr::IsPermit_Call(wstring pFrameTag, const _float & fTimeDelta)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pFrame, false);

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT CFrameMgr::Ready_Frame(wstring pFrameTag, const _float & fCallLimit)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.insert({ pFrameTag, pFrame });

	return S_OK;
}

CFrame * CFrameMgr::Find_Frame(wstring pFrameTag)
{
	auto	iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_Finder2(pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}
