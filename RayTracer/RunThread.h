#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QThread>
#include "RayTracer/Tracer.h"

class RunThread : public QThread
{
    Q_OBJECT
public:
    RunThread(RayTracer::Tracer *target, QObject *parent = 0);
    ~RunThread();

    void run();

    double getTotalTime() const { return totalTime; }

private:
    RayTracer::Tracer *tracer;
    double totalTime;
};

#endif // RUNTHREAD_H
