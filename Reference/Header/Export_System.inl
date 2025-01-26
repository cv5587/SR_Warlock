#include "Export_System.h"
// GraphicDev

HRESULT			Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, dwSizeX, dwSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

_float		Get_TimeDelta(wstring pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void		Update_TimeDelta(wstring pTimerTag)
{
	CTimerMgr::GetInstance()->Update_TimeDelta(pTimerTag);
}
HRESULT		Ready_Timer(wstring pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameMgr

_bool			IsPermit_Call(wstring pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT			Ready_Frame(wstring pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// FontMgr
HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	wstring pFontType,
	wstring pFontTag,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontType, pFontTag, iWidth, iHeight, iWeight);
}

void		Render_Font(wstring pFontTag,
	wstring pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

inline CSoundMgr* Get_SoundMgr()
{
	return CSoundMgr::GetInstance();
}

inline void Ready_SoundMgr()
{
	CSoundMgr::GetInstance()->Ready_Sound();
}


_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}
_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}
HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}
void		Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}

// Destroy
void			Release_System()
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}