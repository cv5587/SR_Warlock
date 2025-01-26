#include "..\..\Header\MeshLoader.h"
#include "..\..\Header\Mesh.h"

IMPLEMENT_SINGLETON(CMeshLoader)

CMeshLoader::CMeshLoader() : m_pGraphicDev(nullptr)
{
}

CMeshLoader::~CMeshLoader()
{
	Free();
}

HRESULT CMeshLoader::LoadMesh(LPDIRECT3DDEVICE9 pGraphicDev,std::string pPath)
{
	Assimp::Importer importer;

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	const aiScene* pScene = importer.ReadFile(pPath,
		aiProcess_Triangulate |
		aiProcess_OptimizeMeshes|
		aiProcess_ConvertToLeftHanded|
		aiProcess_TransformUVCoords |
		aiProcess_JoinIdenticalVertices|
		aiProcess_PreTransformVertices);

	if (pScene == nullptr)
		return E_FAIL;

	processNode(pScene->mRootNode, pScene);

	return S_OK;
}

void CMeshLoader::DrawMesh()
{
	for (size_t i = 0; i < m_vMesh.size(); ++i) {
		m_vMesh[i].Render_Buffer();
	}
}

void CMeshLoader::processNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_vMesh.push_back(processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

CMesh CMeshLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	std::vector<VTXTEX> vertices;
	std::vector<INDEX32> indices;

	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		VTXTEX vertex;

		vertex.vPosition.x = mesh->mVertices[i].x;
		vertex.vPosition.y = mesh->mVertices[i].y;
		vertex.vPosition.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.vTexUV.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.vTexUV.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertex.vNormal = { 0.f,0.f,0.f };

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		_uint j = 0;
		INDEX32 tmpIdx;
		while (j < face.mNumIndices)
		{
			if (j % 3 == 0)
			{
				tmpIdx = {};
				tmpIdx._0 = face.mIndices[j];
				j++;
			}
			else if(j % 3 == 1)
			{
				tmpIdx._1 = face.mIndices[j];
				j++;
			}
			else
			{
				tmpIdx._2 = face.mIndices[j];
				j++;
			}

			if (j % 3 == 0)
			{
				indices.push_back(tmpIdx);
			}
		}
	}

	//if (mesh->mMaterialIndex >= 0) {
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//	std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//}

	CMesh* tmpMesh = CMesh::Create(m_pGraphicDev, std::move(vertices), std::move(indices));

	return Engine::CMesh(*tmpMesh);
}

void CMeshLoader::Free()
{
	Safe_Release(m_pGraphicDev);
}
