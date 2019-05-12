#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>

#include "RayTracer/Tracer.h"

namespace Ui {
class SettingDlg;
}

class QTimer;
class SettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDlg(RayTracer::Tracer *target, QWidget *parent = nullptr);
    ~SettingDlg();

    void setTotalTime(double total);

signals:
    void runRayTracer();
    void saveToImage();

private slots:
    void on_renderPushButton_clicked();
    void on_savePushButton_clicked();

private:
    Ui::SettingDlg *ui;
};

#endif // SETTINGDLG_H
