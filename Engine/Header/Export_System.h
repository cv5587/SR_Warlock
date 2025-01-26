#pragma once

#include "Engine_Define.h"
#include "Engine_Enum.h"
#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "TimerMgr.h"
#include "FontMgr.h"
#include "SoundMgr.h"
#include "InputDev.h"

BEGIN(Engine)

// GraphicDev

inline HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _ulong& dwSizeX, const _ulong& dwSizeY, CGraphicDev** ppGraphicDev);
inline void			Render_Begin(D3DXCOLOR Color);
inline void			Render_End(void);

// TimerMgr

inline _float		Get_TimeDelta(wstring pTimerTag);
inline void		Update_TimeDelta(wstring pTimerTag);
inline HRESULT		Ready_Timer(wstring pTimerTag);

// FrameMgr

inline _bool			IsPermit_Call(wstring pFrameTag, const _float& fTimeDelta);
inline HRESULT			Ready_Frame(wstring pFrameTag, const _float& fCallLimit);

// FontMgr
inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	wstring pFontType,
	wstring pFontTag,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void		Render_Font(wstring pFontTag,
	wstring pString,
	const _vec2* pPos,
	D3DXCOLOR Color);

//SoundMgr
inline CSoundMgr*		Get_SoundMgr();	//SoundMgr 가져가서 사용하면 됨
inline void		Ready_SoundMgr();


// InputDev

inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void		Update_InputDev(void);

// Destroy
inline void			Release_System();


#include "Export_System.inl"

END