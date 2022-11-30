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
    , m_settings(new QSettings("RovSettings.ini", QSettings::IniFormat))
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

int SDLJoystick::get_num_axes(){
    return SDL_JoystickNumAxes(m_joystick);
}
int SDLJoystick::get_num_buttons(){
    return SDL_JoystickNumButtons(m_joystick);
}


void SDLJoystick::timerEvent(QTimerEvent*){
    SDL_JoystickUpdate();
    if(isConnected()){
        //refresh settings
        m_settings->beginGroup("JoystickAxes");
        m_x_id = static_cast<int>(m_settings->value("XID").toInt());
        m_y_id = static_cast<int>(m_settings->value("YID").toInt());
        m_z_id = static_cast<int>(m_settings->value("ZID").toInt());
        m_w_id = static_cast<int>(m_settings->value("WID").toInt());
        m_d_id = static_cast<int>(m_settings->value("DID").toInt());
        m_x_inv = static_cast<int>(m_settings->value("XInverse").toInt());
        m_y_inv = static_cast<int>(m_settings->value("YInverse").toInt());
        m_z_inv = static_cast<int>(m_settings->value("ZInverse").toInt());
        m_w_inv = static_cast<int>(m_settings->value("WInverse").toInt());
        m_d_inv = static_cast<int>(m_settings->value("DInverse").toInt());



        // getting data from joystick
        m_x = map(SDL_JoystickGetAxis(m_joystick, m_x_id), -32768, 32767, -100, 100) * (-1 * m_x_inv);
        m_y = map(SDL_JoystickGetAxis(m_joystick, m_y_id), -32768, 32767, -100, 100) * (-1 * m_y_inv);
        m_z = map(SDL_JoystickGetAxis(m_joystick, m_z_id), -32768, 32767, -100, 100) * (-1 * m_z_inv);
        m_w = map(SDL_JoystickGetAxis(m_joystick, m_w_id), -32768, 32767, -100, 100) * (-1 * m_w_inv);
        m_d = map(SDL_JoystickGetAxis(m_joystick, m_d_id), -32768, 32767, -100, 100) * (-1 * m_d_inv);
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
        m_servo_y = hat==1?1:hat==4?-1:0;

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
            qDebug() << "No joystick found" << Qt::endl;

        }
    }
}
