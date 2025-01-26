#include "stdafx.h"
#include "GameData.h"

#include "Export_System.h"

#include "MeshGroup.h"
#include "Yeti.h"
#include "Looker.h"
#include "Flamer.h"
#include "Item.h"
#include "Toggle.h"
#include "Door.h"
#include "Box.h"
#include "Thorn.h"
#include "Item.h"

CGameData* CGameData::m_pInstance = nullptr;
CGameData* CGameData::GetInstance(void) {
	if (m_pInstance == nullptr)
	{
		MSG_BOX("Not Created GameData");
	}
	return m_pInstance;
}

void CGameData::DestroyInstance(void) {

	if (nullptr != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CGameData::CGameData(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameData::~CGameData()
{
	Free();
}

void CGameData::Free()
{
	Safe_Release(m_pGraphicDev);
	
	for (int i = 0; i < m_pFbxLoaders.size(); ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			Safe_Release(m_pFbxLoaders[i][j]);
		}
	}
	for (int i = 0; i < m_pMeshGroups.size(); ++i)
	{
		if (m_pMeshGroups[i]->Is_Clone())
			continue;

		Safe_Release(m_pMeshGroups[i]);
	}

	for (int i = 0; i < m_mapObjTotalGroups.size(); ++i)
	{
		if (m_bIsCloneTotalGroups[i])
			continue;

		for_each(m_mapObjTotalGroups[i].begin(), m_mapObjTotalGroups[i].end(), CDeleteMap());
	}

	for (int i = 0; i < m_vecObjGroups.size(); ++i)
	{
		for (int j = 0; j < m_vecObjGroups[i].size(); ++j)
		{
			if (m_bIsCloneObjGroups[i][j])
				continue;

			for_each(m_vecObjGroups[i][j].begin(), m_vecObjGroups[i][j].end(), CDeleteMap());
		}
	}

	for (int i = 0; i < m_pLineMgrs.size(); ++i)
	{
		Safe_Release(m_pLineMgrs[i]);
	}
}

void CGameData::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_pInstance != nullptr)
	{
		MSG_BOX("GameData is Already Created");
		return;
	}

	m_pInstance = DBG_NEW CGameData(pGraphicDev);
}

void CGameData::Set_Map_Option(wstring txtFile, wstring pMeshPath, wstring pDMeshPath)
{
	LoadFbx(pMeshPath, pDMeshPath);
	LoadData(txtFile);
	PushLineMgr(1.f, iTotalIndexSoFar);

	//TODO:mapobjGroups삭제 후 고칠것
	m_bIsCloneTotalGroups.resize(m_mapObjTotalGroups.size(), false);
	m_bIsCloneObjGroups.resize(iTotalIndexSoFar + 1, vector<bool>(SPAWN_END, false));
	iTotalIndexSoFar++;
}

void CGameData::Activate_LineCollision(int index)
{
	if (index > iTotalIndexSoFar)
	{
		MSG_BOX("Invalid Index");
		return;
	}
	m_pLineMgrs[index]->Push_Line_Collision();
}

void CGameData::Clear_LineCollision(int index)
{
	if (index >= m_pLineMgrs.size())
	{
		MSG_BOX("index is too large <- CGameData");
		return;
	}

	m_pLineMgrs[index]->Remove_All_Line_Component();
}

