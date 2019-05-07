#include "Hitable.h"

namespace RayTracer
{

	bool Sphere::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
	{
		Vector3D oc = ray.getOrigin() - m_center;
		float a = ray.getDirection().dotProduct(ray.getDirection());
		float b = oc.dotProduct(ray.getDirection());
		float c = oc.dotProduct(oc) - m_radius * m_radius;
		// ???.
		float discriminant = b * b - 1.0f * a * c;
		if (discriminant > 0)
		{
			float temp = (-b - sqrt(b * b - 1.0f * a * c)) / a;
			if (temp > t_min && temp < t_max)
			{
				ret.m_t = temp;
				ret.m_position = ray.pointAt(ret.m_t);
				ret.m_normal = (ret.m_position - m_center) / m_radius;
				ret.m_material = m_material;
				return true;
			}
			temp = (-b + sqrt(b * b - 1.0f * a * c)) / a;
			if (temp > t_min && temp < t_max)
			{
				ret.m_t = temp;
				ret.m_position = ray.pointAt(ret.m_t);
				ret.m_normal = (ret.m_position - m_center) / m_radius;
				ret.m_material = m_material;
				return true;
			}
		}
		return false;
	}

	bool HitableList::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
	{
		HitRecord tempRec;
		bool hitAny = false;
		double closestSoFar = t_max;
		for (unsigned int x = 0; x < m_list.size(); ++x)
		{
			if (m_list[x]->hit(ray, t_min, closestSoFar, tempRec))
			{
				hitAny = true;
				closestSoFar = tempRec.m_t;
				ret = tempRec;
			}
		}
		return hitAny;
	}

}