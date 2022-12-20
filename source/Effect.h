#pragma once

namespace dae
{
	class Texture;

	class Effect final
	{
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();
		Effect(const Effect& other) = delete;
		Effect& operator=(const Effect& other) = delete;
		Effect(Effect&& other) = delete;
		Effect& operator=(Effect&& other) = delete;

		enum class FilteringMethod
		{
			Point, Linear, Anisotropic, END
		};

		ID3DX11Effect* GetEffect() const;
		ID3DX11EffectTechnique* GetTechnique() const;

		inline void SetWorldViewProjectionMatrix(const Matrix& matrix)
		{
			// I know it looks cursed but trust me bro it works
			/*
			The last thing you need to do is update the data every frame using the SetMatrix(...) function of the (c++ side) 
			matrix effect variable, same as you update the vertex buffer etc. Using your camera, you can build the 
			WorldViewProjection matrix that you then pass to that function.
			Hint, you’ll have to reinterpret the Matrix data...
			*/
			m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&matrix));
		}

		void SetDiffuseMap(Texture* pDiffuseTexture);

		// Returns new filtering method
		FilteringMethod CycleFilteringMethods();
	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

		FilteringMethod m_FilteringMethod;

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}
