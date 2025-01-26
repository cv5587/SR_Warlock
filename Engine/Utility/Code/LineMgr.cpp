#include "..\..\Header\LineMgr.h"
#include "Export_Utility.h"
int	CLineMgr::totalLineidx = 0;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Free();
}

HRESULT CLineMgr::Ready_LineMgr(std::vector<unsigned int> vlineGroup)
{
	FAILED_CHECK_RETURN(Add_Line_Component(vlineGroup), E_FAIL);

	return S_OK;
}


HRESULT CLineMgr::Add_Line_Component(std::vector<unsigned int> vlineGroup)
{
	constexpr float EPSILON = 0.1f;

	for (int i = 0; i < vlineGroup.size();++i)
	{
		TCHAR szProtoName[128] = L"Proto_Line%d";

		for (int j = 0; j < vlineGroup[i]; ++j)
		{

			TCHAR	szName[128] = L"";

			wsprintf(szName, szProtoName, totalLineidx);

			CComponent* pComponent = nullptr;
			CLine* pLine = nullptr;

			pComponent = pLine = dynamic_cast<CLine*>(Engine::Clone_Proto(wstring(szName)));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_LineList.push_back(pLine);

			D3DXVECTOR3* arrVtxPos = pLine->Get_ArrVtxPos();

			D3DXVECTOR3 vLine = arrVtxPos[1] - arrVtxPos[0];

			if (abs(vLine.x) < EPSILON)
			{
				D3DXVECTOR3 lowerBound = { arrVtxPos[0].x - 0.2f, 1.f, min(arrVtxPos[1].z, arrVtxPos[0].z) };
				D3DXVECTOR3 upperBound = { arrVtxPos[0].x + 0.2f, 3.f, max(arrVtxPos[1].z, arrVtxPos[0].z) };
				pLine->Set_Bounds(pair<D3DXVECTOR3, D3DXVECTOR3>(lowerBound, upperBound));
				pLine->Set_Is_Z_Wall(false);

			}
			else if (abs(vLine.z < EPSILON))
			{
				D3DXVECTOR3 lowerBound = { min(arrVtxPos[1].x, arrVtxPos[0].x), 1.f, arrVtxPos[0].z - 0.2f };
				D3DXVECTOR3 upperBound = { max(arrVtxPos[1].x, arrVtxPos[0].x), 3.f, arrVtxPos[0].z + 0.2f };
				pLine->Set_Bounds(pair<D3DXVECTOR3, D3DXVECTOR3>(lowerBound, upperBound));
				pLine->Set_Is_Z_Wall(true);

			}
			else
			{
				MSG_BOX("invalid wall");
			}

			totalLineidx++;
		}
	}

	return S_OK;
}

HRESULT CLineMgr::Remove_All_Line_Component()
{
	for (vector<CLine*>::iterator it = m_LineList.begin(); it != m_LineList.end(); ++it)
	{
		Engine::Remove_LineCollider((*it)->Get_Particle_Idx());
	}

	for (int i = 0; i < m_LineList.size(); ++i)
	{
		Safe_Release(m_LineList[i]);
	}

	m_LineList.clear();

	return S_OK;
}

void CLineMgr::Push_Line_Collision()
{
	for (vector<CLine*>::iterator it = m_LineList.begin(); it != m_LineList.end(); ++it)
	{
		unsigned int particleidx = Engine::Add_Line_Collider((*it), (*it)->Get_Bounds().first, (*it)->Get_Bounds().second);
		(*it)->Set_Particle_Idx(particleidx);
	}
}

CLineMgr* CLineMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, std::vector<unsigned int> vlineGroup)
{
	CLineMgr*  pInstance = DBG_NEW CLineMgr();

	if (FAILED(pInstance->Ready_LineMgr(vlineGroup)))
	{
		Safe_Release(pInstance);
		MSG_BOX("LineMgr Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLineMgr::Free()
{
	for (int i = 0; i < m_LineList.size(); ++i)
	{
		Safe_Release(m_LineList[i]);
	}

	m_LineList.clear();
}





