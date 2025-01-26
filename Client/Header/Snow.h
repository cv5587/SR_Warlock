#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include <random>

BEGIN(Engine)
class CTransform;
class CTexture;
class CParticleBuffer;
END

class CSnow :public Engine::CGameObject
{
private:
    explicit CSnow(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles, _float _box);
    explicit CSnow(const CSnow& rhs);
    virtual ~CSnow();

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
    DWORD FtoDw(float f) { return *((DWORD*)&f); }
    

 private:
     Engine::CParticleBuffer* m_pBufferCom;//���ؽ� �İ�ü
     Engine::CTexture* m_pTextureCom;//�ؽ�ó �İ�ü
     Engine::CTransform* m_pTransformCom;//Ʈ������ �İ�ü

     //�θ�ü�� ����
     _float m_fSize;//��ƼŬ ũ��
     _vec3 m_vecOrigin;//��ƼŬ ���� ��
     _int numParticles;//��ƼŬ ���� ���� ����
     _vec3 m_vecmin;
     _vec3 m_vecmax;
     std::random_device rd;
public:
    static CSnow* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize, _ulong _numParticles, _float _box);

protected:
    virtual void Free() override;

};

