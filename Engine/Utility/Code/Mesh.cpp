#include "Mesh.h"

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_pMesh(nullptr), m_dwFVF(FVF_TEX), m_dwTriCnt(0), m_dwVtxCnt(0), m_dwVtxSize(0)
{
}

CMesh::CMesh(const CMesh& rhs)
	:CComponent(rhs), m_pMesh(rhs.m_pMesh), m_dwFVF(rhs.m_dwFVF), m_dwTriCnt(rhs.m_dwTriCnt), m_dwVtxCnt(rhs.m_dwVtxCnt),m_dwVtxSize(rhs.m_dwVtxSize)
{
	m_pMesh->AddRef();
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Ready_Buffer(const FbxMeshInfo& vMesh)
{

	m_dwFVF = FVF_TEX;
	m_dwTriCnt = vMesh.indices.size();
	m_dwVtxCnt = vMesh.vertices.size();
	m_dwVtxSize = sizeof(VTXTEX);

	if (FAILED(D3DXCreateMeshFVF(m_dwTriCnt, m_dwVtxCnt, D3DXMESH_MANAGED | D3DXMESH_32BIT, FVF_TEX, m_pGraphicDev, &m_pMesh)))
	{
		Safe_Release(m_pMesh);
		MSG_BOX("Mesh Create Failed");
		return E_FAIL;
	}
	VTXTEX* Vertices = nullptr;

	if (m_pMesh->LockVertexBuffer(0, (void**)&Vertices) == D3DERR_INVALIDCALL)
	{
		Safe_Release(m_pMesh);
		MSG_BOX("Vertex Buffer Lock Failed");
		return E_FAIL;
	}
	for (int i = 0; i < vMesh.vertices.size(); i++)
	{
		Vertices[i].vPosition = vMesh.vertices[i].vPosition;
		Vertices[i].vTexUV = vMesh.vertices[i].vTexUV;
		Vertices[i].vNormal = vMesh.vertices[i].vNormal;
	}

	if (FAILED(m_pMesh->UnlockVertexBuffer()))
	{
		Safe_Release(m_pMesh);
		MSG_BOX("Vertex Buffer Unlock Failed");
		return E_FAIL;
	}

	INDEX32* Indices = nullptr;

	if (FAILED(m_pMesh->LockIndexBuffer(0, (void**)&Indices)))
	{
		Safe_Release(m_pMesh);
		MSG_BOX("Index Buffer Lock Failed");
		return E_FAIL;
	}

	for (int i = 0; i < vMesh.indices.size(); i++)
	{
		Indices[i]._0 = vMesh.indices[i]._0;
		Indices[i]._1 = vMesh.indices[i]._1;
		Indices[i]._2 = vMesh.indices[i]._2;
	}


	if (FAILED(m_pMesh->UnlockIndexBuffer()))
	{
		Safe_Release(m_pMesh);
		MSG_BOX("Index Buffer Unlock Failed");
		return E_FAIL;
	}

	//if (FAILED(Set_SubSet(faceCount)))
	//{
	//	Safe_Release(m_pMesh);
	//	MSG_BOX("SubSet Divide Failed");
	//	return E_FAIL;
	//}

	return S_OK;
}

void CMesh::Render_Buffer()
{	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pMesh->DrawSubset(0);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



CMesh* CMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const FbxMeshInfo& vMesh)
{
	CMesh* pInstance = DBG_NEW CMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(vMesh)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Mesh Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CMesh::Clone()
{
	return DBG_NEW CMesh(*this);
}


//
//HRESULT CMesh::Set_SubSet(_uint faceCount)
//{
//	if (m_dwTriCnt % faceCount != 0)
//		return E_FAIL;
//
//
//	HRESULT hr;
//	DWORD* attribBuf;
//
//	if (SUCCEEDED(hr = m_pMesh->LockAttributeBuffer(D3DLOCK_DISCARD, &attribBuf)))
//	{
//		_ulong subsetNum = 0;
//		for (int i = 0; i < m_dwTriCnt / faceCount; ++i)
//		{
//			attribBuf[i] = subsetNum;
//			if (i % faceCount == 0)
//				subsetNum++;
//		}
//
//		m_pMesh->UnlockAttributeBuffer();
//	}
//	else
//		return E_FAIL;
//
//	return S_OK;
//}

void CMesh::Free()
{
	Safe_Release(m_pMesh);
	//Safe_Release(m_pVB);
	//Safe_Release(m_pIB);
	CComponent::Free();
}
