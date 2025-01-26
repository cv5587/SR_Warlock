#include"stdafx.h"
#include "Firework.h"
#include "Puddle.h"

//파티클 실행시->add_particle(안에서 각 객체 rest_Particle로 attribute를 초기화 해준다)
//->파티클 각 객체의 정보값을 list에 다 넣어준다
//위작업 완료시 파티클을 그릴 준비가 되었단 의미

CFirework::CFirework(LPDIRECT3DDEVICE9 	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles,_uint _GibComID)
	: Engine::CGameObject(pGraphicDev), m_vecOrigin(_vecOrigin), m_fSize(_fSize), numParticles(_numParticles), m_GibComID(_GibComID), m_GibTexSize(0), m_bPuddle(false)
{
	m_vecOrigin.y = 1.5f;
}

CFirework::CFirework(const CFirework& rhs)
	:Engine::CGameObject(rhs),m_vecOrigin(rhs.m_vecOrigin),m_fSize(rhs.m_fSize), numParticles(rhs.numParticles), m_GibComID(rhs.m_GibComID), m_GibTexSize(rhs.m_GibTexSize), m_bPuddle(rhs.m_bPuddle)
{
}

CFirework::~CFirework()
{
}

HRESULT CFirework::Ready_GameObject()
{//240204 값 받아오며 생성하기 제작
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
//	m_pTransformCom->Set_Pos(m_vecOrigin);
	//m_pTransformCom->Set_Scale(_vec3(0.5f,0.5f,0.5f));


	for (int i = 0; i < numParticles; i++)
	{
		Attribute attribute;
		ResetParticle(&attribute);
		m_pBufferCom->AddParticle(attribute);
	}
	return S_OK;
}

void CFirework::ResetParticle(Attribute* att)
{
	att->_isAlive = true;
	att->_position = m_vecOrigin;	
	att->_com = m_GibComID;	
	if (att->_com==0)
	{
		att->_tex = 0;
		att->_color = D3DXCOLOR(
			GetRandomFloat(0.0f, 1.0f),
			GetRandomFloat(0.0f, 1.0f),
			GetRandomFloat(0.0f, 1.0f),
			1.0f);
	}
	else
	{
		att->_tex = (_uint)GetRandomInt(0, m_GibTexSize);
		att->_color = D3DXCOLOR(0.5f,0.5f,0.5f,1.0f);
	}





	att->_age = GetRandomFloat(0.5f, 1.f);
	if (att->_com == 0)
	{
		D3DXVECTOR3 min = D3DXVECTOR3(-0.1f, 0.1f, -0.1f);
		D3DXVECTOR3 max = D3DXVECTOR3(+0.1f, 0.7f, +0.1f);

		GetRandomVector(
			&att->_velocity,
			&min,
			&max);

		// normalize to make spherical
		D3DXVec3Normalize(
			&att->_velocity,
			&att->_velocity);
		att->_lifeTime = 1.2f;
		att->_velocity *= 3.5f;
	}
	else
	{
		D3DXVECTOR3 min = D3DXVECTOR3(-0.1f, 0.5f, -0.1f);
		D3DXVECTOR3 max = D3DXVECTOR3(+0.1f, 0.9f, +0.1f);

		GetRandomVector(
			&att->_velocity,
			&min,
			&max);

		// normalize to make spherical
		D3DXVec3Normalize(
			&att->_velocity,
			&att->_velocity);
		att->_lifeTime = 4.f;
		att->_velocity *= 7.f;
	}
	
}

void CFirework::Reset()
{
	std::list<Attribute>* BufferList;
	BufferList=m_pBufferCom->Get_AttList();
	std::list<Attribute>::iterator i;
	for (i = BufferList->begin(); i != BufferList->end(); i++)
	{
		ResetParticle(&(*i));
	}
}

