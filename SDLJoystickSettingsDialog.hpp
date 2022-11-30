#ifndef SDLJOYSTICKSETTINGSDIALOG_H
#define SDLJOYSTICKSETTINGSDIALOG_H
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QSettings>
#include <QLabel>

class SDLJoystickSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SDLJoystickSettingsDialog(QWidget* parent = nullptr, void* m_joystick = nullptr);


    void setXAxis(int axis, bool inverse);

    void setYAxis(int axis, bool inverse);

    void setZAxis(int axis, bool inverse);

    void setWAxis(int axis, bool inverse);

    void setDAxis(int axis, bool inverse);

    void setServoXAxis(int axis, bool inverse);

    void setServoYAxis(int axis, bool inverse);

    void setManipulatorAxis(int axis, bool inverse);

    int manipulatorOpenButton();
    void setOpenButton(int button);

    int manipulatorCloseButton();
    void setCloseButton(int button);

    int cameraSelectButton();
    void setCameraSelectButton(int button);
private:
    void readSettings(void* joystick_ptr = nullptr);
    void createLayout(void* joystick_ptr = nullptr);
    void createConnections();

    QScopedPointer<QSettings> m_settings;

    QScopedPointer<QComboBox> m_wAxisList;
    QScopedPointer<QComboBox> m_xAxisList;
    QScopedPointer<QComboBox> m_yAxisList;
    QScopedPointer<QComboBox> m_zAxisList;
    QScopedPointer<QComboBox> m_dAxisList;
    QScopedPointer<QComboBox> m_servoXList;
    QScopedPointer<QComboBox> m_servoYList;
    QScopedPointer<QComboBox> m_openManipulatorList;
    QScopedPointer<QComboBox> m_closeManipulatorList;
    QScopedPointer<QComboBox> m_rotateManipulatorList;
    QScopedPointer<QComboBox> m_cameraSelectButtonList;

    QScopedPointer<QCheckBox> m_wAxisInv;
    QScopedPointer<QCheckBox> m_xAxisInv;
    QScopedPointer<QCheckBox> m_yAxisInv;
    QScopedPointer<QCheckBox> m_zAxisInv;
    QScopedPointer<QCheckBox> m_dAxisInv;
    QScopedPointer<QCheckBox> m_servoXInv;
    QScopedPointer<QCheckBox> m_servoYInv;
    QScopedPointer<QCheckBox> m_manipulatorInv;

    int m_xID;
    int m_yID;
    int m_zID;
    int m_wID;
    int m_dID;
    int m_sxID;
    int m_syID;
    int m_mID;

    enum joystickAxes{
        Y = 0,
        X,
        Z,
        W,
        D,
        PovX,
        PovY,
        U
    };

    int m_openButton;
    int m_closeButton;
    int m_cameraSelectButton;

    bool m_isXInv = false;
    bool m_isYInv = false;
    bool m_isZInv = false;
    bool m_isDInv = false;
    bool m_isWInv = false;
    bool m_isServoXInv = false;
    bool m_isServoYInv = false;
    bool m_isManipulatorInv = false;

};

#endif // SDLJOYSTICKSETTINGSDIALOG_H
