#include <iostream>
#include "RayTracer/Tracer.h"
#include "RayTracer/Texture.h"
#include "RayTracer/Hitable.h"
#include "RayTracer/Material.h"
#include "RayTracer/SimpleMeshHitable.h"
#include "RayTracer/ModelHitable.h"
#include "RayTracer/Camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace RayTracer;

Tracer tracer;

void randomScene()
{
	int n = 500;
	TextureMgr::ptr texMgr = tracer.getTextureMgr();
	MaterialMgr::ptr matMgr = tracer.getMaterialMgr();
	unsigned int checker_unit = texMgr->loadTexture(new CheckerTexture(
		new ConstantTexture(Vector3D(0.2f, 0.3f, 0.1f)),
		new ConstantTexture(Vector3D(0.9f, 0.9f, 0.9f))));
	unsigned int bottle_mat = matMgr->loadMaterial(new Dielectric(1.5f));
	tracer.addObjects(new Sphere(Vector3D(0, -1000.0, 0), 1000,
		matMgr->loadMaterial(new Lambertian(checker_unit))));
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
					tracer.addObjects(new Sphere(center, 0.2,
						matMgr->loadMaterial(new Lambertian
						(texMgr->loadTexture(new ConstantTexture(
							Vector3D(drand48()*drand48(),
								drand48()*drand48(),
								drand48()*drand48())))))));
				// metal
				else if (choose_mat < 0.95f)
					tracer.addObjects(new Sphere(center, 0.2,
						matMgr->loadMaterial(new Metal
						(texMgr->loadTexture(new ConstantTexture(
							Vector3D(
								0.5f*(1.0f + drand48()),
								0.5f*(1.0f + drand48()),
								0.5f*(1.0f + drand48())))),
							0.5f*drand48()))));
				// glass
				else
					tracer.addObjects(new Sphere(center, 0.2, bottle_mat));
			}
		}
	}
	tracer.addObjects(new Sphere(Vector3D(0, 1, 0), 1.0, bottle_mat));
	tracer.addObjects(new Sphere(Vector3D(-4, 1, 0), 1.0,
		matMgr->loadMaterial(new Lambertian(texMgr->loadTexture(new ConstantTexture(
			Vector3D(0.4, 0.2, 0.1)))))));
	tracer.addObjects(new Sphere(Vector3D(4, 1, 0), 1.0,
		matMgr->loadMaterial(new Metal(texMgr->loadTexture(new ConstantTexture(
			Vector3D(0.7, 0.6, 0.5))), 0.0f))));
}

void twoSpheresScene()
{
	TextureMgr::ptr texMgr = tracer.getTextureMgr();
	unsigned int checker_unit = texMgr->loadTexture(new CheckerTexture(
		new ConstantTexture(Vector3D(0.2f, 0.3f, 0.1f)),
		new ConstantTexture(Vector3D(0.9f, 0.9f, 0.9f))));
	unsigned int earth_unit = texMgr->loadTexture(new ImageTexture("./res/earth.jpg"));

	MaterialMgr::ptr matMgr = tracer.getMaterialMgr();
	unsigned int checker_mat = matMgr->loadMaterial(new Lambertian(checker_unit));
	unsigned int earth_mat = matMgr->loadMaterial(new Lambertian(earth_unit));
	unsigned int bottle_mat = matMgr->loadMaterial(new Dielectric(1.5f));

	tracer.addObjects(new Sphere(Vector3D(0, -1000, 0), 1000, checker_mat));
	tracer.addObjects(new Sphere(Vector3D(-2, +1.0, 0), 1.0, earth_mat));
	tracer.addObjects(new Sphere(Vector3D(2, +1.0, 0), 1.0, bottle_mat));
}

