#pragma once

struct VertexColor
{
	Vector4 position;
	Vector4 color;

	// for Input Layout
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

	VertexColor(Vector4 pos, Vector4 color)
		: position(pos), color(color) {}

	VertexColor(Vector4 pos)
		: position(pos), color(1.0f, 1.0f, 1.0f, 1.0f) {}
};

struct VertexTexture
{
	Vector4 position;
	Vector2 uv;

	// for Input Layout
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

	VertexTexture(Vector3 pos, Vector2 uv)
		: position(pos), uv(uv) {}
};

struct StaticVertex
{
	Vector4 position;
	Vector3 normal;
	Vector3 tangent;
	Vector2 uv;

	// for Input Layout
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
};


struct BoneVertex
{
	Vector4 position;
	Vector3 normal;
	Vector3 tangent;
	Vector2 uv;

	int BlendIndices[4] = {};			// 영향 받을 뼈의 인덱스 (최대 4)
	float BlendWeight[4] = {};			// 영향 받을 뼈의 가중치 (합 : 1)

	// for Input Layout
	const static std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

	void AddBoneData(int boneIndex, float weight)
	{
		assert(BlendWeight[0] == 0.0f or BlendWeight[1] == 0.0f or BlendWeight[2] == 0.0f or BlendWeight[3] == 0.0f);

		for(size_t i = 0; i < 4; i++)
		{
			if(BlendWeight[i] == 0.0f)
			{
				BlendIndices[i] = boneIndex;
				BlendWeight[i] = weight;
				return;
			}
		}
	}
};