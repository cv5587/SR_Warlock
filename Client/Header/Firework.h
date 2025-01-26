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
     Engine::CParticleBuffer* m_pBufferCom;//���ؽ� �İ�ü
     Engine::CTransform* m_pTransformCom;//Ʈ������ �İ�ü
     Engine::CTexture* m_pTextureCom;//�ؽ�ó �İ�ü
     Engine::CTexture* m_pGibTextureCom;//���� �İ�ü

     //�θ�ü�� ����
     _float m_fSize;//��ƼŬ ũ��
     _vec3 m_vecOrigin;//��ƼŬ ���� ��
     _int numParticles;//��ƼŬ ���� ���� ����
     _uint m_uiLive;//����ִ� ��ƼŬ ����
     _uint m_GibComID;
     _uint m_GibTexSize;
     std::random_device rd;
     _bool m_bPuddle;
public:
    static CFirework* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles,_uint _GibComID);

protected:
    virtual void Free() override;

};

