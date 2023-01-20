#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera final
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle) :
			origin{ _origin },
			fovAngle{ _fovAngle }
		{
		}


		Vector3 origin{};
		float fovAngle{ 90.f };
		float fov{ tanf((fovAngle * TO_RADIANS) / 2.f) };
		float aspectRatio{ 1.f };

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{};
		float totalYaw{};

		float nearPlane{ 0.1f };
		float farPlane{ 100.f };

		Matrix invViewMatrix{};
		Matrix viewMatrix{};
		Matrix projectionMatrix{};

		float baseMovementSpeed{ 15 };
		float speedMultiplier{ 4 };

		inline bool ShouldVertexBeClipped(const Vector4& v) const
		{
			return v.x < -1.f || v.x > 1.f || v.y < -1.f || v.y > 1.f;
		}

		void Initialize(float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f }, float _aspectRatio = 1.f)
		{
			fovAngle = _fovAngle;
			fov = tanf((fovAngle * TO_RADIANS) / 2.f);

			origin = _origin;

			aspectRatio = _aspectRatio;

			CalculateProjectionMatrix();
		}

		void CalculateViewMatrix()
		{
			//ONB => invViewMatrix
			//Inverse(ONB) => ViewMatrix

			right = Vector3::Cross(Vector3::UnitY, forward);
			up = Vector3::Cross(forward, right);
			// https://gamedev.net/forums/topic/388559-getting-a-up-vector-from-only-having-a-forward-vector/.

			invViewMatrix =
			{
				right,
				up,
				forward,
				origin
			};

			viewMatrix = Matrix::Inverse(invViewMatrix);

			//ViewMatrix => Matrix::CreateLookAtLH(...) [not implemented yet]
			//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
		}

		void CalculateProjectionMatrix()
		{
			projectionMatrix = Matrix::CreatePerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
			//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
		}

		const Matrix& GetViewMatrix() const
		{
			return viewMatrix;
		}

		const Matrix& GetInverseViewMatrix() const
		{
			return invViewMatrix;
		}

		const Matrix& GetProjectionMatrix() const
		{
			return projectionMatrix;
		}

		Matrix GetWorldViewProjection() const
		{
			return GetViewMatrix() * GetProjectionMatrix();
		}

		void Update(const Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			//Camera Update Logic
			//...

			float movementSpeed{ baseMovementSpeed };
			const float rotationSpeed{ 1 / 32.f };
			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

#pragma region Shift
			if (pKeyboardState[SDL_SCANCODE_LSHIFT] || pKeyboardState[SDL_SCANCODE_RSHIFT])
			{
				movementSpeed *= speedMultiplier;
			}
#pragma endregion 

#pragma region FovControls
			/*if (pKeyboardState[SDL_SCANCODE_LEFT])
			{
				if (fovAngle > 10)
				{
					fovAngle -= fovChangingSpeed * deltaTime;
				}
			}
			else if (pKeyboardState[SDL_SCANCODE_RIGHT])
			{
				if (fovAngle < 170)
				{
					fovAngle += fovChangingSpeed * deltaTime;
				}
			}*/
#pragma endregion 

#pragma region KeyboardOnly Controls
			if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])
			{
				origin += forward * movementSpeed * deltaTime;
			}
			else if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN])
			{
				origin -= forward * movementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT])
			{
				origin += right * movementSpeed * deltaTime;
			}
			else if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT])
			{
				origin -= right * movementSpeed * deltaTime;
			}
#pragma endregion 

			//Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

#pragma region Mousebased Movement
			// LMB
			if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				// RMB
				if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					origin += right * (-mouseY * movementSpeed * deltaTime);
				}
				else
				{
					origin += forward * (-mouseY * movementSpeed * deltaTime);
				}
			}
#pragma endregion

#pragma region Rotation
			if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
			{
				forward = Matrix::CreateRotationY(mouseX * rotationSpeed).TransformVector(forward);
				forward = Matrix::CreateRotationX(-mouseY * rotationSpeed).TransformVector(forward);
			}
#pragma endregion
			//Update Matrices
			CalculateViewMatrix();
			CalculateProjectionMatrix(); //Try to optimize this - should only be called once or when fov/aspectRatio changes
		}
	};
}