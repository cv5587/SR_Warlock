#pragma once
#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CMesh : public CComponent
{
private:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);

public:

	CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	virtual HRESULT		Ready_Buffer(const FbxMeshInfo& vMesh);
	void		Render_Buffer();

public:
	static CMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const FbxMeshInfo& vMesh);
	virtual CComponent* Clone();
	
private:
	virtual void	Free();

public:
	_ulong GetNumFaces() { return m_dwTriCnt; }


private:
	HRESULT Set_SubSet(_uint faceCount);




private:
	LPD3DXMESH m_pMesh;
	_ulong		m_dwVtxCnt;
	_ulong		m_dwVtxSize;
	_ulong		m_dwTriCnt;
	_ulong		m_dwFVF;

	vector<_ulong>	m_vOptimizedAdjacencyInfo;
	
};

END

