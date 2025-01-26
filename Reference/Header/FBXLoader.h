#pragma once
#include "GameObject.h"
#include "Engine_Define.h"
#include "Mesh.h"
#include "fbxsdk.h"

BEGIN(Engine)

class ENGINE_DLL CFBXLoader : public CBase
{
public:
	explicit CFBXLoader();
	virtual ~CFBXLoader();

public:

	HRESULT LoadFbx(std::wstring pPath);
	void DrawMesh();

public:
	_int GetMeshCount() { return static_cast<_int>(m_vMesh.size()); }
	const FbxMeshInfo& GetMesh(_int idx) { return m_vMesh[idx]; }
	const std::vector<FbxMeshInfo>& GetMeshVector() { return m_vMesh; }
	std::vector<INDEX32> GetTriangleVector() { return m_vTriangle; }

private:
	void Import(const wstring& path);

	void ParseNode(FbxNode* node);
	void LoadMesh(FbxMesh* mesh);

	void GetNormal(FbxMesh* mesh, FbxMeshInfo* container, _int idx, _int vertexCounter);
	void GetUV(FbxMesh* mesh, FbxMeshInfo* container, _int idx, _int vertexCounter);


private:
	virtual void		Free();

private:
	std::vector<FbxMeshInfo> m_vMesh;

	FbxManager* m_pManager = nullptr;
	FbxScene* m_pScene = nullptr;
	FbxImporter* m_pImporter = nullptr;
	std::vector<INDEX32> m_vTriangle;


};

END