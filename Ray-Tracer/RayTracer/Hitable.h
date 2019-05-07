#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"
#include <vector>

/**
 * @projectName   RayTracer
 * @brief         An object that could be hit by a given ray.
 * @author        YangWC
 * @date          2019-05-06
 */

namespace RayTracer
{

	class Material;
	struct HitRecord
	{
		float m_t;
		Vector3D m_position;
		Vector3D m_normal;
		Material *m_material;
	};

	class Hitable
	{
	public:
		Hitable() = default;
		virtual ~Hitable() {}
		virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const = 0;
	};

	class Sphere : public Hitable
	{
	public:
		float m_radius;
		Vector3D m_center;
		Material *m_material;

		Sphere(const Vector3D &cen, const float r, Material *mat)
			:m_center(cen), m_radius(r), m_material(mat) {}
		~Sphere() { if (m_material)delete m_material; m_material = nullptr; };

		virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
	};

	class HitableList : public Hitable
	{
	public:
		std::vector<Hitable*> m_list;

		HitableList() = default;
		~HitableList() = default;

		void addHitable(Hitable *target) { m_list.push_back(target); }

		void clearHitable()
		{
			for (int x = 0; x < m_list.size(); ++x)
			{
				delete m_list[x];
				m_list[x] = nullptr;
			}
		}

		virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
	};
}

#endif // HITABLE_H
