#include "UI/Core/SweepEffect.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler")

namespace CompletionistUI
{
	using Microsoft::WRL::ComPtr;

	namespace
	{
		constexpr int maxItems = 16;

		// Timings, seconds
		constexpr float fadeIn = 0.35f;
		constexpr float waveStart = 0.2f;
		constexpr float waveTime = 3.0f;	// still moving as it fades, so it's never seen to stop
		constexpr float fadeOut = 0.8f;

		constexpr const char* shader = R"(
cbuffer Sweep : register(b0)
{
	float4 clipToView[3];	// clip space back to a position relative to the camera
	float4 clipOffset;
	float4 origin;			// xyz, radius
	float4 settings;		// fade, near, far, trail
	float4 counts;			// items
	float4 items[16];		// xyz, strength
};

Texture2D<float4> sceneTexture : register(t0);
Texture2D<float>  depthTexture : register(t1);

float4 MainVS(uint id : SV_VertexID) : SV_Position
{
	const float2 uv = float2((id << 1) & 2, id & 2);
	return float4(uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
}

float4 MainPS(float4 pixel : SV_Position) : SV_Target
{
	uint width, height, depthWidth, depthHeight;
	sceneTexture.GetDimensions(width, height);
	depthTexture.GetDimensions(depthWidth, depthHeight);

	const float3 scene = sceneTexture.Load(int3(pixel.xy, 0)).rgb;
	const float2 uv = pixel.xy / float2(width, height);
	const float  depth = depthTexture.Load(int3(uv * float2(depthWidth, depthHeight), 0));

	const float fade = settings.x;
	const float grey = dot(scene, float3(0.299, 0.587, 0.114));

	// Ahead of the wave: dark, cold and close to colourless
	float3 colour = lerp(scene, grey * float3(0.74, 0.86, 0.90), fade * 0.9) * lerp(1.0, 0.58, fade);

	if (depth < 0.99999) {
		const float nearPlane = settings.y;
		const float farPlane = settings.z;
		const float w = nearPlane * farPlane / (farPlane - depth * (farPlane - nearPlane));
		const float3 clipped = float3((uv.x * 2.0 - 1.0) * w, (1.0 - uv.y * 2.0) * w, w) - clipOffset.xyz;
		const float3 position = float3(dot(clipToView[0].xyz, clipped), dot(clipToView[1].xyz, clipped), dot(clipToView[2].xyz, clipped));

		const float radius = origin.w;
		const float away = length(position - origin.xyz);
		const float behind = radius - away;

		// Passed by the wave: a pale wash that keeps the surface's detail and dies away behind the front
		if (behind > 0.0) {
			const float wash = exp(-behind / settings.w) * 0.78 * fade;
			colour = lerp(colour, grey * 0.55 + float3(0.40, 0.42, 0.43), wash);
		}

		// The thin bright line at the front
		const float lineWidth = 7.0 + radius * 0.0035;
		const float front = 1.0 - saturate(abs(behind) / lineWidth);
		colour += float3(0.82, 0.88, 0.90) * front * front * fade;

		// Things worth finding catch the light as the wave reaches them
		for (int i = 0; i < (int)counts.x; ++i) {
			const float reached = saturate((radius - length(items[i].xyz - origin.xyz)) / (radius * 0.08 + 1.0));
			if (reached > 0.0) {
				const float3 offset = position - items[i].xyz;
				const float  halo = exp(-dot(offset, offset) / (85.0 * 85.0));
				// A tint rather than added light, so it can't blow out on a bright surface
				colour = lerp(colour, float3(0.95, 0.72, 0.32) * (grey * 0.6 + 0.4), halo * reached * items[i].w * fade);
			}
		}
	}

	return float4(colour, 1.0);
}
)";

		struct Constants
		{
			float clipToView[3][4];
			float clipOffset[4];
			float origin[4];
			float settings[4];
			float counts[4];
			float items[maxItems][4];
		};
		static_assert(sizeof(Constants) % 16 == 0);

