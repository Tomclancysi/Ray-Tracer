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

    bool HitableList::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        if(m_list.size() < 1)
            return false;
        AABB tempBox;
        bool firstTrue = m_list[0]->boundingBox(t0, t1, tempBox);
        if(!firstTrue)
            return false;
        else
            box = tempBox;
        for(int x = 1;x < m_list.size();++x)
        {
            if(m_list[x]->boundingBox(t0, t1, tempBox))
                box = AABB::surroundingBox(box, tempBox);
            else
                return false;
        }
        return true;
    }

    bool MovingSphere::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        Vector3D center = getCurrentCenter(ray.getTime());
        Vector3D oc = ray.getOrigin() - center;
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
                Vector3D::getSphereUV((ret.m_position - center)/m_radius, ret.m_texcoord);
                ret.m_normal = (ret.m_position - center) / m_radius;
                ret.m_material = m_material;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp > t_min && temp < t_max)
            {
                ret.m_t = temp;
                ret.m_position = ray.pointAt(ret.m_t);
                Vector3D::getSphereUV((ret.m_position - center)/m_radius, ret.m_texcoord);
                ret.m_normal = (ret.m_position - center) / m_radius;
                ret.m_material = m_material;
                return true;
            }
        }
        return false;
    }

    bool MovingSphere::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        AABB box0(m_center0 - Vector3D(m_radius, m_radius, m_radius),
                  m_center0 + Vector3D(m_radius, m_radius, m_radius));
        AABB box1(m_center1 - Vector3D(m_radius, m_radius, m_radius),
                  m_center1 + Vector3D(m_radius, m_radius, m_radius));
        box = AABB::surroundingBox(box0, box1);
        return true;
    }

    Vector3D MovingSphere::getCurrentCenter(float time) const
    {
        return m_center0 + (m_center1 - m_center0) * ((time - m_time0)/(m_time1 - m_time0));
    }

    bool XYRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        float t = (k-ray.getOrigin().z) / ray.getDirection().z;
        if(t < t_min || t > t_max)
            return false;
        float x = ray.getOrigin().x + t * ray.getDirection().x;
        float y = ray.getOrigin().y + t * ray.getDirection().y;
        if(x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        ret.m_texcoord.x = (x - x0)/(x1 - x0);
        ret.m_texcoord.y = (y - y0)/(y1 - y0);
        ret.m_t = t;
        ret.m_position = ray.pointAt(t);
        ret.m_normal = Vector3D(0.0f, 0.0f, 1.0f);
        if(m_flipNormal)
            ret.m_normal = -ret.m_normal;
        ret.m_material = m_material;
        return true;
    }

    bool XYRect::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        box = AABB(Vector3D(x0, y0, k-0.0001f), Vector3D(x1, y1, k+0.0001f));
        return true;
    }

    bool XZRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        float t = (k-ray.getOrigin().y) / ray.getDirection().y;
        if(t < t_min || t > t_max)
            return false;
        float x = ray.getOrigin().x + t * ray.getDirection().x;
        float z = ray.getOrigin().z + t * ray.getDirection().z;
        if(x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        ret.m_texcoord.x = (x - x0)/(x1 - x0);
        ret.m_texcoord.y = (z - z0)/(z1 - z0);
        ret.m_t = t;
        ret.m_position = ray.pointAt(t);
        ret.m_normal = Vector3D(0.0f, 1.0f, 0.0f);
        if(m_flipNormal)
            ret.m_normal = -ret.m_normal;
        ret.m_material = m_material;
        return true;
    }

    bool XZRect::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        box = AABB(Vector3D(x0, k-0.0001f, z0), Vector3D(x1, k+0.0001f, z1));
        return true;
    }

    bool YZRect::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        float t = (k-ray.getOrigin().x) / ray.getDirection().x;
        if(t < t_min || t > t_max)
            return false;
        float y = ray.getOrigin().y + t * ray.getDirection().y;
        float z = ray.getOrigin().z + t * ray.getDirection().z;
        if(y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        ret.m_texcoord.x = (y - y0)/(y1 - y0);
        ret.m_texcoord.y = (z - z0)/(z1 - z0);
        ret.m_t = t;
        ret.m_position = ray.pointAt(t);
        ret.m_normal = Vector3D(1.0f, 0.0f, 0.0f);
        if(m_flipNormal)
            ret.m_normal = -ret.m_normal;
        ret.m_material = m_material;
        return true;
    }

    bool YZRect::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        box = AABB(Vector3D(k-0.0001f, y0, z0), Vector3D(k+0.0001f, y1, z1));
        return true;
    }

    Box::Box(const Vector3D &p0, const Vector3D &p1, Material *mat)
    {
        m_min = p0;
        m_max = p1;
        m_list = new HitableList();
        XYRect *zWall1 = new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, mat);
        XYRect *zWall2 = new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, mat);
        XZRect *yWall1 = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, mat);
        XZRect *yWall2 = new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, mat);
        YZRect *xWall1 = new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, mat);
        YZRect *xWall2 = new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, mat);
        zWall2->setFlipNormal(true);
        yWall2->setFlipNormal(true);
        xWall2->setFlipNormal(true);
        m_list->addHitable(zWall1);
        m_list->addHitable(zWall2);
        m_list->addHitable(yWall1);
        m_list->addHitable(yWall2);
        m_list->addHitable(xWall1);
        m_list->addHitable(xWall2);
    }

    Box::~Box()
    {
        m_list->clearHitable();
        delete m_list;
        m_list = nullptr;
    }

    bool Box::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
    {
        return m_list->hit(ray, t_min, t_max, ret);
    }

    bool Triangle::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const
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
//        if(n_dot_dir > 0.0f)
//            ret.m_normal = -m_normal;
//        else
//            ret.m_normal = m_normal;
        ret.m_material = m_material;
        // judge inside or not.
        Vector3D r = ret.m_position - m_p0;
        Vector3D q1 = m_p1 - m_p0;
        Vector3D q2 = m_p2 - m_p0;
        //r.normalize();q1.normalize();q2.normalize();
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

    bool Triangle::boundingBox(const float &t0, const float &t1, AABB &box) const
    {
        Vector3D minp, maxp;
        minp.x = min(m_p0.x, min(m_p1.x, m_p2.x));
        minp.y = min(m_p0.y, min(m_p1.y, m_p2.y));
        minp.z = min(m_p0.z, min(m_p1.z, m_p2.z));
        maxp.x = max(m_p0.x, max(m_p1.x, m_p2.x));
        maxp.y = max(m_p0.y, max(m_p1.y, m_p2.y));
        maxp.z = max(m_p0.z, max(m_p1.z, m_p2.z));
        box =  AABB(minp, maxp);
        return true;
    }

}
