#include "MainWindow.hpp"
#include "RovSingleton.hpp"
#include "SDLJoystick.hpp"
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

QTextEdit* MainWindow::logWidget = nullptr;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_cameraWidget(new RovCameraWidget(this))
    , m_telemetryWidget(new RovTelemetryWidget(this))
    , m_scaleWidget(new ScaleFactorWidget(this))
    , m_regulatorsWidget(new RegulatorsWidget(this))
    , m_joystick(new SDLJoystick(this))
    , m_debugDialog(new DebugDialog(this))
    , m_logFileWidget(new LogWidget(this))
    , m_jsd(new SDLJoystickSettingsDialog(this, (void*)m_joystick.data()))
{
    logWidget = new QTextEdit;
    createActions();
    createMenus();
    createConnections();
    createDocks();
    setCentralWidget(m_cameraWidget.data());
    setMinimumSize(640, 480);
    qDebug() << "Settings path is: " << QString(settings_path) << Qt::endl;
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus()
{
    QMenu* camera = menuBar()->addMenu(tr("Камера"));
    camera->addAction(m_startCameraAct.data());
    camera->addAction(m_stopCameraAct.data());
    camera->addAction(m_switchCameraAct.data());

    QMenu* settings = menuBar()->addMenu(tr("Настройки"));
    settings->addAction(m_openJoystickSettings.data());
    settings->addAction(m_openDebugDialog.data());

    QMenu* mosaic = menuBar()->addMenu(tr("Мозаика"));
    mosaic->addAction(m_takeMosaicPhoto.data());
    mosaic->addAction(m_mosaicPhotoBack.data());
    mosaic->addAction(m_sendMosaicToFlash.data());

    QMenu* ship = menuBar()->addMenu(tr("Корабль"));
    ship->addAction(m_startStopCountShip.data());

    QMenu* fish = menuBar()->addMenu(tr("Рыбов"));
    fish->addAction(m_takeFishPhoto.data());
    fish->addAction(m_fishPhotoBack.data());
    fish->addAction(m_sendFishToFlash.data());

    QMenu* env = menuBar()->addMenu(tr("Env"));
    env->addAction(m_clearAllPhotos.data());
    env->addAction(m_clearMosaicPhotos.data());
    env->addAction(m_clearFishPhotos.data());
    env->addAction(m_clearAllFlash.data());
    env->addAction(m_clearMosaicFlash.data());
    env->addAction(m_clearFishFlash.data());

    QMenu* autos = menuBar()->addMenu(tr("Autos"));
    autos->addAction(m_startStopTransect.data());
}

void MainWindow::createDocks()
{
    QMenu* view = menuBar()->addMenu(tr("&Вид"));

    QDockWidget* consDock = new QDockWidget(tr("Телеметрия"), this);
    consDock->setObjectName("TelemetryDockWidget");
    consDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    consDock->setWidget(m_telemetryWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, consDock);
    view->addAction(consDock->toggleViewAction());

    QDockWidget* logDock = new QDockWidget(tr("Информация"), this);
    logDock->setObjectName("LogDockWidget");
    logDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    logDock->setWidget(logWidget);
    logWidget->setFont(QFont("Consolas"));
    addDockWidget(Qt::LeftDockWidgetArea, logDock);
    view->addAction(logDock->toggleViewAction());

    QDockWidget* scaleDock = new QDockWidget(tr("Ограничитель тяги"), this);
    scaleDock->setObjectName("ScaleWidget");
    scaleDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    scaleDock->setWidget(m_scaleWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, scaleDock);
    view->addAction(scaleDock->toggleViewAction());

    QDockWidget* regulatorsDock = new QDockWidget(tr("Регуляторы"), this);
    regulatorsDock->setObjectName("RegulatorsWidget");
    regulatorsDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    regulatorsDock->setWidget(m_regulatorsWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, regulatorsDock);
    view->addAction(regulatorsDock->toggleViewAction());

    QDockWidget* logFileDock = new QDockWidget(tr("Логи"), this);
    logFileDock->setObjectName("RegulatorsWidget");
    logFileDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    logFileDock->setWidget(m_logFileWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, logFileDock);
    view->addAction(logFileDock->toggleViewAction());
}

void MainWindow::createConnections()
{
    QObject::connect(m_startCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->startCapture();
    });
    QObject::connect(m_stopCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->stopCapture();
    });
    QObject::connect(m_switchCameraAct.data(), &QAction::triggered, [this](bool) {
        RovSingleton::instance()->control().cameraIndex = (RovSingleton::instance()->control().cameraIndex + 1) % 2;
    });
    QObject::connect(m_openJoystickSettings.data(), &QAction::triggered, [this](bool) {
        m_jsd->show();
    });
    QObject::connect(m_openDebugDialog.data(), &QAction::triggered, [this](bool) {
        m_debugDialog.data()->show();
    });



    //mosaic
    QObject::connect(m_takeMosaicPhoto.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->takeMosaicPhoto();
    });
    QObject::connect(m_mosaicPhotoBack.data(), &QAction::triggered, [this](bool) {
        mosaicPhotoBack();
    });
    QObject::connect(m_sendMosaicToFlash.data(), &QAction::triggered, [this](bool) {
        sendMosaicToFlash();
    });


    //shipLength
    QObject::connect(m_startStopCountShip.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->isCountShip = !m_cameraWidget->isCountShip;
    });


    //fishLength
    QObject::connect(m_takeFishPhoto.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->takeFishPhoto();
    });
    QObject::connect(m_fishPhotoBack.data(), &QAction::triggered, [this](bool) {
        fishPhotoBack();
    });
    QObject::connect(m_sendFishToFlash.data(), &QAction::triggered, [this](bool) {
        sendFishToFlash();
    });


    //env
    QObject::connect(m_clearAllPhotos.data(), &QAction::triggered, [this](bool) {
        clearAllPhotos();
    });
    QObject::connect(m_clearMosaicPhotos.data(), &QAction::triggered, [this](bool) {
        clearMosaicPhotos();
    });
    QObject::connect(m_clearFishPhotos.data(), &QAction::triggered, [this](bool) {
        clearFishPhotos();
    });
    QObject::connect(m_clearAllFlash.data(), &QAction::triggered, [this](bool) {
        clearAllFlash();
    });
    QObject::connect(m_clearMosaicFlash.data(), &QAction::triggered, [this](bool) {
        clearMosaicFlash();
    });
    QObject::connect(m_clearFishFlash.data(), &QAction::triggered, [this](bool) {
        clearFishFlash();
    });


    //transect
    QObject::connect(m_startStopTransect.data(), &QAction::triggered, [this](bool) {
        keepingDepth = RovSingleton::instance()->telemetry().depth;
        keepingYaw = RovSingleton::instance()->telemetry().yaw;
        isTransect = !isTransect;
    });
}

