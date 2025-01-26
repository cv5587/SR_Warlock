#pragma once
#include "Component.h"

BEGIN(Engine)

enum class ColliderType
{
	Sphere, Plane
};

class ENGINE_DLL CBaseCollider : public CComponent
{
protected:
	explicit CBaseCollider(LPDIRECT3DDEVICE9 pGraphicDev, ColliderType colliderType);
	explicit CBaseCollider(const CBaseCollider& rhs);
	virtual ~CBaseCollider();


public:
	virtual bool Intersects(ColliderType Object1,ColliderType Object2) = 0;


public:
	virtual void		Free();

private:
	ColliderType m_eColliderType;
};

END
