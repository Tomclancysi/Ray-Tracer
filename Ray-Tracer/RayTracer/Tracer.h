#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Ray.h"
#include "TextureMgr.h"
#include "MaterialMgr.h"

/**
 * @projectName   RayTracer
 * @brief         Main renderer logic.
 * @author        YangWC
 * @date          2019-05-06
 */

namespace RayTracer
{

	class Camera;
	class Hitable;
	class Vector3D;
	class Vector4D;
	class BVHNode;
	class Tracer
	{
	private:
		class Manager
		{
		public:
			TextureMgr::ptr m_textureMgr;
			MaterialMgr::ptr m_materialMgr;
		};

		class Setting
		{
		public:
			Camera *m_camera;
			int m_parallelForCpu;
			unsigned int m_maxDepth;
			unsigned int m_samplings;
			int m_width, m_height, m_channel;
			double startFrame, endFrame, totalFrameTime;

			Setting() :m_maxDepth(50), m_samplings(10),
				m_channel(4), m_camera(nullptr), m_parallelForCpu(1) {}
			~Setting()
			{
				if (m_camera)delete m_camera;
				m_camera = nullptr;
			}
		};
		Setting m_config;								// Configuration.
		Manager m_manager;								// Manager component.
		BVHNode *m_root;								// BVH tree root node.
		unsigned char *m_image;							// Render target.
		std::vector<Hitable*> m_objects;				// Scene object lists.

	public:
		Tracer();
		~Tracer();

		void setParallelThread(int way) { m_config.m_parallelForCpu = way; }
		void setSamplingNums(unsigned int samp) { m_config.m_samplings = samp; }
		void setRecursionDepth(unsigned int depth) { m_config.m_maxDepth = depth; }

		int getWidth() const { return m_config.m_width; }
		int getHeight() const { return m_config.m_height; }
		int getChannel() const { return m_config.m_channel; }
		double getTotalFrameTime() const { return m_config.totalFrameTime; }
		unsigned int getSamplings() const { return m_config.m_samplings; }
		unsigned int getRecursionDepth() const { return m_config.m_maxDepth; }
		unsigned char *getImage() const { return m_image; }
		Camera *getCamera() const { return m_config.m_camera; }
		TextureMgr::ptr getTextureMgr() const { return m_manager.m_textureMgr; }
		MaterialMgr::ptr getMaterialMgr() const { return m_manager.m_materialMgr; }

		void addObjects(Hitable *target);
		void initialize(int w, int h, int c = 4);
		void beginFrame();
		void endFrame();

		unsigned char *render(double &totalTime);

	private:
		Vector4D tracing(const Ray &r, Hitable *world, int depth);
		void drawPixel(unsigned int x, unsigned int y, const Vector4D &color);

		void rawSerialRender(Hitable *scene);
		void parallelThreadRender(Hitable *scene);
	};

}

#endif // RENDERER_H
