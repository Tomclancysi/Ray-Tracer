//#include "RenderImage.h"

//using namespace RayTracer;

//RenderImage::RenderImage(int w, int h, QObject *parent)
//    : QObject(parent), width(w), height(h), channel(4)
//{
//    running = false;
//    tracer = new RayTracing;
//    tracer->initialize(w, h);

//    tracer->setRecursionDepth(50);
//    connect(tracer, &RayTracing::onePixelFinished, this, &RenderImage::onePixelFinished);
//}

//RenderImage::~RenderImage()
//{
//    if(tracer) delete tracer;
//    tracer = nullptr;
//}

//void RenderImage::render()
//{
//    running = true;

//    unsigned char *result = tracer->render();

//    emit frameOut(result);

//    running = false;
//}

//int RenderImage::getChannel() const
//{
//    return channel;
//}

//unsigned char *RenderImage::getImage() const
//{
//    return tracer->getImage();
//}

//void RenderImage::onePixelFinished()
//{
//    emit frameOut(tracer->getImage());
//}

//int RenderImage::getHeight() const
//{
//    return height;
//}

//int RenderImage::getWidth() const
//{
//    return width;
//}
