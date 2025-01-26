#include "Line.h"

CLine::CLine(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_pLine(nullptr)
{
}

CLine::CLine(const CLine& rhs)
	:CComponent(rhs), m_pLine(rhs.m_pLine), m_vVtxPos{rhs.m_vVtxPos[0],rhs.m_vVtxPos[1]}
{
	m_pLine->AddRef();
}

CLine::~CLine()
{
}

HRESULT CLine::Ready_Buffer(const pair<D3DXVECTOR3, D3DXVECTOR3>& line)
{
	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_pLine->SetWidth(20);
	m_pLine->SetAntialias(true);

	m_vVtxPos[0] = line.first;
	m_vVtxPos[1] = line.second;

	return S_OK;
}

HRESULT CLine::Render_Buffer()
{
	D3DXMATRIX matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX matTotal = matWorld * matView * matProj;


	if ((D3DXERR_INVALIDDATA || D3DERR_INVALIDCALL) == m_pLine->DrawTransform(m_vVtxPos, 2, &matTotal, D3DCOLOR_ARGB(255,0,0,0)))
	{
		return E_FAIL;
	}

	return S_OK;
}


CLine* CLine::Create(LPDIRECT3DDEVICE9 pGraphicDev, const pair<D3DXVECTOR3, D3DXVECTOR3>& line)
{
	CLine* pInstance = DBG_NEW CLine(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(line)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Line Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CLine::Clone()
{
	return DBG_NEW CLine(*this);
}

void CLine::Free()
{
	if (m_pLine)
		Safe_Release(m_pLine);
	CComponent::Free();
}
