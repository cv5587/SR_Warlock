#include "..\..\Header\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true)
{
	m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
	_ulong	dwRefCnt(0);
	dwRefCnt = Safe_Release(m_pGraphicDev);
}
