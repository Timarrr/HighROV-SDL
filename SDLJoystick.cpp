#include "SDLJoystick.hpp"
#include "RovSingleton.hpp"
#include "SDL.h"
#include "QDebug"
bool camsel_prev_state = false;

float map(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

SDLJoystick::SDLJoystick(QObject *parent)
    : QObject{parent}
    , m_jsd(new SDLJoystickSettingsDialog)
{
    SDL_Init(SDL_INIT_JOYSTICK);
    m_joystick = SDL_JoystickOpen(0);
    qDebug() << SDL_JoystickName(m_joystick) << Qt::endl;
    startTimer(50);

}

SDLJoystick::~SDLJoystick(){
    SDL_JoystickClose(m_joystick);
    SDL_Quit();
}

float SDLJoystick::xAxis(){
    return m_x;
}

float SDLJoystick::yAxis(){
    return m_y;
}

float SDLJoystick::zAxis(){
    return m_z;
}

float SDLJoystick::wAxis(){
    return m_w;
}

float SDLJoystick::dAxis(){
    return m_d;
}
int SDLJoystick::servoXAxis(){
    return m_servo_x;
}
int SDLJoystick::servoYAxis(){
    return m_servo_y;
}
bool SDLJoystick::isConnected(){
    return (bool)SDLJoystick::m_joystick;
}

SDL_Joystick* SDLJoystick::get_joystick(){
    return m_joystick;
}


void SDLJoystick::timerEvent(QTimerEvent*){
    SDL_JoystickUpdate();
    if(isConnected()){
        // getting data from joystick
        m_x = map(SDL_JoystickGetAxis(m_joystick, 0), -32768, 32767, -100, 100);
        m_y = map(SDL_JoystickGetAxis(m_joystick, 1), -32768, 32767, -100, 100);
        m_z = map(SDL_JoystickGetAxis(m_joystick, 2), -32768, 32767, -100, 100);
        m_w = map(SDL_JoystickGetAxis(m_joystick, 3), -32768, 32767, -100, 100);
        m_d = map(SDL_JoystickGetAxis(m_joystick, 4), -32768, 32767, -100, 100);
        m_x = (m_x + m_x_old)/2;
        m_y = (m_y + m_y_old)/2;
        m_z = (m_z + m_z_old)/2;
        m_w = (m_w + m_w_old)/2;
        m_d = (m_d + m_d_old)/2;
        m_x_old = m_x;
        m_y_old = m_y;
        m_z_old = m_z;
        m_w_old = m_w;
        m_d_old = m_d;
//        qDebug() << "W: " << m_w << " X: " << m_x << " Y: " << m_y << " Z: " << m_z << " D: " << m_d << Qt::endl;
        //cameras
        int hat = SDL_JoystickGetHat(m_joystick, 0);
        m_servo_y = hat==0?0:hat==1?1:hat==4?-1:0;

        //manipulator
        m_manipulator_open_close = SDL_JoystickGetButton(m_joystick, 1) - SDL_JoystickGetButton(m_joystick, 0);



        //setting the data
        RovSingleton::instance()->control().axisX = static_cast<qint8>(m_x);
        RovSingleton::instance()->control().axisY = static_cast<qint8>(m_y);
        RovSingleton::instance()->control().axisZ = static_cast<qint8>(m_z);
        RovSingleton::instance()->control().axisW = static_cast<qint8>(m_w);
        RovSingleton::instance()->control().axisD = static_cast<qint8>(m_d);
        RovSingleton::instance()->control().cameraRotation[0] = static_cast<qint8>(m_servo_y);
        RovSingleton::instance()->control().cameraRotation[1] = static_cast<qint8>(-m_servo_y);

        RovSingleton::instance()->control().manipulatorRotation = static_cast<qint8>(m_manipulator_rotate);
        RovSingleton::instance()->control().manipulatorOpenClose = static_cast<qint8>(m_manipulator_open_close);
        bool camsel = SDL_JoystickGetButton(m_joystick, 3);
        if(!camsel && camsel_prev_state)
            RovSingleton::instance()->control().cameraIndex = 1 - RovSingleton::instance()->control().cameraIndex;
        camsel_prev_state = camsel;
        qDebug() << "camsel: " << camsel << "prev_camsel: " << camsel_prev_state << "SR latch state: " << m_camsel << Qt::endl;
        if(SDL_JoystickGetButton(m_joystick, 6)) RovSingleton::instance()->setThrustScaleFactor(0.25f);
        if(SDL_JoystickGetButton(m_joystick, 5)) RovSingleton::instance()->setThrustScaleFactor(0.50f);
        if(SDL_JoystickGetButton(m_joystick, 4)) RovSingleton::instance()->setThrustScaleFactor(1.00f);



    }
    else{
        if(SDL_NumJoysticks()>0){
            m_joystick = SDL_JoystickOpen(0);
        }
        else{
            qDebug() << "No joystick found" << endl;

        }
    }
}

SDLJoystickSettingsDialog *SDLJoystick::settingsDialog(){
    return m_jsd.data();
}