		// Everything the effect changes, put back afterwards
		class SavedState
		{
		public:
			explicit SavedState(ID3D11DeviceContext* a_context) :
				_context(a_context) {
				_context->OMGetRenderTargets(1, &_target, &_depth);
				_context->OMGetBlendState(&_blend, _blendFactor, &_sampleMask);
				_context->OMGetDepthStencilState(&_depthState, &_stencilRef);
				_context->RSGetState(&_raster);
				_context->RSGetViewports(&_viewportCount, _viewports);
				_context->IAGetInputLayout(&_layout);
				_context->IAGetPrimitiveTopology(&_topology);
				_context->VSGetShader(&_vertex, nullptr, nullptr);
				_context->PSGetShader(&_pixel, nullptr, nullptr);
				_context->GSGetShader(&_geometry, nullptr, nullptr);
				_context->PSGetConstantBuffers(0, 1, &_constants);
				ID3D11ShaderResourceView* resources[2]{};
				_context->PSGetShaderResources(0, 2, resources);
				_resources[0].Attach(resources[0]);
				_resources[1].Attach(resources[1]);
			}

			~SavedState() {
				_context->OMSetRenderTargets(1, _target.GetAddressOf(), _depth.Get());
				_context->OMSetBlendState(_blend.Get(), _blendFactor, _sampleMask);
				_context->OMSetDepthStencilState(_depthState.Get(), _stencilRef);
				_context->RSSetState(_raster.Get());
				_context->RSSetViewports(_viewportCount, _viewports);
				_context->IASetInputLayout(_layout.Get());
				_context->IASetPrimitiveTopology(_topology);
				_context->VSSetShader(_vertex.Get(), nullptr, 0);
				_context->PSSetShader(_pixel.Get(), nullptr, 0);
				_context->GSSetShader(_geometry.Get(), nullptr, 0);
				_context->PSSetConstantBuffers(0, 1, _constants.GetAddressOf());
				ID3D11ShaderResourceView* resources[2]{ _resources[0].Get(), _resources[1].Get() };
				_context->PSSetShaderResources(0, 2, resources);
			}

			SavedState(const SavedState&) = delete;
			SavedState& operator=(const SavedState&) = delete;

		private:
			ID3D11DeviceContext*             _context;
			ComPtr<ID3D11RenderTargetView>   _target;
			ComPtr<ID3D11DepthStencilView>   _depth;
			ComPtr<ID3D11BlendState>         _blend;
			float                            _blendFactor[4]{};
			UINT                             _sampleMask{ 0 };
			ComPtr<ID3D11DepthStencilState>  _depthState;
			UINT                             _stencilRef{ 0 };
			ComPtr<ID3D11RasterizerState>    _raster;
			UINT                             _viewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
			D3D11_VIEWPORT                   _viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
			ComPtr<ID3D11InputLayout>        _layout;
			D3D11_PRIMITIVE_TOPOLOGY         _topology{ D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED };
			ComPtr<ID3D11VertexShader>       _vertex;
			ComPtr<ID3D11PixelShader>        _pixel;
			ComPtr<ID3D11GeometryShader>     _geometry;
			ComPtr<ID3D11Buffer>             _constants;
			ComPtr<ID3D11ShaderResourceView> _resources[2];
		};

		ComPtr<ID3DBlob> Compile(const char* a_entry, const char* a_target) {

			ComPtr<ID3DBlob> code;
			ComPtr<ID3DBlob> errors;
			if (FAILED(D3DCompile(shader, std::strlen(shader), "CompletionistSweep", nullptr, nullptr, a_entry, a_target, D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &errors))) {
				return nullptr;
			}
			return code;
		}
	}

	//---------------------------------------------------
	//-- Sweep ( Reach For ) ----------------------------
	//---------------------------------------------------

	// About 350 m, further when that's what it takes to reach the find, up to about 570 m
	float SweepEffect::ReachFor(float a_nearest) {

		return std::clamp(a_nearest * 1.15f, 25000.0f, 40000.0f);
	}

	//---------------------------------------------------
	//-- Sweep ( Prepare ) ------------------------------
	//---------------------------------------------------

