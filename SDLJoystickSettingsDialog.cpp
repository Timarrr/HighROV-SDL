#include "SDLJoystickSettingsDialog.hpp"
#include "SDLJoystick.hpp"
#include "MainWindow.hpp"
#include "RovSingleton.hpp"
#include "SDLJoystick.hpp"
#include "SDL.h"

#include <QVBoxLayout>

SDLJoystickSettingsDialog::SDLJoystickSettingsDialog(QWidget* parent, void* joystick_ptr)
    : QDialog(parent)
    , m_settings(new QSettings(settings_path, QSettings::IniFormat))
    , m_wAxisList(new QComboBox(this))
    , m_xAxisList(new QComboBox(this))
    , m_yAxisList(new QComboBox(this))
    , m_zAxisList(new QComboBox(this))
    , m_dAxisList(new QComboBox(this))
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
    , m_dAxisInv(new QCheckBox(this))
    , m_servoXInv(new QCheckBox(this))
    , m_servoYInv(new QCheckBox(this))
    , m_manipulatorInv(new QCheckBox(this))
{
    SDLJoystick* m_joystick = (SDLJoystick*) joystick_ptr;
    readSettings(m_joystick);
    createLayout(m_joystick);
    createConnections();
}

void SDLJoystickSettingsDialog::setXAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("XID", axis);
    m_settings->setValue("XInverse", inverse);

    m_xID = axis;
    m_isXInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setYAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("YID", axis);
    m_settings->setValue("YInverse", inverse);

    m_yID = axis;
    m_isYInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setZAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("ZID", axis);
    m_settings->setValue("ZInverse", inverse);
    m_zID = axis;
    m_isZInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setWAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("WID", axis);
    m_settings->setValue("WInverse", inverse);
    m_wID = axis;
    m_isWInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setDAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("DID", axis);
    m_settings->setValue("DInverse", inverse);
    m_wID = axis;
    m_isWInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setServoXAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("sXID", axis);
    m_settings->setValue("sXInverse", inverse);
    m_sxID = axis;
    m_isServoXInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setServoYAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("sYID", axis);
    m_settings->setValue("sYInverse", inverse);
    m_syID = axis;
    m_isServoYInv = inverse;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::setManipulatorAxis(int axis, bool inverse)
{
    m_settings->beginGroup("JoystickAxes");
    m_settings->setValue("mID", axis);
    m_settings->setValue("mInverse", inverse);
    m_mID = axis;
    m_isManipulatorInv = inverse;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::manipulatorOpenButton()
{
    return m_openButton;
}

void SDLJoystickSettingsDialog::setOpenButton(int button)
{
    m_settings->beginGroup("JoystickButtons");
    m_settings->setValue("Open", button);
    m_openButton = button;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::manipulatorCloseButton()
{
    return m_closeButton;
}

void SDLJoystickSettingsDialog::setCloseButton(int button)
{
    m_settings->beginGroup("JoystickButtons");
    m_settings->setValue("Close", button);
    m_closeButton = button;
    m_settings->endGroup();
}

int SDLJoystickSettingsDialog::cameraSelectButton()
{
    return m_cameraSelectButton;
}

void SDLJoystickSettingsDialog::setCameraSelectButton(int button)
{
    m_settings->beginGroup("JoystickButtons");
    m_settings->setValue("CamSel", button);
    m_cameraSelectButton = button;
    m_settings->endGroup();
}

void SDLJoystickSettingsDialog::createLayout(void* joystick_ptr)
{
    //change type cus cpp
    SDLJoystick* m_joystick = (SDLJoystick*) joystick_ptr;
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

    auto createBox = [](QComboBox* box, SDLJoystick* m_joystick) {
        for (int axis = 0; axis < m_joystick->get_num_axes() ; axis++) {
            box->addItem(QString("Ось #%1").arg(axis));
        }
    };

    auto createButtonBox = [](QComboBox* box, SDLJoystick* m_joystick) {
        for (int button = 0; button < m_joystick->get_num_buttons(); button++) {
            box->addItem(QString("Кнопка #%1").arg(button));
        }
    };

    createBox(m_wAxisList.data(), m_joystick);
    createBox(m_xAxisList.data(), m_joystick);
    createBox(m_yAxisList.data(), m_joystick);
    createBox(m_zAxisList.data(), m_joystick);
    createBox(m_dAxisList.data(), m_joystick);
    createBox(m_servoXList.data(), m_joystick);
    createBox(m_servoYList.data(), m_joystick);
    createBox(m_rotateManipulatorList.data(), m_joystick);
    createButtonBox(m_openManipulatorList.data(), m_joystick);
    createButtonBox(m_closeManipulatorList.data(), m_joystick);
    createButtonBox(m_cameraSelectButtonList.data(), m_joystick);

    addToLayout(m_wAxisList.data(), QString(tr("Тяга на ось W:")), layout, m_wAxisInv.data());
    addToLayout(m_xAxisList.data(), QString(tr("Тяга на ось X:")), layout, m_xAxisInv.data());
    addToLayout(m_yAxisList.data(), QString(tr("Тяга на ось Y:")), layout, m_yAxisInv.data());
    addToLayout(m_zAxisList.data(), QString(tr("Тяга на ось Z:")), layout, m_zAxisInv.data());
    addToLayout(m_dAxisList.data(), QString(tr("Тяга на ось D:")), layout, m_dAxisInv.data());
    addToLayout(m_servoXList.data(), QString(tr("Тяга на сервопривод 1:")), layout, m_servoXInv.data());
    addToLayout(m_servoYList.data(), QString(tr("Тяга на сервопривод 2:")), layout, m_servoYInv.data());
    addToLayout(m_rotateManipulatorList.data(), QString(tr("Поворот манипулятора:")), layout, m_manipulatorInv.data());
    addButtonsToLayout(m_openManipulatorList.data(), QString(tr("Открыть манипулятор:")), layout);
    addButtonsToLayout(m_closeManipulatorList.data(), QString(tr("Закрыть манипулятор:")), layout);
    addButtonsToLayout(m_cameraSelectButtonList.data(), QString(tr("Переключить камеру:")), layout);

    m_wAxisList.data()->setCurrentIndex(m_wID);
    m_xAxisList.data()->setCurrentIndex(m_xID);
    m_yAxisList.data()->setCurrentIndex(m_yID);
    m_zAxisList.data()->setCurrentIndex(m_zID);
    m_dAxisList.data()->setCurrentIndex(m_dID);
    m_servoXList.data()->setCurrentIndex(m_sxID);
    m_servoYList.data()->setCurrentIndex(m_syID);
    m_rotateManipulatorList.data()->setCurrentIndex(m_mID);
    m_openManipulatorList.data()->setCurrentIndex(m_openButton);
    m_closeManipulatorList.data()->setCurrentIndex(m_closeButton);
    m_cameraSelectButtonList.data()->setCurrentIndex(m_cameraSelectButton);
    setLayout(layout);
}

void SDLJoystickSettingsDialog::readSettings(void* joystick_ptr)
{
    SDLJoystick* m_joystick = (SDLJoystick*) joystick_ptr;

    if(m_joystick == nullptr) return;

    if(m_joystick->isConnected())
        SDL_JoystickUpdate();
    else return;

    m_settings->beginGroup("JoystickAxes");
    m_dID = static_cast<int>(m_settings->value("DID", D).toInt());
    m_wID = static_cast<int>(m_settings->value("WID", W).toInt());
    m_xID = static_cast<int>(m_settings->value("XID", X).toInt());
    m_yID = static_cast<int>(m_settings->value("YID", Y).toInt());
    m_zID = static_cast<int>(m_settings->value("ZID", Z).toInt());
    m_sxID = static_cast<int>(m_settings->value("sXID", PovX).toInt());
    m_syID = static_cast<int>(m_settings->value("sYID", PovY).toInt());
    m_mID = static_cast<int>(m_settings->value("mID", U).toInt());
    m_settings->endGroup();

    m_settings->beginGroup("JoystickAxes");
    m_isWInv = m_settings->value("WInverse", false).toBool();
    m_wAxisInv.data()->setChecked(m_isWInv);
    m_isXInv = m_settings->value("XInverse", false).toBool();
    m_xAxisInv.data()->setChecked(m_isXInv);
    m_isYInv = m_settings->value("YInverse", false).toBool();
    m_yAxisInv.data()->setChecked(m_isYInv);
    m_isZInv = m_settings->value("ZInverse", false).toBool();
    m_zAxisInv.data()->setChecked(m_isZInv);
    m_isDInv = m_settings->value("ZInverse", false).toBool();
    m_dAxisInv.data()->setChecked(m_isDInv);
    m_isServoXInv = m_settings->value("sXInverse", false).toBool();
    m_servoXInv.data()->setChecked(m_isServoXInv);
    m_isServoYInv = m_settings->value("sYInverse", false).toBool();
    m_servoYInv.data()->setChecked(m_isServoYInv);
    m_isManipulatorInv = m_settings->value("mInverse", false).toBool();
    m_manipulatorInv->setChecked(m_isManipulatorInv);
    m_settings->endGroup();

    m_settings->beginGroup("JoystickButtons");
    m_openButton = m_settings->value("Open", 1).toInt();
    m_closeButton = m_settings->value("Close", 0).toInt();
    m_cameraSelectButton = m_settings->value("CamSel", 3).toInt();
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

    QObject::connect(m_dAxisList.data(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        this->setDAxis(index, m_dAxisInv.data()->isChecked());
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

    QObject::connect(m_dAxisInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setDAxis(m_dAxisList.data()->currentIndex(), m_dAxisInv.data()->isChecked());
    });

    QObject::connect(m_servoXInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setServoXAxis(m_servoXList.data()->currentIndex(), m_servoXInv.data()->isChecked());
    });

    QObject::connect(m_servoYInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setServoYAxis(m_servoYList.data()->currentIndex(), m_servoYInv.data()->isChecked());
    });

    QObject::connect(m_manipulatorInv.data(), &QCheckBox::stateChanged, [this](int index) {
        Q_UNUSED(index)
        this->setManipulatorAxis(m_rotateManipulatorList.data()->currentIndex(), m_manipulatorInv.data()->isChecked());
    });
}
