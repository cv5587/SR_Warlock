#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

//TODO : �浹ó�� �Ϲ� �Ѿ˰� �ٸ��� PASSPROP���� ����?
//Flamer, SHambler ���
class CIceBullet : public Engine::CGameObject
{
protected:
	explicit CIceBullet(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart); //TexNum 0 1 2 ����, 3 ȭ��
	explicit CIceBullet(const CIceBullet& rhs);
	virtual ~CIceBullet();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	override;
	virtual void Render_GameObject()						 override;
public:

	bool						Get_Dead() { return m_bDead; }
private:
	virtual  HRESULT			Add_Component();

	_float						DistanceToPlayer(CPlayer* pPlayer); // �÷��̾� ��ġ == ī�޶� Eye�Ÿ� ���

	//�� �� �Լ��� �������� ���
	void						TexNamer();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

protected:
	CPlayer* m_pPlayer;

	_int				m_iTexNum;
	_vec3				m_vStart;

	wstring m_szTexName;	//�ؽ�ó �̸�

	_float				m_fFrame = 0.f;
	_uint				m_iMaxFrame = 1;
	_float				m_fFrameSpeed = 0.f;

	_int		m_iDamage;
	_float		m_fSpeed;
	_float		m_fAtkRng;
	_float		m_fAtkTimer;	//������ �ð� ��� �뵵 ����

	bool		m_bDead;
	bool		m_bInit;


public:
	static CIceBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart);

public:
	virtual void Free() override;
};
