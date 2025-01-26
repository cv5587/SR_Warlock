#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"




namespace Engine
{

	struct EParticle
	{
		_vec3 _posiotion;
		D3DCOLOR	_color;
		_float _size;
		static const _ulong PFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};
	struct Attribute
	{
		_vec3 _position;
		_vec3 _velocity;
		_vec3 _acceleration;
		_float  _lifeTime;
		_float  _age;
		D3DXCOLOR	_color;
		D3DXCOLOR	_colorFade;
		_uint	_com;
		_uint	_tex;
		_bool	_isAlive;
	};

	typedef struct tagVertexColor
	{
		_vec3		vPosition;	
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;	

	struct FbxMeshInfo
	{
		std::wstring								name;
		std::vector<VTXTEX>						vertices;
		std::vector<INDEX32>			indices;
	};

	struct FbxMeshInfo2
	{
		std::vector<D3DXVECTOR3>	vertices;
		std::vector<INDEX32>		indices;
	};


}
#endif // Engine_Struct_h__