void MainWindow::createActions()
{
    m_startCameraAct.reset(new QAction(tr("Начать захват изображения"), this));
    m_stopCameraAct.reset(new QAction(tr("Остановить захват изображения"), this));
    m_switchCameraAct.reset(new QAction(tr("Переключить камеру (мультиплексор)"), this));
    m_openJoystickSettings.reset(new QAction(tr("Настройки джойстика"), this));
    m_openDebugDialog.reset(new QAction(tr("Отладка движителей"), this));

    //mosaic
    m_takeMosaicPhoto.reset(new QAction(tr("Фото"), this));
    m_mosaicPhotoBack.reset(new QAction(tr("Назад"), this));
    m_sendMosaicToFlash.reset(new QAction(tr("Отправить"), this));

    //shipLength
    m_startStopCountShip.reset(new QAction(tr("Измерение корабля"), this));

    //fishLength
    m_takeFishPhoto.reset(new QAction(tr("Фото"), this));
    m_fishPhotoBack.reset(new QAction(tr("Назад"), this));
    m_sendFishToFlash.reset(new QAction(tr("Отправить"), this));


    m_takeShipPhoto.reset(new QAction(tr("Фото"), this));

    //env
    m_clearAllPhotos.reset(new QAction(tr("Очистить все"), this));
    m_clearMosaicPhotos.reset(new QAction(tr("Очистить мозаику"), this));
    m_clearFishPhotos.reset(new QAction(tr("Очистить рыбов"), this));
    m_clearAllFlash.reset(new QAction(tr("Очистить все флешка"), this));
    m_clearMosaicFlash.reset(new QAction(tr("Очистить мозаику флешка"), this));
    m_clearFishFlash.reset(new QAction(tr("Очистить рыбов флешка"), this));

    //transect
    m_startStopTransect.reset(new QAction(tr("Трансекта (помогите)"), this));
}


