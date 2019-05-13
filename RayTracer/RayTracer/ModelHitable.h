#ifndef MODELHITABLE_H
#define MODELHITABLE_H

#include "MeshHitable.h"

/**
 * @projectName   RayTracer
 * @brief         Model loader.
 * @author        YangWC
 * @date          2019-05-12
 */

namespace RayTracer
{

class ModelHitable : public MeshHitable
{
private:
    Vector3D m_center;
    Vector3D m_scale;

public:
    ModelHitable(const std::string &path, Vector3D pos, Vector3D len, Material *mat);
    virtual ~ModelHitable() = default;

private:
    // Obj file loader.
    void loadObjFile(const std::string &path);
};

}

#endif // MODELHITABLE_H