	bool SweepEffect::Prepare(ID3D11Device* a_device) {

		if (_failed) {
			return false;
		}
		if (_pixel) {
			return true;
		}

		_failed = true;
		const auto vertex = Compile("MainVS", "vs_5_0");
		const auto pixel = Compile("MainPS", "ps_5_0");
		if (!vertex || !pixel) {
			return false;
		}
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader>  pixelShader;
		if (FAILED(a_device->CreateVertexShader(vertex->GetBufferPointer(), vertex->GetBufferSize(), nullptr, &vertexShader)) ||
			FAILED(a_device->CreatePixelShader(pixel->GetBufferPointer(), pixel->GetBufferSize(), nullptr, &pixelShader))) {
			return false;
		}

		D3D11_BUFFER_DESC buffer{};
		buffer.ByteWidth = sizeof(Constants);
		buffer.Usage = D3D11_USAGE_DYNAMIC;
		buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_RASTERIZER_DESC raster{};
		raster.FillMode = D3D11_FILL_SOLID;
		raster.CullMode = D3D11_CULL_NONE;

		D3D11_BLEND_DESC blend{};
		blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;

		D3D11_DEPTH_STENCIL_DESC depth{};

		if (FAILED(a_device->CreateBuffer(&buffer, nullptr, &_constants)) ||
			FAILED(a_device->CreateRasterizerState(&raster, &_raster)) ||
			FAILED(a_device->CreateBlendState(&blend, &_blend)) ||
			FAILED(a_device->CreateDepthStencilState(&depth, &_depthState))) {
			return false;
		}

		_vertex = vertexShader;
		_pixel = pixelShader;
		_failed = false;
		return true;
	}

	//---------------------------------------------------
	//-- Sweep ( Prepare Scene ) ------------------------
	//---------------------------------------------------

	// A shader can't read the target it's drawing to, so it reads a copy
	bool SweepEffect::PrepareScene(ID3D11Device* a_device, const D3D11_TEXTURE2D_DESC& a_desc) {

		if (_scene && _sceneWidth == a_desc.Width && _sceneHeight == a_desc.Height && _sceneFormat == a_desc.Format) {
			return true;
		}
		_sceneView.Reset();
		_scene.Reset();

		auto desc = a_desc;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		if (FAILED(a_device->CreateTexture2D(&desc, nullptr, &_scene)) || FAILED(a_device->CreateShaderResourceView(_scene.Get(), nullptr, &_sceneView))) {
			_scene.Reset();
			return false;
		}
		_sceneWidth = a_desc.Width;
		_sceneHeight = a_desc.Height;
		_sceneFormat = a_desc.Format;
		return true;
	}

	//---------------------------------------------------
	//-- Sweep ( Draw ) ---------------------------------
	//---------------------------------------------------