//mosaic
void MainWindow::mosaicPhotoBack()
{
    if (m_cameraWidget->mosaicPhotoNum < 2) {
        m_cameraWidget->mosaicPhotoNum -= 1;
        QDir dir("../photo/mosaic");
        dir.remove(QString::number(m_cameraWidget->mosaicPhotoNum) + ".png");
    }
}

void MainWindow::sendMosaicToFlash()
{
    QDir dir(flashPath + "photo/mosaic");
    if (dir.exists())
    {
        for (int i = 0; i < 9; i++) {
            QFile::copy("../photo/mosaic/" + QString::number(i) + ".png", flashPath + "photo/mosaic/" + QString::number(i) + ".png");
        }
    }
}


//fishLength
void MainWindow::fishPhotoBack()
{
    if (m_cameraWidget->currentFishNum > 1)
    {
        QDir dir("../photo/fish/");
        if (dir.exists()) {
            dir.remove(QString::number(m_cameraWidget->currentFishNum) + ".png");
            m_cameraWidget->currentFishNum -= 1;
        }
    }
}

void MainWindow::sendFishToFlash()
{
    QDir dir(flashPath + "photo/fish");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++) {
            QFile::copy("../photo/fish/" + QString::number(i) + ".png", flashPath + "photo/fish/" + QString::number(i) + ".png");
        }
    }
}

void MainWindow::shipPhotoBack()
{
    if (m_cameraWidget->shipNum > 1)
    {
        QDir dir("../photo/ship/");
        if (dir.exists()) {
            dir.remove(QString::number(m_cameraWidget->shipNum) + ".png");
            m_cameraWidget->currentFishNum -= 1;
        }
    }
}

void MainWindow::sendShipToFlash()
{
    QDir dir(flashPath + "photo/ship");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->shipCount; i++) {
            QFile::copy("../photo/ship/" + QString::number(i) + ".png", flashPath + "photo/ship/" + QString::number(i) + ".png");
        }
    }
}



//env
void MainWindow::clearAllPhotos()
{
    clearMosaicPhotos();
    clearFishPhotos();
}

void MainWindow::clearMosaicPhotos()
{
    QDir dir("../photo/mosaic");
    if (dir.exists())
    {
        for (int i = 1; i < 9; i++) {
            dir.remove(QString::number(i) + ".png");
        }
        m_cameraWidget->mosaicPhotoNum = 1;
    }
}

void MainWindow::clearFishPhotos()
{
    QDir dir("../photo/fish");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++) {
            dir.remove(QString::number(i) + ".png");
        }
        m_cameraWidget->currentFishNum = 1;
    }
}

void MainWindow::clearShipPhotos()
{
    QDir dir("../photo/ship");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->shipCount; i++) {
            dir.remove(QString::number(i) + ".png");
        }
        m_cameraWidget->shipNum = 1;
    }
}


