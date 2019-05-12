#include "Tracer.h"

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

#include "Hitable.h"
#include "Camera.h"
#include "Material.h"

#include <random>

using namespace std;

namespace RayTracer
{

	Tracer::Tracer()
		:m_image(nullptr)
	{
	}

	Tracer::~Tracer()
	{
		if (m_image) delete m_image;
		m_image = nullptr;
	}

	void Tracer::initialize(int w, int h, int c)
	{
		m_config.m_width = w;
		m_config.m_height = h;
		if (m_image != nullptr) delete m_image;
		m_image = new unsigned char[m_config.m_width * m_config.m_height * m_config.m_channel];
	}

	unsigned char *Tracer::render()
	{
		// viewport
		Vector3D lower_left_corner(-2.0, -1.0, -1.0);
		Vector3D horizontal(4.0, 0.0, 0.0);
		Vector3D vertical(0.0, 2.0, 0.0);
		Vector3D origin(0.0, 0.0, 0.0);

		// scene
		Hitable* world = randomScene();

		// camera
		Vector3D lookfrom(3, 4, 10);
		Vector3D lookat(0, 0, 0);
		float dist_to_focus = 10.0f;
		float aperture = 0.1f;
		Camera camera(lookfrom, lookat, 45,
			static_cast<float>(m_config.m_width) / m_config.m_height, aperture, dist_to_focus);
		int samples = 50;

		for (int row = m_config.m_height - 1; row >= 0; --row)
		{
			for (int col = 0; col < m_config.m_width; ++col)
			{
				Vector4D color;
				for (int sps = 0; sps < samples; ++sps)
				{
					float u = static_cast<float>(col + drand48()) / static_cast<float>(m_config.m_width);
					float v = static_cast<float>(row + drand48()) / static_cast<float>(m_config.m_height);
					Ray ray = camera.getRay(u, v);
					color += tracing(ray, world, 0);
				}
				color /= static_cast<float>(samples);
				color.w = 1.0f;
				// gamma correction.
				color = Vector4D(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);
				drawPixel(col, row, color);
			}
		}
		reinterpret_cast<HitableList*>(world)->clearHitable();
		delete world;
		return m_image;
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

	void Tracer::setRecursionDepth(int depth)
	{
		m_config.m_maxDepth = depth;
	}

	void Tracer::setCamera(const Vector3D & cameraPos, const Vector3D & target, const Vector3D & worldUp, float fovy, float aspect, float aperture, float focus_dist)
	{
	}

	Hitable *Tracer::randomScene()
	{
		int n = 500;
		HitableList *list = new HitableList();
		list->addHitable(new Sphere(Vector3D(0, -1000.0, 0), 1000, new Lambertian(Vector3D(0.5, 0.5, 0.5))));
		for (int a = -11; a < 11; ++a)
		{
			for (int b = -11; b < 11; ++b)
			{
				float choose_mat = drand48();
				Vector3D center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
				if ((center - Vector3D(4, 0.2, 0)).getLength() > 0.9)
				{
					// diffuse.
					if (choose_mat < 0.4f)
						list->addHitable(new Sphere(center, 0.2, new Lambertian
						(Vector3D(drand48()*drand48(),
							drand48()*drand48(),
							drand48()*drand48()))));
					// metal
					else if (choose_mat < 0.6f)
						list->addHitable(new Sphere(center, 0.2, new Metal
						(Vector3D(0.5f*(1.0f + drand48()),
							0.5f*(1.0f + drand48()),
							0.5f*(1.0f + drand48())),
							0.5f*drand48())));
					// glass
					else
						list->addHitable(new Sphere(center, 0.2, new Dielectric
						(1.5f)));
				}
			}
		}
		list->addHitable(new Sphere(Vector3D(0, 1, 0), 1.0, new Dielectric(1.5f)));
		list->addHitable(new Sphere(Vector3D(-4, 1, 0), 1.0, new Lambertian(Vector3D(0.4, 0.2, 0.1))));
		list->addHitable(new Sphere(Vector3D(4, 1, 0), 1.0, new Metal(Vector3D(0.7, 0.6, 0.5), 0.0f)));
		return list;
	}

	Vector4D Tracer::tracing(const Ray &r, Hitable *world, int depth)
	{
		HitRecord rec;
		if (world->hit(r, 0.001f, FLT_MAX, rec))
		{
			Ray scattered;
			Vector3D attenuation;
			if (depth < m_config.m_maxDepth && rec.m_material->scatter(r, rec, attenuation, scattered))
				return attenuation * tracing(scattered, world, depth + 1);
			else
				return Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
			//return backgroundColor(Ray(rec.m_position, target - rec.m_position), world) * 0.5f;
			//return rec.normal * 0.5f + Vector3D(0.5f, 0.5f, 0.5f);
		}
		else
		{
			float t = 0.5f * (r.getDirection().y + 1.0f);
			Vector4D ret = Vector3D(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3D(0.5f, 0.7f, 1.0f) * t;
			ret.w = 1.0f;
			return ret;
		}
	}

	float Tracer::hitSphere(const Vector3D &center, const float &radius, const Ray &ray)
	{
		Vector3D oc = ray.getOrigin() - center;
		float a = ray.getDirection().dotProduct(ray.getDirection());
		float b = oc.dotProduct(ray.getDirection()) * 2.0f;
		float c = oc.dotProduct(oc) - radius * radius;
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			return -1.0f;
		else
			return (-b - sqrt(discriminant)) / (2.0f * a);
	}

}
