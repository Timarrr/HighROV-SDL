#include "SDLJoystickSettingsDialog.hpp"
#include "MainWindow.hpp"
#include "RovSingleton.hpp"
#include "SDLJoystick.hpp"
#include "SDL.h"

#include <QVBoxLayout>

SDLJoystickSettingsDialog::SDLJoystickSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_settings(new QSettings("RovSettings.ini", QSettings::IniFormat))
    , m_wAxisList(new QComboBox(this))
    , m_xAxisList(new QComboBox(this))
    , m_yAxisList(new QComboBox(this))
    , m_zAxisList(new QComboBox(this))
    , m_servoXList(new QComboBox(this))
    , m_servoYList(new QComboBox(this))
    , m_openManipulatorList(new QComboBox(this))
    , m_closeManipulatorList(new QComboBox(this))
    , m_rotateManipulatorList(new QComboBox(this))
    , m_cameraSelectButtonList(new QComboBox(this))
    , m_wAxisInv(new QCheckBox(this))
    , m_xAxisInv(new QCheckBox(this))
    , m_yAxisInv(new QCheckBox(this))
    , m_zAxisInv(new QCheckBox(this))
    , m_servoXInv(new QCheckBox(this))
    , m_servoYInv(new QCheckBox(this))
    , m_manipulatorInv(new QCheckBox(this))
    , m_joystickStatus(new QLabel(this))

{
    readSettings();
    createLayout();
    createConnections();
}

int SDLJoystickSettingsDialog::wAxis()
{
    return m_wAxis;
}

bool SDLJoystickSettingsDialog::isWInverted()
{
    return m_isWInv;
}

void SDLJoystickSettingsDialog::setWAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickAxisW", axis);
    m_settings->setValue("JoystickAxisWIsInv", inverse);
    m_wAxis = axis;
    m_isWInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::xAxis()
{
    return m_xAxis;
}

bool SDLJoystickSettingsDialog::isXInverted()
{
    return m_isXInv;
}

void SDLJoystickSettingsDialog::setXAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickAxisX", axis);
    m_settings->setValue("JoystickAxisXIsInv", inverse);

    m_xAxis = axis;
    m_isXInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::yAxis()
{
    return m_yAxis;
}

bool SDLJoystickSettingsDialog::isYInverted()
{
    return m_isYInv;
}

void SDLJoystickSettingsDialog::setYAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickAxisY", axis);
    m_settings->setValue("JoystickAxisYIsInv", inverse);

    m_yAxis = axis;
    m_isYInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::zAxis()
{
    return m_zAxis;
}

bool SDLJoystickSettingsDialog::isZInverted()
{
    return m_isZInv;
}

void SDLJoystickSettingsDialog::setZAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickAxisZ", axis);
    m_settings->setValue("JoystickAxisZIsInv", inverse);
    m_zAxis = axis;
    m_isZInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::servoXAxis()
{
    return m_servoXAxis;
}

bool SDLJoystickSettingsDialog::isServoXInverted()
{
    return m_isServoXInv;
}

void SDLJoystickSettingsDialog::setServoXAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickServoXAxis", axis);
    m_settings->setValue("JoystickServoXAxisInv", inverse);
    m_servoXAxis = axis;
    m_isServoXInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::servoYAxis()
{
    return m_servoYAxis;
}

bool SDLJoystickSettingsDialog::isServoYInverted()
{
    return m_isServoYInv;
}

void SDLJoystickSettingsDialog::setServoYAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickServoYAxis", axis);
    m_settings->setValue("JoystickServoYAxisInv", inverse);
    m_servoYAxis = axis;
    m_isServoYInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::manipulatorAxis()
{
    return m_manipulatorAxis;
}

bool SDLJoystickSettingsDialog::isManipulatorInverted()
{
    return m_isManipulatorInv;
}

void SDLJoystickSettingsDialog::setManipulatorAxis(int axis, bool inverse)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickManipulatorAxis", axis);
    m_settings->setValue("JoystickManipulatorAxisInv", inverse);
    m_manipulatorAxis = axis;
    m_isManipulatorInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::manipulatorOpenButton()
{
    return m_openButton;
}

void SDLJoystickSettingsDialog::setOpenButton(int button)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickOpenButton", button);
    m_openButton = button;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::manipulatorCloseButton()
{
    return m_closeButton;
}

void SDLJoystickSettingsDialog::setCloseButton(int button)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickCloseButton", button);
    m_closeButton = button;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::cameraSelectButton()
{
    return m_cameraSelectButton;
}

