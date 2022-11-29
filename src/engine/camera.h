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
		const math::matrix m_vRight   = math::matrix({{1.f, 0.f, 0.f, 1.f}});
		const math::matrix m_vForward       = math::matrix({{0.f, 0.f, 1.f, 1.f}});
		const math::matrix m_vUp            = math::matrix({{0.f, 1.f, 0.f, 1.f}});
		bool world_to_screen(const ImVec3& in, ImVec2& out);
		ImVec3 m_vViewAngles;
	 private:
		math::matrix m_vOrigin;
		ImVec2 m_vViewPortSize;
		float m_fNear{};
		float m_fFar{};
		float m_fhFov{};
		[[nodiscard]] inline math::matrix translation_matrix() const;
		[[nodiscard]] inline math::matrix orientation_matrix() const;
		[[nodiscard]] math::matrix view_matrix() const;
		[[nodiscard]] math::matrix projection_matrix() const;

 	};

} // engine