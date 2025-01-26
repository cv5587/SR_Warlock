#pragma once
#include "GameObject.h"

#include "Mesh.h"


BEGIN(Engine)



class ENGINE_DLL CMeshLoader : public CBase
{
	DECLARE_SINGLETON(CMeshLoader)


private:
	explicit CMeshLoader();
	virtual ~CMeshLoader();

public:

	HRESULT LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev, std::string pPath);
	void DrawMesh();

private:
	//void processNode(aiNode* node, const aiScene* scene);
	//CMesh processMesh(aiMesh* mesh, const aiScene* scene);



private:
	virtual void		Free();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	std::vector<CMesh> m_vMesh;



};

END