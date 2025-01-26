#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CLine :
    public CComponent
{
private:
	explicit CLine(LPDIRECT3DDEVICE9 pGraphicDev);

public:

	CLine(const CLine& rhs);
	virtual ~CLine();

public:
	virtual HRESULT		Ready_Buffer(const pair<D3DXVECTOR3, D3DXVECTOR3>& line);
	HRESULT		Render_Buffer();

public:
	static CLine* Create(LPDIRECT3DDEVICE9 pGraphicDev, const pair<D3DXVECTOR3,D3DXVECTOR3>& line);
	virtual CComponent* Clone();

	D3DXVECTOR3	Get_Centre_Pos() { return 0.5f * (m_vVtxPos[0] + m_vVtxPos[1]); };
	D3DXVECTOR3* Get_ArrVtxPos() { return m_vVtxPos; }

	void Set_Bounds(pair<D3DXVECTOR3,D3DXVECTOR3> bounds) 
	{
		m_lowerBound = bounds.first;
		m_upperBound = bounds.second;
	}

	pair<D3DXVECTOR3, D3DXVECTOR3> Get_Bounds() { return pair<D3DXVECTOR3, D3DXVECTOR3>(m_lowerBound, m_upperBound); }
	bool Is_Z_Wall() { return m_bis_zWall; }
	void Set_Is_Z_Wall(bool isZWall) { m_bis_zWall = isZWall; }

	void Set_Particle_Idx(unsigned int uParticleIdx) { m_uParticleIdx = uParticleIdx; }
	unsigned int Get_Particle_Idx() { return m_uParticleIdx; }


	float m_fDistance = 0.f;
private:
	virtual void	Free();


private:
	LPD3DXLINE m_pLine;
	D3DXVECTOR3 m_vVtxPos[2];
	D3DXVECTOR3	m_lowerBound;
	D3DXVECTOR3	m_upperBound;
	unsigned int m_uParticleIdx;

	OBJ_TYPE m_eObjType = OT_WALL;
	bool m_bis_zWall = false;

};

END