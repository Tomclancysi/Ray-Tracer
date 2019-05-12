#include "Tracer.h"

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

#include "Hitable.h"
#include "Camera.h"
#include "Material.h"
#include "BVHNode.h"
#include "Texture.h"

#include <random>
#include <time.h>
#include <QDebug>

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

    unsigned char *Tracer::render(double &totalTime)
    {
        clock_t start, finish;

        start = clock();
        // viewport
        Vector3D lower_left_corner(-2.0, -1.0, -1.0);
        Vector3D horizontal(4.0, 0.0, 0.0);
        Vector3D vertical(0.0, 2.0, 0.0);
        Vector3D origin(0.0, 0.0, 0.0);

        // scene
        //Hitable* world = randomScene();
        //Hitable *world = twoSpheresScene();
        Hitable *world = simpleLightScene();
        //Hitable *world = cornellBoxScene();
        std::vector<Hitable*> list = reinterpret_cast<HitableList*>(world)->getHitableList();

        // create bvh tree.
        BVHNode* sceneNode = new BVHNode(list, 0, list.size());
        Hitable* hitableNode = reinterpret_cast<Hitable*>(sceneNode);

        // camera
        Vector3D lookfrom(15, 4, 2);
        Vector3D lookat(0, 0, 0);
//        Vector3D lookfrom(278, 278, -800);
//        Vector3D lookat(278, 278, 0);
        //float dist_to_focus = (lookfrom - lookat).getLength();
        float dist_to_focus = 10.0f;
        float aperture = 0.0f;
        Camera camera(lookfrom, lookat, 60,
            static_cast<float>(m_config.m_width) / m_config.m_height,
                      aperture, dist_to_focus,
                      0.0, 1.0);
        int samples = 1200;

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
                    color += tracing(ray, hitableNode, 0);
                }
                color /= static_cast<float>(samples);
                color.w = 1.0f;
                // gamma correction.
                color = Vector4D(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);
                drawPixel(col, row, color);
            }
        }

        // destory bvh tree.
        BVHNode::destoryBVHTree(sceneNode);
        // clear scene object list.
        reinterpret_cast<HitableList*>(world)->clearHitable();
        delete world;

        finish = clock();
        totalTime = static_cast<double>(finish - start)/CLOCKS_PER_SEC;

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

    Vector4D Tracer::tracing(const Ray &r, Hitable *world, int depth)
    {
        HitRecord rec;
        if (world->hit(r, 0.001f, FLT_MAX, rec))
        {
            Ray scattered;
            Vector3D attenuation;
            Vector3D emitted = rec.m_material->emitted(rec.m_texcoord.x, rec.m_texcoord.y, rec.m_position);
            if (depth < m_config.m_maxDepth && rec.m_material->scatter(r, rec, attenuation, scattered))
                return emitted + attenuation * tracing(scattered, world, depth + 1);
            else
                return emitted;
        }
        else
        {
//            float t = 0.5f * (r.getDirection().y + 1.0f);
//            Vector4D ret = Vector3D(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3D(0.5f, 0.7f, 1.0f) * t;
//            ret.w = 1.0f;
//            return ret;
            return Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }

    void Tracer::setRecursionDepth(int depth)
    {
        m_config.m_maxDepth = depth;
    }

    void Tracer::setCamera(const Vector3D & cameraPos,
                           const Vector3D & target,
                           const Vector3D & worldUp,
                           float fovy, float aspect,
                           float aperture, float focus_dist)
    {
    }

    Hitable *Tracer::randomScene()
    {
        int n = 500;
        HitableList *list = new HitableList();

        Texture *checker = new CheckerTexture(
                    new ConstantTexture(Vector3D(0.2, 0.3, 0.1)),
                    new ConstantTexture(Vector3D(0.9, 0.9, 0.9)));
        list->addHitable(new Sphere(Vector3D(0, -1000.0, 0), 1000,
                                    new Lambertian(checker)));
        for (int a = -11; a < 11; ++a)
        {
            for (int b = -11; b < 11; ++b)
            {
                float choose_mat = drand48();
                Vector3D center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
                if ((center - Vector3D(4, 0.2, 0)).getLength() > 0.9)
                {
                    // diffuse.
                    if (choose_mat < 0.8f)
                        list->addHitable(new Sphere(center, 0.2, new Lambertian
                        (new ConstantTexture(
                             Vector3D(drand48()*drand48(),
                            drand48()*drand48(),
                            drand48()*drand48())))));
//                        list->addHitable(new MovingSphere(center, center+Vector3D(0,0.5*drand48(),0),
//                                                          0.0,1.0,0.2,
//                                                          new Lambertian(
//                                                              Vector3D(drand48()*drand48(),
//                                                                       drand48()*drand48(),
//                                                                       drand48()*drand48()))));
//                    // metal
                    else if (choose_mat < 0.95f)
                        list->addHitable(new Sphere(center, 0.2, new Metal
                        (new ConstantTexture(
                             Vector3D(0.5f*(1.0f + drand48()),
                            0.5f*(1.0f + drand48()),
                            0.5f*(1.0f + drand48()))),
                            0.5f*drand48())));
                    // glass
                    else
                        list->addHitable(new Sphere(center, 0.2, new Dielectric
                        (1.5f)));
                }
            }
        }
        list->addHitable(new Sphere(Vector3D(0, 1, 0), 1.0,
                                    new Dielectric(1.5f)));
        list->addHitable(new Sphere(Vector3D(-4, 1, 0), 1.0,
                                    new Lambertian(new ConstantTexture(
                                                       Vector3D(0.4, 0.2, 0.1)))));
        list->addHitable(new Sphere(Vector3D(4, 1, 0), 1.0,
                                    new Metal(new ConstantTexture(
                                                  Vector3D(0.7, 0.6, 0.5)), 0.0f)));
        return list;
    }

    Hitable *Tracer::twoSpheresScene()
    {
        Texture *checker = new CheckerTexture(
                    new ConstantTexture(Vector3D(0.2f,0.3f,0.1f)),
                    new ConstantTexture(Vector3D(0.9f,0.9f,0.9f)));
        HitableList *list = new HitableList();
        ImageTexture *earth = new ImageTexture("./res/earth.jpg");
        list->addHitable(new Sphere(Vector3D(0, -1000, 0),1000, new Lambertian(checker)));
        list->addHitable(new Sphere(Vector3D(-2, +1.0, 0),1.0, new Lambertian(earth)));
        list->addHitable(new Sphere(Vector3D(2, +1.0, 0),1.0, new Dielectric(1.5f)));
        return list;
    }

    Hitable *Tracer::simpleLightScene()
    {
        Texture *checker = new CheckerTexture(
                    new ConstantTexture(Vector3D(0.2f,0.3f,0.1f)),
                    new ConstantTexture(Vector3D(0.9f,0.9f,0.9f)));
        HitableList *list = new HitableList();
        //ImageTexture *earth = new ImageTexture("./res/earth.jpg");
        Vector3D p0(0,3,0),p1(0,0,4),p2(4,0,0);
        list->addHitable(new Sphere(Vector3D(0, -1000, 0), 1000, new Lambertian(checker)));
        //list->addHitable(new Sphere(Vector3D(0, +2.0f, 0), 2.0, new Lambertian(earth)));
        list->addHitable(new Triangle(p0, p1, p2, new DiffuseLight(
                                          new ConstantTexture(Vector3D(4,4,4)))));
        list->addHitable(new Sphere(Vector3D(0, +2.0f, 4.5), 2.0, new Dielectric(1.5f)));
        list->addHitable(new Sphere(Vector3D(0, +2.0f,-4.5), 2.0, new Dielectric(1.5f)));
//        list->addHitable(new Sphere(Vector3D(0, +7.0f, 0), 2.0, new DiffuseLight(
//                                        new ConstantTexture(Vector3D(4.0f, 4.0f, 4.0f)))));
//        list->addHitable(new XYRect(3, 5, 1, 3, -2, new DiffuseLight(
//                                        new ConstantTexture(Vector3D(4,4,4)))));
        return list;
    }

    Hitable *Tracer::cornellBoxScene()
    {
        HitableList *list = new HitableList();
        Material *red = new Lambertian(new ConstantTexture(Vector3D(0.65, 0.05, 0.05)));
        Material *white = new Lambertian(new ConstantTexture(Vector3D(0.73, 0.73, 0.73)));
        Material *green = new Lambertian(new ConstantTexture(Vector3D(0.12, 0.45, 0.15)));
        Material *light = new DiffuseLight(new ConstantTexture(Vector3D(15, 15, 15)));
        YZRect *leftWall = new YZRect(0, 555, 0, 555, 555, green);
        YZRect *rightWall  = new YZRect(0, 555, 0, 555,   0, red);
        XZRect *lightBlub = new XZRect(213, 343, 227, 332, 554, light);
        XZRect *bottomWall = new XZRect(0, 555, 0, 555, 0, white);
        XZRect *topWall = new XZRect(0, 555, 0, 555, 555, white);
        XYRect *backWall = new XYRect(0, 555, 0, 555, 555, white);
        backWall->setFlipNormal(true);
        topWall->setFlipNormal(true);
        leftWall->setFlipNormal(true);

        list->addHitable(rightWall);
        list->addHitable(leftWall);
        list->addHitable(lightBlub);
        list->addHitable(bottomWall);
        list->addHitable(topWall);
        list->addHitable(backWall);

        list->addHitable(new Box(Vector3D(130, 0, 65), Vector3D(295, 165, 230), white));
        list->addHitable(new Box(Vector3D(265, 0, 295), Vector3D(430, 330, 460), white));
        return list;
    }

}
