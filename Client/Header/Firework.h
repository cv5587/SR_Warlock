#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include <random>

BEGIN(Engine)
class CTransform;
class CTexture;
class CParticleBuffer;
END

class CFirework :public Engine::CGameObject
{
private:
    explicit CFirework(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles, _uint _GibComID);
    explicit CFirework(const CFirework& rhs);
    virtual ~CFirework();

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const _float& fTimeDelta)					 override;
    virtual void Render_GameObject()						 override;

protected:
    HRESULT			Add_Component();
    void ResetParticle(Attribute* att);
    void Reset();
    void GetRandomVector(_vec3* out, _vec3* min, _vec3* max);
    _float GetRandomFloat(_float lowBound, _float highBound);
    _uint GetRandomInt(_uint lowBound, _uint highBound);
    DWORD FtoDw(float f) { return *((DWORD*)&f); }


 private:
     Engine::CParticleBuffer* m_pBufferCom;//버텍스 컴객체
     Engine::CTransform* m_pTransformCom;//트렌스폼 컴객체
     Engine::CTexture* m_pTextureCom;//텍스처 컴객체
     Engine::CTexture* m_pGibTextureCom;//조각 컴객체

     //부모객체에 있음
     _float m_fSize;//파티클 크기
     _vec3 m_vecOrigin;//파티클 생성 점
     _int numParticles;//파티클 버퍼 생성 갯수
     _uint m_uiLive;//살아있는 파티클 갯수
     _uint m_GibComID;
     _uint m_GibTexSize;
     std::random_device rd;
     _bool m_bPuddle;
public:
    static CFirework* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles,_uint _GibComID);

protected:
    virtual void Free() override;

};

