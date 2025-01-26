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
	_ulong _vbSize;//���۰� �����Ҽ� �ִ� ��ƼŬ ��(����Ʈ ��ƼŬ�̶� ���ϳ��� �Ѱ�ü ,tricol�� 4���� 1��ü)
	_ulong _vbOffset;//���۸� �� 4�ܰ� ���� ���� ������ �ܰ��� ǥ��
	_ulong _vbBatchSize;//�� ���۴� ��ƼŬ��

	//_vec3 m_vecOrigin;//��ƼŬ ���� ��


	std::list<Attribute> m_listParticles;//��ƼŬ �Ӽ�����Ʈ


	//��� �ΰ� �Ⱦ�
	_float m_fEmitRate;//�ʴ� ��ƼŬ��
	_int m_iMaxParticles;//�ִ� ��ƼŬ��
public:
	static CParticleBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

public:
	virtual void	Free();
};

END

