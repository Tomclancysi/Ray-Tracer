#include "Window.h"
#include "ui_Window.h"
#include "SettingDlg.h"

#include "RenderImage.h"

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include "RunThread.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace RayTracer;

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    isRunning = false;
    canvas = nullptr;

    // tracer
    tracer = new Tracer();
    tracer->initialize(width(), height());

    // setting dialog.
    setting = new SettingDlg(tracer, this);
    connect(setting, &SettingDlg::runRayTracer, this, &Window::runRayTracer);
    connect(setting, &SettingDlg::saveToImage, this, &Window::saveImage);
    setting->show();

    // timer
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Window::receiveFrame);

    // thread
    runable = new RunThread(tracer, this);
    connect(runable, &QThread::finished, this, &Window::finishedRayTracer);
}

Window::~Window()
{
    delete ui;
    delete canvas;
    delete tracer;
    delete setting;
    delete runable;
    delete timer;
    tracer = nullptr;
    canvas = nullptr;
    setting = nullptr;
    runable = nullptr;
    timer = nullptr;
}

void Window::runRayTracer()
{
//    if(isRunning)
//        return;
    if(runable->isRunning())
        return;
    //isRunning = true;
    tracer->setRecursionDepth(50);
    runable->start();
    timer->start(1000);
    //tracer->render();
    //isRunning = false;
    QMessageBox::information(this, "Tip", "Begin to render.");
}

void Window::finishedRayTracer()
{
    timer->stop();
    receiveFrame();
    setting->setTotalTime(runable->getTotalTime());
    setting->on_savePushButton_clicked();
}

void Window::receiveFrame()
{
    if(canvas) delete canvas;
    canvas = new QImage(tracer->getImage(), width(), height(), QImage::Format_RGBA8888);
    (*canvas) = canvas->mirrored(false, true);
    update();
}

void Window::saveImage()
{
    if(tracer->getImage() == nullptr)
        return;
    // Save to image file.
    stbi_flip_vertically_on_write(1);
    stbi_write_png("./ret/result.png",
                   tracer->getWidth(),
                   tracer->getHeight(),
                   4,
                   static_cast<void*>(tracer->getImage()),
                   tracer->getWidth()*4);
    QMessageBox::information(this, "Tip", "Save the image successfully.");
}

void Window::paintEvent(QPaintEvent *event)
{
    if(canvas)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *canvas);
    }
    QWidget::paintEvent(event);
}
