#pragma once
#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CParticleBuffer : public CComponent
{
private:
	explicit CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	CParticleBuffer(const CParticleBuffer& rhs);
	virtual ~CParticleBuffer();
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();
	virtual void		RemoveDeadParticles();
	virtual void		AddParticle(Attribute&	attribute);
	//void		Set_Origin(_vec3 _position) { m_vecOrigin = _position; }
	std::list<Attribute>* Get_AttList() { return &m_listParticles; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	_ulong _vbSize;//버퍼가 보관할수 있는 파티클 수(포인트 파티클이라 점하나가 한객체 ,tricol은 4점이 1객체)
	_ulong _vbOffset;//버퍼를 총 4단계 쓴다 그중 버퍼의 단계을 표시
	_ulong _vbBatchSize;//한 버퍼당 파티클수

	//_vec3 m_vecOrigin;//파티클 생성 점


	std::list<Attribute> m_listParticles;//파티클 속성리스트


	//요고 두개 안씀
	_float m_fEmitRate;//초당 파티클수
	_int m_iMaxParticles;//최대 파티클수
public:
	static CParticleBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

public:
	virtual void	Free();
};

END

