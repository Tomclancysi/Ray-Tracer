#include "RunThread.h"

RunThread::RunThread(RayTracer::Tracer *target, QObject *parent)
    :QThread(parent)
{
    tracer = target;
}

RunThread::~RunThread()
{

}

void RunThread::run()
{
    tracer->render(totalTime);
}
