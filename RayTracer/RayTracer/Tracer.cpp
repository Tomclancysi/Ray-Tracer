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

    // scene
    //Hitable* world = randomScene();
    //Hitable *world = twoSpheresScene();
    simpleLightScene();
    //Hitable *world = cornellBoxScene();

    for(int x = 0;x < m_list.size();++ x)
    {
        m_list[x]->preRendering();
    }

    // create bvh tree.
    BVHNode* sceneNode = new BVHNode(m_list, 0, m_list.size());
    Hitable* hitableNode = reinterpret_cast<Hitable*>(sceneNode);

    // camera
    //Vector3D lookfrom(2, 6, 12);
    //Vector3D lookfrom(7, 6, 18);
    Vector3D lookfrom(0, 6, 18);
    //    Vector3D lookfrom(10, 20, 12);
    //        Vector3D lookfrom(0, 16, 0);
    Vector3D lookat(0, 0, 0);
    //        Vector3D lookfrom(278, 278, -800);
    //        Vector3D lookat(278, 278, 0);
    //float dist_to_focus = (lookfrom - lookat).getLength();
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    Camera camera(lookfrom, lookat, 45,
                  static_cast<float>(m_config.m_width) / m_config.m_height,
                  aperture, dist_to_focus,
                  0.0, 1.0);
    int samples = 500;

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

    clearHitableList();
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

