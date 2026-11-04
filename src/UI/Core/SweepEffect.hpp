#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <d3d11.h>
#include <wrl/client.h>

namespace CompletionistUI
{
	// A wave that runs out from the player over the world, drawn from the depth buffer so it hugs
	// the ground and walls. No game code in here: it takes plain numbers and D3D objects
	class SweepEffect
	{
	public:
		static constexpr float duration{ 9.0f };	// the gold stays on the finds until here
		static constexpr float waveDuration{ 3.2f };	// the wave and the darkened screen are gone by here
		static constexpr int   maxItems{ 16 };

		using Point = std::array<double, 3>;

		struct View
		{
			float worldToClip[4][4]{};	// NiCamera's worldToCam: row, column
			Point camera{};
			float nearPlane{ 1.0f };
			float farPlane{ 1.0f };
		};

		struct Wave
		{
			float              elapsed{ 0.0f };
			Point              origin{};
			float              reach{ 25000.0f };	// game units
			std::vector<Point> items;				// first is the one that was marked
			bool               firstMarked{ false };
		};

		// a_target and a_backBuffer are the frame being finished
		bool Draw(ID3D11Device* a_device, ID3D11DeviceContext* a_context, ID3D11Texture2D* a_backBuffer, ID3D11RenderTargetView* a_target, ID3D11ShaderResourceView* a_depth, const View& a_view, const Wave& a_wave);

		// How far the wave should go to be sure of reaching something this far away
		[[nodiscard]] static float ReachFor(float a_nearest);

	private:
		bool Prepare(ID3D11Device* a_device);
		bool PrepareScene(ID3D11Device* a_device, const D3D11_TEXTURE2D_DESC& a_desc);

		Microsoft::WRL::ComPtr<ID3D11VertexShader>       _vertex;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>        _pixel;
		Microsoft::WRL::ComPtr<ID3D11Buffer>             _constants;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>    _raster;
		Microsoft::WRL::ComPtr<ID3D11BlendState>         _blend;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  _depthState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>          _scene;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _sceneView;
		std::uint32_t                                    _sceneWidth{ 0 };
		std::uint32_t                                    _sceneHeight{ 0 };
		DXGI_FORMAT                                      _sceneFormat{ DXGI_FORMAT_UNKNOWN };
		bool                                             _failed{ false };
	};
}
