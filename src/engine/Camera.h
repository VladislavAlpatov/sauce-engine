//
// Created by Vlad on 29.11.2022.
//
#pragma once
#include "../math/ImVec3.h"
#include "../math/matrix.h"


namespace engine
{

	class Camera
	{
	 public:
		Camera();
		Camera(const ImVec3& vOrigin, float hFov, ImVec2 viewportSize);
		ImVec3 GetAbsOrigin();
		void   SetAbsOrigin(const ImVec3& vec);
		const ImVec3 m_vRight   = {1.f, 0.f, 0.f};
		const ImVec3 m_vForward = {0.f, 1.f, 0.f};
		const ImVec3 m_vUp      = {0.f, 0.f, 1.f};

	 private:
		math::matrix m_vOrigin;
		ImVec3 m_vViewAngles;
		ImVec2 m_vViewPortSize;
		float m_fNear{};
		float m_fFar{};
		float m_fhFov{};
		[[nodiscard]] inline math::matrix translation_matrix() const;
		[[nodiscard]] inline math::matrix orientation_matrix() const;
		[[nodiscard]] math::matrix view_matrix() const;
		[[nodiscard]] math::matrix view_projection_matrix() const;
		bool world_to_screen(const ImVec3& in, ImVec2& out);

 	};

} // engine