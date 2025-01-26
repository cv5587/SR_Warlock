
#include "ParticleBuffer.h"

CParticleBuffer::CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_vb(nullptr), _vbSize(2048), _vbBatchSize(512),_vbOffset(0)
{
}

//���� ��ü�� �������� �־��ִ� ������ ��ü ���뷮�� �׽� �����ϱ� ����?
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
	//2048���� ��ƼŬ�� ������ ������ ���� �� ����
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(_vbSize * sizeof(EParticle),	// ������ ���ؽ� ������ ũ��
		D3DUSAGE_DYNAMIC|D3DUSAGE_POINTS|D3DUSAGE_WRITEONLY,			// ���ؽ� ������ ����(0�� ��� ���� ����, d3dusage_dynamic�� ��� ���� ����)
		EParticle::PFVF,	// �Է¹��� ���ؽ� �Ӽ� �ɼ�
		D3DPOOL_DEFAULT, // ��������
		&m_vb,	
		NULL),	// ���� ����
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
					//�׸��� 2048���� 4����� (512) �׷��� ������. �׸��� �� 4�׷��� �����ϴµ� offset�� �� �׷��� �ѹ���(����� ��ƼŬ�ѹ�) , batchsize�� �� �׷��� ũ�� (512�� ����)
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
