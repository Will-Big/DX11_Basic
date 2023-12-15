#include "pch.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include "PerFrameCB.h"
#include "PerObjectCB.h"

Renderer::Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: m_Device(device), m_DeviceContext(deviceContext)
{
}

Renderer::~Renderer()
{
}

void Renderer::SetPerFrame(const FrameSettings& settings)
{
	// Constant Buffers
	// Camera Data
	if (settings.camera)
	{
		static ConstantBuffer<CameraData> wd{ m_Device, m_DeviceContext };
		wd.Update(settings.camera);
		SetConstantBuffer<CameraData>(wd);
	}

	// Light Data
	if (settings.light)
	{
		static ConstantBuffer<LightData> ld{ m_Device, m_DeviceContext };
		ld.Update(settings.light);
		SetConstantBuffer<LightData>(ld);
	}
}

void Renderer::SetPerObject(const ObjectSettings& settings)
{
	// Vertex Buffer
	{
		const std::shared_ptr<VertexBuffer>& vb = settings.vertexBuffer;

		if (vb.get() == nullptr)
		{
			LOG_ERROR(L"nullptr : VertexBuffer");
			return;
		}

		uint32_t vbStride = vb->GetStride();
		uint32_t vbOffset = vb->GetOffset();

		m_DeviceContext->IASetVertexBuffers(0, 1, vb->GetComPtr().GetAddressOf(), &vbStride, &vbOffset);
	}

	// Index Buffer
	{
		const std::shared_ptr<IndexBuffer>& ib = settings.indexBuffer;

		if (ib.get() == nullptr)
		{
			LOG_ERROR(L"nullptr : IndexBuffer");
			return;
		}

		m_IndexCount = ib->GetIndexCount();
		m_DeviceContext->IASetIndexBuffer(ib->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	// Textures Mapping
	{
		const std::array<std::shared_ptr<Texture>, btdTextureType_END>& ts = settings.textures;

		// Material Data
		static ConstantBuffer<MaterialData> md{ m_Device, m_DeviceContext };
		// todo : ��� ������ ����
		MaterialData dataStruct {0, 20.0f, 2.0f};


		for(uint32_t texType = btdTextureType_DIFFUSE; texType < btdTextureType_END; texType++)
		{
			if (ts[texType] != nullptr)
			{
				m_DeviceContext->PSSetShaderResources(texType, 1, ts[texType]->GetComPtr().GetAddressOf());

				dataStruct.ShaderScope |= 1 << (texType); 
			}
		}

		md.Update(&dataStruct);
		SetConstantBuffer(md);
	}

	// Constant Buffers
	// Transform Data
	if (settings.transform)
	{
		static ConstantBuffer<TransformData> td{ m_Device, m_DeviceContext };

		// todo : �θ� ���ӿ�����Ʈ�� �ѹ��� �ϰ� ����
		td.Update(settings.transform);
		SetConstantBuffer(td);
	}
}

void Renderer::SetMatrixPallete(const MatrixPalleteSettings& settings)
{
	static ConstantBuffer<MatrixPallete> mp{ m_Device, m_DeviceContext };
	
	mp.Update(settings.pallete);
	SetConstantBuffer(mp);
}

void Renderer::SetInputLayout(std::shared_ptr<InputLayout> layout)
{
	m_DeviceContext->IASetInputLayout(layout->GetInputLayout().Get());
}

void Renderer::Draw()
{
	m_DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
}


/*
�� ������Ʈ�� ��� ���۸� ��� �ִ� ���:
	����:
		�������� �����ǰ� å���� ��Ȯ�մϴ�. �� ������Ʈ�� �ڽ��� ���¸� ������ �����մϴ�.
		�����Ͱ� ����� ���� ��� ���۸� ������Ʈ�ϸ� �ǹǷ� ȿ������ �� �ֽ��ϴ�.
		���� ó���� ĳ�� Ȱ���� �� ������ �� �ֽ��ϴ�.
	����:
		�޸� ��뷮�� �þ �� �ֽ��ϴ�. �� ������Ʈ�� �ڽ��� ��� ���۸� ���� �־�� �ϱ� �����Դϴ�.
		��� ������ ������ �������� �� �ֽ��ϴ�. ���� ������Ʈ ���� ����ȭ�� �ʿ��� �� �ֽ��ϴ�.

������ ������Ʈ���� ��� ���۸� ����� ���:
	����:
		�޸� ȿ������ �����ϴ�. �ʿ��� ���� ��� ���۸� �����ϰ� ������Ʈ�մϴ�.
		������ ������������ �帧�� �ܼ������ϴ�. ��� ������ ���� �����ʹ� ������ ������Ʈ���� ���� �����˴ϴ�.
	����:
		������ ������Ʈ�� ���⼺�� ������ �� �ֽ��ϴ�. ��� ��� �����͸� ó���ؾ� �ϹǷ� ������ �������� �� �ֽ��ϴ�.
		��� ������ ���� �� �Ź� ��� ���۸� ������ϰų� ������Ʈ�ؾ� �ϹǷ�, ���� ���ϰ� �߻��� �� �ֽ��ϴ�.
*/