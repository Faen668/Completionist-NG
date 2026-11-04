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
		// Timings, seconds
		constexpr float fadeIn = 0.35f;
		constexpr float waveStart = 0.2f;
		constexpr float waveTime = 3.0f;	// still moving as it fades, so it's never seen to stop
		constexpr float fadeOut = 0.8f;
		constexpr float pillarRise = 0.45f;
		constexpr float pillarFadeOut = 1.5f;
		constexpr float pillarLeave = 0.65f;	// how long one takes to lift away once its find is collected
		constexpr float pillarHeight = 220.0f;	// about 3 m. Taller further off, so it never shrinks to nothing

		constexpr const char* shader = R"(
cbuffer Sweep : register(b0)
{
	float4 clipToView[3];	// clip space back to a position relative to the camera
	float4 clipOffset;
	float4 origin;			// xyz, radius
	float4 settings;		// fade, near, far, trail
	float4 counts;			// pillars, seconds since the start
	float4 items[16];		// xyz, height
	float4 looks[16];		// size, strength, lift, flash
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
	const bool   hasDepth = depth < 0.99999;
	const float  nearPlane = settings.y;
	const float  farPlane = settings.z;
	const float  w = hasDepth ? nearPlane * farPlane / (farPlane - depth * (farPlane - nearPlane)) : 1e9;

	const float fade = settings.x;
	const float grey = dot(scene, float3(0.299, 0.587, 0.114));

	// Ahead of the wave: dark, cold and close to colourless
	float3 colour = lerp(scene, grey * float3(0.74, 0.86, 0.90), fade * 0.9) * lerp(1.0, 0.58, fade);

	if (hasDepth) {
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
	}

	// A pillar of light on each find the wave has reached. Everything here goes by the line through this pixel,
	// so a pillar also shows against the sky and goes behind whatever stands in front of it
	const float2 screen = float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0);
	const float3 start = float3(-dot(clipToView[0].xyz, clipOffset.xyz), -dot(clipToView[1].xyz, clipOffset.xyz), -dot(clipToView[2].xyz, clipOffset.xyz));
	const float3 ray = float3(dot(clipToView[0].xyz, float3(screen, 1.0)), dot(clipToView[1].xyz, float3(screen, 1.0)), dot(clipToView[2].xyz, float3(screen, 1.0)));
	const float  rayLength = dot(ray, ray);
	const float  pixelSize = length(float3(clipToView[0].x, clipToView[1].x, clipToView[2].x)) * 2.0 / width;
	const float  time = counts.y;
	const float3 gold = float3(1.0, 0.72, 0.28);
	const float3 pale = float3(1.0, 0.95, 0.80);

	for (int i = 0; i < (int)counts.x; ++i) {
		const float size = looks[i].x;
		const float strength = looks[i].y;
		const float height = items[i].w;
		const float3 base = items[i].xyz + float3(0.0, 0.0, looks[i].z * size) - start;

		// The nearest point of the upright line to this pixel's line
		const float along = (dot(base, ray) - ray.z * base.z) / max(rayLength - ray.z * ray.z, 0.000001);
		const float up = clamp(along * ray.z - base.z, 0.0, height);
		const float3 nearest = base + float3(0.0, 0.0, up);
		const float beamW = dot(nearest, ray) / rayLength;
		if (beamW <= 0.0) {
			continue;
		}
		const float gap = length(nearest - ray * beamW);
		const float pixelThere = pixelSize * beamW;
		if (gap > 40.0 * size + pixelThere * 4.0) {
			continue;
		}

		if (beamW <= w) {
			const float top = 1.0 - up / max(height, 1.0);
			const float pulse = 0.84 + 0.16 * sin(time * 2.4 + i * 1.3);
			const float coreWidth = max(2.2 * size, pixelThere);
			const float glowWidth = max(8.0 * size, pixelThere * 2.5);
			const float core = exp(-(gap * gap) / (coreWidth * coreWidth));
			const float glow = exp(-(gap * gap) / (glowWidth * glowWidth));

			// A dark edge first, so it still shows on snow, then the light
			colour *= 1.0 - glow * top * 0.42 * strength;
			colour += gold * glow * top * (0.55 * pulse + looks[i].w) * strength;
			colour += pale * core * pow(top, 0.7) * pulse * strength;
		}

		// A few sparks drifting up it
		for (int j = 0; j < 5; ++j) {
			const float climb = frac(time * 0.28 + j * 0.411 + i * 0.37);
			const float swing = 5.0 + (j % 3) * 2.5;
			const float3 spark = base + float3(sin(j * 2.4 + time * 1.1) * swing * size, cos(j * 1.9 + time * 0.9) * swing * size, climb * height * 0.95);
			const float sparkW = dot(spark, ray) / rayLength;
			if (sparkW > 0.0 && sparkW <= w) {
				const float3 off = spark - ray * sparkW;
				const float sparkSize = max(1.5 * size, pixelSize * sparkW);
				colour += pale * exp(-dot(off, off) / (sparkSize * sparkSize)) * sin(climb * 3.14159) * strength;
			}
		}

		// The ring it stands in
		if (abs(ray.z) > 0.0001) {
			const float ringW = base.z / ray.z;
			if (ringW > 0.0 && ringW <= w + 12.0 * size) {
				const float across = length(ray.xy * ringW - base.xy);
				const float ringWidth = max(1.2 * size, pixelSize * ringW);
				const float ring = (across - 20.0 * size) / ringWidth;
				colour += gold * (exp(-ring * ring) * 0.9 + exp(-(across * across) / (484.0 * size * size)) * looks[i].w) * strength;
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
			float items[SweepEffect::maxItems][4];
			float looks[SweepEffect::maxItems][4];
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
		const float fade = std::min(std::clamp(a_wave.elapsed / fadeIn, 0.0f, 1.0f), std::clamp((waveDuration - a_wave.elapsed) / fadeOut, 0.0f, 1.0f));
		const float pillarFade = std::clamp((duration - a_wave.elapsed) / pillarFadeOut, 0.0f, 1.0f);

		// Seconds since the wave reached this place, below zero until it does
		const auto sinceReached = [&](const Point& a_place) {
			const double x = a_place[0] - a_wave.origin[0];
			const double y = a_place[1] - a_wave.origin[1];
			const double z = a_place[2] - a_wave.origin[2];
			const float  away = static_cast<float>(std::sqrt(x * x + y * y + z * z));
			if (away > a_wave.reach) {
				return -1.0f;
			}
			return a_wave.elapsed - (waveStart + waveTime * std::pow(away / a_wave.reach, 1.0f / 2.2f));
		};

		// A find that's just gone from the list has been collected: its pillar lifts away
		if (a_wave.elapsed < _lastElapsed) {
			_lastItems.clear();
			_leaving.clear();
		}
		if (a_wave.items.size() < _lastItems.size()) {
			std::size_t kept = 0;
			for (const auto& place : _lastItems) {
				if (kept < a_wave.items.size() && std::abs(place[0] - a_wave.items[kept][0]) + std::abs(place[1] - a_wave.items[kept][1]) + std::abs(place[2] - a_wave.items[kept][2]) < 300.0) {
					++kept;
				} else if (sinceReached(place) > 0.0f) {
					_leaving.push_back({ place, a_wave.elapsed });
				}
			}
		}
		_lastItems = a_wave.items;
		_lastElapsed = a_wave.elapsed;
		std::erase_if(_leaving, [&](const Leaving& a_one) { return a_wave.elapsed - a_one.since >= pillarLeave; });

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

		// Bigger with distance so it keeps its size on screen, but fainter, so a full room doesn't turn to clutter
		int count = 0;
		const auto addPillar = [&](const Point& a_place, float a_grown, float a_strength, float a_lift, float a_flash) {
			if (count >= maxItems) {
				return;
			}
			relative(a_place, constants.items[count]);
			const float* place = constants.items[count];
			const float  away = std::sqrt(place[0] * place[0] + place[1] * place[1] + place[2] * place[2]);
			const float  size = std::max(1.0f, away * 0.045f / pillarHeight);
			const float  further = std::clamp(away / 15000.0f, 0.0f, 1.0f);
			constants.items[count][3] = pillarHeight * size * a_grown;
			constants.looks[count][0] = size;
			constants.looks[count][1] = a_strength * (1.0f - further * 0.3f);
			constants.looks[count][2] = a_lift;
			constants.looks[count][3] = a_flash;
			++count;
		};
		for (std::size_t i = 0; i < a_wave.items.size(); ++i) {
			const float age = sinceReached(a_wave.items[i]);
			if (age <= 0.0f) {
				continue;
			}
			const float risen = std::clamp(age / pillarRise, 0.0f, 1.0f);
			const float grown = 1.0f - (1.0f - risen) * (1.0f - risen) * (1.0f - risen);
			addPillar(a_wave.items[i], grown, (i == 0 && a_wave.firstMarked ? 1.0f : 0.8f) * risen * pillarFade, 0.0f, 1.0f - risen);
		}
		for (const auto& one : _leaving) {
			const float gone = (a_wave.elapsed - one.since) / pillarLeave;
			addPillar(one.place, 1.0f, (1.0f - gone) * pillarFade, 90.0f * gone * (2.0f - gone), (1.0f - gone) * 0.8f);
		}
		constants.counts[0] = static_cast<float>(count);
		constants.counts[1] = a_wave.elapsed;

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
