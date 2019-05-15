#ifndef SIMPLEMESHHITABLE_H
#define SIMPLEMESHHITABLE_H

#include "MeshHitable.h"

/**
 * @projectName   RayTracer
 * @brief         Some simple meshes.
 * @author        YangWC
 * @date          2019-05-12
 */

namespace RayTracer
{

	class Triangle : public MeshHitable
	{
	public:
		Triangle(Vector3D pos, Vector3D len, unsigned int mat);
		virtual ~Triangle() = default;
	};

	class Cube : public MeshHitable
	{
	public:
		Cube(Vector3D pos, Vector3D len, unsigned int mat);
		virtual ~Cube() = default;

	};

	class Plane : public MeshHitable
	{
	public:
		Plane(Vector3D pos, Vector3D len, unsigned int mat);
		virtual ~Plane() = default;
	};

}

#endif // SIMPLEMESHHITABLE_H
