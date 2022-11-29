//
// Created by Vlad on 29.11.2022.
//
#pragma once
#include "Camera.h"
#include "../math/trigonometry.h"

namespace engine
{
	Camera::Camera()
	{
		m_fNear = 0.1f;
		m_fFar  = 100.f;
		m_fhFov = 90.f;
	}
	Camera::Camera(const ImVec3& vOrigin, float hFov, ImVec2 viewportSize) : Camera()
	{
		m_fhFov = abs(hFov);
		m_vOrigin = math::matrix(1, 4);

		m_vOrigin.at(0, 0) = vOrigin.x;
		m_vOrigin.at(0, 1) = vOrigin.y;
		m_vOrigin.at(0, 2) = vOrigin.z;
		m_vViewPortSize = viewportSize;
	}
	math::matrix Camera::translation_matrix() const
	{
		return math::matrix({
			{1.f, 0.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{-m_vOrigin.at(0, 0), -m_vOrigin.at(0, 1), -m_vOrigin.at(0, 2), 1.f},
		});
	}
	math::matrix Camera::orientation_matrix() const
	{
		return math::matrix({
			{m_vRight.x,        m_vUp.x,        m_vForward.x,        0.f },
			{m_vRight.y,        m_vUp.y,        m_vForward.y,        0.f },
			{m_vRight.z,        m_vUp.z,        m_vForward.z,        0.f },
			{0.f,               0.f,            0.f,                 1.f }
		});
	}
	ImVec3 Camera::GetAbsOrigin()
	{
		return {m_vOrigin.at(0,0),m_vOrigin.at(0,1), m_vOrigin.at(0,2)};
	}
	void Camera::SetAbsOrigin(const ImVec3& vec)
	{
		m_vOrigin.at(0,0) = vec.x;
		m_vOrigin.at(0,1) = vec.y;
		m_vOrigin.at(0,2) = vec.z;

	}
	math::matrix Camera::view_matrix() const
	{
		return translation_matrix() * orientation_matrix();
	}
	math::matrix Camera::projection_matrix() const
	{
		const float fRight = tanf(math::deg_to_rad(m_fhFov) / 2.f);
		const float fLeft  = -fRight;

		const float vFov = math::deg_to_rad(m_fhFov) * (m_vViewPortSize.y / m_vViewPortSize.x);

		const float fTop   = tanf(vFov/ 2.f);
		const float fBottom = -fTop;

		return math::matrix({
			{2.f / (fRight - fLeft), 0.f,                               0.f,                                              0.f},
			{0.f,                    2.f / (fTop - fBottom),            0.f,                                              0.f},
			{0.f,                    0.f,                               (m_fFar + m_fNear) / (m_fFar - m_fNear),          1.f},
			{0.f,                    0.f,                               -2.f * m_fNear * m_fFar / (m_fFar - m_fNear),     0.f},
		});

	}
	bool Camera::world_to_screen(const ImVec3& in, ImVec2& out)
	{
		auto projected = math::matrix({{in.x, in.y, in.z, 1.f}}) * (view_matrix() * projection_matrix());

		if (projected.at(0, 3) <= 0.f)
			return false;

		projected /= projected.at(0, 3);

		auto to_screen_matrix = math::matrix({
			{m_vViewPortSize.x / 2,  0.f,                     0.f, 0.f},
			{0.f,                    -m_vViewPortSize.y  / 2, 0.f, 0.f},
			{0.f,                    0.f,                     1.f, 0.f},
			{m_vViewPortSize.x  / 2, m_vViewPortSize.y  / 2,  0.f, 1.f},
		});

		projected *= to_screen_matrix;
		out = {projected.at(0,0), projected.at(0, 1)};
		return true;

	}
} // engine