void simpleLightScene()
{
	// load texture.
	TextureMgr::ptr texMgr = tracer.getTextureMgr();
	unsigned int checker_unit = texMgr->loadTexture(new CheckerTexture(
		new ConstantTexture(Vector3D(0.0f, 0.0f, 0.0f)),
		new ConstantTexture(Vector3D(0.9f, 0.9f, 0.9f))));
	unsigned int earth_unit = texMgr->loadTexture(new ImageTexture("./res/earth.jpg"));
	unsigned int white_unit = texMgr->loadTexture(new ConstantTexture(Vector3D(1.0, 1.0, 1.0)));
	unsigned int gray_unit = texMgr->loadTexture(new ConstantTexture(Vector3D(0.75, 0.75, 0.75)));
	unsigned int tree_unit = texMgr->loadTexture(new ImageTexture("./res/tree.png"));
	unsigned int lowPoly_unit = texMgr->loadTexture(new ImageTexture("./res/lowPolyTree.png"));
	unsigned int board_unit = texMgr->loadTexture(new ImageTexture("./res/board.bmp"));
	unsigned int person_unit = texMgr->loadTexture(new ImageTexture("./res/person.png"));

	// load material.
	MaterialMgr::ptr matMgr = tracer.getMaterialMgr();
	unsigned int bottle_mat = matMgr->loadMaterial(new Dielectric(1.5f));
	unsigned int earth_mat = matMgr->loadMaterial(new Lambertian(earth_unit));
	unsigned int whiteMetal_mat = matMgr->loadMaterial(new Metal(white_unit, 0.0f));
	unsigned int grayMetal_mat = matMgr->loadMaterial(new Metal(gray_unit, 0.00f));
	unsigned int checkerLambert_mat = matMgr->loadMaterial(new Lambertian(checker_unit));
	unsigned int treeLambert_mat = matMgr->loadMaterial(new Lambertian(tree_unit));
	unsigned int lowPolyTreeLambert_mat = matMgr->loadMaterial(new Lambertian(lowPoly_unit));
	unsigned int boardLambert_mat = matMgr->loadMaterial(new Lambertian(board_unit));
	unsigned int personLambert_mat = matMgr->loadMaterial(new Lambertian(person_unit));

	Vector3D p0(0, 4, 0), p1(-4, 0, 0), p2(4, 0, 0);
	tracer.addObjects(new Sphere(Vector3D(+5.5, +1.0f, 0), 1.0, bottle_mat));
	tracer.addObjects(new Sphere(Vector3D(-6.5, +1.0f, 3), 1.0, earth_mat));
	tracer.addObjects(new Sphere(Vector3D(-5.5, +2.0f, -3.0), 2.0, whiteMetal_mat));
	Plane *plane = new Plane(Vector3D(0, 0, 0), Vector3D(100, 100, 100), grayMetal_mat);
	Cube *box = new Cube(Vector3D(0, 1.5, -4), Vector3D(1, 1.5, 1), checkerLambert_mat);
	//    ModelHitable *bunny = new ModelHitable("./res/stall.obj", Vector3D(0,2.5,0),
	//                                           Vector3D(1.0,1.0,1.0),
	//                                           new Lambertian(new ImageTexture("./res/stall.png")));
	//    bunny->rotate(Vector3D(0,1,0), 180.0f);
	//        ModelHitable *bunny = new ModelHitable("./res/tank.obj", Vector3D(0,3,0),
	//                                               Vector3D(0.5,0.5,0.5),new Lambertian(
	//                                                   new ConstantTexture(
	//                                                       Vector3D(0.7, 0.0, 0.0))));
	ModelHitable *tree1 = new ModelHitable("./res/tree.obj", Vector3D(-11, 1.2, -5),
		Vector3D(2.7, 2.7, 2.7), treeLambert_mat);
	ModelHitable *tree2 = new ModelHitable("./res/tree.obj", Vector3D(4, 1.2, -4),
		Vector3D(2.0, 2.0, 2.0), treeLambert_mat);
	ModelHitable *tree3 = new ModelHitable("./res/lowPolyTree.obj", Vector3D(-1, +12, +4),
		Vector3D(0.15, 0.15, 0.15), lowPolyTreeLambert_mat);
	ModelHitable *tree4 = new ModelHitable("./res/tree.obj", Vector3D(-8, 1.2, -6),
		Vector3D(3.0, 3.0, 3.0), treeLambert_mat);
	ModelHitable *tree5 = new ModelHitable("./res/tree.obj", Vector3D(-12, 1.2, -9),
		Vector3D(3.5, 3.5, 3.5), treeLambert_mat);
	ModelHitable *board = new ModelHitable("./res/board.obj", Vector3D(0, +3, -9),
		Vector3D(1.0, 1.0, 1.0), boardLambert_mat);
	//    ModelHitable *robot = new ModelHitable("./res/robot.obj", Vector3D(+8,+2.8,-6),
	//                                           Vector3D(3.2,3.2,3.2),
	//                                           new Lambertian(new ImageTexture("./res/robot.jpg")));
	//ModelHitable *robot = new ModelHitable("./res/robot.obj", Vector3D(-4.5, +2.8, +10),
	//	Vector3D(3.2, 3.2, 3.2),
	//	new Lambertian(new ImageTexture("./res/robot.jpg")));
	ModelHitable *person = new ModelHitable("./res/person.obj", Vector3D(+0, +5.3, -0),
		Vector3D(0.5, 0.5, 0.5), personLambert_mat);
	//    ModelHitable *dragon = new ModelHitable("./res/dragon.obj", Vector3D(+5,+5,5),
	//                                           Vector3D(1.0,1.0,1.0),
	//                                           new Dielectric(2.4f));
	//ModelHitable *bunny = new ModelHitable("./res/bunny.obj", Vector3D(+5, +5, 5),
	//	Vector3D(0.6, 0.6, 0.6),
	//	new Metal(new ConstantTexture
	//	(Vector3D(1.0, 0.8, 0.0)), 0.3f));
	//robot->rotate(Vector3D(0,1,0), -30.0f);
	board->rotate(Vector3D(0, 1, 0), 90.0f);
	box->rotate(Vector3D(0, 1, 0), 45.0f);
	tracer.addObjects(plane);
	tracer.addObjects(box);
	tracer.addObjects(tree1);
	tracer.addObjects(tree2);
	tracer.addObjects(tree3);
	tracer.addObjects(tree4);
	tracer.addObjects(tree5);
	tracer.addObjects(board);
	//m_list.push_back(robot);
	tracer.addObjects(person);
	//m_list.push_back(bunny);
	//    m_list.push_back(new Sphere(Vector3D(0, +8.0f, 0), 2.0, new DiffuseLight(
	//                                    new ConstantTexture(Vector3D(4.0f, 4.0f, 4.0f)))));
}


int main()
{
	// initialize.
	tracer.initialize(800, 600);
	tracer.setRecursionDepth(50);
	tracer.setSamplingNums(1);

	// create scene.
	simpleLightScene();
	//randomScene();
	//twoSpheresScene();
	Camera *camera = tracer.getCamera();

	// render.
	double totalTime = 0.0f;
	unsigned char * ret = nullptr;
	tracer.beginFrame();
	{
		cout << "4\n";
		ret = tracer.render(totalTime);
		cout << "5\n";
	}
	tracer.endFrame();
	cout << "6\n";

	// write to png image.
	stbi_flip_vertically_on_write(1);
	stbi_write_png("./result.png",
		tracer.getWidth(),
		tracer.getHeight(),
		4,
		static_cast<void*>(tracer.getImage()),
		tracer.getWidth() * 4);
	cout << "Rendering over!!\n";
	cout << "Total->" << totalTime << std::endl;
	return 0;
}