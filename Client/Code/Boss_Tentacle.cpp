#include "stdafx.h"
#include "..\Header\Boss_Tentacle.h"
#include "MonsterBullet.h"
#include "Effect.h"

CBoss_Tentacle::CBoss_Tentacle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_bSuperArmor(false), m_bDead(false)
{
}

CBoss_Tentacle::CBoss_Tentacle(const CBoss_Tentacle& rhs)
	: CMonster(rhs), m_bSuperArmor(rhs.m_bSuperArmor), m_bDead(rhs.m_bDead)
{
	Ready_GameObject();
}

CBoss_Tentacle::~CBoss_Tentacle()
{
}

HRESULT CBoss_Tentacle::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iMaxHealth = m_iCurHealth = 30;
	m_iDamage = 5;
	m_fSpeed = 1.f;
	m_fDtctRng = 30.f;
	m_fAtkRng = 15.f;
	m_bInit = false;
	m_bHit = false;
	m_bAtk = false;
	m_fAtkTimer = 0.f;
	m_fStiffTimer = 0.f;
	m_fStiffSecond = 3.f;
	m_pTransformCom->Set_Pos(5.f, 1.f, 5.f);
	m_vPrePos = m_pTransformCom->Get_Pos();
	m_szName = L"Tentacle";

	m_eState = MONSTER_IDLE;
	m_ePreState = MONSTER_END;

	//test
	m_eObjType = OT_MONSTER;

	m_vPrePos = m_pTransformCom->Get_Pos();
	m_lowerBound = { m_vPrePos.x - 0.2f, m_vPrePos.y - 0.8f, m_vPrePos.z - 0.2f };
	m_upperBound = { m_vPrePos.x + 0.2f, m_vPrePos.y + 0.8f, m_vPrePos.z + 0.2f };
	CGameObject::Push_CollisionMgr(m_eObjType, m_lowerBound, m_upperBound);

	return S_OK;
}

Engine::_int CBoss_Tentacle::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);

	m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

	////버그 있으면 수정요청 바람..

	if (m_bHit)
	{
		Boss_Tentacle_Take_Dmg(fTimeDelta);
	}

	switch (m_eState)
	{
	case MONSTER_IDLE:
	{
		if (!m_bInit)
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
			m_bInit = true;
		}

		_float d = DistanceToPlayer(m_pPlayer);
		if (m_fDtctRng > d && m_fAtkRng > d)
		{
			m_fAtkTimer += fTimeDelta;
			if (3.f < m_fAtkTimer)
			{
				m_eState = MONSTER_ATK;
				m_fAtkTimer = 0.f;
				m_fFrame = 0.f;
			}
		}
	}
	break;
	case MONSTER_RUN:		// == Tentacle In
	{
		if (m_iMaxFrame <= (_uint)(m_fFrame))
		{
			m_eState = MONSTER_IDLE;
			m_fFrame = 0.f;
		}
	}
	break;
	case MONSTER_ATK:
	{
		if (m_iMaxFrame <= (_uint)(m_fFrame))
		{
			_vec3 vSrc;

			m_pTransformCom->Get_Info(INFO_POS, &vSrc);
			//총알 생성
			CGameObject* pGameObject;
			_vec3 vFrom;
			vFrom = m_pTransformCom->Get_Pos();
			pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_LOOKER, vFrom, m_pPlayer->Get_TransformCom_Pointer()->Get_Pos());

			if (pGameObject != nullptr)
			{
				CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
				//총알 크기 조정
				transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

				Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossTentacleBullet", pGameObject);
			}

			wstring szSoundFile = L"";

			int i = rand() % 15;
			if (5 > i)
			{
				szSoundFile = L"Tapeworm_death_01.wav";
			}
			else if (10 > i)
			{
				szSoundFile = L"Tapeworm_death_02.wav";
			}
			else
			{
				szSoundFile = L"Tapeworm_death_03.wav";
			}

			Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

			m_eState = MONSTER_IDLE;
			m_fFrame = 0.f;
		}

	}
	break;
	//맞으면 투명해지는 문제
	case MONSTER_HIT:		// == Tentacle Out
	{
		if (m_iMaxFrame <= (_uint)(m_fFrame))
		{
			m_bSuperArmor = true;

			//if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
			{
				m_fStiffTimer += 10 * fTimeDelta;

				m_fFrame = 8.f;

				if (5.f < m_fStiffTimer)
				{
					m_eState = MONSTER_RUN;
					m_fFrame = 0.f;
					m_bSuperArmor = false;
				}
			}

		}
	}
	break;
	case MONSTER_DEAD:
	{
		if (m_iMaxFrame <= (_uint)(m_fFrame))
		{
			//TODO : 매끄러운 사망 구현
			return OBJ_DEAD;
			//return OBJ_DEAD;
		}

	}
	break;
	}

	//if (!m_lBullets.empty())
	//{
	//	for (auto iter : m_lBullets)
	//	{
	//		iter->Update_GameObject(fTimeDelta);
	//	}
	//}
	//TODO ; 몬스터 불렛 리스트 업데이트

	return OBJ_NOEVENT;
}