_uint CGameData::PushLineMgr(float terrainHeight, int index)
{
	std::vector<FbxMeshInfo> vecMeshGroup = m_pFbxLoaders[index][0]->GetMeshVector();
	m_FloorIdx.resize(iTotalIndexSoFar + 1);

	map<std::pair<D3DXVECTOR3, D3DXVECTOR3>, bool> mapEdge;
	constexpr float EPSILON = 0.1f;


	pair<D3DXVECTOR3, D3DXVECTOR3> tmpPair;
	auto func = [&tmpPair, &EPSILON](pair<pair<D3DXVECTOR3, D3DXVECTOR3>, bool> pair2)
		{
			if (D3DXVec3LengthSq(&(tmpPair.first - pair2.first.first)) < EPSILON &&
				D3DXVec3LengthSq(&(tmpPair.second - pair2.first.second)) < EPSILON
				)
			{
				return true;
			}
			else if (D3DXVec3LengthSq(&(tmpPair.second - pair2.first.first)) < EPSILON &&
				D3DXVec3LengthSq(&(tmpPair.first - pair2.first.second)) < EPSILON
				)
			{
				return true;
			}
			else return false;

		};

	static int MeshIdx = 0;
	
	for (auto& MeshInfo : vecMeshGroup)
	{
		vector<bool> triIsFloor(MeshInfo.indices.size(),false);
		int cnt = 0;
		for (auto& indices : MeshInfo.indices)
		{
			VTXTEX vtx0 = MeshInfo.vertices[indices._0];
			VTXTEX vtx1 = MeshInfo.vertices[indices._1];
			VTXTEX vtx2 = MeshInfo.vertices[indices._2];

			if (vtx0.vPosition.y < terrainHeight - 0.5 || vtx0.vPosition.y > terrainHeight + 0.5 ||
				vtx1.vPosition.y < terrainHeight - 0.5 || vtx1.vPosition.y > terrainHeight + 0.5 ||
				vtx2.vPosition.y < terrainHeight - 0.5 || vtx2.vPosition.y > terrainHeight + 0.5)
				continue;

			triIsFloor[cnt] = true;
			cnt++;
			tmpPair = pair<D3DXVECTOR3, D3DXVECTOR3>(vtx1.vPosition, vtx0.vPosition);

			auto& iter = find_if(mapEdge.begin(), mapEdge.end(), func);
			if (mapEdge.end() == iter)
			{
				mapEdge.insert({ tmpPair,false });
			}
			else
			{
				if (iter->second == false)
				{
					iter->second = true;
				}
			}

			tmpPair = pair<D3DXVECTOR3, D3DXVECTOR3>(vtx2.vPosition, vtx1.vPosition);

			iter = find_if(mapEdge.begin(), mapEdge.end(), func);
			if (mapEdge.end() == iter)
			{
				mapEdge.insert({ tmpPair,false });
			}
			else
			{
				if (iter->second == false)
				{
					iter->second = true;
				}
			}
			tmpPair = pair<D3DXVECTOR3, D3DXVECTOR3>(vtx0.vPosition, vtx2.vPosition);
			iter = find_if(mapEdge.begin(), mapEdge.end(), func);
			if (mapEdge.end() == iter)
			{
				mapEdge.insert({ tmpPair,false });
			}
			else
			{
				if (iter->second == false)
				{
					iter->second = true;
				}
			}
		}

		int notPlaneCnt = std::count(triIsFloor.begin(), triIsFloor.end(), false);

		if (notPlaneCnt == 0)
		{
			m_FloorIdx[iTotalIndexSoFar].push_back(MeshIdx);
		}

		MeshIdx++;
	}


	for (auto& iter = mapEdge.begin(); iter != mapEdge.end();)
	{
		if (iter->second == true)
		{
			iter = mapEdge.erase(iter);
		}
		else
		{
			++iter;
		}
	}


	//맵0 예외처리 해야함
	if (index == 0)
	{
		tmpPair = pair<D3DXVECTOR3, D3DXVECTOR3>(_vec3{ -8.f, 1.f, 10.f }, _vec3{ -3.f,1.f, 10.f });
		mapEdge.insert({ tmpPair,false });
	}

	const _tchar* szProtoName = L"Proto_Line%d";

	int i = 0;
	for (auto& iter : mapEdge)
	{
		TCHAR	szFileName[128] = L"";

		int idx = i + m_uCurLineNum;
		wsprintf(szFileName, szProtoName, idx);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(szFileName), CLine::Create(m_pGraphicDev, iter.first)), E_FAIL);
		i++;
	}

	m_uCurLineNum += mapEdge.size();


	vector<unsigned int> vlineGroup;
	vlineGroup.push_back(mapEdge.size());
	Engine::CLineMgr*  pLineMgr = Engine::Create_LineMgr(m_pGraphicDev, vlineGroup);
	m_pLineMgrs.emplace_back(pLineMgr);
	return S_OK;
}



