#include "DynamicBuffer.h"


CDynamicBuffer::CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev),m_pVB(nullptr), m_pIB(nullptr), m_dwVtxCnt(0), m_dwTriCnt(0), m_dwVtxSize(0), m_dwFVF(0), m_dwIdxSize(0)
{
}

CDynamicBuffer::CDynamicBuffer(const CDynamicBuffer& rhs)
	: CComponent(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB), m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwTriCnt(rhs.m_dwTriCnt),
	m_dwVtxSize(rhs.m_dwVtxSize),m_dwFVF(rhs.m_dwFVF), m_dwIdxSize(rhs.m_dwIdxSize), m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CDynamicBuffer::~CDynamicBuffer()
{

}

void CDynamicBuffer::Free()
{
	CComponent::Free();
}

HRESULT CDynamicBuffer::Ready_Buffer()
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(m_dwVtxCnt * m_dwVtxSize,	// 생성할 버텍스 버퍼의 크기
		D3DUSAGE_DYNAMIC,			// 버텍스 버퍼의 종류(0인 경우 정적 버퍼, d3dusage_dynamic인 경우 동적 버퍼)
		m_dwFVF,	// 버텍스 속성 옵션
		D3DPOOL_DEFAULT, // 정적 버퍼인 managed
		&m_pVB,	// 생성할 버텍스 버퍼 컴 객체
		NULL),	// 공유 설정
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer
	(m_dwTriCnt * m_dwIdxSize,	// 생성할 인덱스 버퍼의 크기
		0,			// 인덱스 버퍼의 종류(0인 경우 정적 버퍼, d3dusage_dynamic인 경우 동적 버퍼)
		m_IdxFmt,	// 인덱스 속성 옵션
		D3DPOOL_MANAGED, // 정적 버퍼인 managed
		&m_pIB,	// 생성할 인덱스 버퍼 컴 객체
		NULL),	// 공유 설정
		E_FAIL);

	return S_OK;
}


void CDynamicBuffer::Render_Buffer()
{
}
