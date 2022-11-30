QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RovUI
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    SDLJoystick.cpp \
    SDLJoystickSettingsDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    Crc.cpp \
    RovTelemetryWidget.cpp \
    RovCameraWidget.cpp \
    ScaleFactorWidget.cpp \
    RovSingleton.cpp \
    RovUdpConnection.cpp \
    RegulatorsWidget.cpp \
    DebugDialog.cpp \
    LogWidget.cpp

HEADERS += \
    MainWindow.hpp \
    CallOnce.hpp \
    SDLJoystick.hpp \
    SDLJoystickSettingsDialog.hpp \
    Singleton.hpp \
    Crc.hpp \
    RovTelemetryWidget.hpp \
    RovCameraWidget.hpp \
    ScaleFactorWidget.hpp \
    RovSingleton.hpp \
    RovUdpConnection.hpp \
    RovDataTypes.hpp \
    RegulatorsWidget.hpp \
    DebugDialog.hpp \
    LogWidget.hpp


RESOURCES +=

unix{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
    PKGCONFIG += sdl2
}
