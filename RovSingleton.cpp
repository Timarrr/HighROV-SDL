#include "RovSingleton.hpp"

#include <QDebug>
#include <QTimer>

qint64 duration = 0;

qint8 directions[6] = {-1,-1,-1,1,-1,1};

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

void RovSingleton::setThrustScaleFactor(float scale)
{
    m_scaleFactor = scale;
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

            if (datagram.size() == 29) { // probably v1 telemetry msg
                ec = m_telemetry.fromRangerTelemetryMsgV1(datagram);
            } else if (quint8(datagram[0]) == RovTelemetry::header_telemetry) {
                ec = m_telemetry.fromRangerTelemetryMsgV2(datagram);
            }

            if (ec != RovTelemetry::RovTelemetryErrorCode::NoError) {
                qInfo() << RovTelemetry::fromErrorToString(ec);
                return;
            }

            emit telemetryUpdated();
        });

    QObject::connect(m_transmitTimer.data(), &QTimer::timeout, [this]() {
        if (m_isTransmit) {
            m_controlData.axisW *= m_scaleFactor*.5; // rotation
            m_controlData.axisX *= m_scaleFactor; // lag
            m_controlData.axisZ *= (m_scaleFactor); // up-down
            m_controlData.axisY *= m_scaleFactor; // forward-backward
            if(!m_controlData.debugFlag){
                qint8 w = m_controlData.axisW;
                qint8 x = m_controlData.axisX;
                qint8 y = -m_controlData.axisY;
                qint8 z = m_controlData.axisZ;
                qint8 d = m_controlData.axisD;

                m_controlData.thrusterPower[0] = directions[0] * (y+x+w);
                m_controlData.thrusterPower[1] = directions[1] * (y-x-w);
                m_controlData.thrusterPower[2] = directions[2] * (y-x+w);
                m_controlData.thrusterPower[3] = directions[3] * (y+x-w);

                m_controlData.thrusterPower[4] = directions[4] * (20+z+d);
                m_controlData.thrusterPower[5] = directions[5] * (-20+z-d);
            }

            if (m_telemetry.version == 2) {
                auto data = m_controlData.toRangerControlMsgV2();
                m_udpConnection->transmitDatagram(data);
            } else {
//                m_udpConnection->transmitDatagram(m_controlData.toRangerControlMsgV1());
                qWarning() << "Unsupported ROV firmware version, please update your ROV" << Qt::endl;
            }
        }
    });
}
