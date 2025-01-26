
#include "ParticleBuffer.h"

CParticleBuffer::CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_vb(nullptr), _vbSize(2048), _vbBatchSize(512),_vbOffset(0)
{
}

//복사 객체도 고정값을 넣어주는 이유는 전체 쓸용량은 항시 고정하기 때문?
CParticleBuffer::CParticleBuffer(const CParticleBuffer& rhs)
	:CComponent(rhs), m_vb(rhs.m_vb),  _vbSize(2048), _vbBatchSize(512), _vbOffset(0)
{
	m_vb->AddRef();
}

CParticleBuffer::~CParticleBuffer()
{
}

_int CParticleBuffer::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

HRESULT CParticleBuffer::Ready_Buffer()
{
	//2048개의 파티클을 수용할 공간을 제작 한 상태
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(_vbSize * sizeof(EParticle),	// 생성할 버텍스 버퍼의 크기
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY,			// 버텍스 버퍼의 종류(0인 경우 정적 버퍼, d3dusage_dynamic인 경우 동적 버퍼)
		EParticle::PFVF,	// 입력받을 버텍스 속성 옵션
		D3DPOOL_DEFAULT, // 동적버퍼
		&m_vb,	
		NULL),	// 공유 설정
		E_FAIL);


	return S_OK;
}

void CParticleBuffer::Render_Buffer()
{
		m_pGraphicDev->SetFVF(EParticle::PFVF);
		m_pGraphicDev->SetStreamSource(0, m_vb, 0, sizeof(EParticle));
		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		EParticle* v = 0;

		m_vb->Lock(
			_vbOffset * sizeof(EParticle),
			_vbBatchSize * sizeof(EParticle),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		_ulong numParticlesInBatch = 0;

		list<Attribute>::iterator i;
		for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
		{
			if (i->_isAlive)
			{
					v->_posiotion = i->_position;
					v->_color = (D3DCOLOR)i->_color;
					v++;
					numParticlesInBatch++;

				if (numParticlesInBatch==_vbBatchSize)
				{
					m_vb->Unlock();
					//그릴때 2048개를 4등분의 (512) 그룹을 나눈다. 그리구 각 4그룹을 실행하는데 offset은 각 그룹의 넘버링(실행될 파티클넘버) , batchsize는 각 그룹의 크기 (512로 고정)
					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					_vbOffset += _vbBatchSize;

					if (_vbOffset>=_vbSize)
					{
						_vbOffset = 0;
					}

					m_vb->Lock
					(
						_vbOffset * sizeof(EParticle),
						_vbBatchSize * sizeof(EParticle),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD
					);

					numParticlesInBatch = 0;
				}
			}
		}
		
		m_vb->Unlock();

		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,
																_vbOffset,
																numParticlesInBatch);
		}

		_vbOffset += _vbBatchSize;

	

}



void CParticleBuffer::RemoveDeadParticles()
{
	list<Attribute>::iterator	i = m_listParticles.begin();

	while (i != m_listParticles.end())
	{
		if (i->_isAlive == FALSE)
		{
			i = m_listParticles.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void CParticleBuffer::AddParticle(Attribute&	attribute)
{
	m_listParticles.push_back(attribute);
}



CParticleBuffer* CParticleBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticleBuffer* pInstance = new CParticleBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CParticleBuffer::Clone(void)
{

	return new CParticleBuffer(*this);
}

void CParticleBuffer::Free()
{
	if (m_vb)
		Safe_Release(m_vb);

	CComponent::Free();
}
