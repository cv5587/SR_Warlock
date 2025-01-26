#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CProp : public Engine::CGameObject
{
public:
	explicit CProp(const CProp& rhs);

protected:
	explicit CProp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


public:
	static CProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);

protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component();
	virtual void				TexNamer(); //�� ������Ʈ�� ���� �ٸ� �ؽ�ó ���� ��� Add_Component �� ���� �߰�

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	//�ؽ�ó ���� �Լ�
	//���� ���� �Լ�

	_float				m_fFrame = 0.f;
	_int				m_iMaxFrame;

	std::wstring m_szTexName;	//�ؽ�ó �̸�


};

