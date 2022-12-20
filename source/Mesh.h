#pragma once
#include "Effect.h"
#include "DataTypes.h"

namespace dae
{
	class Texture;

	class Mesh final
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh(Mesh&& other) = delete;
		Mesh& operator=(Mesh&& other) = delete;
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext) const;

		void SetWorldViewProjectionMatrix(const Matrix& matrix);

		// Returns new filtering method
		Effect::FilteringMethod CycleFilteringMethods();

	private:
		std::unique_ptr<Effect> m_pEffect{};
		std::unique_ptr<Texture> m_pDiffuseTexture{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pVertexBuffer{};

		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pIndexBuffer{};
	};
}

