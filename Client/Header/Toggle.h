#pragma once
#include "Base.h"
#include "InteractiveProp.h"

class CToggle : public CInteractiveProp
{
public:
	explicit CToggle(const CToggle& rhs);
private:
	explicit CToggle(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eToggleID);
	virtual ~CToggle();


public:
	static CToggle* Create(LPDIRECT3DDEVICE9	pGraphicDev, TOGGLEID eTogID);
	static CToggle* Create(LPDIRECT3DDEVICE9	pGraphicDev, TOGGLEID eTogID, D3DXVECTOR3 vPos, float fYAngle);
	void	Set_ToggleOn() { m_bToggleOn = true; }


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


private:
	virtual HRESULT Ready_GameObject()						 override;
	HRESULT			Ready_GameObject(D3DXVECTOR3 vPos, float fYAngle);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;
	virtual void Free()										 override;

	virtual  HRESULT	Add_Component()						override;
	virtual void		TexNamer()							override;
private:
	void Detect_Player();

private:
	bool	m_bToggleOn;
	TOGGLEID m_eToggleID;
	DOORID  m_eDoorID;
	bool	m_bIsZToggle;
};

//색깔문이 열리는 조건 : 
// 1. 플레이어가 같은 색깔 토글을 눌렀다(m_bUnlock == true)
// 2. 플레이어가 키 입력 반경 안에 있다
// 3. 플레이어가 상호작용 키를 눌렀다

// 토글 눌리는 조건 :
// 1. 플레이어가 같은 색깔 열쇠를 가지고 있다
// 2. 플레이어가 키 입력 반경 안에 있다
// 3. 플레이어가 상호작용 키를 눌렀다

// 토글이 눌리면 어떻게 되는가
// 1. 토글의 텍스처가 바뀜
// 2. 토글 ID에 따라서 같은 색깔 문들의 m_bUnlock = true
// 
//가다와서할거 : 스위치 기믹 . 충돌처리로 근처에 있는 문들 다 올리던가
//키 먹고 스위치 켜야 열 수 있었나
//TODO : 애니메이션 돌리기