#include "UI/ModelPictures.hpp"

#include <DirectXPackedVector.h>
#include <ScreenGrab.h>
#include <WICTextureLoader.h>
#include <deque>
#include <filesystem>
#include <optional>
#include <wincodec.h>
#include <array>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <wrl/client.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace CompletionistUI
{
	namespace
	{
		using Microsoft::WRL::ComPtr;

		constexpr UINT  iconSize = 128;
		constexpr int   drawsPerFrame = 3;	// so opening a page doesn't stall a frame
		constexpr int   loadsAtOnce = 4;	// models queued on the game thread at a time
		constexpr int   drawsWhilePreparing = 8;
		constexpr int   loadsWhilePreparing = 16;
		constexpr int   diskPerFrame = 12;	// kept pictures read back a frame
		constexpr std::size_t heldPictures = 400;	// on the card at once before the unseen ones go
		constexpr int   textureWait = 60;	// frames to wait for a texture before drawing without it
		constexpr float pitch = 0.52f;	// looking down at it a little, about 30 degrees
		constexpr float yaw = 0.61f;	// and from a little to one side, about 35 degrees

		enum class Stage : std::uint8_t
		{
			kNone,
			kLoading,
			kLoaded,
			kDone,
			kFailed,
		};

		// A texture the item's record puts on one of its model's shapes in place of the model's
		// own, as books get their covers
		struct Swap
		{
			std::string                  shape;
			RE::NiPointer<RE::NiTexture> texture;
		};

		struct Icon
		{
			Stage                            stage{ Stage::kNone };
			RE::NiPointer<RE::NiNode>        model;
			std::vector<Swap>                swaps;
			ComPtr<ID3D11ShaderResourceView> view;
			int                              waited{ 0 };
			bool                             checked{ false };	// looked for a kept picture
			std::optional<std::filesystem::path> file;	// the kept picture
			int                              lastUsed{ 0 };	// frame it was last shown
		};

		std::mutex                               lock;
		std::unordered_map<std::uint32_t, Icon>  icons;
		int                                      loading{ 0 };
		std::vector<std::uint32_t>               items;	// everything with a picture, for preparing
		std::deque<std::uint32_t>                queue;	// still to prepare
		int                                      prepared{ 0 };
		int                                      total{ 0 };
		bool                                     redrawAll{ false };

		//---------------------------------------------------
		//-- Model Pictures ( Maths ) -----------------------
		//---------------------------------------------------

		// Gamebryo's way round: a point is turned by the rows of the rotation, scaled, then moved
		using Vec3 = std::array<float, 3>;
		using Matrix = std::array<std::array<float, 3>, 3>;

		struct Affine
		{
			Matrix rotate{ { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } } };	// scale folded in
			Vec3   translate{ 0.0f, 0.0f, 0.0f };
		};

		Matrix Multiply(const Matrix& a_left, const Matrix& a_right) {

			Matrix out{};
			for (int row = 0; row < 3; ++row) {
				for (int column = 0; column < 3; ++column) {
					out[row][column] = a_left[row][0] * a_right[0][column] + a_left[row][1] * a_right[1][column] + a_left[row][2] * a_right[2][column];
				}
			}
			return out;
		}

		Vec3 Apply(const Matrix& a_matrix, const Vec3& a_point) {

			return { a_matrix[0][0] * a_point[0] + a_matrix[0][1] * a_point[1] + a_matrix[0][2] * a_point[2],
				a_matrix[1][0] * a_point[0] + a_matrix[1][1] * a_point[1] + a_matrix[1][2] * a_point[2],
				a_matrix[2][0] * a_point[0] + a_matrix[2][1] * a_point[1] + a_matrix[2][2] * a_point[2] };
		}

		Vec3 Apply(const Affine& a_transform, const Vec3& a_point) {

			const auto turned = Apply(a_transform.rotate, a_point);
			return { turned[0] + a_transform.translate[0], turned[1] + a_transform.translate[1], turned[2] + a_transform.translate[2] };
		}

		// The child's place under its parent
		Affine Compose(const Affine& a_parent, const RE::NiTransform& a_local) {

			Matrix local{};
			for (int row = 0; row < 3; ++row) {
				for (int column = 0; column < 3; ++column) {
					local[row][column] = a_local.rotate.entry[row][column] * a_local.scale;
				}
			}
			Affine out;
			out.rotate = Multiply(a_parent.rotate, local);
			out.translate = Apply(a_parent, Vec3{ a_local.translate.x, a_local.translate.y, a_local.translate.z });
			return out;
		}

		//---------------------------------------------------
		//-- Model Pictures ( Shader ) ----------------------
		//---------------------------------------------------

		// Positions in the shape's own space, the world, view and fit to the picture in one
		// matrix; normals unpacked from bytes, lit from above and in front, both sides alike
		constexpr const char* shaderSource = R"(
cbuffer Shape : register(b0)
{
	row_major float4x4 toPicture;
	row_major float4x4 toView;
	float4             options;	// textured, has normals, has uvs
};

struct VertexIn
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

Texture2D    diffuse : register(t0);
SamplerState wrap : register(s0);

VertexOut VS(VertexIn input)
{
	VertexOut output;
	output.position = mul(toPicture, float4(input.position, 1.0f));
	output.uv = options.z > 0.5f ? input.uv : float2(0.0f, 0.0f);
	output.normal = options.y > 0.5f ? mul((float3x3)toView, input.normal.xyz * 2.0f - 1.0f) : float3(0.0f, -1.0f, 0.0f);
	return output;
}

float4 PS(VertexOut input) : SV_TARGET
{
	float4 colour = options.x > 0.5f ? diffuse.Sample(wrap, input.uv) : float4(0.62f, 0.58f, 0.52f, 1.0f);
	clip(colour.a - 0.3f);
	float3 light = normalize(float3(-0.45f, -0.65f, 0.6f));
	float  lit = 0.42f + 0.7f * abs(dot(normalize(input.normal), light));
	return float4(colour.rgb * lit, 1.0f);
}
)";

		struct ShapeConstants
		{
			float toPicture[16];
			float toView[16];
			float options[4];
		};

		//---------------------------------------------------
		//-- Model Pictures ( Device Objects ) --------------
		//---------------------------------------------------

		struct Device
		{
			bool                              ready{ false };
			bool                              failed{ false };
			ID3D11Device*                     device{ nullptr };
			ID3D11DeviceContext*              context{ nullptr };
			ComPtr<ID3DBlob>                  vertexCode;
			ComPtr<ID3D11VertexShader>        vertexShader;
			ComPtr<ID3D11PixelShader>         pixelShader;
			ComPtr<ID3D11Buffer>              constants;
			ComPtr<ID3D11SamplerState>        sampler;
			ComPtr<ID3D11RasterizerState>     raster;
			ComPtr<ID3D11DepthStencilState>   depthState;
			ComPtr<ID3D11BlendState>          blend;
			ComPtr<ID3D11DepthStencilView>    depth;
			std::unordered_map<std::uint64_t, ComPtr<ID3D11InputLayout>> layouts[2];	// positions as halves, as floats
		};
		Device gpu;

		bool Prepare() {

			if (gpu.ready || gpu.failed) {
				return gpu.ready;
			}
			gpu.failed = true;
			const auto* data = RE::BSGraphics::Renderer::GetRendererDataSingleton();
			gpu.device = data ? reinterpret_cast<ID3D11Device*>(data->forwarder) : nullptr;
			gpu.context = data ? reinterpret_cast<ID3D11DeviceContext*>(data->context) : nullptr;
			if (!gpu.device || !gpu.context) {
				return false;
			}

			ComPtr<ID3DBlob> pixelCode;
			ComPtr<ID3DBlob> errors;
			const auto       length = std::strlen(shaderSource);
			if (FAILED(D3DCompile(shaderSource, length, "ModelPictures", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &gpu.vertexCode, &errors)) ||
				FAILED(D3DCompile(shaderSource, length, "ModelPictures", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pixelCode, &errors))) {
				ERROR("Model pictures: shader didn't compile: {}", errors ? static_cast<const char*>(errors->GetBufferPointer()) : "");
				return false;
			}
			gpu.device->CreateVertexShader(gpu.vertexCode->GetBufferPointer(), gpu.vertexCode->GetBufferSize(), nullptr, &gpu.vertexShader);
			gpu.device->CreatePixelShader(pixelCode->GetBufferPointer(), pixelCode->GetBufferSize(), nullptr, &gpu.pixelShader);

			D3D11_BUFFER_DESC buffer{ sizeof(ShapeConstants), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0 };
			gpu.device->CreateBuffer(&buffer, nullptr, &gpu.constants);

			D3D11_SAMPLER_DESC sampler{};
			sampler.Filter = D3D11_FILTER_ANISOTROPIC;
			sampler.MaxAnisotropy = 8;
			sampler.AddressU = sampler.AddressV = sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler.MaxLOD = D3D11_FLOAT32_MAX;
			gpu.device->CreateSamplerState(&sampler, &gpu.sampler);

			// Both sides, since a model's winding isn't known and leaves and wings are thin
			D3D11_RASTERIZER_DESC raster{};
			raster.FillMode = D3D11_FILL_SOLID;
			raster.CullMode = D3D11_CULL_NONE;
			raster.DepthClipEnable = TRUE;
			gpu.device->CreateRasterizerState(&raster, &gpu.raster);

			D3D11_DEPTH_STENCIL_DESC depthState{};
			depthState.DepthEnable = TRUE;
			depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthState.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			gpu.device->CreateDepthStencilState(&depthState, &gpu.depthState);

			D3D11_BLEND_DESC blend{};
			blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			gpu.device->CreateBlendState(&blend, &gpu.blend);

			D3D11_TEXTURE2D_DESC depthDesc{};
			depthDesc.Width = depthDesc.Height = iconSize;
			depthDesc.MipLevels = depthDesc.ArraySize = 1;
			depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthDesc.SampleDesc.Count = 1;
			depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			ComPtr<ID3D11Texture2D> depthTexture;
			if (FAILED(gpu.device->CreateTexture2D(&depthDesc, nullptr, &depthTexture)) || FAILED(gpu.device->CreateDepthStencilView(depthTexture.Get(), nullptr, &gpu.depth))) {
				return false;
			}
			if (!gpu.vertexShader || !gpu.pixelShader || !gpu.constants || !gpu.sampler || !gpu.raster || !gpu.depthState || !gpu.blend) {
				return false;
			}
			gpu.failed = false;
			gpu.ready = true;
			return true;
		}

		//---------------------------------------------------
		//-- Model Pictures ( Layout For ) ------------------
		//---------------------------------------------------

		// The game packs each shape's vertices to suit it: positions as floats or halves, uvs as
		// halves, normals as bytes, each at an offset the vertex description gives. A missing
		// one points at the start and the shader is told to leave it alone
		ID3D11InputLayout* LayoutFor(std::uint64_t a_desc, bool a_full) {

			auto& layouts = gpu.layouts[a_full ? 1 : 0];
			if (const auto it = layouts.find(a_desc); it != layouts.end()) {
				return it->second.Get();
			}
			using Vertex = RE::BSGraphics::Vertex;
			const auto flags = static_cast<std::uint16_t>(a_desc >> 44);
			const auto offset = [&](Vertex::Attribute a_attribute) { return static_cast<UINT>((a_desc >> (4 * a_attribute + 2)) & 0x3C); };
			const bool full = a_full;
			const D3D11_INPUT_ELEMENT_DESC elements[] = {
				{ "POSITION", 0, full ? DXGI_FORMAT_R32G32B32_FLOAT : DXGI_FORMAT_R16G16B16A16_FLOAT, 0, offset(Vertex::VA_POSITION), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, (flags & Vertex::VF_UV) ? offset(Vertex::VA_TEXCOORD0) : 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (flags & Vertex::VF_NORMAL) ? offset(Vertex::VA_NORMAL) : 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			ComPtr<ID3D11InputLayout> layout;
			gpu.device->CreateInputLayout(elements, 3, gpu.vertexCode->GetBufferPointer(), gpu.vertexCode->GetBufferSize(), &layout);
			return layouts.emplace(a_desc, layout).first->second.Get();
		}

		//---------------------------------------------------
		//-- Model Pictures ( Collect Shapes ) --------------
		//---------------------------------------------------

		// What's drawn: a whole shape, or one part of a skinned one, with its own buffers
		struct Shape
		{
			RE::BSTriShape*           shape;
			Affine                    transform;	// to the model's root
			ID3D11ShaderResourceView* texture;
			bool                      wantsTexture;
			ID3D11Buffer*             vertices;
			ID3D11Buffer*             indices;
			std::uint64_t             desc;	// the game's vertex description, the one the points were read with
			UINT                      triangles;
			UINT                      vertexCount;
			std::uint64_t             shapeDesc;	// the shape's own, for a skinned part to try too
			UINT                      stride{ 0 };	// found while reading the points
		};

		const RE::NiTexture* SwapFor(const std::vector<Swap>& a_swaps, const char* a_shape) {

			const auto it = std::ranges::find_if(a_swaps, [&](const Swap& a_swap) { return a_shape && _stricmp(a_swap.shape.c_str(), a_shape) == 0; });
			return it != a_swaps.end() ? it->texture.get() : nullptr;
		}

		// Why shapes were left out, for the log when a model draws nothing
		struct Skipped
		{
			int hidden{ 0 };
			int skinned{ 0 };
			int noBuffers{ 0 };
			int otherGeometry{ 0 };
		};

		// Every visible, unskinned shape with its buffers, and where it sits under the root
		void CollectShapes(RE::NiAVObject* a_object, const Affine& a_parent, const std::vector<Swap>& a_swaps, std::vector<Shape>& a_shapes, Skipped& a_skipped, int a_depth = 0) {

			if (!a_object || a_depth > 32) {
				return;
			}
			if (a_object->GetFlags().any(RE::NiAVObject::Flag::kHidden)) {
				++a_skipped.hidden;
				return;
			}
			const auto transform = Compose(a_parent, a_object->local);
			if (auto* node = a_object->AsNode()) {
				for (auto& child : node->GetChildren()) {
					CollectShapes(child.get(), transform, a_swaps, a_shapes, a_skipped, a_depth + 1);
				}
				return;
			}
			auto* shape = a_object->AsTriShape();
			if (!shape) {
				a_skipped.otherGeometry += a_object->AsGeometry() ? 1 : 0;
				return;
			}
			// A skinned shape, as books are so they can open, keeps its vertices with its skin, a
			// buffer per part; drawn as they lie, it's the model at rest
			const auto& runtime = shape->GetGeometryRuntimeData();
			struct Buffers
			{
				ID3D11Buffer* vertices;
				ID3D11Buffer* indices;
				std::uint64_t desc;
				UINT          triangles;
				UINT          vertexCount;
			};
			std::vector<Buffers> parts;
			const auto           descOf = [](const RE::BSGraphics::VertexDesc& a_desc) { return *reinterpret_cast<const std::uint64_t*>(&a_desc); };
			if (runtime.skinInstance) {
				const auto* skin = runtime.skinInstance->skinPartition.get();
				for (std::uint32_t i = 0; skin && i < skin->numPartitions; ++i) {
					const auto& part = skin->partitions[i];
					if (part.buffData && part.buffData->vertexBuffer && part.buffData->indexBuffer && part.triangles > 0) {
						parts.push_back({ reinterpret_cast<ID3D11Buffer*>(part.buffData->vertexBuffer), reinterpret_cast<ID3D11Buffer*>(part.buffData->indexBuffer), descOf(part.vertexDesc), part.triangles, skin->vertexCount });
					}
				}
				if (parts.empty()) {
					++a_skipped.skinned;
					return;
				}
			} else if (runtime.rendererData && runtime.rendererData->vertexBuffer && runtime.rendererData->indexBuffer) {
				const auto& counts = shape->GetTrishapeRuntimeData();
				parts.push_back({ reinterpret_cast<ID3D11Buffer*>(runtime.rendererData->vertexBuffer), reinterpret_cast<ID3D11Buffer*>(runtime.rendererData->indexBuffer), descOf(runtime.vertexDesc), counts.triangleCount, counts.vertexCount });
			} else {
				++a_skipped.noBuffers;
				return;
			}

			// The diffuse texture, from a lit material or an effect one
			ID3D11ShaderResourceView* texture = nullptr;
			bool                      wantsTexture = false;
			auto*                     property = runtime.shaderProperty.get();
			if (auto* lit = netimmerse_cast<RE::BSLightingShaderProperty*>(property); lit && lit->material) {
				const auto& source = static_cast<RE::BSLightingShaderMaterialBase*>(lit->material)->diffuseTexture;
				wantsTexture = true;
				if (source && source->rendererTexture) {
					texture = reinterpret_cast<ID3D11ShaderResourceView*>(source->rendererTexture->resourceView);
				}
			} else if (auto* effect = netimmerse_cast<RE::BSEffectShaderProperty*>(property); effect && effect->material) {
				const auto& source = static_cast<RE::BSEffectShaderMaterial*>(effect->material)->sourceTexture;
				wantsTexture = true;
				if (source && source->rendererTexture) {
					texture = reinterpret_cast<ID3D11ShaderResourceView*>(source->rendererTexture->resourceView);
				}
			}
			// The record's own texture for this shape, where it has one
			if (const auto* swap = SwapFor(a_swaps, a_object->name.c_str())) {
				const auto* source = netimmerse_cast<const RE::NiSourceTexture*>(swap);
				wantsTexture = true;
				texture = source && source->rendererTexture ? reinterpret_cast<ID3D11ShaderResourceView*>(source->rendererTexture->resourceView) : nullptr;
			}
			for (const auto& part : parts) {
				a_shapes.push_back({ shape, transform, texture, wantsTexture, part.vertices, part.indices, part.desc, part.triangles, part.vertexCount, descOf(runtime.vertexDesc) });
			}
		}

		//---------------------------------------------------
		//-- Model Pictures ( Read Positions ) --------------
		//---------------------------------------------------

		// The shape's points, copied back from the card once. How they're packed is checked
		// rather than trusted: each layout the game's descriptions or the buffer's size give is
		// read both as floats and as halves, and the one whose points are sane, and inside the
		// shape's bounds where it can, is kept, with its stride, for drawing too
		std::vector<Vec3> ReadPositions(Shape& a_shape, bool& a_full, std::uint32_t a_formID) {

			auto* buffer = a_shape.vertices;
			if (!buffer || a_shape.vertexCount == 0) {
				return {};
			}
			D3D11_BUFFER_DESC bufferDesc{};
			buffer->GetDesc(&bufferDesc);
			D3D11_BUFFER_DESC stagingDesc{ bufferDesc.ByteWidth, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ, 0, 0 };
			ComPtr<ID3D11Buffer> staging;
			if (FAILED(gpu.device->CreateBuffer(&stagingDesc, nullptr, &staging))) {
				return {};
			}
			gpu.context->CopyResource(staging.Get(), buffer);
			D3D11_MAPPED_SUBRESOURCE mapped{};
			if (FAILED(gpu.context->Map(staging.Get(), 0, D3D11_MAP_READ, 0, &mapped))) {
				return {};
			}
			const std::vector<std::uint8_t> bytes(static_cast<const std::uint8_t*>(mapped.pData), static_cast<const std::uint8_t*>(mapped.pData) + bufferDesc.ByteWidth);
			gpu.context->Unmap(staging.Get(), 0);

			struct Candidate
			{
				std::uint64_t desc;
				UINT          stride;
			};
			std::vector<Candidate> candidates;
			const auto             add = [&](std::uint64_t a_desc, UINT a_stride) {
				if (a_stride > 0 && std::ranges::none_of(candidates, [&](const Candidate& a_candidate) { return a_candidate.desc == a_desc && a_candidate.stride == a_stride; })) {
					candidates.push_back({ a_desc, a_stride });
				}
			};
			const UINT sized = bufferDesc.ByteWidth / a_shape.vertexCount;
			for (const auto desc : { a_shape.desc, a_shape.shapeDesc }) {
				add(desc, static_cast<UINT>(desc & 0xF) * 4);
				add(desc, sized - sized % 4);
			}

			const auto& bound = a_shape.shape->GetModelData().modelBound;
			struct Reading
			{
				std::vector<Vec3> points;
				std::size_t       sane{ 0 };
				std::size_t       inside{ 0 };
				float             spread{ FLT_MAX };
			};
			const auto read = [&](const Candidate& a_candidate, bool a_floats) {
				Reading    reading;
				const UINT at = static_cast<UINT>((a_candidate.desc >> 2) & 0x3C);
				const UINT count = std::min<UINT>(a_shape.vertexCount, bufferDesc.ByteWidth / a_candidate.stride);
				if (at + (a_floats ? 12u : 6u) > a_candidate.stride) {
					return reading;
				}
				Vec3 low{ FLT_MAX, FLT_MAX, FLT_MAX };
				Vec3 high{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
				for (UINT i = 0; i < count; ++i) {
					const auto* vertex = bytes.data() + i * a_candidate.stride + at;
					Vec3        point{};
					if (a_floats) {
						std::memcpy(point.data(), vertex, sizeof(float) * 3);
					} else {
						std::uint16_t half[3];
						std::memcpy(half, vertex, sizeof(half));
						point = { DirectX::PackedVector::XMConvertHalfToFloat(half[0]), DirectX::PackedVector::XMConvertHalfToFloat(half[1]), DirectX::PackedVector::XMConvertHalfToFloat(half[2]) };
					}
					reading.points.push_back(point);
					const bool sane = std::ranges::all_of(point, [](float a_value) { return std::isfinite(a_value) && std::abs(a_value) < 8192.0f; });
					if (!sane) {
						continue;
					}
					++reading.sane;
					for (int axis = 0; axis < 3; ++axis) {
						low[axis] = std::min(low[axis], point[axis]);
						high[axis] = std::max(high[axis], point[axis]);
					}
					const float dx = point[0] - bound.center.x;
					const float dy = point[1] - bound.center.y;
					const float dz = point[2] - bound.center.z;
					reading.inside += std::sqrt(dx * dx + dy * dy + dz * dz) <= bound.radius * 1.5f + 1.0f ? 1 : 0;
				}
				if (reading.sane > 0) {
					reading.spread = std::max({ high[0] - low[0], high[1] - low[1], high[2] - low[2] });
				}
				return reading;
			};

			// All of them sane first, then the most inside the bounds, then the tightest
			Reading   best;
			Candidate chosen{ a_shape.desc, static_cast<UINT>(a_shape.desc & 0xF) * 4 };
			bool      chosenFloats = false;
			for (const auto& candidate : candidates) {
				for (const bool floats : { true, false }) {
					auto reading = read(candidate, floats);
					const auto better = std::tuple(reading.sane, reading.inside, -reading.spread) > std::tuple(best.sane, best.inside, -best.spread);
					if (!reading.points.empty() && better) {
						best = std::move(reading);
						chosen = candidate;
						chosenFloats = floats;
					}
				}
			}
			// Logged only when some of the points still don't make sense
			if (best.sane < best.points.size()) {
				INFO("Model pictures: {:08X} part, descriptions {:016X} and {:016X}, {} bytes for {} vertices, read with stride {} as {}, {} of {} sane, {} inside, {:.1f} across",
					a_formID, a_shape.desc, a_shape.shapeDesc, bufferDesc.ByteWidth, a_shape.vertexCount, chosen.stride, chosenFloats ? "floats" : "halves", best.sane, best.points.size(), best.inside, best.spread);
			}
			a_shape.desc = chosen.desc;
			a_shape.stride = chosen.stride;
			a_full = chosenFloats;

			// Only the sane ones frame the picture
			std::erase_if(best.points, [](const Vec3& a_point) { return !std::ranges::all_of(a_point, [](float a_value) { return std::isfinite(a_value) && std::abs(a_value) < 8192.0f; }); });
			return best.points;
		}

		//---------------------------------------------------
		//-- Model Pictures ( State Backup ) ----------------
		//---------------------------------------------------

		// What we change on the game's context, put back as it was
		struct StateBackup
		{
			ComPtr<ID3D11InputLayout>        layout;
			D3D11_PRIMITIVE_TOPOLOGY         topology{};
			ComPtr<ID3D11Buffer>             vertexBuffer;
			UINT                             stride{ 0 };
			UINT                             offset{ 0 };
			ComPtr<ID3D11Buffer>             indexBuffer;
			DXGI_FORMAT                      indexFormat{};
			UINT                             indexOffset{ 0 };
			ComPtr<ID3D11VertexShader>       vertexShader;
			ComPtr<ID3D11Buffer>             vertexConstants;
			ComPtr<ID3D11PixelShader>        pixelShader;
			ComPtr<ID3D11Buffer>             pixelConstants;
			ComPtr<ID3D11ShaderResourceView> resource;
			ComPtr<ID3D11SamplerState>       sampler;
			ComPtr<ID3D11RasterizerState>    raster;
			D3D11_VIEWPORT                   viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
			UINT                             viewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
			ComPtr<ID3D11RenderTargetView>   target;
			ComPtr<ID3D11DepthStencilView>   depth;
			ComPtr<ID3D11BlendState>         blend;
			float                            blendFactor[4]{};
			UINT                             sampleMask{ 0 };
			ComPtr<ID3D11DepthStencilState>  depthState;
			UINT                             stencilRef{ 0 };

			explicit StateBackup(ID3D11DeviceContext* a_context) :
				context(a_context)
			{
				context->IAGetInputLayout(&layout);
				context->IAGetPrimitiveTopology(&topology);
				context->IAGetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
				context->IAGetIndexBuffer(&indexBuffer, &indexFormat, &indexOffset);
				context->VSGetShader(&vertexShader, nullptr, nullptr);
				context->VSGetConstantBuffers(0, 1, &vertexConstants);
				context->PSGetShader(&pixelShader, nullptr, nullptr);
				context->PSGetConstantBuffers(0, 1, &pixelConstants);
				context->PSGetShaderResources(0, 1, &resource);
				context->PSGetSamplers(0, 1, &sampler);
				context->RSGetState(&raster);
				context->RSGetViewports(&viewportCount, viewports);
				context->OMGetRenderTargets(1, &target, &depth);
				context->OMGetBlendState(&blend, blendFactor, &sampleMask);
				context->OMGetDepthStencilState(&depthState, &stencilRef);
			}

			~StateBackup()
			{
				context->IASetInputLayout(layout.Get());
				context->IASetPrimitiveTopology(topology);
				auto* buffer = vertexBuffer.Get();
				context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
				context->IASetIndexBuffer(indexBuffer.Get(), indexFormat, indexOffset);
				context->VSSetShader(vertexShader.Get(), nullptr, 0);
				auto* vertexConstantsBuffer = vertexConstants.Get();
				context->VSSetConstantBuffers(0, 1, &vertexConstantsBuffer);
				context->PSSetShader(pixelShader.Get(), nullptr, 0);
				auto* pixelConstantsBuffer = pixelConstants.Get();
				context->PSSetConstantBuffers(0, 1, &pixelConstantsBuffer);
				auto* view = resource.Get();
				context->PSSetShaderResources(0, 1, &view);
				auto* samplerState = sampler.Get();
				context->PSSetSamplers(0, 1, &samplerState);
				context->RSSetState(raster.Get());
				context->RSSetViewports(viewportCount, viewports);
				auto* targetView = target.Get();
				context->OMSetRenderTargets(1, &targetView, depth.Get());
				context->OMSetBlendState(blend.Get(), blendFactor, sampleMask);
				context->OMSetDepthStencilState(depthState.Get(), stencilRef);
			}

			StateBackup(const StateBackup&) = delete;
			StateBackup& operator=(const StateBackup&) = delete;

		private:
			ID3D11DeviceContext* context;
		};

		//---------------------------------------------------
		//-- Model Pictures ( Cache ) -----------------------
		//---------------------------------------------------

		// Drawn pictures are kept as small PNGs, named for the item and for what it's drawn from:
		// its model and the textures its record swaps in. A different mesh or texture path gets a
		// new name, so it's drawn again; redrawing everything clears the folder
		constexpr std::uint32_t pictureVersion = 1;	// raised when the drawing itself changes

		std::filesystem::path CacheFolder() {

			return std::filesystem::path(L"Data/SKSE/Plugins/CompletionistData/Pictures");
		}

		const RE::TESModel* ModelOf(RE::TESForm* a_form) {

			if (auto* armour = a_form ? a_form->As<RE::TESObjectARMO>() : nullptr) {
				return std::addressof(armour->worldModels[RE::SEXES::kMale]);
			}
			return a_form ? a_form->As<RE::TESModel>() : nullptr;
		}

		std::optional<std::filesystem::path> CachePath(std::uint32_t a_formID) {

			auto*       form = RE::TESForm::LookupByID(a_formID);
			const auto* file = form ? form->GetFile(0) : nullptr;
			const auto* model = ModelOf(form);
			const char* path = model ? model->GetModel() : nullptr;
			if (!file || !path || !*path) {
				return std::nullopt;
			}

			// FNV-1a, the same on every run
			std::uint32_t hash = 2166136261u;
			const auto    mix = [&](std::string_view a_text) {
                for (const char c : a_text) {
                    hash = (hash ^ static_cast<std::uint8_t>(std::tolower(static_cast<unsigned char>(c)))) * 16777619u;
                }
                hash = (hash ^ 0xFFu) * 16777619u;
			};
			mix(path);
			mix(std::to_string(pictureVersion));
			if (const auto* swapped = skyrim_cast<const RE::TESModelTextureSwap*>(model)) {
				for (std::uint32_t i = 0; i < swapped->numAlternateTextures; ++i) {
					const auto& alternate = swapped->alternateTextures[i];
					const char* texture = alternate.textureSet ? alternate.textureSet->GetTexturePath(RE::BSTextureSet::Texture::kDiffuse) : nullptr;
					mix(alternate.name3D.c_str());
					mix(texture ? texture : "");
				}
			}
			const auto name = fmt::format("{}_{:06X}_{:08X}.png", file->GetFilename(), form->GetLocalFormID(), hash);
			return CacheFolder() / name;
		}

		void SaveToCache(ID3D11Texture2D* a_picture, std::uint32_t a_formID) {

			const auto path = CachePath(a_formID);
			if (!path) {
				return;
			}
			std::error_code error;
			std::filesystem::create_directories(path->parent_path(), error);
			if (FAILED(DirectX::SaveWICTextureToFile(gpu.context, a_picture, GUID_ContainerFormatPng, path->c_str(), &GUID_WICPixelFormat32bppBGRA, nullptr, true))) {
				INFO("Model pictures: couldn't save {}", path->string());
			}
		}

		// A kept picture back on the card, with its mips made again
		bool LoadFromCache(Icon& a_icon) {

			if (!a_icon.file || !Prepare()) {
				return false;
			}
			ComPtr<ID3D11ShaderResourceView> view;
			const auto result = DirectX::CreateWICTextureFromFileEx(gpu.device, gpu.context, a_icon.file->c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS, DirectX::WIC_LOADER_DEFAULT, nullptr, &view);
			if (FAILED(result)) {
				a_icon.file.reset();
				return false;
			}
			a_icon.view = std::move(view);
			return true;
		}

		//---------------------------------------------------
		//-- Model Pictures ( Draw ) ------------------------
		//---------------------------------------------------

		// The model turned a little and looked down on, fitted to the picture by its points.
		// False while its textures are still loading, so it's tried again on a later frame
		bool Draw(Icon& a_icon, std::uint32_t a_formID) {

			std::vector<Shape> shapes;
			Skipped            skipped;
			CollectShapes(a_icon.model.get(), Affine{}, a_icon.swaps, shapes, skipped);
			if (shapes.empty()) {
				a_icon.stage = Stage::kFailed;
				INFO("Model pictures: {:08X} has nothing to draw: {} hidden, {} skinned, {} without buffers, {} other geometry", a_formID, skipped.hidden, skipped.skinned, skipped.noBuffers, skipped.otherGeometry);
				return true;
			}
			const bool texturesReady = std::ranges::all_of(shapes, [](const Shape& a_shape) { return !a_shape.wantsTexture || a_shape.texture; });
			if (!texturesReady && ++a_icon.waited < textureWait) {
				return false;
			}
			if (!texturesReady) {
				INFO("Model pictures: {:08X} drawn without some of its textures, which never loaded", a_formID);
			}

			// Each shape's own points, read back from the card, to frame the picture on what's
			// really there; how wide the positions are is checked against the shape's bounds
			std::vector<std::vector<Vec3>> points(shapes.size());
			std::vector<bool>              full(shapes.size(), false);
			std::size_t                    vertices = 0;
			for (std::size_t i = 0; i < shapes.size(); ++i) {
				bool isFull = false;
				points[i] = ReadPositions(shapes[i], isFull, a_formID);
				full[i] = isFull;
				vertices += points[i].size();
			}

			// Turned about the up axis, then tipped so the top comes toward the viewer, who
			// looks along +y with z up
			Matrix turn{ { { std::cos(yaw), -std::sin(yaw), 0.0f }, { std::sin(yaw), std::cos(yaw), 0.0f }, { 0.0f, 0.0f, 1.0f } } };
			Matrix tip{ { { 1.0f, 0.0f, 0.0f }, { 0.0f, std::cos(pitch), -std::sin(pitch) }, { 0.0f, std::sin(pitch), std::cos(pitch) } } };
			const auto camera = Multiply(tip, turn);

			Vec3 low{ FLT_MAX, FLT_MAX, FLT_MAX };
			Vec3 high{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
			for (std::size_t i = 0; i < shapes.size(); ++i) {
				for (const auto& point : points[i]) {
					const auto seen = Apply(camera, Apply(shapes[i].transform, point));
					for (int axis = 0; axis < 3; ++axis) {
						low[axis] = std::min(low[axis], seen[axis]);
						high[axis] = std::max(high[axis], seen[axis]);
					}
				}
			}
			if (vertices == 0 || high[0] < low[0]) {
				a_icon.stage = Stage::kFailed;
				INFO("Model pictures: {:08X} has no points to draw", a_formID);
				return true;
			}
			const float across = std::max(high[0] - low[0], high[2] - low[2]);
			const float half = std::max(across * 0.55f, 0.001f);
			const float centreX = (low[0] + high[0]) * 0.5f;
			const float centreZ = (low[2] + high[2]) * 0.5f;
			const float deep = std::max((high[1] - low[1]) * 1.04f, 0.001f);

			// The picture: mips so it stays smooth when the list shows it small
			D3D11_TEXTURE2D_DESC desc{};
			desc.Width = desc.Height = iconSize;
			desc.MipLevels = 0;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			ComPtr<ID3D11Texture2D>        picture;
			ComPtr<ID3D11RenderTargetView> target;
			if (FAILED(gpu.device->CreateTexture2D(&desc, nullptr, &picture)) || FAILED(gpu.device->CreateRenderTargetView(picture.Get(), nullptr, &target)) || FAILED(gpu.device->CreateShaderResourceView(picture.Get(), nullptr, &a_icon.view))) {
				a_icon.stage = Stage::kFailed;
				return true;
			}

			auto* context = gpu.context;
			{
				StateBackup backup(context);
				constexpr float clear[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
				context->ClearRenderTargetView(target.Get(), clear);
				context->ClearDepthStencilView(gpu.depth.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
				auto* targetView = target.Get();
				context->OMSetRenderTargets(1, &targetView, gpu.depth.Get());
				context->OMSetBlendState(gpu.blend.Get(), nullptr, 0xFFFFFFFF);
				context->OMSetDepthStencilState(gpu.depthState.Get(), 0);
				const D3D11_VIEWPORT viewport{ 0.0f, 0.0f, static_cast<float>(iconSize), static_cast<float>(iconSize), 0.0f, 1.0f };
				context->RSSetViewports(1, &viewport);
				context->RSSetState(gpu.raster.Get());
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				context->VSSetShader(gpu.vertexShader.Get(), nullptr, 0);
				context->PSSetShader(gpu.pixelShader.Get(), nullptr, 0);
				auto* constants = gpu.constants.Get();
				context->VSSetConstantBuffers(0, 1, &constants);
				context->PSSetConstantBuffers(0, 1, &constants);
				auto* sampler = gpu.sampler.Get();
				context->PSSetSamplers(0, 1, &sampler);

				for (std::size_t i = 0; i < shapes.size(); ++i) {
					const auto& shape = shapes[i];
					if (points[i].empty()) {
						continue;
					}
					const auto desc64 = shape.desc;
					const auto flags = static_cast<std::uint16_t>(desc64 >> 44);
					const UINT stride = shape.stride;

					// Shape space to the picture: across, up, and depth into it, fitted to the points
					const auto     turned = Multiply(camera, shape.transform.rotate);
					const auto     moved = Apply(camera, shape.transform.translate);
					ShapeConstants values{};
					for (int column = 0; column < 3; ++column) {
						values.toPicture[0 * 4 + column] = turned[0][column] / half;
						values.toPicture[1 * 4 + column] = turned[2][column] / half;
						values.toPicture[2 * 4 + column] = turned[1][column] / deep;
						for (int row = 0; row < 3; ++row) {
							values.toView[row * 4 + column] = turned[row][column];
						}
					}
					values.toPicture[0 * 4 + 3] = (moved[0] - centreX) / half;
					values.toPicture[1 * 4 + 3] = (moved[2] - centreZ) / half;
					values.toPicture[2 * 4 + 3] = (moved[1] - low[1]) / deep + 0.01f;
					values.toPicture[3 * 4 + 3] = 1.0f;
					values.toView[3 * 4 + 3] = 1.0f;
					values.options[0] = shape.texture ? 1.0f : 0.0f;
					values.options[1] = (flags & RE::BSGraphics::Vertex::VF_NORMAL) ? 1.0f : 0.0f;
					values.options[2] = (flags & RE::BSGraphics::Vertex::VF_UV) ? 1.0f : 0.0f;
					context->UpdateSubresource(gpu.constants.Get(), 0, nullptr, &values, 0, 0);

					auto* texture = shape.texture;
					context->PSSetShaderResources(0, 1, &texture);
					context->IASetInputLayout(LayoutFor(desc64, full[i]));
					auto*      vertexBuffer = shape.vertices;
					const UINT offset = 0;
					context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
					context->IASetIndexBuffer(shape.indices, DXGI_FORMAT_R16_UINT, 0);
					context->DrawIndexed(shape.triangles * 3, 0, 0);
				}
				ID3D11ShaderResourceView* none = nullptr;
				context->PSSetShaderResources(0, 1, &none);
			}
			context->GenerateMips(a_icon.view.Get());
			a_icon.stage = Stage::kDone;
			SaveToCache(picture.Get(), a_formID);
			if (auto path = CachePath(a_formID); path && std::filesystem::exists(*path)) {
				a_icon.file = std::move(path);
			}
			a_icon.checked = true;
			a_icon.model.reset();
			a_icon.swaps.clear();
			return true;
		}

		//---------------------------------------------------
		//-- Model Pictures ( Load ) ------------------------
		//---------------------------------------------------

		// On the game thread, through the game's model database, as it loads any model: an
		// armour's model on the ground, anything else's own, with the textures its record
		// puts on it
		void Load(std::uint32_t a_formID) {

			const auto* source = ModelOf(RE::TESForm::LookupByID(a_formID));
			const char* path = source ? source->GetModel() : nullptr;

			RE::NiPointer<RE::NiNode> model;
			std::vector<Swap>         swaps;
			if (path && *path) {
				RE::BSModelDB::DBTraits::ArgsType args{};
				if (RE::BSModelDB::Demand(path, model, args) != RE::BSResource::ErrorCode::kNone || !model) {
					model.reset();
					INFO("Model pictures: couldn't load {} for {:08X}", path, a_formID);
				}
			}
			const auto* swapped = source ? skyrim_cast<const RE::TESModelTextureSwap*>(source) : nullptr;
			for (std::uint32_t i = 0; model && swapped && i < swapped->numAlternateTextures; ++i) {
				const auto& alternate = swapped->alternateTextures[i];
				const char* texture = alternate.textureSet ? alternate.textureSet->GetTexturePath(RE::BSTextureSet::Texture::kDiffuse) : nullptr;
				if (!texture || !*texture) {
					continue;
				}
				RE::NiPointer<RE::NiTexture> loaded;
				RE::BSShaderManager::GetTexture(texture, true, loaded, false);
				if (!loaded && _strnicmp(texture, "textures\\", 9) != 0) {
					RE::BSShaderManager::GetTexture(("textures\\" + std::string(texture)).c_str(), true, loaded, false);
				}
				if (!loaded) {
					INFO("Model pictures: {:08X} swaps {} on {} for {}, which didn't load", a_formID, alternate.name3D.c_str(), path, texture);
				}
				if (loaded) {
					swaps.push_back({ alternate.name3D.c_str(), std::move(loaded) });
				}
			}

			std::scoped_lock guard(lock);
			--loading;
			auto& icon = icons[a_formID];
			icon.model = std::move(model);
			icon.swaps = std::move(swaps);
			icon.stage = icon.model ? Stage::kLoaded : Stage::kFailed;
		}
	}

	namespace
	{
		//---------------------------------------------------
		//-- Model Pictures ( Step ) ------------------------
		//---------------------------------------------------

		// One step for one picture: from the disk when it's kept, else load the model and draw
		// it. Budgets are per frame; preparing everything allows more
		struct Budget
		{
			int frame{ -1 };
			int drawn{ 0 };
			int fromDisk{ 0 };
		};
		Budget budget;

		void Step(Icon& a_icon, std::uint32_t a_formID, bool a_preparing) {

			const int draws = a_preparing ? drawsWhilePreparing : drawsPerFrame;
			const int loads = a_preparing ? loadsWhilePreparing : loadsAtOnce;
			switch (a_icon.stage) {
			case Stage::kNone:
				if (!a_icon.checked) {
					a_icon.checked = true;
					if (auto path = CachePath(a_formID); path && std::filesystem::exists(*path)) {
						a_icon.file = std::move(path);
					}
				}
				if (a_icon.file) {
					if (budget.fromDisk < diskPerFrame) {
						++budget.fromDisk;
						if (LoadFromCache(a_icon)) {
							a_icon.stage = Stage::kDone;
						}
					}
					return;
				}
				// A few at a time, so a long page doesn't queue hundreds of loads on the game at once
				if (loading >= loads) {
					return;
				}
				if (auto* tasks = SKSE::GetTaskInterface()) {
					a_icon.stage = Stage::kLoading;
					++loading;
					tasks->AddTask([a_formID]() { Load(a_formID); });
				} else {
					a_icon.stage = Stage::kFailed;
				}
				return;
			case Stage::kLoaded:
				if (budget.drawn >= draws || !Prepare()) {
					return;
				}
				++budget.drawn;
				Draw(a_icon, a_formID);
				return;
			default:
				return;
			}
		}

		// Pictures not on screen lately leave the card once there are many; they're on disk
		void Trim(int a_frame) {

			std::size_t held = 0;
			for (const auto& [formID, icon] : icons) {
				held += icon.view ? 1 : 0;
			}
			if (held <= heldPictures) {
				return;
			}
			for (auto& [formID, icon] : icons) {
				if (icon.view && icon.file && a_frame - icon.lastUsed > 120) {
					icon.view.Reset();
					icon.stage = Stage::kNone;
				}
			}
		}
	}

	//---------------------------------------------------
	//-- Model Pictures ( Get ) -------------------------
	//---------------------------------------------------

	void* ModelPictures::Get(std::uint32_t a_formID) {

		std::unique_lock guard(lock);
		const int frame = ImGui::GetFrameCount();
		if (budget.frame != frame) {
			budget = { frame, 0, 0 };
		}
		auto& icon = icons[a_formID];
		icon.lastUsed = frame;
		if (icon.stage != Stage::kDone) {
			Step(icon, a_formID, false);
		}
		return icon.stage == Stage::kDone ? icon.view.Get() : nullptr;
	}

	//---------------------------------------------------
	//-- Model Pictures ( Tick ) ------------------------
	//---------------------------------------------------

	// Render thread, once a frame while the window's open: a redraw clears what's kept, then
	// preparing works through its list, each picture saved and let go of unless it's on screen
	void ModelPictures::Tick() {

		std::unique_lock guard(lock);
		const int frame = ImGui::GetFrameCount();
		if (budget.frame != frame) {
			budget = { frame, 0, 0 };
		}

		if (redrawAll) {
			redrawAll = false;
			std::error_code error;
			std::filesystem::remove_all(CacheFolder(), error);
			for (auto& [formID, icon] : icons) {
				if (icon.stage != Stage::kLoading) {
					icon = Icon{};
				}
			}
		}

		const auto limit = queue.size();
		for (std::size_t visit = 0; visit < limit && !queue.empty(); ++visit) {
			const auto formID = queue.front();
			queue.pop_front();
			auto& icon = icons[formID];
			if (icon.stage == Stage::kNone && !icon.checked) {
				icon.checked = true;
				if (auto path = CachePath(formID); path && std::filesystem::exists(*path)) {
					icon.file = std::move(path);
				}
			}
			// Kept already, or nothing to draw
			if (icon.file || icon.stage == Stage::kFailed || icon.stage == Stage::kDone) {
				++prepared;
				continue;
			}
			Step(icon, formID, true);
			if (icon.stage == Stage::kDone || icon.stage == Stage::kFailed) {
				++prepared;
				if (icon.stage == Stage::kDone && icon.file && frame - icon.lastUsed > 2) {
					icon.view.Reset();
					icon.stage = Stage::kNone;
				}
				continue;
			}
			// Still loading: others behind it get their turn
			queue.push_back(formID);
			if (budget.drawn >= drawsWhilePreparing && loading >= loadsWhilePreparing) {
				break;
			}
		}
		if (queue.empty()) {
			total = 0;
			prepared = 0;
		}
		Trim(frame);
	}

	//---------------------------------------------------
	//-- Model Pictures ( Prepare All ) -----------------
	//---------------------------------------------------

	void ModelPictures::SetItems(std::vector<std::uint32_t> a_formIDs) {

		std::unique_lock guard(lock);
		items = std::move(a_formIDs);
	}

	void ModelPictures::PrepareAll(bool a_redraw) {

		std::unique_lock guard(lock);
		redrawAll = a_redraw;
		queue.assign(items.begin(), items.end());
		total = static_cast<int>(queue.size());
		prepared = 0;
	}

	std::pair<int, int> ModelPictures::Progress() {

		std::unique_lock guard(lock);
		return { prepared, total };
	}

	// Game thread, before asking: how many items have no picture kept yet
	std::size_t ModelPictures::Missing() {

		std::vector<std::uint32_t> list;
		{
			std::unique_lock guard(lock);
			list = items;
		}
		std::size_t missing = 0;
		for (const auto formID : list) {
			const auto path = CachePath(formID);
			missing += path && !std::filesystem::exists(*path) ? 1 : 0;
		}
		return missing;
	}
}
