#ifndef SDLJOYSTICK_H
#define SDLJOYSTICK_H

#pragma once
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
    int get_num_axes();
    int get_num_buttons();

    SDLJoystickSettingsDialog *settingsDialog();

private slots:
    void timerEvent(QTimerEvent*);
private:

    QScopedPointer<QSettings> m_settings;

    SDL_Joystick *m_joystick;
    float m_x = 0; float m_x_old = 0; int m_x_id = 0; bool m_x_inv = false;
    float m_y = 0; float m_y_old = 0; int m_y_id = 1; bool m_y_inv = false;
    float m_z = 0; float m_z_old = 0; int m_z_id = 2; bool m_z_inv = false;
    float m_w = 0; float m_w_old = 0; int m_w_id = 3; bool m_w_inv = false;
    float m_d = 0; float m_d_old = 0; int m_d_id = 4; bool m_d_inv = false;

    int m_servo_x = 0; int m_servo_y = 0;

    int m_manipulator_rotate = 0;
    int m_manipulator_open_close = 0;
    bool m_camsel_pressed = false;
    bool m_camsel = false;

signals:

};

#endif // SDLJOYSTICK_H