void MainWindow::clearAllFlash()
{
    clearMosaicFlash();
    clearFishFlash();
}
void MainWindow::Clear() {
    QDir dirf("../photo/fish");
    QDir dirm("../photo/mosaic");
    QDir dirs("../photo/ship");
    dirf.removeRecursively();
    dirm.removeRecursively();
    dirs.removeRecursively();
    QDir().mkdir("../photo/fish");
    QDir().mkdir("../photo/mosaic");
    QDir().mkdir("../photo/ship");
}
void MainWindow::clearMosaicFlash()
{
    QDir dir(flashPath + "photo/mosaic");
    if (dir.exists()) {
        for (int i = 1; i < 9; i++) {
            dir.remove(QString::number(i) + ".png");
        }
    }
}

void MainWindow::clearFishFlash()
{
    QDir dir(flashPath + "photo/fish");
    if (dir.exists()) {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++)
        {
            dir.remove(QString::number(i) + ".png");
        }
    }
}

void MainWindow::clearFlash() {
    QDir dirf(flashPath + "photo/fish");
    QDir dirm(flashPath + "photo/mosaic");
    QDir dirs(flashPath + "photo/ship");
    dirf.removeRecursively();
    dirm.removeRecursively();
    dirs.removeRecursively();
    QDir().mkdir(flashPath + "photo/fish");
    QDir().mkdir(flashPath + "photo/mosaic");
    QDir().mkdir(flashPath + "photo/ship");
}

void MainWindow::unmounting() {
    std::string str = "gio mount -ue \"";
    str += flashPath.toStdString();
    str += "\"";
    system(str.c_str());
}

//hotkeys
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Z:
            m_cameraWidget->takeShipPhoto();
            break;
        case Qt::Key_X:
            qDebug() << "ship photo back";
            shipPhotoBack();
            break;
        case Qt::Key_C:
            qDebug() << "clear ship photos";
            clearShipPhotos();
            break;
        case Qt::Key_V:
            qDebug() << "sent ship to flash";
            sendShipToFlash();
            break;

        case Qt::Key_Q:
            m_cameraWidget->takeMosaicPhoto();
            break;
        case Qt::Key_W:
            qDebug() << "mosaic photo back";
            mosaicPhotoBack();
            break;
        case Qt::Key_E:
            qDebug() << "clear mosaic photos";
            clearMosaicPhotos();
            break;
        case Qt::Key_R:
            qDebug() << "sent mosaic to flash";
            sendMosaicToFlash();
            break;

        case Qt::Key_A:
            m_cameraWidget->takeFishPhoto();
            break;
        case Qt::Key_S:
            qDebug() << "fish photo back";
            fishPhotoBack();
            break;
        case Qt::Key_D:
            qDebug() << "clear fish photos";
            clearFishPhotos();
            break;
        case Qt::Key_F:
            qDebug() << "sent fish to flash";
            sendFishToFlash();
            break;
        case Qt::Key_B:
            qDebug() << "clear all";
            Clear();
            break;
        case Qt::Key_N:
            qDebug() << "clear flash";
            clearFlash();
            break;
        case Qt::Key_U:
            qDebug() << "Flash unmount";
            unmounting();
            break;

        case Qt::Key_CapsLock:
            m_cameraWidget->isCountShip = !m_cameraWidget->isCountShip;
            qDebug() << "counting ship" << m_cameraWidget->isCountShip;
            break;
    }
}

//transect
void MainWindow::transect()
{
    if (isTransect)
    {
//        float current_time = QDateTime::currentMSecsSinceEpoch();

//        float depth_error = RovSingleton::instance()->telemetry().depth - keepingDepth;
//        float yaw_error = RovSingleton::instance()->telemetry().yaw - keepingYaw;

//        float z_power = 0;
//        float power1 = 0;
//        float power2 = 0;
//        float power3 = 0;
//        float power4 = 0;

//        float power_value = error * 70;
//        float diff_value = 5 / (current_time - m_depth_prev_time) * (error - m_depth_prev_error);

//        m_power = clamp(power_value + diff_value, 100, -100) * 0.6;

//        RovSingleton::instance()->control().axisZ = static_cast<qint8>(m_power);

//        m_depth_prev_time = current_time;
//        m_depth_prev_error = error;
    }
}