void CBoss_Tentacle::LateUpdate_GameObject(const float& fTimeDelta)
{
	Motion_Change();

	__super::LateUpdate_GameObject(fTimeDelta);
	if (m_iCurHealth != m_iPreHealth) m_iPreHealth = m_iCurHealth;

}

void CBoss_Tentacle::Render_GameObject()
{
	if (m_eState == MONSTER_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
	{
		return;
	}

	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_Tentacle::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	OBJ_TYPE ot = pGameObject->Get_ObjType();

	if (OT_PLAYER_BULLET == ot)
	{
		Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());
		m_fStiffTimer = 0.f;

		//피격 이펙트 출력
		CGameObject* pEffect;
		_vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
		pEffect = CEffect::Create(m_pGraphicDev, FX_BLOOD_GREEN, vPos);

		if (pEffect != nullptr)
		{
			//이펙트 플레이어 방향으로 조금 이동
			_vec3 vDir;
			D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
			CTransform* transf = dynamic_cast<CTransform*>(pEffect->Get_TransformCom_Pointer());
			transf->Set_Pos((vPos + vDir * 0.1f));
			//이펙트 크기 조정
			transf->Set_Scale({ 0.5f, 0.5f, 0.5f });
			CGameObject* pParticleObject;
			pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.08f, 6, 6);
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ShamblerEffect", pEffect);
		}
	}
	__super::Action_On_Collision(pGameObject, fTimeDelta);
}

void CBoss_Tentacle::Action_On_Collision(vector<CLine*> pvecLines)
{

}

HRESULT CBoss_Tentacle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Textures
	pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Tentacle_IdleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Tentacle_IdleTexture", pComponent });

	pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Tentacle_InTexture"));//Run == In 으로 쓸 예정
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Tentacle_InTexture", pComponent });

	pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Tentacle_OutTexture")); //Hit == Out 으로 쓸 예정
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Tentacle_OutTexture", pComponent });

	pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Tentacle_AtkTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Tentacle_AtkTexture", pComponent });

	pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Tentacle_DeadTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Tentacle_DeadTexture", pComponent });
	//Texture Ends

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CBoss_Tentacle* CBoss_Tentacle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_Tentacle* pInstance = DBG_NEW CBoss_Tentacle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_Tentacle::Free()
{
	CMonster::Free();
	//for_each(m_lBullets.begin(), m_lBullets.end(), CDeleteObj());
}

void CBoss_Tentacle::Motion_Change()
{
	if (m_eState != m_ePreState)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
		{
			m_pTextureCom = m_pTexCom_Idle;
			m_iMaxFrame = 1;
			m_fFrameSpeed = 1.f;
		}
		break;
		case MONSTER_RUN:
		{
			m_pTextureCom = m_pTexCom_Run;
			m_iMaxFrame = 6;
			m_fFrameSpeed = 10.f;
		}
		break;
		case MONSTER_ATK:
		{
			m_pTextureCom = m_pTexCom_Atk;
			m_iMaxFrame = 10;
			m_fFrameSpeed = 7.f;
		}
		break;
		case MONSTER_HIT:
		{
			m_pTextureCom = m_pTexCom_Hit;
			m_iMaxFrame = 9;
			m_fFrameSpeed = 10.f;
		}
		break;
		case MONSTER_DEAD:
		{
			m_pTextureCom = m_pTexCom_Dead;
			m_iMaxFrame = 12;
			m_fFrameSpeed = 10.f;
			m_bDead = true;
		}
		break;
		}

		m_ePreState = m_eState;

	}

}

void CBoss_Tentacle::Boss_Tentacle_Take_Dmg(const _float& fTimeDelta)
{
	if (m_bSuperArmor)
	{
		m_iCurHealth = m_iPreHealth;
		return;
	}

	wstring szSoundFile = L"";

	if (m_eState != MONSTER_DEAD)
	{
		if (0 < m_iCurHealth)
		{
			int i = rand() % 15;
			if (5 > i)
			{
				szSoundFile = L"Tapeworm_pain_01.wav";
			}
			else if (10 > i)
			{
				szSoundFile = L"Tapeworm_pain_02.wav";
			}
			else
			{
				szSoundFile = L"Tapeworm_pain_03.wav";
			}

			m_fFrame = 0.f;
			m_eState = MONSTER_HIT;
		}
		else if (0 >= m_iCurHealth)
		{

			int i = rand() % 15;
			if (5 > i)
			{
				szSoundFile = L"Tapeworm_death_01.wav";
			}
			else if (10 > i)
			{
				szSoundFile = L"Tapeworm_death_02.wav";
			}
			else
			{
				szSoundFile = L"Tapeworm_death_03.wav";
			}

			m_eState = MONSTER_DEAD;
			m_fFrame = 0.f;
			CGameObject* pGameObject;
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			pGameObject = CEffect::Create(m_pGraphicDev, FX_OVERKILL, vPos);

			if (pGameObject != nullptr)
			{
				Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"d%d", pGameObject);
			}

		}
		Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);
		m_bHit = false;
	}
}

