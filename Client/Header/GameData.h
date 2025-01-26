#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Export_Utility.h"

class CMeshGroup;

class CGameData : public CBase
{

	DECLARE_SINGLETON(CGameData)

private:
	explicit CGameData(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameData();

private:
	virtual void	Free();

public:
	static void	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	   Set_Map_Option(wstring txtFile, wstring pMeshPath, wstring pDMeshPath);
	
	void	   Activate_LineCollision(int index);

	void	   Clear_LineCollision(int index);

	const multimap<std::wstring, CGameObject*>& Get_TotalGroup(int index) 
	{ 
		if (index >= m_pMeshGroups.size())
		{
			MSG_BOX("Invalid index");
		}
		return m_mapObjTotalGroups[index]; 
	}

	const multimap<std::wstring, CGameObject*>& Get_ObjGroup(int firstIdx,int secondIdx)
	{
		if (firstIdx >= m_vecObjGroups.size())
		{
			MSG_BOX("Invalid first index");
		}

		if (secondIdx >= m_vecObjGroups[firstIdx].size())
		{
			MSG_BOX("Invalid second index");
		}

		return m_vecObjGroups[firstIdx][secondIdx];
	}

	CMeshGroup* Get_MeshGroup(int index) 
	{
		if (index >= m_pMeshGroups.size())
		{
			MSG_BOX("Invalid index");
		}
		return m_pMeshGroups[index]; 
	}


	void Set_IsCloneTotalGroups(int index) 
	{ 
		if (index < m_bIsCloneTotalGroups.size())
			m_bIsCloneTotalGroups[index] = true; 
	}
	
	void Set_ActiveObjGroup(int firstIdx, int secondIdx)
	{
		if (firstIdx < m_bIsCloneObjGroups.size() &&  secondIdx < m_bIsCloneObjGroups[firstIdx].size())
			m_bIsCloneObjGroups[firstIdx][secondIdx] = true;
	}
	bool IsActiveObjGroup(int firstIdx, int secondIdx)
	{
		if (firstIdx < m_bIsCloneObjGroups.size() && secondIdx < m_bIsCloneObjGroups[firstIdx].size())
			return m_bIsCloneObjGroups[firstIdx][secondIdx];
	}

	vector<int>	Get_FloorMeshIndices(int iSceneindex) { return m_FloorIdx[iSceneindex]; }


private:
	HRESULT LoadData(wstring txtFile);
	HRESULT LoadFbx(std::wstring pPath, std::wstring pDPath);

	_uint	PushLineMgr(float terrainHeight, int index);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
private:
	vector<std::array<Engine::CFBXLoader*, 2>> m_pFbxLoaders;
	vector<CMeshGroup*> m_pMeshGroups;
	vector<multimap<std::wstring, CGameObject*>> m_mapObjTotalGroups;
	vector<bool> m_bIsCloneTotalGroups;
	vector<Engine::CLineMgr*> m_pLineMgrs;


	vector<vector<int>>	m_FloorIdx;

	vector<vector<multimap<std::wstring, CGameObject*>>> m_vecObjGroups;
	vector<vector<bool>> m_bIsCloneObjGroups;
	unsigned int m_uCurMeshNum = 0;
	unsigned int m_uCurDMeshNum = 0;
	unsigned int m_uCurLineNum = 0;

	unsigned int iTotalIndexSoFar = 0;

};