void Tracer::clearHitableList()
{
    for (int x = 0; x < m_list.size(); ++x)
    {
        delete m_list[x];
        m_list[x] = nullptr;
    }
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
        float t = 0.5f * (r.getDirection().y + 1.0f);
        Vector4D ret = Vector3D(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3D(0.5f, 0.7f, 1.0f) * t;
        ret.w = 1.0f;
        return ret;
        //        return Vector4D(0.0f, 0.0f, 0.0f, 1.0f);
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

void Tracer::randomScene()
{
    int n = 500;

    Texture *checker = new CheckerTexture(
                new ConstantTexture(Vector3D(0.2, 0.3, 0.1)),
                new ConstantTexture(Vector3D(0.9, 0.9, 0.9)));
    m_list.push_back(new Sphere(Vector3D(0, -1000.0, 0), 1000,
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
                    m_list.push_back(new Sphere(center, 0.2, new Lambertian
                                                (new ConstantTexture(
                                                     Vector3D(drand48()*drand48(),
                                                              drand48()*drand48(),
                                                              drand48()*drand48())))));
                // metal
                else if (choose_mat < 0.95f)
                    m_list.push_back(new Sphere(center, 0.2, new Metal
                                                (new ConstantTexture(
                                                     Vector3D(0.5f*(1.0f + drand48()),
                                                              0.5f*(1.0f + drand48()),
                                                              0.5f*(1.0f + drand48()))),
                                                 0.5f*drand48())));
                // glass
                else
                    m_list.push_back(new Sphere(center, 0.2, new Dielectric
                                                (1.5f)));
            }
        }
    }
    m_list.push_back(new Sphere(Vector3D(0, 1, 0), 1.0,
                                new Dielectric(1.5f)));
    m_list.push_back(new Sphere(Vector3D(-4, 1, 0), 1.0,
                                new Lambertian(new ConstantTexture(
                                                   Vector3D(0.4, 0.2, 0.1)))));
    m_list.push_back(new Sphere(Vector3D(4, 1, 0), 1.0,
                                new Metal(new ConstantTexture(
                                              Vector3D(0.7, 0.6, 0.5)), 0.0f)));
}

void Tracer::twoSpheresScene()
{
    Texture *checker = new CheckerTexture(
                new ConstantTexture(Vector3D(0.2f,0.3f,0.1f)),
                new ConstantTexture(Vector3D(0.9f,0.9f,0.9f)));
    ImageTexture *earth = new ImageTexture("./res/earth.jpg");
    m_list.push_back(new Sphere(Vector3D(0, -1000, 0),1000, new Lambertian(checker)));
    m_list.push_back(new Sphere(Vector3D(-2, +1.0, 0),1.0, new Lambertian(earth)));
    m_list.push_back(new Sphere(Vector3D(2, +1.0, 0),1.0, new Dielectric(1.5f)));
}

void Tracer::simpleLightScene()
{
    Texture *checker = new CheckerTexture(
                new ConstantTexture(Vector3D(0.0f,0.0f,0.0f)),
                new ConstantTexture(Vector3D(0.9f,0.9f,0.9f)));
    ImageTexture *earth = new ImageTexture("./res/earth.jpg");
    Vector3D p0(0,4,0),p1(-4, 0,0),p2(4,0,0);
    //        m_list.push_back(new Sphere(Vector3D(0, -1000, 0), 1000, new Lambertian(checker)));
    //        m_list.push_back(new TTriangle(p0, p1, p2, new Metal(new ConstantTexture(
    //                                                Vector3D(0.7, 0.6, 0.5)), 0.0f)));
    //        m_list.push_back(new Sphere(Vector3D(0, +2.0f, 4.5), 2.0, new Dielectric(1.5f)));
    m_list.push_back(new Sphere(Vector3D(+5.5, +1.0f, 0), 1.0, new Dielectric(1.5f)));
    m_list.push_back(new Sphere(Vector3D(-6.5, +1.0f, 3), 1.0, new Lambertian(earth)));
    m_list.push_back(new Sphere(Vector3D(-5.5, +2.0f, -3.0), 2.0,
                                new Metal(
                                    new ConstantTexture(Vector3D(1.0, 1.0, 1.0)), 0.0f)));
    Plane *plane = new Plane(Vector3D(0,0,0), Vector3D(100,100,100),
                             new Metal(new ConstantTexture(Vector3D(0.75,0.75,0.75)), 0.05f));
    Cube *box = new Cube(Vector3D(0,1.5,-4),Vector3D(1,1.5,1), new Lambertian(checker));
    //    ModelHitable *bunny = new ModelHitable("./res/stall.obj", Vector3D(0,2.5,0),
    //                                           Vector3D(1.0,1.0,1.0),
    //                                           new Lambertian(new ImageTexture("./res/stall.png")));
    //    bunny->rotate(Vector3D(0,1,0), 180.0f);
    //        ModelHitable *bunny = new ModelHitable("./res/tank.obj", Vector3D(0,3,0),
    //                                               Vector3D(0.5,0.5,0.5),new Lambertian(
    //                                                   new ConstantTexture(
    //                                                       Vector3D(0.7, 0.0, 0.0))));
    ModelHitable *tree1 = new ModelHitable("./res/tree.obj", Vector3D(-11,1.2,-5),
                                           Vector3D(2.7,2.7,2.7),
                                           new Lambertian(new ImageTexture("./res/tree.png")));
    ModelHitable *tree2 = new ModelHitable("./res/tree.obj", Vector3D(4,1.2,-4),
                                           Vector3D(2.0,2.0,2.0),
                                           new Lambertian(new ImageTexture("./res/tree.png")));
    ModelHitable *tree3 = new ModelHitable("./res/lowPolyTree.obj", Vector3D(-1,+12,+4),
                                           Vector3D(0.15,0.15,0.15),
                                           new Lambertian(new ImageTexture("./res/lowPolyTree.png")));
    ModelHitable *tree4 = new ModelHitable("./res/tree.obj", Vector3D(-8,1.2,-6),
                                           Vector3D(3.0,3.0,3.0),
                                           new Lambertian(new ImageTexture("./res/tree.png")));
    ModelHitable *tree5 = new ModelHitable("./res/tree.obj", Vector3D(-12,1.2,-9),
                                           Vector3D(3.5,3.5,3.5),
                                           new Lambertian(new ImageTexture("./res/tree.png")));
    ModelHitable *board = new ModelHitable("./res/board.obj", Vector3D(0,+3,-9),
                                           Vector3D(1.0,1.0,1.0),
                                           new Lambertian(new ImageTexture("./res/board.bmp")));
    ModelHitable *robot = new ModelHitable("./res/robot.obj", Vector3D(+8,+2.8,-6),
                                           Vector3D(3.2,3.2,3.2),
                                           new Lambertian(new ImageTexture("./res/robot.jpg")));
    ModelHitable *person = new ModelHitable("./res/person.obj", Vector3D(+0,+5.3,-0),
                                           Vector3D(0.5,0.5,0.5),
                                           new Lambertian(new ImageTexture("./res/person.png")));
//    ModelHitable *dragon = new ModelHitable("./res/dragon.obj", Vector3D(+0,+2.8,0),
//                                           Vector3D(1.0,1.0,1.0),
//                                           new Dielectric(2.4f));
    robot->rotate(Vector3D(0,1,0), -30.0f);
    board->rotate(Vector3D(0,1,0), 90.0f);
    box->rotate(Vector3D(0,1,0),45.0f);
    //plane->rotate(Vector3D(0,1,0), 90.0f);
    m_list.push_back(plane);
    m_list.push_back(box);
    m_list.push_back(tree1);
    m_list.push_back(tree2);
    m_list.push_back(tree3);
    m_list.push_back(tree4);
    m_list.push_back(tree5);
    m_list.push_back(board);
    m_list.push_back(robot);
    m_list.push_back(person);
    //    for(int x = 0;x < 20;x ++)
    //    {
    //        ModelHitabl
    //    }
    //    m_list.push_back(new Sphere(Vector3D(0, +8.0f, 0), 2.0, new DiffuseLight(
    //                                    new ConstantTexture(Vector3D(4.0f, 4.0f, 4.0f)))));
}

}
