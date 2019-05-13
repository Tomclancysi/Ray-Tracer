#include "Hitable.h"

namespace RayTracer
{

    bool Sphere::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        Vector3D oc = ray.getOrigin() - m_center;
        float a = ray.getDirection().dotProduct(ray.getDirection());
        float b = oc.dotProduct(ray.getDirection());
        float c = oc.dotProduct(oc) - m_radius * m_radius;
        // discriminant
        float discriminant = b * b - a * c;
        if (discriminant > 0)
        {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp > t_min && temp < t_max)
            {
                ret.m_t = temp;
                ret.m_position = ray.pointAt(ret.m_t);
                Vector3D::getSphereUV((ret.m_position - m_center)/m_radius, ret.m_texcoord);
                ret.m_normal = (ret.m_position - m_center) / m_radius;
                ret.m_material = m_material;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp > t_min && temp < t_max)
            {
                ret.m_t = temp;
                ret.m_position = ray.pointAt(ret.m_t);
                Vector3D::getSphereUV((ret.m_position - m_center)/m_radius, ret.m_texcoord);
                ret.m_normal = (ret.m_position - m_center) / m_radius;
                ret.m_material = m_material;
                return true;
            }
        }
        return false;
    }

    bool Sphere::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        box = AABB(m_center - Vector3D(m_radius, m_radius, m_radius),
                   m_center + Vector3D(m_radius, m_radius, m_radius));
        return true;
    }


//    bool XYRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
//    {
//        float t = (k-ray.getOrigin().z) / ray.getDirection().z;
//        if(t < t_min || t > t_max)
//            return false;
//        float x = ray.getOrigin().x + t * ray.getDirection().x;
//        float y = ray.getOrigin().y + t * ray.getDirection().y;
//        if(x < x0 || x > x1 || y < y0 || y > y1)
//            return false;
//        ret.m_texcoord.x = (x - x0)/(x1 - x0);
//        ret.m_texcoord.y = (y - y0)/(y1 - y0);
//        ret.m_t = t;
//        ret.m_position = ray.pointAt(t);
//        ret.m_normal = Vector3D(0.0f, 0.0f, 1.0f);
//        if(m_flipNormal)
//            ret.m_normal = -ret.m_normal;
//        ret.m_material = m_material;
//        return true;
//    }

//    bool XYRect::boundingBox(const float &t0, const float &t1, AABB &box) const
//    {
//        box = AABB(Vector3D(x0, y0, k-0.0001f), Vector3D(x1, y1, k+0.0001f));
//        return true;
//    }

//    bool XZRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
//    {
//        float t = (k-ray.getOrigin().y) / ray.getDirection().y;
//        if(t < t_min || t > t_max)
//            return false;
//        float x = ray.getOrigin().x + t * ray.getDirection().x;
//        float z = ray.getOrigin().z + t * ray.getDirection().z;
//        if(x < x0 || x > x1 || z < z0 || z > z1)
//            return false;
//        ret.m_texcoord.x = (x - x0)/(x1 - x0);
//        ret.m_texcoord.y = (z - z0)/(z1 - z0);
//        ret.m_t = t;
//        ret.m_position = ray.pointAt(t);
//        ret.m_normal = Vector3D(0.0f, 1.0f, 0.0f);
//        if(m_flipNormal)
//            ret.m_normal = -ret.m_normal;
//        ret.m_material = m_material;
//        return true;
//    }

//    bool XZRect::boundingBox(const float &t0, const float &t1, AABB &box) const
//    {
//        box = AABB(Vector3D(x0, k-0.0001f, z0), Vector3D(x1, k+0.0001f, z1));
//        return true;
//    }

//    bool YZRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
//    {
//        float t = (k-ray.getOrigin().x) / ray.getDirection().x;
//        if(t < t_min || t > t_max)
//            return false;
//        float y = ray.getOrigin().y + t * ray.getDirection().y;
//        float z = ray.getOrigin().z + t * ray.getDirection().z;
//        if(y < y0 || y > y1 || z < z0 || z > z1)
//            return false;
//        ret.m_texcoord.x = (y - y0)/(y1 - y0);
//        ret.m_texcoord.y = (z - z0)/(z1 - z0);
//        ret.m_t = t;
//        ret.m_position = ray.pointAt(t);
//        ret.m_normal = Vector3D(1.0f, 0.0f, 0.0f);
//        if(m_flipNormal)
//            ret.m_normal = -ret.m_normal;
//        ret.m_material = m_material;
//        return true;
//    }

//    bool YZRect::boundingBox(const float &t0, const float &t1, AABB &box) const
//    {
//        box = AABB(Vector3D(k-0.0001f, y0, z0), Vector3D(k+0.0001f, y1, z1));
//        return true;
//    }

    bool TTriangle::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        float n_dot_dir = m_normal.dotProduct(ray.getDirection());
        // no intersection.
        if(equal(n_dot_dir, 0.0))
            return false;
        float d = -m_normal.dotProduct(m_p0);
        float t = -(m_normal.dotProduct(ray.getOrigin()) + d)/n_dot_dir;
        if(t < t_min || t > t_max)
            return false;
        ret.m_t = t;
        ret.m_position = ray.pointAt(t);
        ret.m_material = m_material;
        // judge inside or not.
        Vector3D r = ret.m_position - m_p0;
        Vector3D q1 = m_p1 - m_p0;
        Vector3D q2 = m_p2 - m_p0;
        float q1_squaredLen = q1.getSquaredLength();
        float q2_squaredLen = q2.getSquaredLength();
        float q1_dot_q2 = q1.dotProduct(q2);
        float r_dot_q1 = r.dotProduct(q1);
        float r_dot_q2 = r.dotProduct(q2);
        float determinant = 1.0f/(q1_squaredLen * q2_squaredLen - q1_dot_q2 * q1_dot_q2);

        float omega1 = determinant * (q2_squaredLen * r_dot_q1 - q1_dot_q2 * r_dot_q2);
        float omega2 = determinant * (-q1_dot_q2 * r_dot_q1 + q1_squaredLen * r_dot_q2);
        if(omega1 + omega2 > 1.0f || omega1 < 0.0f || omega2 < 0.0f)
            return false;
        return true;
    }

    bool TTriangle::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        Vector3D minp, maxp;
        minp.x = fmin(m_p0.x, fmin(m_p1.x, m_p2.x));
        minp.y = fmin(m_p0.y, fmin(m_p1.y, m_p2.y));
        minp.z = fmin(m_p0.z, fmin(m_p1.z, m_p2.z));
        maxp.x = fmax(m_p0.x, fmax(m_p1.x, m_p2.x));
        maxp.y = fmax(m_p0.y, fmax(m_p1.y, m_p2.y));
        maxp.z = fmax(m_p0.z, fmax(m_p1.z, m_p2.z));
        box =  AABB(minp, maxp);
        return true;
    }

}