void SDLJoystickSettingsDialog::setCameraSelectButton(int button)
{
    m_settings->beginGroup("Joystick");
    m_settings->setValue("JoystickCameraSelectButton", button);
    m_cameraSelectButton = button;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;

    auto addToLayout = [](QComboBox* box, QString lName, QVBoxLayout* layout, QCheckBox* cbox) {
        QHBoxLayout* hLayout = new QHBoxLayout;
        QLabel* name = new QLabel(lName);
        hLayout->addWidget(name);
        hLayout->addWidget(box);
        hLayout->addWidget(cbox);
        layout->addLayout(hLayout);
    };

    auto addButtonsToLayout = [](QComboBox* box, QString lName, QVBoxLayout* layout) {
        QHBoxLayout* hLayout = new QHBoxLayout;
        QLabel* name = new QLabel(lName);
        hLayout->addWidget(name);
        hLayout->addWidget(box);
        layout->addLayout(hLayout);
    };

    auto createBox = [this](QComboBox* box) {
//        SDL_Joystick *joystick = MainWindow::get_joystick()->get_joystick();
        for (int axis = 0; axis < 6 ; ++axis) {
            box->addItem(QString("Ось #%1").arg(axis));
        }
    };

    auto createButtonBox = [this](QComboBox* box) {
//        SDL_Joystick *joystick = MainWindow::get_joystick()->get_joystick();
        for (int buttons = 0; buttons < 11; ++buttons) {
            box->addItem(QString("Кнопка #%1").arg(buttons));
        }
    };

    createBox(m_wAxisList.data());
    createBox(m_xAxisList.data());
    createBox(m_yAxisList.data());
    createBox(m_zAxisList.data());
    createBox(m_servoXList.data());
    createBox(m_servoYList.data());
    createBox(m_rotateManipulatorList.data());
    createButtonBox(m_openManipulatorList.data());
    createButtonBox(m_closeManipulatorList.data());
    createButtonBox(m_cameraSelectButtonList.data());

    addToLayout(m_wAxisList.data(), QString(tr("Тяга на ось W:")), layout, m_wAxisInv.data());
    addToLayout(m_xAxisList.data(), QString(tr("Тяга на ось X:")), layout, m_xAxisInv.data());
    addToLayout(m_yAxisList.data(), QString(tr("Тяга на ось Y:")), layout, m_yAxisInv.data());
    addToLayout(m_zAxisList.data(), QString(tr("Тяга на ось Z:")), layout, m_zAxisInv.data());
    addToLayout(m_servoXList.data(), QString(tr("Тяга на сервопривод 1:")), layout, m_servoXInv.data());
    addToLayout(m_servoYList.data(), QString(tr("Тяга на сервопривод 2:")), layout, m_servoYInv.data());
    addToLayout(m_rotateManipulatorList.data(), QString(tr("Поворот манипулятора:")), layout, m_manipulatorInv.data());
    addButtonsToLayout(m_openManipulatorList.data(), QString(tr("Открыть манипулятор:")), layout);
    addButtonsToLayout(m_closeManipulatorList.data(), QString(tr("Закрыть манипулятор:")), layout);
    addButtonsToLayout(m_cameraSelectButtonList.data(), QString(tr("Переключить камеру:")), layout);

    layout->addWidget(m_joystickStatus.data());

    m_wAxisList.data()->setCurrentIndex(static_cast<int>(wAxis()));
    m_xAxisList.data()->setCurrentIndex(static_cast<int>(xAxis()));
    m_yAxisList.data()->setCurrentIndex(static_cast<int>(yAxis()));
    m_zAxisList.data()->setCurrentIndex(static_cast<int>(zAxis()));
    m_servoXList.data()->setCurrentIndex(static_cast<int>(servoXAxis()));
    m_servoYList.data()->setCurrentIndex(static_cast<int>(servoYAxis()));
    m_rotateManipulatorList.data()->setCurrentIndex(static_cast<int>(manipulatorAxis()));
    m_openManipulatorList.data()->setCurrentIndex(static_cast<int>(m_openButton));
    m_closeManipulatorList.data()->setCurrentIndex(static_cast<int>(m_closeButton));
    m_cameraSelectButtonList.data()->setCurrentIndex(static_cast<int>(m_cameraSelectButton));
    setLayout(layout);
}

