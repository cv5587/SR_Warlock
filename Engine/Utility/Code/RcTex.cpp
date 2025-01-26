#include "..\..\Header\RcTex.h"

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex & rhs)
	:CVIBuffer(rhs)
{
}


CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer(float width, float height)
{

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32; 
	
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { - width / 2, height / 2, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { width / 2, height / 2, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { width / 2, - height / 2, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };
		
	pVertex[3].vPosition = { -width / 2, -height / 2, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0; 
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex *		pInstance = DBG_NEW CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(2.0f,2.0f)))
	{
		Safe_Release(pInstance);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, float width, float height)
{
	CRcTex* pInstance = DBG_NEW CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(width, height)))
	{
		Safe_Release(pInstance);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CRcTex::Clone()
{
	return DBG_NEW CRcTex(*this);
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
