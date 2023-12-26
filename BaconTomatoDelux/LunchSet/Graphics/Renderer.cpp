#include "pch.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include "FixedCB.h"
#include "PerFrameCB.h"
#include "PerObjectCB.h"

Renderer::Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: m_Device(device), m_DeviceContext(deviceContext)
{
}

Renderer::~Renderer()
{
}

void Renderer::SetFixed(const FixedSettings& settings)
{
	if (settings.vsFixed)
	{
		static ConstantBuffer<VSFixedData> vf{ m_Device, m_DeviceContext };
		vf.Update(settings.vsFixed);
		SetConstantBuffer<VSFixedData>(vf);
	}
}

void Renderer::SetPerFrame(const FrameSettings& settings)
{
	// Constant Buffers
	// VS Camera Data
	if (settings.vsCamera)
	{
		static ConstantBuffer<VSCameraData> vc{ m_Device, m_DeviceContext };
		vc.Update(settings.vsCamera);
		SetConstantBuffer<VSCameraData>(vc);
	}

	// PS Camera Data
	if (settings.psCamera)
	{
		static ConstantBuffer<PSCameraData> pc{ m_Device, m_DeviceContext };
		pc.Update(settings.psCamera);
		SetConstantBuffer<PSCameraData>(pc);
	}

	// Light Data
	if (settings.light)
	{
		static ConstantBuffer<PSLightData> ld{ m_Device, m_DeviceContext };
		ld.Update(settings.light);
		SetConstantBuffer<PSLightData>(ld);
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
	if (settings.textures)
	{
		const std::array<std::shared_ptr<Texture>, btdTextureType_END> ts = *settings.textures;

		// Material Data
		static ConstantBuffer<PsMaterialData> md{ m_Device, m_DeviceContext };
		// todo : ��� ������ ����
		PsMaterialData dataStruct{ 0, 20.0f, 2.0f };


		for (uint32_t texType = btdTextureType_DIFFUSE; texType < btdTextureType_END; texType++)
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
		static ConstantBuffer<VSObjectData> td{ m_Device, m_DeviceContext };

		// todo : �θ� ���ӿ�����Ʈ�� �ѹ��� �ϰ� ����
		td.Update(settings.transform);
		SetConstantBuffer(td);
	}
}

void Renderer::SetMatrixPallete(const MatrixPalleteSettings& settings)
{
	static ConstantBuffer<VsMatrixPallete> mp{ m_Device, m_DeviceContext };

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

void Renderer::AddRenderQueue(const RenderQueueSettings& settings)
{
	m_RenderQueue.push_back(std::move(settings));
}

void Renderer::SortRenderQueue()
{
	std::sort(m_RenderQueue.begin(), m_RenderQueue.end(),
		[](const RenderQueueSettings& lhs, const RenderQueueSettings& rhs)
		{
			return lhs.textures < rhs.textures;
		});
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
