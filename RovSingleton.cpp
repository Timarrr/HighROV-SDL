#include "RovSingleton.hpp"

#include <QDebug>
#include <QTimer>

qint64 duration = 0;

qint8 directions[8] = {1,1,1,1,1,1,1,1}; //TODO: this

RovSingleton::~RovSingleton() {}

RovSingleton* RovSingleton::instance()
{
    return Singleton<RovSingleton>::instance(RovSingleton::createInstance);
}

RovTelemetry& RovSingleton::telemetry() {
    return m_telemetry;
}

RovControl& RovSingleton::control()
{
    return m_controlData;
}

void RovSingleton::setTransmit(bool transmit)
{
    m_isTransmit = transmit;
}

void RovSingleton::setTransmitTime(int time)
{
    m_transmitTimer->start(time);
}

void RovSingleton::setThrustScaleFactorW(float scale)
{
    m_scaleFactorW = scale;
}

void RovSingleton::setThrustScaleFactorX(float scale)
{
    m_scaleFactorX = scale;
}

void RovSingleton::setThrustScaleFactorY(float scale)
{
    m_scaleFactorY = scale;
}

void RovSingleton::setThrustScaleFactorZ(float scale)
{
    m_scaleFactorZ = scale;
}

void RovSingleton::setThrustScaleFactorD(float scale)
{
    m_scaleFactorD = scale;
}


RovSingleton::RovSingleton(QObject* parent)
    : m_transmitTimer(new QTimer(this))
    , m_udpConnection(new RovUdpConnection(this))

{
    Q_UNUSED(parent)
    m_transmitTimer->start(100);
    createConnections();
}

RovSingleton* RovSingleton::createInstance()
{
    return new RovSingleton();
}

void RovSingleton::createConnections()
{
    QObject::connect(m_udpConnection.data(), &RovUdpConnection::dataReceived,
        [this](QByteArray datagram) {
            if (datagram.size() == 4 && quint8(datagram[0]) == RovHello::header_hello) {
                m_telemetry.version = RovHello::getVersion(datagram);
                return;
            }

            RovTelemetry::RovTelemetryErrorCode ec = RovTelemetry::RovTelemetryErrorCode::WrongDataSize; // TODO?

            if (quint8(datagram[0]) == RovTelemetry::header_telemetry) {
                ec = m_telemetry.fromRangerTelemetryMsgV2(datagram);
            }
            else {
                qWarning() << "Unsupported ROV firmware version, please update your ROV's firmware" << Qt::endl;
            }

            if (ec != RovTelemetry::RovTelemetryErrorCode::NoError) {
                qInfo() << RovTelemetry::fromErrorToString(ec);
                return;
            }

            emit telemetryUpdated();
        });

    QObject::connect(m_transmitTimer.data(), &QTimer::timeout, [this]() {
        if (m_isTransmit) {
            if(!m_controlData.debugFlag){
                qint8 w = m_controlData.axisW * m_scaleFactorW;// rotation
                qint8 x = m_controlData.axisX * m_scaleFactorX;// lag
                qint8 y = m_controlData.axisY * m_scaleFactorY;// forward-backward
                qint8 z = m_controlData.axisZ * m_scaleFactorZ;// up-down
                qint8 d = m_controlData.axisD * m_scaleFactorD;

                //TODO: directions and axes setup

                m_controlData.thrusterPower[0] = directions[0] * (y+x+w);
                m_controlData.thrusterPower[1] = directions[1] * (y-x-w);
                m_controlData.thrusterPower[2] = directions[2] * (y-x+w);
                m_controlData.thrusterPower[3] = directions[3] * (y+x-w);

                m_controlData.thrusterPower[4] = directions[4] * (z+d);
                m_controlData.thrusterPower[5] = directions[5] * (z+d);
                m_controlData.thrusterPower[6] = directions[6] * (z-d);
                m_controlData.thrusterPower[7] = directions[7] * (z-d);

            }

            if (m_telemetry.version == 2) {
                auto data = m_controlData.toRangerControlMsgV2();
                m_udpConnection->transmitDatagram(data);
            } else {
                qWarning() << "Unsupported ROV firmware version, please update your ROV's firmware" << Qt::endl;
            }
        }
    });
}
