#include "DynamicRcTex.h"

CDynamicRcTex::CDynamicRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CDynamicBuffer(pGraphicDev), m_bDirisRight(false)
{
}

CDynamicRcTex::CDynamicRcTex(const CDynamicRcTex& rhs)
	:CDynamicBuffer(rhs), m_Us{rhs.m_Us[0],rhs.m_Us[1]}, m_bDirisRight(rhs.m_bDirisRight)
{
}

CDynamicRcTex::~CDynamicRcTex()
{
}

HRESULT CDynamicRcTex::Ready_Buffer(bool DirisRight, float width, float height)
{
	m_bDirisRight = DirisRight;
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CDynamicBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -width / 2, height / 2, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { width / 2, height / 2, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { width / 2, -height / 2, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -width / 2, -height / 2, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	m_Us[0] = 0.f;
	m_Us[1] = 1.f;

	return S_OK;
}

_int CDynamicRcTex::Update_Component(const _float& fTimeDelta)
{

	if (m_bDirisRight)
	{
		if (m_Us[1] < m_Us[0])
		{
			m_Us[1] = 0.5f;
			m_Us[0] = 0.f;
		}
		else
		{
			m_Us[0] += fTimeDelta * 0.1;

			m_Us[1] += fTimeDelta * 0.1;
		}
	}
	else
	{

		m_Us[0] -= fTimeDelta * 0.1;

		m_Us[1] -= fTimeDelta * 0.1;

		if (m_Us[1] < m_Us[0])
		{
			m_Us[1] = 0.5f;
			m_Us[0] = 0.f;
		}

	}

		

	return 0;
}


void CDynamicRcTex::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwFVF);

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vTexUV.x = m_Us[0];
	pVertex[0].vTexUV.y = 0.f;

	pVertex[1].vTexUV.x = m_Us[1];
	pVertex[1].vTexUV.y = 0.f;

	pVertex[2].vTexUV.x = m_Us[1];
	pVertex[2].vTexUV.y = 1.f;

	pVertex[3].vTexUV.x = m_Us[0];
	pVertex[3].vTexUV.y = 1.f;

		

	m_pVB->Unlock();

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}


CDynamicRcTex* CDynamicRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, bool DirisRight, float width, float height)
{
	CDynamicRcTex* pInstance = DBG_NEW CDynamicRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(DirisRight, width, height)))
	{
		Safe_Release(pInstance);
		MSG_BOX("DynamicRcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CDynamicRcTex::Clone()
{
	return DBG_NEW CDynamicRcTex(*this);
}

void CDynamicRcTex::Free()
{
	CDynamicBuffer::Free();
}
