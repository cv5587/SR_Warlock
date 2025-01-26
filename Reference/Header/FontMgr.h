#pragma once

#include "Base.h"
#include "MyFont.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		wstring pFontType,
		wstring pFontTag,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void		Render_Font(wstring pFontTag,
		wstring pString,
		const _vec2* pPos,
		D3DXCOLOR Color);

private:
	map<wstring, CMyFont*>			m_mapFont;

private:
	CMyFont*		Find_Font(wstring pFontTag);

public:
	virtual void	Free();

};

END