_float CFirework::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound)
	{
		return lowBound;
	}
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dist(0, 1.f);

	return (dist(gen) * (highBound - lowBound)) + lowBound;
}
_uint CFirework::GetRandomInt(_uint lowBound, _uint highBound)
{
	if (lowBound >= highBound)
	{
		return lowBound;
	}
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist(lowBound, highBound);

	return dist(gen);
}
void CFirework::GetRandomVector(_vec3* out, _vec3* min, _vec3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

_int CFirework::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PARTICLE, this);


	std::list<Attribute>* BufferList;
	BufferList = m_pBufferCom->Get_AttList();

	std::list<Attribute>::iterator i;

	for (i = BufferList->begin(); i != BufferList->end(); i++)
	{
		m_uiLive = 0;
		// 살아있는 파티클만 업데이트
		if (i->_isAlive)
		{
			m_uiLive++;
			i->_velocity.y -= GRAVITY * fTimeDelta*1.5f;
			i->_position += (i->_velocity) * fTimeDelta*1.5f ;
			if (0>i->_velocity.y)
			{
				D3DXVec3Scale(&i->_acceleration, &i->_velocity, -0.1);
				i->_velocity -= i->_acceleration * fTimeDelta;
			}
			i->_age += fTimeDelta;

			if (i->_age > i->_lifeTime) // 파티클 죽임
			{
				m_uiLive--;
				i->_isAlive = false;
			}
			else if (i->_isAlive ==true&&(i->_position).y <= 1.15f)
			{
				i->_position.y = 1.1f;
				i->_velocity = { 0,0,0 };
				if (i->_com!=0&& !m_bPuddle)
				{
					_vec3 pos = i->_position;
					pos.y = 1.01f+ GetRandomFloat(-0.01f,0.01f);
					CGameObject* pGameObject = CPuddle::Create(m_pGraphicDev, pos);
					pGameObject->Get_TransformCom_Pointer()->Set_Angle({ D3DXToRadian(90),0,0 });
					Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Puddle", pGameObject);
					m_bPuddle = true;
					//피소환
				}
			}
		}
	}

	//만약 단발성 이펙트면 하고 지워주고 
	//지속적인 쓰이는 것은 지워주지않고 계속 쓴다 (파티클 어트리튜브 리스트 값)
	m_pBufferCom->RemoveDeadParticles();


	CGameObject::Update_GameObject(fTimeDelta);
	if (m_uiLive==0)
	{
		return OBJ_DEAD;
	}

	return 0;
}

void CFirework::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFirework::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	

	std::list<Attribute>* BufferList;
	BufferList = m_pBufferCom->Get_AttList();

for(auto& iter :*BufferList)
{
		m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));

		m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.f));

		m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.f));
		m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.f));
		m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.f));

		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		if (0==iter._com)
		{
			m_pTextureCom->Set_Texture(0);
		}
		else
		{
			m_pGibTextureCom->Set_Texture(iter._tex);
		}

		m_pBufferCom->Render_Buffer();


		

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

HRESULT CFirework::Add_Component()
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent=m_pBufferCom = dynamic_cast<CParticleBuffer*>(Engine::Clone_Proto(L"Proto_Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle", pComponent });	



	switch (m_GibComID)
	{
	case 0:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RedSpot"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_RedSpot", pComponent });
		break;
	case 1:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GibYeti"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_GibYeti", pComponent });
		m_GibTexSize = 2;
		break;
	case 2:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GibFlamer"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_GibFlamer", pComponent });
		m_GibTexSize = 1;
		break;
	case 3:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GibLooker"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_GibLooker", pComponent });
		m_GibTexSize = 3;
		break;
	case 4:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GibShambler"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_GibShambler", pComponent });
		m_GibTexSize = 2;
		break;
	case 5:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_GibThingy"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_GibThingy", pComponent });
		m_GibTexSize = 3;
		break;
	case 6:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YellowSpot"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_YellowSpot", pComponent });
		break;
	case 7:
		pComponent = m_pGibTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PurpleSpot"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_PurpleSpot", pComponent });
		break;
	default:
		break;
	}

	return S_OK;
}

CFirework* CFirework::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles,_uint _GibComID)
{
	CFirework* pInstance = new CFirework(pGraphicDev,_vecOrigin,_fSize, _numParticles, _GibComID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFirework::Free()
{
	__super::Free();
}