HRESULT CGameData::LoadData(wstring txtFile)
{
	multimap<std::wstring, CGameObject*> mapObj;
	wifstream		fin;
	fin.open(txtFile);

	if (!fin.fail())
	{

		TCHAR sztexGroupSize[256] = L"";

		fin.getline(sztexGroupSize, MAX_PATH);
		int texGroupSize = stoi(sztexGroupSize);

		for (int i = 0; i < texGroupSize; ++i)
		{
			Safe_Release((*(m_pMeshGroups.back()->Get_TextureGroup()))[i]);
		}
		m_pMeshGroups.back()->Reserve_vecTexIdx(texGroupSize);
		for (int i = 0; i < texGroupSize; ++i)
		{
			TCHAR szName[256] = L"";
			fin.getline(szName, 256, '|');

			if (wcsncmp(szName, L"Proto", 5) != 0)
			{
				MSG_BOX("GameData Load Failed...");
				return E_FAIL;
			}

			TCHAR	szTileNum[256] = L"";
			fin.getline(szTileNum, 256);
			int tileNum = stoi(szTileNum);

			CTexture* pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(szName));
			m_pMeshGroups.back()->Set_TexIdx(i, tileNum);
			(*(m_pMeshGroups.back()->Get_TextureGroup()))[i] = pTextureCom;
		}

		TCHAR szDTexGroupSize[256] = L"";

		fin.getline(szDTexGroupSize, MAX_PATH);
		int dTexGroupSize = stoi(szDTexGroupSize);

		m_pMeshGroups.back()->Reserve_vecTexIdx(texGroupSize+ dTexGroupSize);

		for (int i = texGroupSize; i < texGroupSize + dTexGroupSize; ++i)
		{
			Safe_Release((*(m_pMeshGroups.back()->Get_TextureGroup()))[i]);
		}

		for (int i = texGroupSize; i < texGroupSize + dTexGroupSize; ++i)
		{
			TCHAR szName[256] = L"";
			fin.getline(szName, 256, '|');

			if (wcsncmp(szName, L"Proto", 5) != 0)
			{
				MSG_BOX("GameData Load Failed...");
				return E_FAIL; 
			}

			TCHAR	szTileNum[256] = L"";
			fin.getline(szTileNum, 256);
			int tileNum = stoi(szTileNum);

			CTexture* pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(szName));
			m_pMeshGroups.back()->Set_TexIdx(i, tileNum);
			(*(m_pMeshGroups.back()->Get_TextureGroup()))[i] = pTextureCom;
		}

		TCHAR szObjGroupSize[256] = L"";
		vector<multimap<std::wstring, CGameObject*>> tmpVecObjGroups(SPAWN_END);

		fin.getline(szObjGroupSize, MAX_PATH);
		int objGroupSize = stoi(szObjGroupSize);

		for (int i = 0; i < objGroupSize; ++i)
		{
			TCHAR szName[256] = L"";

			fin.getline(szName, 256, '|');

			if (wcsncmp(szName, L"Yeti", 3) == 0 || wcsncmp(szName, L"Looker", 3) == 0 || wcsncmp(szName, L"Flamer", 3) == 0)
			{
				TCHAR	vx[256] = L"";
				TCHAR	vy[256] = L"";
				TCHAR	vz[256] = L"";
				TCHAR	szSpawnGroup[256] = L"";
				int		iSpawnGroup(0);
				_vec3	pos(0, 0, 0);
				CGameObject* pGameObj = nullptr;

				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szSpawnGroup, 256);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				iSpawnGroup = stoi(szSpawnGroup);

				if (wcsncmp(szName, L"Yeti", 3) == 0)
					pGameObj = CYeti::Create(m_pGraphicDev, pos);
				else if (wcsncmp(szName, L"Looker", 3) == 0)
					pGameObj = CLooker::Create(m_pGraphicDev, pos);
				else if (wcsncmp(szName, L"Flamer", 3) == 0)
					pGameObj = CFlamer::Create(m_pGraphicDev, pos);


				//TODO: enumType 별로 분기해서 각 그룹에 넣어줄 것
				switch (iSpawnGroup)
				{
				case SPAWN0:
					tmpVecObjGroups[SPAWN0].insert({ szName,pGameObj });
					break;
				case SPAWN1:
					tmpVecObjGroups[SPAWN1].insert({ szName,pGameObj });
					break;
				case SPAWN2:
					tmpVecObjGroups[SPAWN2].insert({ szName,pGameObj });
					break;
				case SPAWN3:
					tmpVecObjGroups[SPAWN3].insert({ szName,pGameObj });
					break;
				case SPAWN4:
					tmpVecObjGroups[SPAWN4].insert({ szName,pGameObj });
					break;
				case SPAWN5:
					tmpVecObjGroups[SPAWN5].insert({ szName,pGameObj });
					break;
				case SPAWN6:
					tmpVecObjGroups[SPAWN6].insert({ szName,pGameObj });
					break;
				case SPAWN7:
					tmpVecObjGroups[SPAWN7].insert({ szName,pGameObj });
					break;
				case SPAWN8:
					tmpVecObjGroups[SPAWN8].insert({ szName,pGameObj });
					break;
				case SPAWN9:
					tmpVecObjGroups[SPAWN9].insert({ szName,pGameObj });
					break;
				case SPAWN10:
					tmpVecObjGroups[SPAWN10].insert({ szName,pGameObj });
					break;
				case SPAWN11:
					tmpVecObjGroups[SPAWN11].insert({ szName,pGameObj });
					break;
				case SPAWN12:
					tmpVecObjGroups[SPAWN12].insert({ szName,pGameObj });
					break;
				default:
					MSG_BOX("invalid spawn group");
					break;
				}

				//mapObj.insert({ szName,pGameObj });
			}
			else if (wcsncmp(szName, L"TOG_NORMAL", 3) == 0 || wcsncmp(szName, L"TOG_RED", 3) == 0 || wcsncmp(szName, L"TOG_YELLOW", 3) == 0 || wcsncmp(szName, L"TOG_BLUE", 3) == 0
				|| wcsncmp(szName, L"DOOR_GREY", 3) == 0 || wcsncmp(szName, L"DOOR_RED", 3) == 0 || wcsncmp(szName, L"DOOR_YELLOW", 3) == 0 || wcsncmp(szName, L"DOOR_BLUE", 3) == 0)
			{
				TCHAR	szObjKey[256] = L"";
				int		objKey = 0;
				_vec3	pos(0, 0, 0);
				TCHAR	vx[256] = L"";
				TCHAR	vy[256] = L"";
				TCHAR	vz[256] = L"";
				TCHAR	roty[256] = L"";
				float	Roty = 0;
				TCHAR	szSpawnGroup[256] = L"";
				int		iSpawnGroup(0);
				CGameObject* pGameObj = nullptr;

				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(roty, 256, '|');
				fin.getline(szObjKey, 256, '|');
				fin.getline(szSpawnGroup, 256);

				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				Roty = stof(roty);
				objKey = stoi(szObjKey);
				iSpawnGroup = stoi(szSpawnGroup);


				//TODO:DOOR일때 생성자 하나 더 만들어서 door별로 열릴때 spwan그룹에 있는 몬스터 소환하게 하기 99일때는 아무 동작 안하게끔
				if (wcsncmp(szName, L"TOG", 3) == 0)
					pGameObj = CToggle::Create(m_pGraphicDev, (TOGGLEID)objKey, pos, D3DXToRadian(Roty));
				else if (wcsncmp(szName, L"DOOR", 3) == 0)
					pGameObj = CDoor::Create(m_pGraphicDev, (DOORID)objKey ,pos, D3DXToRadian(Roty), (SpawnGroup)iSpawnGroup);

				mapObj.insert({ szName,pGameObj });
			}
			else if (wcsncmp(szName, L"BOX", 3) == 0 || wcsncmp(szName, L"PILLAR", 3) == 0 || wcsncmp(szName, L"Thorn", 3) == 0)
			{
				TCHAR	szObjKey[256] = L"";
				int		objKey = 0;
				_vec3	pos(0, 0, 0);
				TCHAR	vx[256] = L"";
				TCHAR	vy[256] = L"";
				TCHAR	vz[256] = L"";
				TCHAR	roty[256] = L"";
				float	Roty = 0;
				CGameObject* pGameObj = nullptr;

				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szObjKey, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				objKey = stoi(szObjKey);

				if (objKey < 3)
				{
					pGameObj = CBox::Create(m_pGraphicDev, (BOXID)objKey);
				}
				else if (objKey == 3)
				{
					pGameObj = CThorn::Create(m_pGraphicDev);
				}


				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				pGameObj->Set_PrePos(pos);
				mapObj.insert({ szName,pGameObj });
			}
			else if (wcsncmp(szName, L"BULLET", 3) == 0 || wcsncmp(szName, L"EXP", 3) == 0 || wcsncmp(szName, L"HEALTH", 3) == 0 || wcsncmp(szName, L"MANA", 3) == 0 || wcsncmp(szName, L"KEY", 3) == 0)
			{
				TCHAR	szObjKey[256] = L"";
				int		objKey = 0;
				_vec3	pos(0, 0, 0);
				TCHAR	vx[256] = L"";
				TCHAR	vy[256] = L"";
				TCHAR	vz[256] = L"";
				TCHAR	szItemNum[256] = L"";
				int itemNum = 0;
				CGameObject* pGameObj = nullptr;

				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szObjKey, 256, '|');
				fin.getline(szItemNum, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				objKey = stoi(szObjKey);
				itemNum = stoi(szItemNum);
				pGameObj = CItem::Create(m_pGraphicDev, pos,(ITEMTYPE)objKey, itemNum);
				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				pGameObj->Set_PrePos(pos);
				mapObj.insert({ szName,pGameObj });
			}
		}

		fin.close();

		m_mapObjTotalGroups.emplace_back(mapObj);
		m_vecObjGroups.emplace_back(tmpVecObjGroups);
	}
}

