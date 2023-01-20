#pragma once
#include "ShadedEffect.h"
#include "DataTypes.h"

namespace dae
{
	class Texture;

	class Mesh final
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::string& objFilePath, std::unique_ptr<Effect> pEffect);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh(Mesh&& other) = delete;
		Mesh& operator=(Mesh&& other) = delete;
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext) const;

		void RotateX(float angle);
		void RotateY(float angle);
		void RotateZ(float angle);

		void UpdateViewMatrices(const Matrix& viewProjectionMatrix, const Matrix& inverseViewMatrix);

		// Returns new filtering method
		void CycleFilteringMethods();

	private:
		std::unique_ptr<Effect> m_pEffect{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3D11Buffer* m_pVertexBuffer{};

		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pIndexBuffer{};

		// WorldOrientation
		Matrix m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
		Matrix m_RotationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
		Matrix m_ScaleMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
	};
}

