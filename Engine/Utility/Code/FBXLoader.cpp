#include "FBXLoader.h"
#include "Export_Utility.h"

CFBXLoader::CFBXLoader()
{
}

CFBXLoader::~CFBXLoader()
{
}

HRESULT CFBXLoader::LoadFbx(std::wstring pPath)
{
	// 파일 데이터 로드
	Import(pPath);

	// 로드된 데이터 파싱 (Mesh/Material/Skin)
	ParseNode(m_pScene->GetRootNode());

    return S_OK;
}

void CFBXLoader::DrawMesh()
{
}

void CFBXLoader::Import(const wstring& path)
{
	// FBX SDK 관리자 객체 생성
	m_pManager = FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	FbxIOSettings* settings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(settings);

	// FbxImporter 객체 생성
	m_pScene = FbxScene::Create(m_pManager, "");

	m_pImporter = FbxImporter::Create(m_pManager, "");

	string strPath = ws2s(path);
	m_pImporter->Initialize(strPath.c_str(), -1, m_pImporter->GetIOSettings());

	m_pImporter->Import(m_pScene);

	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다.
	FbxGeometryConverter geometryConverter(m_pManager);
	geometryConverter.Triangulate(m_pScene, true);

	m_pImporter->Destroy();
}

void CFBXLoader::ParseNode(FbxNode* node)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute)
	{
		switch (attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(node->GetMesh());
			break;
		}
	}

	// Tree 구조 재귀 호출
	const _int childCount = node->GetChildCount();
	for (_int i = 0; i < childCount; ++i)
		ParseNode(node->GetChild(i));
}

void CFBXLoader::LoadMesh(FbxMesh* mesh)
{
	m_vMesh.push_back(FbxMeshInfo());
	FbxMeshInfo& meshInfo = m_vMesh.back();

	meshInfo.name = s2ws(mesh->GetName());

	const _int vertexCount = mesh->GetControlPointsCount();
	meshInfo.vertices.resize(vertexCount);

	// Position
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (_int i = 0; i < vertexCount; ++i)
	{
		meshInfo.vertices[i].vPosition.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo.vertices[i].vPosition.y = static_cast<float>(controlPoints[i].mData[1]);
		meshInfo.vertices[i].vPosition.z = static_cast<float>(controlPoints[i].mData[2]);
	}

	const _int polygonSize = mesh->GetPolygonSize(0);
	if (polygonSize != 3)
		MSG_BOX("PolygonSize is not Three");

	_int arrIdx[3];
	_int vertexCounter = 0; // 정점의 개수

	const _int triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다
	for (_int i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (_int j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
		{
			_int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출
			arrIdx[j] = controlPointIndex;

			GetNormal(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetUV(mesh, &meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			vertexCounter++;
		}

		INDEX32 tmpIdx;

		tmpIdx._0 = arrIdx[0];
		tmpIdx._1 = arrIdx[2];
		tmpIdx._2 = arrIdx[1];


		meshInfo.indices.push_back(tmpIdx);
		m_vTriangle.push_back(tmpIdx);
	}
}

void CFBXLoader::GetNormal(FbxMesh* mesh, FbxMeshInfo* container, _int idx, _int vertexCounter)
{
	if (mesh->GetElementNormalCount() == 0)
		return;

	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	_int normalIdx = 0;

	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = idx;
		else
			normalIdx = normal->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);
	container->vertices[idx].vNormal.x = static_cast<float>(vec.mData[0]);
	container->vertices[idx].vNormal.y = static_cast<float>(vec.mData[1]);
	container->vertices[idx].vNormal.z = static_cast<float>(vec.mData[2]);
}

void CFBXLoader::GetUV(FbxMesh* mesh, FbxMeshInfo* meshInfo, _int idx, _int uvIndex)
{
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(uvIndex);
	meshInfo->vertices[idx].vTexUV.x = static_cast<float>(uv.mData[0]);
	meshInfo->vertices[idx].vTexUV.y = 1.f - static_cast<float>(uv.mData[1]);
}

void CFBXLoader::Free()
{
	if (m_pScene)
		m_pScene->Destroy();
	if (m_pManager)
		m_pManager->Destroy();

}
