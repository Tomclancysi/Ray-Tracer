#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T14:40:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayTracer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        Window.cpp \
    RayTracer/Math/Matrix4x4.cpp \
    RayTracer/Math/Quaternion.cpp \
    RayTracer/Math/Vector2D.cpp \
    RayTracer/Math/Vector3D.cpp \
    RayTracer/Math/Vector4D.cpp \
    SettingDlg.cpp \
    RayTracer/Ray.cpp \
    RayTracer/Camera.cpp \
    RayTracer/Material.cpp \
    RayTracer/Hitable.cpp \
    RayTracer/Tracer.cpp \
    RunThread.cpp \
    RayTracer/AABB.cpp \
    RayTracer/BVHNode.cpp \
    RayTracer/Texture.cpp \
    RayTracer/MeshHitable.cpp \
    RayTracer/Transform3D.cpp \
    RayTracer/SimpleMeshHitable.cpp \
    RayTracer/ModelHitable.cpp

HEADERS += \
        Window.h \
    RayTracer/Math/MathUtils.h \
    RayTracer/Math/Matrix4x4.h \
    RayTracer/Math/Quaternion.h \
    RayTracer/Math/Vector2D.h \
    RayTracer/Math/Vector3D.h \
    RayTracer/Math/Vector4D.h \
    SettingDlg.h \
    RayTracer/Ray.h \
    RayTracer/Hitable.h \
    RayTracer/Camera.h \
    RayTracer/Material.h \
    RayTracer/Tracer.h \
    RunThread.h \
    RayTracer/AABB.h \
    RayTracer/BVHNode.h \
    RayTracer/Texture.h \
    RayTracer/MeshHitable.h \
    RayTracer/Transform3D.h \
    RayTracer/SimpleMeshHitable.h \
    RayTracer/ModelHitable.h

FORMS += \
        Window.ui \
    SettingDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/include

