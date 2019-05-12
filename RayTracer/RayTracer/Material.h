#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

/**
 * @projectName   RayTracer
 * @brief         Object's material class.
 * @author        YangWC
 * @date          2019-05-06
 */

namespace RayTracer
{

    class Material
    {
    public:
        Material() = default;
        virtual ~Material() = default;

        virtual bool scatter(const Ray &in, const HitRecord &rec,
            Vector3D &attenuation, Ray &scattered) const = 0;

        virtual Vector3D emitted(const float &u, const float &v, const Vector3D &p) const
        {
            return Vector3D(0.0f, 0.0f, 0.0f);
        }
    };

    class Lambertian : public Material
    {
    private:
        Texture *m_albedo;

    public:
        Lambertian(Texture *a) : m_albedo(a) {}
        virtual ~Lambertian() { if(m_albedo)delete m_albedo;m_albedo = nullptr; }

        virtual bool scatter(const Ray &in, const HitRecord &rec,
            Vector3D &attenuation, Ray &scattered) const;
    };

    class Metal : public Material
    {
    private:
        float m_fuzz;
        Texture *m_albedo;

    public:
        Metal(Texture *a, const float &f) : m_albedo(a), m_fuzz(f) { if (f > 1.0f)m_fuzz = 1.0f; }
        virtual ~Metal() { if(m_albedo)delete m_albedo;m_albedo = nullptr; }

        virtual bool scatter(const Ray &in, const HitRecord &rec,
            Vector3D &attenuation, Ray &scattered) const;
    };

    class Dielectric : public Material
    {
    private:
        float refIdx;

        float schlick(float cosine, float ref_idx) const
        {
            float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
            r0 = r0 * r0;
            return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
        }

    public:
        Dielectric(float ri) : refIdx(ri) {}
        virtual ~Dielectric() = default;

        virtual bool scatter(const Ray &in, const HitRecord &rec,
            Vector3D &attenuation, Ray &scattered) const;
    };

    class DiffuseLight : public Material
    {
    private:
        Texture *m_emitTex;
    public:
        DiffuseLight(Texture *a) : m_emitTex(a) { }

        virtual bool scatter(const Ray &in, const HitRecord &rec,
            Vector3D &attenuation, Ray &scattered) const
        {
            return false;
        }

        virtual Vector3D emitted(const float &u, const float &v, const Vector3D &p) const
        {
            return m_emitTex->sample(u, v, p);
        }
    };

}

#endif // MATERIAL_H
