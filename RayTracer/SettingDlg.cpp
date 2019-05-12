#include "SettingDlg.h"
#include "ui_SettingDlg.h"

#include <QMessageBox>

#include "RenderImage.h"

using namespace RayTracer;

SettingDlg::SettingDlg(Tracer *target, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDlg)
{
    ui->setupUi(this);
}

SettingDlg::~SettingDlg()
{
    delete ui;
}

void SettingDlg::setTotalTime(double total)
{
    ui->timeLineEdit->setText(QString("%1").arg(total));
}

void SettingDlg::on_renderPushButton_clicked()
{
    emit runRayTracer();
}

void SettingDlg::on_savePushButton_clicked()
{
    emit saveToImage();
}
