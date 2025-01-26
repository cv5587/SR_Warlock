#include "CollisionMgr.h"
#include "DynamicAABBTree.h"
#include "GameObject.h"
#include "LIne.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CCollisionMgr)



bool CCollisionMgr::overlaps(CGameObject* pGameObject1, CGameObject* pGameObject2) const
{
	float fRadius = pGameObject1->Get_Radius();
	if (fRadius == 0.f)
	{
		MSG_BOX("GameObject 1 is not possess Radius");
	}
	float fRadius2 = pGameObject2->Get_Radius();
	if (fRadius2 == 0.f)
	{
		MSG_BOX("GameObject 2 is not possess Radius");
	}

	_vec3 vDistance = pGameObject1->Get_TransformCom_Pointer()->Get_Pos() - pGameObject2->Get_TransformCom_Pointer()->Get_Pos();
	_float fDistance = D3DXVec3LengthSq(&vDistance);

	return fDistance < (fRadius + fRadius2);
}


CCollisionMgr::CCollisionMgr() : _tree{}, _particleIdx(0), _LineParticleIdx(100000)
{
	if (Ready_CollisionMgr() == E_FAIL)
	{
		Safe_Release(m_pInstance);
		MSG_BOX("CollisionMgr Create Failed");
	}
}

CCollisionMgr::~CCollisionMgr()
{
	Free();
}

HRESULT CCollisionMgr::Ready_CollisionMgr()
{
	double maxDisp = 0.01;

	_periodicity = { true,true,true };

	_tree[0] = DBG_NEW Tree(maxDisp, _periodicity);


	return S_OK;
}

unsigned int CCollisionMgr::Add_Collider(CGameObject* pGameObject, double radius)
{
	if (pGameObject->Get_TransformCom_Pointer() == nullptr)
	{
		MSG_BOX("pGameObject's TransformCom_Pointer is nullptr");
		return 999999;
	}
	_tree[0]->insertParticle(_particleIdx, pGameObject->Get_TransformCom_Pointer()->Get_Pos(), radius);
	_mapObjs.emplace(std::pair<unsigned int, CGameObject*>(_particleIdx, pGameObject));
	int particleIdx = _particleIdx;
	_particleIdx++;
	return particleIdx;
}

unsigned int CCollisionMgr::Add_Collider(CGameObject* pGameObject, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	_tree[0]->insertParticle(_particleIdx, lowerBound, upperBound);
	_mapObjs.emplace(std::pair<unsigned int, CGameObject*>(_particleIdx, pGameObject));
	int particleIdx = _particleIdx;
	_particleIdx++;
	return particleIdx;
}

unsigned int CCollisionMgr::Add_Line_Collider(CLine* pComponent, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound)
{
	_tree[0]->insertParticle(_LineParticleIdx, lowerBound, upperBound);
	_mapLines.emplace(std::pair<unsigned int, CLine*>(_LineParticleIdx, pComponent));
	int lineParticleIdx = _LineParticleIdx;
	_LineParticleIdx++;
	return lineParticleIdx;
}

HRESULT CCollisionMgr::Remove_Collider(unsigned int particleIdx)
{
	_mapObjs.erase(particleIdx);		
	_tree[0]->removeParticle(particleIdx);
	return S_OK;
}

HRESULT CCollisionMgr::Remove_LineCollider(unsigned int lineParticleIdx)
{
	_mapLines.erase(lineParticleIdx);		//오류있는지 확인하기
	_tree[0]->removeParticle(lineParticleIdx);
	return S_OK;
}

HRESULT CCollisionMgr::Render_Colliders()
{

	for (auto& objPair : _mapObjs)
	{
		FAILED_CHECK_RETURN(objPair.second->Draw_Box_Collider(),E_FAIL);
	}

	return S_OK;
}


//bool CCollisionMgr::overlaps(D3DXVECTOR3& position1, D3DXVECTOR3& position2, const std::vector<bool>& periodicity, const D3DXVECTOR3& boxSize, double cutOff)
//{
//	// Calculate particle separation.
//	D3DXVECTOR3 separation;
//	separation.x = (position1[0] - position2[0]);
//	separation.y = (position1[1] - position2[1]);
//	separation.z = (position1[2] - position2[2]);
//
//	// Calculate minimum image separation.
//	minimumImage(separation, periodicity, boxSize);
//
//	double rSqd = separation[0] * separation[0] + separation[1] * separation[1] + separation[2] * separation[2];
//
//	if (rSqd < cutOff) return true;
//	else return false;
//}





void CCollisionMgr::LateUpdate_CollisionMgr(float fTimeDelta)
{
	if (_particleIdx == 0)
		return;

	if ((_mapObjs.size()+_mapLines.size()) != _tree[0]->nParticles())
	{
		MSG_BOX("list and particles are differnt size");
		return;
	}



	for (auto& iter : _mapObjs)
	{
		_tree[0]->updateParticle(iter.first, iter.second->Get_Bounds().first, iter.second->Get_Bounds().second, true);
	}

	for (auto& iter : _mapLines)
	{
		_tree[0]->updateParticle(iter.first, iter.second->Get_Bounds().first, iter.second->Get_Bounds().second, true);
	}
	
	for (auto& iter : _mapObjs)
	{
		std::vector<CLine*> pLines;
		std::vector<unsigned int> particles = _tree[0]->query(iter.second->Get_ParticleIdx());
		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i] > 99999)
				pLines.push_back(_mapLines[particles[i]]);
			else
			{
				if (particles[i] != iter.first /*&& overlaps(iter.second, _mapObjs[particles[i]])*/)
				{
					iter.second->Action_On_Collision(_mapObjs[particles[i]], fTimeDelta);
				}
			}
		}

		if (pLines.empty() == false)
		{
			iter.second->Action_On_Collision(pLines);
		}
	}



}

void CCollisionMgr::Free()
{
	_mapObjs.clear();
	_mapLines.clear();

	_tree[0]->removeAll();
	delete _tree[0];
}
