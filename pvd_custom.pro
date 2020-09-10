#-------------------------------------------------
#
# Project created by QtCreator 2020-08-14T11:10:21
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pvd_custom
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


INCLUDEPATH += \
        3rd/PhysX/PhysX_3.4/Include \
        3rd/PhysX/PxShared/include

DEFINES += PX_FOUNDATION_STATIC_LIB PX_PHYSX_STATIC_LIB


LIBS_PATH_PHYSX    = ../3rd/PhysX/PhysX_3.4/Lib/vc15win64
LIBS_PATH_PXSHARED = ../3rd/PhysX/PxShared/lib/vc15win64

LIBS += \
        $${LIBS_PATH_PHYSX}/LowLevel.lib \
        $${LIBS_PATH_PHYSX}/LowLevelAABB.lib \
        $${LIBS_PATH_PHYSX}/LowLevelCloth.lib \
        $${LIBS_PATH_PHYSX}/LowLevelDynamics.lib \
        $${LIBS_PATH_PHYSX}/LowLevelParticles.lib \
        $${LIBS_PATH_PHYSX}/PhysX3_x64.lib \
        $${LIBS_PATH_PHYSX}/PhysX3CharacterKinematic_x64.lib \
        $${LIBS_PATH_PHYSX}/PhysX3Common_x64.lib \
        $${LIBS_PATH_PHYSX}/PhysX3Cooking_x64.lib \
        $${LIBS_PATH_PHYSX}/PhysX3Extensions.lib \
        $${LIBS_PATH_PHYSX}/PhysX3Vehicle.lib \
        $${LIBS_PATH_PHYSX}/SceneQuery.lib \
        $${LIBS_PATH_PHYSX}/SimulationController.lib \
        $${LIBS_PATH_PXSHARED}/PsFastXml_x64.lib \
        $${LIBS_PATH_PXSHARED}/PxFoundation_x64.lib \
        $${LIBS_PATH_PXSHARED}/PxPvdSDK_x64.lib \
        $${LIBS_PATH_PXSHARED}/PxTask_x64.lib \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
