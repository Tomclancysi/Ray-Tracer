#ifndef RENDERER_H
#define RENDERER_H

#include "Ray.h"

/**
 * @projectName   RayTracer
 * @brief         Main renderer loop.
 * @author        YangWC
 * @date          2019-05-06
 */

namespace RayTracer
{

	class Hitable;
	class Vector3D;
	class Vector4D;
	class RayTracing
	{
	private:
		class Setting
		{
		public:
			int m_maxDepth;
			int m_width, m_height, m_channel;
			
			Setting():m_maxDepth(50), m_channel(4) {}
		};
		Setting m_config;
		unsigned char *m_image;

	public:
		RayTracing();
		~RayTracing();

		void initialize(int w, int h, int c = 4);

		unsigned char *render();

		int getWidth() const { return m_config.m_width; }
		int getHeight() const { return m_config.m_height; }
		int getChannel() const { return m_config.m_channel; }
		int getRecursionDepth() const { return m_config.m_maxDepth; }
		unsigned char *getImage() const { return m_image; }
		
		void setRecursionDepth(int depth);
		void setCamera(const Vector3D &cameraPos, const Vector3D &target, const Vector3D &worldUp,
			float fovy, float aspect, float aperture, float focus_dist);

	private:
		Hitable *randomScene();

		Vector4D tracing(const Ray &r, Hitable *world, int depth);

		float hitSphere(const Vector3D &center, const float &radius, const Ray &ray);
		void drawPixel(unsigned int x, unsigned int y, const Vector4D &color);
	};

}

#endif // RENDERER_H
