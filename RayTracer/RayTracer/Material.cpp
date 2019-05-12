#include "Material.h"

namespace RayTracer
{
    bool Lambertian::scatter(const Ray &in, const HitRecord &rec,
        Vector3D &attenuation, Ray &scattered) const
    {
        Vector3D target = rec.m_position + rec.m_normal + Vector3D::randomInUnitSphere();
        scattered = Ray(rec.m_position, target - rec.m_position, in.getTime());
        attenuation = m_albedo->sample(rec.m_texcoord.x, rec.m_texcoord.y, rec.m_position);
        return true;
    }

    bool Metal::scatter(const Ray &in, const HitRecord &rec,
        Vector3D &attenuation, Ray &scattered) const
    {
        Vector3D reflectedDir = Vector3D::reflect(in.getDirection(), rec.m_normal);
        scattered = Ray(rec.m_position, reflectedDir + Vector3D::randomInUnitSphere() * m_fuzz);
        attenuation = m_albedo->sample(rec.m_texcoord.x, rec.m_texcoord.y, rec.m_position);
        return (scattered.getDirection().dotProduct(rec.m_normal) > 0.0f);
    }

    bool Dielectric::scatter(const Ray &in, const HitRecord &rec,
        Vector3D &attenuation, Ray &scattered) const
    {
        Vector3D outward_normal;
        Vector3D reflected = Vector3D::reflect(in.getDirection(), rec.m_normal);
        float ni_over_nt;
        attenuation = Vector3D(1.0f, 1.0f, 1.0f);
        Vector3D refracted;
        float reflect_prob;
        float cosine;
        // from inside to outside.
        if (in.getDirection().dotProduct(rec.m_normal) > 0.0f)
        {
            outward_normal = -rec.m_normal;
            ni_over_nt = refIdx;
            cosine = /*refIdx * */in.getDirection().dotProduct(rec.m_normal) / in.getDirection().getLength();
        }
        // from outside to inside.
        else
        {
            outward_normal = rec.m_normal;
            ni_over_nt = 1.0 / refIdx;
            cosine = -in.getDirection().dotProduct(rec.m_normal) / in.getDirection().getLength();
        }

        if (Vector3D::refract(in.getDirection(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, refIdx);
        }
        else
        {
            scattered = Ray(rec.m_position, reflected);
            reflect_prob = 1.0f;
        }
        if (drand48() < reflect_prob)
            scattered = Ray(rec.m_position, reflected);
        else
            scattered = Ray(rec.m_position, refracted);
        return true;
    }
}
