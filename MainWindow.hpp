#pragma once
#include "RovCameraWidget.hpp"
#include "DebugDialog.hpp"
#include "LogWidget.hpp"
#include "RegulatorsWidget.hpp"
#include "RovCameraWidget.hpp"
#include "RovTelimetryWidget.hpp"
#include "ScaleFactorWidget.hpp"
#include "SDLJoystick.hpp"
#include <QAction>
#include <QMainWindow>
#include <QTextEdit>
#include <QDir>
#include <QString>
#include <QKeyEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);

    ~MainWindow();
    static QTextEdit* logWidget;

    QScopedPointer<RovCameraWidget> m_cameraWidget;

    void keyPressEvent(QKeyEvent *e);

    SDLJoystick* get_joystick();
signals:
    void fishPhoto();

private:
    void createMenus();

    void createDocks();

    void createConnections();

    void createActions();

    void takePhoto();

    void sendPhotoToFlash();

    QScopedPointer<QAction> m_startCameraAct;
    QScopedPointer<QAction> m_stopCameraAct;
    QScopedPointer<QAction> m_switchCameraAct;
    QScopedPointer<QAction> m_openJoystickSettings;
    QScopedPointer<QAction> m_openDebugDialog;

    QScopedPointer<RovTelimetryWidget> m_telimetryWidget;

    QScopedPointer<ScaleFactorWidget> m_scaleWidget;
    QScopedPointer<RegulatorsWidget> m_regulatorsWidget;
    QScopedPointer<SDLJoystick> m_joystick;
    QScopedPointer<DebugDialog> m_debugDialog;
    QScopedPointer<LogWidget> m_logFileWidget;

    //mosaic
    QScopedPointer<QAction> m_takeMosaicPhoto;
    QScopedPointer<QAction> m_mosaicPhotoBack;
    QScopedPointer<QAction> m_sendMosaicToFlash;
    void mosaicPhotoBack();
    void sendMosaicToFlash();
    //shipLength
    QScopedPointer<QAction> m_startStopCountShip;

    //fishLength
    QScopedPointer<QAction> m_takeFishPhoto;
    QScopedPointer<QAction> m_fishPhotoBack;
    QScopedPointer<QAction> m_sendFishToFlash;
    void fishPhotoBack();
    void sendFishToFlash();


    //fishLength
    QScopedPointer<QAction> m_takeShipPhoto;
    QScopedPointer<QAction> m_fishShipBack;
    QScopedPointer<QAction> m_sendShipToFlash;

    void sendShipToFlash();
    void shipPhotoBack();

    //env
    QScopedPointer<QAction> m_clearAllPhotos;
    QScopedPointer<QAction> m_clearMosaicPhotos;
    QScopedPointer<QAction> m_clearFishPhotos;
    QScopedPointer<QAction> m_clearAllFlash;
    QScopedPointer<QAction> m_clearMosaicFlash;
    QScopedPointer<QAction> m_clearFishFlash;
    void clearAllPhotos();
    void clearMosaicPhotos();
    void clearFishPhotos();
    void clearShipPhotos();
    void clearAllFlash();
    void Clear();
    void clearMosaicFlash();
    void clearFishFlash();
    void clearFlash();
    void clearShipFlash();

    //transect
    QScopedPointer<QAction> m_startStopTransect;
    void transect();
    QTimer* transectTimer;
    bool isTransect = false;
    float keepingDepth;
    float keepingYaw;
    void unmounting();
    //config
    QString flashPath = "/media/rcteam/USB DISK/";
};
