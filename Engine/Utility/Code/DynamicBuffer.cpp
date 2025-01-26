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
	(m_dwVtxCnt * m_dwVtxSize,	// ������ ���ؽ� ������ ũ��
		D3DUSAGE_DYNAMIC,			// ���ؽ� ������ ����(0�� ��� ���� ����, d3dusage_dynamic�� ��� ���� ����)
		m_dwFVF,	// ���ؽ� �Ӽ� �ɼ�
		D3DPOOL_DEFAULT, // ���� ������ managed
		&m_pVB,	// ������ ���ؽ� ���� �� ��ü
		NULL),	// ���� ����
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer
	(m_dwTriCnt * m_dwIdxSize,	// ������ �ε��� ������ ũ��
		0,			// �ε��� ������ ����(0�� ��� ���� ����, d3dusage_dynamic�� ��� ���� ����)
		m_IdxFmt,	// �ε��� �Ӽ� �ɼ�
		D3DPOOL_MANAGED, // ���� ������ managed
		&m_pIB,	// ������ �ε��� ���� �� ��ü
		NULL),	// ���� ����
		E_FAIL);

	return S_OK;
}


void CDynamicBuffer::Render_Buffer()
{
}
