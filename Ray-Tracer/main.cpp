#include <iostream>
#include "RayTracer/RayTracing.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace RayTracer;

int main()
{
	RayTracing tracer;
	tracer.initialize(800, 600);
	tracer.setRecursionDepth(10);
	unsigned char * ret = tracer.render();

	stbi_flip_vertically_on_write(1);
	stbi_write_png("./result.png",
		tracer.getWidth(),
		tracer.getHeight(),
		4,
		static_cast<void*>(tracer.getImage()),
		tracer.getWidth() * 4);
	cout << "Rendering over!!\n";
	return 0;
}