	bool SweepEffect::Draw(ID3D11Device* a_device, ID3D11DeviceContext* a_context, ID3D11Texture2D* a_backBuffer, ID3D11RenderTargetView* a_target, ID3D11ShaderResourceView* a_depth, const View& a_view, const Wave& a_wave) {

		if (!a_depth || !Prepare(a_device)) {
			return false;
		}
		D3D11_TEXTURE2D_DESC desc{};
		a_backBuffer->GetDesc(&desc);
		if (desc.SampleDesc.Count != 1 || !PrepareScene(a_device, desc)) {
			return false;
		}

		// Worked out relative to the camera: the world's coordinates are too large for a shader's floats
		const auto& m = a_view.worldToClip;
		const auto  term = [&](int a_row) {
			return m[a_row][0] * a_view.camera[0] + m[a_row][1] * a_view.camera[1] + m[a_row][2] * a_view.camera[2] + m[a_row][3];
		};
		const double a[3][3]{
			{ m[0][0], m[0][1], m[0][2] },
			{ m[1][0], m[1][1], m[1][2] },
			{ m[3][0], m[3][1], m[3][2] },
		};
		const double determinant = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
		if (std::abs(determinant) < 1e-12) {
			return false;
		}

		const auto relative = [&](const Point& a_point, float* a_out) {
			for (int i = 0; i < 3; ++i) {
				a_out[i] = static_cast<float>(a_point[i] - a_view.camera[i]);
			}
		};

		const float travelled = std::clamp((a_wave.elapsed - waveStart) / waveTime, 0.0f, 1.0f);
		// Starts at a walk around the player and picks up speed, so it covers a whole cave or valley in the time
		const float radius = a_wave.reach * std::pow(travelled, 2.2f);
		const float fade = std::min(std::clamp(a_wave.elapsed / fadeIn, 0.0f, 1.0f), std::clamp((duration - a_wave.elapsed) / fadeOut, 0.0f, 1.0f));

		Constants constants{};
		constants.clipToView[0][0] = static_cast<float>((a[1][1] * a[2][2] - a[1][2] * a[2][1]) / determinant);
		constants.clipToView[0][1] = static_cast<float>((a[0][2] * a[2][1] - a[0][1] * a[2][2]) / determinant);
		constants.clipToView[0][2] = static_cast<float>((a[0][1] * a[1][2] - a[0][2] * a[1][1]) / determinant);
		constants.clipToView[1][0] = static_cast<float>((a[1][2] * a[2][0] - a[1][0] * a[2][2]) / determinant);
		constants.clipToView[1][1] = static_cast<float>((a[0][0] * a[2][2] - a[0][2] * a[2][0]) / determinant);
		constants.clipToView[1][2] = static_cast<float>((a[0][2] * a[1][0] - a[0][0] * a[1][2]) / determinant);
		constants.clipToView[2][0] = static_cast<float>((a[1][0] * a[2][1] - a[1][1] * a[2][0]) / determinant);
		constants.clipToView[2][1] = static_cast<float>((a[0][1] * a[2][0] - a[0][0] * a[2][1]) / determinant);
		constants.clipToView[2][2] = static_cast<float>((a[0][0] * a[1][1] - a[0][1] * a[1][0]) / determinant);
		constants.clipOffset[0] = static_cast<float>(term(0));
		constants.clipOffset[1] = static_cast<float>(term(1));
		constants.clipOffset[2] = static_cast<float>(term(3));
		relative(a_wave.origin, constants.origin);
		constants.origin[3] = radius;
		constants.settings[0] = fade;
		constants.settings[1] = a_view.nearPlane;
		constants.settings[2] = a_view.farPlane;
		constants.settings[3] = std::max(500.0f, radius * 0.75f);
		const int count = std::min(static_cast<int>(a_wave.items.size()), maxItems);
		constants.counts[0] = static_cast<float>(count);
		for (int i = 0; i < count; ++i) {
			relative(a_wave.items[static_cast<std::size_t>(i)], constants.items[i]);
			constants.items[i][3] = i == 0 && a_wave.firstMarked ? 0.8f : 0.35f;
		}

		D3D11_MAPPED_SUBRESOURCE mapped{};
		if (FAILED(a_context->Map(_constants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
			return false;
		}
		std::memcpy(mapped.pData, &constants, sizeof(constants));
		a_context->Unmap(_constants.Get(), 0);

		a_context->CopyResource(_scene.Get(), a_backBuffer);

		SavedState saved(a_context);

		D3D11_VIEWPORT viewport{};
		viewport.Width = static_cast<float>(desc.Width);
		viewport.Height = static_cast<float>(desc.Height);
		viewport.MaxDepth = 1.0f;

		a_context->OMSetRenderTargets(1, &a_target, nullptr);
		a_context->OMSetBlendState(_blend.Get(), nullptr, 0xFFFFFFFF);
		a_context->OMSetDepthStencilState(_depthState.Get(), 0);
		a_context->RSSetState(_raster.Get());
		a_context->RSSetViewports(1, &viewport);
		a_context->IASetInputLayout(nullptr);
		a_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		a_context->VSSetShader(_vertex.Get(), nullptr, 0);
		a_context->GSSetShader(nullptr, nullptr, 0);
		a_context->PSSetShader(_pixel.Get(), nullptr, 0);
		a_context->PSSetConstantBuffers(0, 1, _constants.GetAddressOf());
		ID3D11ShaderResourceView* resources[2]{ _sceneView.Get(), a_depth };
		a_context->PSSetShaderResources(0, 2, resources);

		a_context->Draw(3, 0);

		ID3D11ShaderResourceView* none[2]{ nullptr, nullptr };
		a_context->PSSetShaderResources(0, 2, none);
		return true;
	}
}
