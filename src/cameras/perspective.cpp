// std includes
#include <iostream>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "perspective.h"
#include "../macros.h"
#include "core/input.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

	CameraPersp::CameraPersp(Transform transform, Film film, float speed, float sensitivity, float fov) : Camera(transform, film, speed, sensitivity), m_fov(std::tan((fov * (PI / 180.0f)) * 0.5f))
	{
		LOG("Camera: Created a new perspective camera.");
	}

	void CameraPersp::update(float dt)
	{
		if (g_iKeys[SDL_SCANCODE_W])
		{
			move(m_transform.getOrientation().getForwardVector(), dt * m_speed);
		}
		else if (g_iKeys[SDL_SCANCODE_S])
		{
			move(m_transform.getOrientation().getForwardVector(), -dt * m_speed);
		}
		if (g_iKeys[SDL_SCANCODE_A])
		{
			move(m_transform.getOrientation().getRightVector(), dt * m_speed);
		}
		else if (g_iKeys[SDL_SCANCODE_D])
		{
			move(m_transform.getOrientation().getRightVector(), -dt * m_speed);
		}
		if (g_iKeys[SDL_SCANCODE_R])
		{
			move(m_transform.getOrientation().getUpVector(), dt * m_speed);
		}
		else if (g_iKeys[SDL_SCANCODE_F])
		{
			move(m_transform.getOrientation().getUpVector(), -dt * m_speed);
		}

		if (g_iKeys[SDL_SCANCODE_UP])
		{
			rotate(m_transform.getOrientation().getRightVector(), -dt * m_sensitivity);
		}
		else if (g_iKeys[SDL_SCANCODE_DOWN])
		{
			rotate(m_transform.getOrientation().getRightVector(), dt * m_sensitivity);
		}
		if (g_iKeys[SDL_SCANCODE_LEFT])
		{
			rotate(m_transform.getOrientation().getUpVector(), dt * m_sensitivity);
		}
		else if (g_iKeys[SDL_SCANCODE_RIGHT])
		{
			rotate(m_transform.getOrientation().getUpVector(), -dt * m_sensitivity);
		}
		if (g_iKeys[SDL_SCANCODE_Q])
		{
			rotate(m_transform.getOrientation().getForwardVector(), dt * m_sensitivity);
		}
		else if (g_iKeys[SDL_SCANCODE_E])
		{
			rotate(m_transform.getOrientation().getForwardVector(), -dt * m_sensitivity);
		}

		if (g_iKeys[SDL_SCANCODE_KP_PLUS])
		{
			m_fov += 0.1f * dt;
			m_film.clearSamples();
		}
		else if (g_iKeys[SDL_SCANCODE_KP_MINUS])
		{
			m_fov -= 0.1f * dt;
			m_film.clearSamples();
		}
	}

	void CameraPersp::calcCamRay(const int x, const int y, Ray & ray) const
	{
		// Tent filter for each ray's xy directions
		float r1 = 2.0f * pseudorand(), dx = r1 < 1.0f ? std::sqrt(r1) - 1.0f : 1.0f - std::sqrt(2.0f - r1);
		float r2 = 2.0f * pseudorand(), dy = r2 < 1.0f ? std::sqrt(r2) - 1.0f : 1.0f - std::sqrt(2.0f - r2);

		// Construct the ray's direction vector and aim it towards the virtual screen's pixel
		auto x_norm = ((m_film.getResolutionX() * 0.5f - x + dx) / m_film.getResolutionX() * m_film.getAspectRatio()) * m_fov;
		auto y_norm = ((m_film.getResolutionY() * 0.5f - y + dy) / m_film.getResolutionY()) * m_fov;
		auto v_norm = vec3(x_norm, y_norm, 1.0f);

		// Rotate the ray direction based on camera orientation
		quaternion q = m_transform.getOrientation();
		quaternion q_inv = q.conjugate();
		quaternion w(0.0f, v_norm.x, v_norm.y, v_norm.z);
		quaternion r = q * w * q_inv;

		// Assign final parameters to the output &ray
		ray.setOrigin(m_transform.getPosition());
		ray.setDirection(vec3(r.x, r.y, r.z));
	}

	void CameraPersp::setFoV(float fov)
	{
		m_fov = std::tan((fov * (PI / 180.0f)) * 0.5f);
	}

	float CameraPersp::getFoV() const
	{
		return m_fov;
	}

}
