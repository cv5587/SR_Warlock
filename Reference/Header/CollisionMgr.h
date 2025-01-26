#pragma once
#include "Base.h"
#include "Engine_Define.h"


BEGIN(Engine)

class Tree;
class CGameObject;
class CLine;

class ENGINE_DLL CCollisionMgr :
    public CBase
{
    DECLARE_SINGLETON(CCollisionMgr)

public:
    HRESULT Ready_CollisionMgr();
    void LateUpdate_CollisionMgr(float fTimeDelta);

    unsigned int Add_Collider(CGameObject* pGameObject, double radius = 1.0);
    unsigned int Add_Collider(CGameObject* pGameObject, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);

    unsigned int Add_Line_Collider(CLine* pComponent, D3DXVECTOR3 lowerBound, D3DXVECTOR3 upperBound);


    HRESULT Remove_Collider(unsigned int particleIdx);
    HRESULT Remove_LineCollider(unsigned int lineParticleIdx);

    //for debug
    HRESULT Render_Colliders();



    //bool overlaps(D3DXVECTOR3& position1, D3DXVECTOR3& position2,
    //    const std::vector<bool>& periodicity, const D3DXVECTOR3& boxSize, double cutOff);

    // aabb와 겹치는지 체크 (Sphere)
    bool overlaps(CGameObject* pGameObject1, CGameObject* pGameObject2) const;

private:
    explicit CCollisionMgr();
    virtual ~CCollisionMgr();


private:
    virtual void	Free();


private:
    Tree* _tree[2];
    unsigned int _particleIdx;
    unsigned int _LineParticleIdx;
    map<unsigned int, CGameObject*>			_mapObjs;

    //맵 바뀔때 초기화 해주고 다음 라인들 넣어주기
    map<unsigned int, CLine*>          _mapLines;
    std::vector<bool> _periodicity;
    D3DXVECTOR3 _boxSize;

    
};

END