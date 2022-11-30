#ifndef SDLJOYSTICK_H
#define SDLJOYSTICK_H

#include <QObject>
#include "SDL.h"
#include "SDLJoystickSettingsDialog.hpp"

class SDLJoystick : public QObject
{
    Q_OBJECT
public:
    SDLJoystick(QObject *parent = nullptr);
    ~SDLJoystick();
    float xAxis();
    float yAxis();
    float zAxis();
    float wAxis();
    float dAxis();
    int servoXAxis();
    int servoYAxis();
    bool isConnected();
    SDL_Joystick *get_joystick();

    SDLJoystickSettingsDialog *settingsDialog();

private slots:
    void timerEvent(QTimerEvent*);
private:

    SDL_Joystick *m_joystick;
    float m_x = 0; float m_x_old = 0;
    float m_y = 0; float m_y_old = 0;
    float m_z = 0; float m_z_old = 0;
    float m_w = 0; float m_w_old = 0;
    float m_d = 0; float m_d_old = 0;

    int m_servo_x = 0; int m_servo_y = 0;

    int m_manipulator_rotate = 0;
    int m_manipulator_open_close = 0;
    bool m_camsel_pressed = false;
    bool m_camsel = false;

    QScopedPointer<SDLJoystickSettingsDialog> m_jsd;
signals:

};

#endif // SDLJOYSTICK_H
