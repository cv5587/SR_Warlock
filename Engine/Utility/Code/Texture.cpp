#include "..\..\Header\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize * 2);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTUREID eType,wstring pPath, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9* pTexture = NULL;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR	szFileName[128] = L"";

		wsprintf(szFileName, pPath.c_str(), i);

		switch (eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

IDirect3DBaseTexture9* CTexture::Get_Texture(const _uint& iIndex)
{
	return m_vecTexture[iIndex];
}

void CTexture::Set_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

CComponent* CTexture::Clone()
{
	return DBG_NEW CTexture(*this);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType,wstring pPath, const _uint& iCnt)
{
	CTexture* pInstance = DBG_NEW CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eType, pPath, iCnt)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
}
