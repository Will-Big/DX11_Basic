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
		// todo : 상수 데이터 삭제
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

		// todo : 부모 게임오브젝트당 한번만 하게 변경
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
각 컴포넌트가 상수 버퍼를 들고 있는 방식:
	장점:
		데이터의 소유권과 책임이 명확합니다. 각 컴포넌트는 자신의 상태를 스스로 관리합니다.
		데이터가 변경될 때만 상수 버퍼를 업데이트하면 되므로 효율적일 수 있습니다.
		병렬 처리와 캐시 활용이 더 쉬워질 수 있습니다.
	단점:
		메모리 사용량이 늘어날 수 있습니다. 각 컴포넌트가 자신의 상수 버퍼를 갖고 있어야 하기 때문입니다.
		상수 버퍼의 관리가 복잡해질 수 있습니다. 여러 컴포넌트 간의 동기화가 필요할 수 있습니다.

렌더러 컴포넌트에서 상수 버퍼를 만드는 방식:
	장점:
		메모리 효율성이 높습니다. 필요할 때만 상수 버퍼를 생성하고 업데이트합니다.
		렌더링 파이프라인의 흐름이 단순해집니다. 모든 렌더링 관련 데이터는 렌더러 컴포넌트에서 직접 관리됩니다.
	단점:
		렌더러 컴포넌트의 복잡성이 증가할 수 있습니다. 모든 상수 데이터를 처리해야 하므로 로직이 복잡해질 수 있습니다.
		상수 데이터 변경 시 매번 상수 버퍼를 재생성하거나 업데이트해야 하므로, 성능 저하가 발생할 수 있습니다.
*/
