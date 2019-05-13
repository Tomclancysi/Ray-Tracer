#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"
#include "AABB.h"
#include "Math/Vector2D.h"
#include <vector>
#include <QDebug>

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
        Vector2D m_texcoord;
        Vector3D m_position;
        Vector3D m_normal;
        Material *m_material;
    };

    struct Vertex
    {
        Vector3D m_position;
        Vector3D m_normal;
        Vector2D m_texcoord;
    };

    class Hitable
    {
    public:
        Hitable() = default;
        virtual ~Hitable() = default;
        virtual bool isLeaf() const { return true; }
        virtual void preRendering() {}
        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const = 0;
        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const = 0;
    };

    class Sphere : public Hitable
    {
    public:
        float m_radius;
        Vector3D m_center;
        Material *m_material;

        Sphere(const Vector3D &cen, const float r, Material *mat)
            :m_center(cen), m_radius(r), m_material(mat) {}
        virtual ~Sphere() { if (m_material)delete m_material; m_material = nullptr; };

        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const;
    };

    class TTriangle : public Hitable
    {
    public:
        Vector3D m_normal;
        Vector3D m_p0, m_p1, m_p2;
        Material *m_material;

        TTriangle(Vector3D p0, Vector3D p1, Vector3D p2, Material *mat)
            :m_p0(p0), m_p1(p1), m_p2(p2), m_material(mat)
        {
            m_normal = (p1 - p0).crossProduct(p2 - p0);
            m_normal.normalize();
        }
        virtual ~TTriangle() { if (m_material)delete m_material; m_material = nullptr; }

        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const;
    };

//    class XYRect : public Hitable
//    {
//    private:
//        bool m_flipNormal;
//        float x0, x1, y0, y1, k;
//        Material *m_material;

//    public:
//        XYRect() = default;
//        XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material *mat)
//            :x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), m_material(mat) {}
//        virtual ~XYRect() { /*if (m_material)delete m_material; m_material = nullptr;*/ };

//        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
//        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const;

//        void setFlipNormal(bool target){m_flipNormal = target;}
//    };

//    class XZRect : public Hitable
//    {
//    private:
//        bool m_flipNormal;
//        float x0, x1, z0, z1, k;
//        Material *m_material;

//    public:
//        XZRect() = default;
//        XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material *mat)
//            :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), m_material(mat), m_flipNormal(false) {}
//        virtual ~XZRect() { /*if (m_material)delete m_material; m_material = nullptr;*/ };

//        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
//        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const;

//        void setFlipNormal(bool target){m_flipNormal = target;}
//    };

//    class YZRect : public Hitable
//    {
//    private:
//        bool m_flipNormal;
//        float y0, y1, z0, z1, k;
//        Material *m_material;

//    public:
//        YZRect() = default;
//        YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material *mat)
//            :y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), m_material(mat), m_flipNormal(false) {}
//        virtual ~YZRect() { /*if (m_material)delete m_material; m_material = nullptr;*/ };

//        virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
//        virtual bool boundingBox(const float &t0, const float &t1, AABB &box) const;

//        void setFlipNormal(bool target){m_flipNormal = target;}
//    };
}

#endif // HITABLE_H
