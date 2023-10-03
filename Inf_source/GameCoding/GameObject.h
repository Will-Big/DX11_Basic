#pragma once

class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();

	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	void Render(shared_ptr<Pipeline> pipeline);

private:
	ComPtr<ID3D11Device> _device;

	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<RasterizerState> _rasterizerState;
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

private:
	// SRT
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

