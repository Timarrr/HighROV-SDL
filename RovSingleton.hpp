#pragma once

#include "RovDataTypes.hpp"
#include "RovUdpConnection.hpp"
#include "Singleton.hpp"

class RovSingleton : public QObject {
    Q_OBJECT
public:
    ~RovSingleton();

    static RovSingleton* instance();

    RovTelemetry& telemetry();

    RovControl& control();

    void setTransmit(bool transmit);

    void setTransmitTime(int time);

    void setThrustScaleFactor(float scale);
    int depthRegulator();
signals:
    void telemetryUpdated();

private:
    explicit RovSingleton(QObject* parent = nullptr);

    static RovSingleton* createInstance();

    void createConnections();

    QScopedPointer<QTimer> m_transmitTimer;
    bool m_isTransmit = true;
    QScopedPointer<RovUdpConnection> m_udpConnection;

    RovControl m_controlData;
    RovTelemetry m_telemetry;
    float m_scaleFactor = 1.0f;
};
