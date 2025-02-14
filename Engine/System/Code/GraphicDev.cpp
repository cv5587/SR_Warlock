#include "..\..\Header\GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev() : m_pSDK(nullptr), m_pGraphicDev(nullptr)
{
}

CGraphicDev::~CGraphicDev()
{
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND hWnd, 
										WINMODE eMode, 
										const _ulong & dwSizeX, 
										const _ulong & dwSizeY, 
										CGraphicDev ** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_RETURN(m_pSDK, E_FAIL);

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	FAILED_CHECK_RETURN(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps), E_FAIL);

	_ulong		dwFlag(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		
	D3DPRESENT_PARAMETERS			d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = dwSizeX;
	d3dpp.BackBufferHeight = dwSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	FAILED_CHECK_RETURN(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev), E_FAIL);

	*ppGraphicDev = this;
	
	return S_OK;
}

void CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, Color, 1.f, 0);
	m_pGraphicDev->BeginScene();
}

void CGraphicDev::Render_End(void)
{
	m_pGraphicDev->EndScene();
	if (FAILED(m_pGraphicDev->Present(NULL, NULL, NULL, NULL)))
		MSG_BOX("Lost m_pGraphicDev ");
}

void CGraphicDev::Free()
{
	_ulong	dwRefCnt(0);

	if (dwRefCnt = Safe_Release(m_pGraphicDev))
		MSG_BOX("m_pGraphicDev Release Failed");
	
	Safe_Release(m_pSDK);
}