HRESULT CGameData::LoadFbx(std::wstring pPath, std::wstring pDPath)
{
	std::array<Engine::CFBXLoader*, 2> pFbxloader{ nullptr, nullptr };
	pFbxloader[0] = new Engine::CFBXLoader();
	pFbxloader[0]->LoadFbx(pPath);

	const _tchar* szProtoName = L"Proto_Mesh%d";

	for (_uint i = 0; i < pFbxloader[0]->GetMeshVector().size(); ++i)
	{
		TCHAR	szFileName[128] = L"";
		int idx = i + m_uCurMeshNum;

		wsprintf(szFileName, szProtoName, idx);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(szFileName), CMesh::Create(m_pGraphicDev, pFbxloader[0]->GetMesh(i))), E_FAIL);
	}

	m_uCurMeshNum += pFbxloader[0]->GetMeshVector().size();

	pFbxloader[1] = new Engine::CFBXLoader();
	pFbxloader[1]->LoadFbx(pDPath);

	const _tchar* szProtoDName = L"Proto_DMesh%d";

	for (_uint i = 0; i < pFbxloader[1]->GetMeshVector().size(); ++i)
	{
		TCHAR	szFileName[128] = L"";

		int idx = i + m_uCurDMeshNum;
		wsprintf(szFileName, szProtoDName, idx);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(szFileName), CMesh::Create(m_pGraphicDev, pFbxloader[1]->GetMesh(i))), E_FAIL);
	}

	m_uCurDMeshNum += pFbxloader[1]->GetMeshVector().size();

	CMeshGroup* pMeshGroup = nullptr;
	pMeshGroup = CMeshGroup::Create(m_pGraphicDev, pFbxloader[0]->GetMeshVector(), pFbxloader[1]->GetMeshVector());
	NULL_CHECK_RETURN(pMeshGroup, E_FAIL);



	m_pMeshGroups.emplace_back(pMeshGroup);
	m_pFbxLoaders.emplace_back(pFbxloader);
}