void SDLJoystickSettingsDialog::readSettings()
{
    SDL_JoystickUpdate();
    m_settings->beginGroup("Joystick");

    m_zAxis = static_cast<int>(m_settings->value("JoystickAxisD", D).toInt());
    m_wAxis = static_cast<int>(m_settings->value("JoystickAxisW", W).toInt());
    m_xAxis = static_cast<int>(m_settings->value("JoystickAxisX", X).toInt());
    m_yAxis = static_cast<int>(m_settings->value("JoystickAxisY", Y).toInt());
    m_zAxis = static_cast<int>(m_settings->value("JoystickAxisZ", Z).toInt());
    m_servoXAxis = static_cast<int>(m_settings->value("JoystickServoXAxis", PovX).toInt());
    m_servoYAxis = static_cast<int>(m_settings->value("JoystickServoYAxis", PovY).toInt());
    m_manipulatorAxis = static_cast<int>(m_settings->value("JoystickManipulatorAxis", U).toInt());

    m_isWInv = m_settings->value("JoystickAxisWIsInv", false).toBool();
    m_wAxisInv.data()->setChecked(m_isWInv);
    m_isXInv = m_settings->value("JoystickAxisXIsInv", false).toBool();
    m_xAxisInv.data()->setChecked(m_isXInv);
    m_isYInv = m_settings->value("JoystickAxisYIsInv", false).toBool();
    m_yAxisInv.data()->setChecked(m_isYInv);
    m_isZInv = m_settings->value("JoystickAxisZIsInv", false).toBool();
    m_zAxisInv.data()->setChecked(m_isZInv);
    m_isServoXInv = m_settings->value("JoystickServoXAxisInv", false).toBool();
    m_servoXInv.data()->setChecked(m_isServoXInv);
    m_isServoYInv = m_settings->value("JoystickServoYAxisInv", false).toBool();
    m_servoYInv.data()->setChecked(m_isServoYInv);
    m_isManipulatorInv = m_settings->value("JoystickManipulatorAxisInv", false).toBool();
    m_manipulatorInv->setChecked(m_isManipulatorInv);

    m_openButton = m_settings->value("JoystickOpenButton", 1).toInt();
    m_closeButton = m_settings->value("JoystickCloseButton", 0).toInt();
    m_cameraSelectButton = m_settings->value("JoystickCameraSelectButton", 3).toInt();

    m_settings->endGroup();
}


void SDLJoystickSettingsDialog::createConnections()
{
    QObject::connect(m_wAxisList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setWAxis(index, m_wAxisInv.data()->isChecked());
    });

    QObject::connect(m_xAxisList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setXAxis(index, m_xAxisInv.data()->isChecked());
    });

    QObject::connect(m_yAxisList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setYAxis(index, m_yAxisInv.data()->isChecked());
    });

    QObject::connect(m_zAxisList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setZAxis(index, m_zAxisInv.data()->isChecked());
    });

    QObject::connect(m_servoXList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setServoXAxis(index, m_servoXInv.data()->isChecked());
    });

    QObject::connect(m_servoYList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setServoYAxis(index, m_servoYInv.data()->isChecked());
    });

    QObject::connect(m_rotateManipulatorList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setManipulatorAxis(index, m_manipulatorInv.data()->isChecked());
    });

    QObject::connect(m_openManipulatorList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setOpenButton(index);
    });

    QObject::connect(m_closeManipulatorList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setCloseButton(index);
    });

    QObject::connect(m_cameraSelectButtonList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setCameraSelectButton(index);
    });

    QObject::connect(m_wAxisInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setWAxis(m_wAxisList.data()->currentIndex(), m_wAxisInv.data()->isChecked());
    });

    QObject::connect(m_xAxisInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setXAxis(m_xAxisList.data()->currentIndex(), m_xAxisInv.data()->isChecked());
    });

    QObject::connect(m_yAxisInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setYAxis(m_yAxisList.data()->currentIndex(), m_yAxisInv.data()->isChecked());
    });

    QObject::connect(m_zAxisInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setZAxis(m_zAxisList.data()->currentIndex(), m_zAxisInv.data()->isChecked());
    });

    QObject::connect(m_servoXInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setZAxis(m_servoXList.data()->currentIndex(), m_servoXInv.data()->isChecked());
    });

    QObject::connect(m_servoYInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setZAxis(m_servoYList.data()->currentIndex(), m_servoYInv.data()->isChecked());
    });

    QObject::connect(m_manipulatorInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setManipulatorAxis(m_rotateManipulatorList.data()->currentIndex(), m_manipulatorInv.data()->isChecked());
    });
}
