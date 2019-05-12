#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "RayTracer/Tracer.h"

namespace Ui {
class Window;
}

class QTimer;
class RunThread;
class SettingDlg;
class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

protected slots:
    void runRayTracer();
    void finishedRayTracer();
    void receiveFrame();
    void saveImage();

private:
    void paintEvent(QPaintEvent *) override;

private:
    Ui::Window *ui;
    QImage *canvas;
    bool isRunning;
    SettingDlg *setting;
    QTimer *timer;
    RunThread *runable;
    RayTracer::Tracer *tracer;

};

#endif // WINDOW_H
