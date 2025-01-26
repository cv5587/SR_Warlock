#pragma once
#include "Base.h"
#include "Export_Utility.h"

class CPlayer;
class CMiniMapUITex;

class CMiniMap :
    public CBase
{
    DECLARE_SINGLETON(CMiniMap)

private:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniMap();

private:
	HRESULT					Add_Component(vector<int> FloorMeshIdx);
	void					UpdateCamera(const _float& fTimeDelta);


	virtual void			Free();

public:
	HRESULT Ready_MiniMap(CPlayer* pPlayer, vector<int> FloorMeshIdx);
	virtual _int Update_MiniMap(const _float& fTimeDelta);
	virtual void LateUpdate_MiniMap(const float& fTimeDelta);
	void	Render_MiniMap();

public:
	static HRESULT			Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, vector<int> FloorMeshIdx);
	
	
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	LPDIRECT3DSURFACE9		m_pMiniMapSurface = nullptr;

	D3DVIEWPORT9			m_MiniMapVP;
private:
	CPlayer*				m_pPlayer = nullptr;
	Engine::CCamera*		m_pCamera = nullptr;
	CMiniMapUITex*					m_pUITex = nullptr;


	vector<Engine::CMesh*>	m_pMeshGroupCom;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;


};

