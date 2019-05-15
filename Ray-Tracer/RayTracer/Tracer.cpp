#include "Tracer.h"

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

#include "Hitable.h"
#include "Camera.h"
#include "Material.h"
#include "BVHNode.h"
#include "Texture.h"
#include "SimpleMeshHitable.h"
#include "ModelHitable.h"

#include <random>
#include <time.h>

#include "tbb/parallel_for.h"

using namespace std;
using namespace tbb;

namespace RayTracer
{

	Tracer::Tracer()
		:m_image(nullptr), m_root(nullptr)
	{
	}

	Tracer::~Tracer()
	{
		if (m_image) delete m_image;
		m_image = nullptr;
		//endFrame();
	}

	void Tracer::initialize(int w, int h, int c)
	{
		// Image width and height.
		m_config.m_width = w;
		m_config.m_height = h;
		if (m_image != nullptr) delete m_image;
		if (m_config.m_camera != nullptr) delete m_config.m_camera;

		// Camera initialization.
		Vector3D lookfrom(0, 6, 21);
		Vector3D lookat(0, 0, 0);
		float dist_to_focus = 10.0f;
		float aperture = 0.0f;
		m_config.m_camera = new Camera(lookfrom, lookat, 45,
			static_cast<float>(m_config.m_width) / m_config.m_height,
			aperture, dist_to_focus);

		// Image buffer.
		m_image = new unsigned char[m_config.m_width * m_config.m_height * m_config.m_channel];

		// clear something.
		//endFrame();

		// manager.
		if (m_manager.m_textureMgr == nullptr)
			m_manager.m_textureMgr = TextureMgr::getSingleton();
		if (m_manager.m_materialMgr == nullptr)
			m_manager.m_materialMgr = MaterialMgr::getSingleton();
	}

	void Tracer::addObjects(Hitable * target)
	{
		m_objects.push_back(target);
	}

	void Tracer::beginFrame()
	{
		for (int x = 0; x < m_objects.size(); ++x)
			m_objects[x]->preRendering();
		m_root = new BVHNode(m_objects, 0, m_objects.size());
	}

	unsigned char *Tracer::render(double &totalTime)
	{
		m_config.startFrame = clock();

		Hitable* hitableNode = reinterpret_cast<Hitable*>(m_root);

		if (m_config.m_parallelForCpu == 1)
		{
			parallelThreadRender(hitableNode);
		}
		else
		{
			rawSerialRender(hitableNode);
		}

		m_config.endFrame = clock();
		m_config.totalFrameTime = static_cast<double>(m_config.endFrame - m_config.startFrame) / CLOCKS_PER_SEC;
		totalTime = m_config.totalFrameTime;

		return m_image;
	}

	void Tracer::endFrame()
	{
		// clear scene objects.
		BVHNode::destoryBVHTree(m_root);
		for (int x = 0; x < m_objects.size(); ++x)
		{
			delete m_objects[x];
			m_objects[x] = nullptr;
		}
	}

	void Tracer::drawPixel(unsigned int x, unsigned int y, const Vector4D &color)
	{
		if (x < 0 || x >= m_config.m_width || y < 0 || y >= m_config.m_height)
			return;
		unsigned int index = (y * m_config.m_width + x) * m_config.m_channel;
		m_image[index + 0] = static_cast<unsigned char>(255 * color.x);
		m_image[index + 1] = static_cast<unsigned char>(255 * color.y);
		m_image[index + 2] = static_cast<unsigned char>(255 * color.z);
		m_image[index + 3] = static_cast<unsigned char>(255 * color.w);
	}

	void Tracer::rawSerialRender(Hitable *scene)
	{
		for (int row = m_config.m_height - 1; row >= 0; --row)
		{
			for (int col = 0; col < m_config.m_width; ++col)
			{
				Vector4D color;
				for (int sps = 0; sps < m_config.m_samplings; ++sps)
				{
					float u = static_cast<float>(col + drand48()) / static_cast<float>(m_config.m_width);
					float v = static_cast<float>(row + drand48()) / static_cast<float>(m_config.m_height);
					Ray ray = m_config.m_camera->getRay(u, v);
					color += tracing(ray, scene, 0);
				}
				color /= static_cast<float>(m_config.m_samplings);
				color.w = 1.0f;
				// gamma correction.
				color = Vector4D(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);
				drawPixel(col, row, color);
			}
		}
	}

	void Tracer::parallelThreadRender(Hitable *scene)
	{
		parallel_for(blocked_range<size_t>(0, m_config.m_height * m_config.m_width, 10000),
			[&](blocked_range<size_t>& r)
		{
			for (size_t i = r.begin(); i != r.end(); ++i)
			{
				Vector4D color;
				size_t col = i % m_config.m_width;
				size_t row = i / m_config.m_width;
				for (int x = 0; x < m_config.m_samplings; ++x)
				{
					float u = static_cast<float>(col + drand48()) / static_cast<float>(m_config.m_width);
					float v = static_cast<float>(row + drand48()) / static_cast<float>(m_config.m_height);
					Ray ray = m_config.m_camera->getRay(u, v);
					color += tracing(ray, scene, 0);
				}
				color /= static_cast<float>(m_config.m_samplings);
				color.w = 1.0f;
				// gamma correction.
				color = Vector4D(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);
				drawPixel(col, row, color);
			}
		}, auto_partitioner());
	}

	Vector4D Tracer::tracing(const Ray &r, Hitable *world, int depth)
	{
		HitRecord rec;
		if (world->hit(r, 0.001f, FLT_MAX, rec))
		{
			Ray scattered;
			Vector3D attenuation;
			Material::ptr material = m_manager.m_materialMgr->getMaterial(rec.m_material);
			Vector3D emitted = material->emitted(rec.m_texcoord.x, rec.m_texcoord.y, rec.m_position);
			if (depth < m_config.m_maxDepth && material->scatter(r, rec, attenuation, scattered))
				return emitted + attenuation * tracing(scattered, world, depth + 1);
			else
				return emitted;
		}
		else
		{
			float t = 0.5f * (r.getDirection().y + 1.0f);
			Vector4D ret = Vector3D(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3D(0.5f, 0.7f, 1.0f) * t;
			ret.w = 1.0f;
			return ret;
			//return Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}
