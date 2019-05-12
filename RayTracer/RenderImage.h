#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include "RayTracer/Tracer.h"

//class RenderImage : public QObject
//{
//    Q_OBJECT
//public:
//    explicit RenderImage(int w, int h, QObject *parent = nullptr);
//    ~RenderImage();

//    bool isRunning(){return running;}

//    int getWidth() const;
//    int getHeight() const;
//    int getChannel() const;
//    unsigned char *getImage() const;

//signals:
//    void frameOut(unsigned char *image);

//public slots:
//    void onePixelFinished();
//    void render();

//private:
//    bool running;
//    RayTracer::RayTracing *tracer;
//    int width, height, channel;
//};

#endif // RENDERTHREAD_H
