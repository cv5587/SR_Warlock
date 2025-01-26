#include"stdafx.h"
#include "Snow.h"



//파티클 실행시->add_particle(안에서 각 객체 rest_Particle로 attribute를 초기화 해준다)
//->파티클 각 객체의 정보값을 list에 다 넣어준다
//위작업 완료시 파티클을 그릴 준비가 되었단 의미

CSnow::CSnow(LPDIRECT3DDEVICE9 	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles, _float _box)
	: Engine::CGameObject(pGraphicDev), m_vecOrigin(_vecOrigin), m_fSize(_fSize), numParticles(_numParticles)
{
	m_vecmin = _vec3(-_box, 0, -_box);
	m_vecmax = _vec3(_box, _box, _box);	
}

CSnow::CSnow(const CSnow& rhs)
	:Engine::CGameObject(rhs),m_vecOrigin(rhs.m_vecOrigin),m_fSize(rhs.m_fSize), numParticles(rhs.numParticles), m_vecmin(rhs.m_vecmin), m_vecmax(rhs.m_vecmax)
{
}

CSnow::~CSnow()
{
}

HRESULT CSnow::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(m_vecOrigin);	

	for (int i = 0; i < numParticles; i++)
	{
		Attribute attribute;
		ResetParticle(&attribute);
		m_pBufferCom->AddParticle(attribute);
	}
	return S_OK;
}

void CSnow::ResetParticle(Attribute* att)
{
	att->_isAlive = true;

	GetRandomVector(	&att->_position,	&m_vecmin,	&m_vecmax);
	att->_tex = 0;
	att->_com = 0;
	att->_position.y = m_vecmax.y;

	att->_velocity.x = GetRandomFloat(0.f, 1.f) * -3.f;
	att->_velocity.y = GetRandomFloat(0.f, 1.f) * -10.f;
	att->_velocity.z = 0.f;

	att->_color = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
}
void CSnow::Reset()
{
	std::list<Attribute>* BufferList;
	BufferList=m_pBufferCom->Get_AttList();
	std::list<Attribute>::iterator i;
	for (i = BufferList->begin(); i != BufferList->end(); i++)
	{
		ResetParticle(&(*i));
	}
}
_float CSnow::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound)
	{
		return lowBound;
	}
	std::mt19937 gen(rd());

	//_float f = (rand() % 10000) * 0.0001f;
	std::uniform_real_distribution<float> dist(0,1.f);		

	return (dist(gen) * (highBound - lowBound)) + lowBound;
}

void CSnow::GetRandomVector(_vec3* out, _vec3* min, _vec3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

_int CSnow::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PARTICLE, this);



	std::list<Attribute>* BufferList;
	BufferList = m_pBufferCom->Get_AttList();

	std::list<Attribute>::iterator i;

	for (i = BufferList->begin(); i != BufferList->end(); i++)
	{
		i->_position += i->_velocity * fTimeDelta;
		if (i->_position.x >= m_vecmin.x && i->_position.y >= m_vecmin.y && i->_position.z >= m_vecmin.z &&
			i->_position.x <= m_vecmax.x && i->_position.y <= m_vecmax.y && i->_position.z <= m_vecmax.z)
		{
			i->_isAlive = TRUE;
		}
		else
		{
			i->_isAlive = FALSE;
			ResetParticle(&(*i));
		}
	}

	//만약 단발성 이펙트면 하고 지워주고 
	//지속적인 쓰이는 것은 지워주지않고 계속 쓴다 (파티클 어트리튜브 리스트 값)
	//m_pBufferCom->RemoveDeadParticles();

	CGameObject::Update_GameObject(fTimeDelta);


	return 0;
}

void CSnow::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CSnow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	

	std::list<Attribute>* BufferList;
	BufferList = m_pBufferCom->Get_AttList();

	if (!BufferList->empty())
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

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

HRESULT CSnow::Add_Component()
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent=m_pBufferCom = dynamic_cast<CParticleBuffer*>(Engine::Clone_Proto(L"Proto_Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Particle", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_WhiteSpot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_WhiteSpot", pComponent });
	return S_OK;
}

CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles, _float _box)
{
	CSnow* pInstance = new CSnow(pGraphicDev,_vecOrigin,_fSize, _numParticles,_box);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSnow::Free()
{
	__super::Free();
}
