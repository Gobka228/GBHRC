#include "Engine.h"

using namespace Application::Render;

ID3D11RenderTargetView* Engine::get_render_target(IDXGISwapChain* pSwap) const
{

	if (this->pRenderTargetView == nullptr)
	{
		ID3D11Texture2D* pBackBuffer;
		pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (pBackBuffer)
			pDevice->CreateRenderTargetView(pBackBuffer, nullptr, (ID3D11RenderTargetView**)&pRenderTargetView);
		pBackBuffer->Release();
	}

	return this->pRenderTargetView;
}

Resolution Engine::get_resolution(const HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Resolution{ (UINT)(rect.right - rect.left),(UINT)(rect.bottom - rect.top) };
}

Engine::Engine(const HWND hwnd, ID3D11Device* pDevice,
	IDXGISwapChain* pSwapChain)
{
	this->window = hwnd;
	this->pDevice = pDevice;
	pDevice->GetImmediateContext(&this->pDevContext);

	this->pRenderTargetView = this->get_render_target(pSwapChain);

	this->initialize();
	this->mOrtho = get_ortho_matrix();
	this->initialize_scene();
}

Engine* Engine::append_scene(Render::Scene* scene)
{
	scene->set_resolution(get_resolution(this->window));
	this->pScenes.push_back(scene);
	return this;
}

void Engine::wnd_proc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE)
	{
		auto res = get_resolution(this->window);
		for (auto* scene : this->pScenes)
			scene->set_resolution(res);
		return;
	}

	for (auto* scene : this->pScenes)
	{
		//scene->wnd_proc(msg, wParam, lParam);
	}
}

bool Engine::initialize()
{

	ID3D10Blob* pBlob = this->create_vs_shader();
	if (pBlob == nullptr)
	{
		DEBUG_LOG("[PG][-] CANNOT VS CREATE SHADER");
		return false;
	}

	HRESULT hr = pDevice->CreateInputLayout(
		GVertex::Layout,
		ARRAYSIZE(GVertex::Layout),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&this->pVertexLayout
	);

	create_ps_shader(pBlob);

	safe_release(pBlob);

	if (FAILED(hr))
	{
		DEBUG_LOG("[PG][-] CANNOT CREATE INPUT LAYOUT");
		return false;
	}

	return true;
}

ID3D10Blob* Engine::create_vs_shader()
{
	ID3D10Blob* pBlob = nullptr;
	HRESULT hr;

	if (!this->compile_shader(szShadez, "VS", "vs_5_0", &pBlob))
		return nullptr;

	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
		return nullptr;

	return pBlob;
}

ID3DBlob* Engine::create_ps_shader(ID3DBlob* blob)
{
	if (!this->compile_shader(szShadez, "PS", "ps_5_0", &blob))
		return nullptr;

	HRESULT hr = pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
		return nullptr;

	return blob;
}

GVertex::VertexBuffer* Engine::make_vertex_buffer(const UINT size) const
{
	auto* ptr = new GVertex::VertexBuffer();
	auto* pArray = new GVertex::Vertex[size];
	ptr->create(this->pDevice, pArray, size);
	return ptr;
}

bool Engine::compile_shader(const char* szShader, const char* szEntrypoint, const char* szTarget,
	ID3D10Blob** pBlob)
{
	ID3D10Blob* pErrorBlob = nullptr;

	auto hr = D3DCompile(szShader, strlen(szShader), 0, nullptr, nullptr, szEntrypoint, szTarget, D3DCOMPILE_ENABLE_STRICTNESS, 0, pBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			char szError[256]{ 0 };
			memcpy(szError, pErrorBlob->GetBufferPointer(), pErrorBlob->GetBufferSize());
			DEBUG_LOG("[D3D][COMPILE_ERROR]" << szError);
		}
		return false;
	}
	return true;
}

bool Engine::initialize_scene()
{
	HRESULT hr;

	ConstantBuffer _constantBuffer;
	_constantBuffer.mProjection = this->mOrtho;

	D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
	vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &_constantBuffer;

	hr = this->pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->pConstantBuffer);

	if (FAILED(hr))
	{
		DEBUG_LOG("CANNOT CREATE CONSTANT BUFFER");
	}

	return true;
}

DirectX::XMMATRIX Engine::get_ortho_matrix()
{
	UINT numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	this->pDevContext->RSGetViewports(&numViewports, pViewports);

	return DirectX::XMMatrixOrthographicLH(
		(float)pViewports[0].Width,
		(float)pViewports[0].Height,
		0.0f,
		1.0f
	);
}

void Engine::update_scene()
{
	for (auto* scene : pScenes)
		scene->update(this->pDevContext);
}

void Engine::present() const
{
	this->pDevContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	ConstantBuffer cb;
	cb.mProjection = DirectX::XMMatrixTranspose(mOrtho);
	cb.xOffset = 0.f;
	cb.yOffset = 0.f;

	this->pDevContext->UpdateSubresource(this->pConstantBuffer, 0, nullptr, &cb, 0, 0);
	this->pDevContext->VSSetConstantBuffers(0, 1, &this->pConstantBuffer);

	this->pDevContext->IASetInputLayout(pVertexLayout);
	this->pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pDevContext->VSSetShader(pVertexShader, nullptr, 0);
	pDevContext->PSSetShader(pPixelShader, nullptr, 0);

	pDevContext->RSSetViewports(1, pViewports);

	//pDevContext->RSSetState();

	for (auto* scene : this->pScenes)
		scene->render(this->pDevContext);

	pDevContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}