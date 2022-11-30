#pragma once

#include "Crc.hpp"
#include <QDataStream>
#include <QIODevice>
#include <QtCore>

struct RovControl {
    static const uint8_t header_control = 0xAC; // only v2 and later
    qint8 version = 0;

    qint8 axisX = 0; //! -100/100;
    qint8 axisY = 0; //! -100/100;
    qint8 axisZ = 0; //! -100/100;
    qint8 axisW = 0; //! -100/100;
    qint8 axisD = 0; //! -100/100;
    qint8 cameraRotation[2] = { 0, 0 };
    qint8 thrusterPower[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    quint8 debugFlag = 0;
    qint8 manipulatorRotation = 0; //! -100/100
    qint8 manipulatorOpenClose = 0; //! -1 close/+1 open;
    qint8 cameraIndex = 0;

    QByteArray toRangerControlMsgV1()
    {
        QByteArray ba;

        QDataStream in(&ba, QIODevice::WriteOnly);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        in << debugFlag;
        for (int i = 0; i < 6; i++) {
            qint8 t = thrusterPower[i];
            in << t;
        }
        in << manipulatorRotation;
        for (qint8 c : cameraRotation) {
            in << c;
        }
        in << manipulatorOpenClose;
        in << cameraIndex;
        in << calculateCRC(ba.data(), ba.size());

        return ba;
    }

    QByteArray toRangerControlMsgV2()
    {
        QByteArray ba;

        QDataStream in(&ba, QIODevice::WriteOnly);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);

        version = 2;

        // begin v1
        in << header_control;
        in << version;
//        in << axisX;
//        in << axisY;
//        in << axisZ;
//        in << axisW;
        in << debugFlag;
        for (int i = 0; i < 10; i++) {
            qint8 t = thrusterPower[i];
            in << t;
        }
        in << manipulatorRotation;
        for (qint8 c : cameraRotation) {
            in << c;
        }
        in << manipulatorOpenClose;
//        in << regulators;
//        in << desiredDepth;

        // begin v2
//        in << desiredYaw;
        in << cameraIndex;

        in << calculateCRC(ba.data(), ba.size());
        return ba;
    }
};
Q_DECLARE_METATYPE(RovControl)

struct RovTelemetry {
    enum class RovTelemetryErrorCode : int {
        NoError = 0,
        WrongCrc = 1,
        WrongDataSize = 2,
    };

    static const uint8_t header_telemetry = 0xAE; // only v2 and later

    uint8_t header = 0;
    int8_t version = 0;
    float depth = 0.0f;
    float temperature = 0.0f;
    float pitch = 0.0f; //! -180/180;
    float yaw = 0.0f; //! 0 - 360;
    float roll = 0.0f; //! -180/180;
    float ammeter = 0.0f;
    float voltmeter = 0.0f;
    quint8 regulatorsFeedback = 0;
    qint8 manipulatorAngle = 0; //! ???
    qint8 manipulatorState = 0; //! -1 close/+1 open;
    qint8 cameraIndex = 0;

    RovTelemetryErrorCode fromRangerTelemetryMsgV1(QByteArray& ba)
    {
        QDataStream out(&ba, QIODevice::ReadOnly);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);

        out >> depth;
        out >> pitch;
        out >> yaw;
        out >> roll;
        out >> ammeter;
        out >> voltmeter;
        out >> regulatorsFeedback;
        out >> manipulatorAngle;
        out >> manipulatorState;
        qint16 crc = 0;
        out >> crc;

        qint16 currentCrc = calculateCRC(ba.data(), ba.size() - 2);

        if (currentCrc != crc) {
            depth = 0.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            roll = 0.0f;
            voltmeter = 0.0f;
            ammeter = 0.0f;
            regulatorsFeedback = 0;
            manipulatorAngle = 0;
            manipulatorState = 0;
            return RovTelemetryErrorCode::WrongCrc;
        }

        return RovTelemetryErrorCode::NoError;
    }

    RovTelemetryErrorCode fromRangerTelemetryMsgV2(QByteArray& ba)
    {
        QDataStream out(&ba, QIODevice::ReadOnly);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);

        out >> header;
        out >> version;
        out >> depth;
        out >> pitch;
        out >> yaw;
        out >> roll;
        out >> ammeter;
        out >> voltmeter;
        out >> regulatorsFeedback;
        out >> manipulatorAngle;
        out >> manipulatorState;
        out >> cameraIndex;
        out >> temperature;
        qint16 crc = 0;
        out >> crc;

        qint16 currentCrc = calculateCRC(ba.data(), ba.size() - 2);

        if (currentCrc != crc) {
            depth = 0.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            roll = 0.0f;
            voltmeter = 0.0f;
            ammeter = 0.0f;
            regulatorsFeedback = 0;
            manipulatorAngle = 0;
            manipulatorState = 0;
            cameraIndex = 0;
            return RovTelemetryErrorCode::WrongCrc;
        }

        return RovTelemetryErrorCode::NoError;
    }

    static QString fromErrorToString(RovTelemetryErrorCode ec)
    {
        QMap<RovTelemetryErrorCode, QString> ec2str;
        ec2str[RovTelemetryErrorCode::NoError] = "No error";
        ec2str[RovTelemetryErrorCode::WrongCrc] = "CRC missmatch";
        ec2str[RovTelemetryErrorCode::WrongDataSize] = "Wrong data size";

        return ec2str[ec];
    }
};
Q_DECLARE_METATYPE(RovTelemetry)

struct RovHello {
    static const uint8_t header_hello = 0xAA;

    uint8_t header = 0;
    int8_t version = 0;

    static int getVersion(QByteArray& ba)
    {
        QDataStream out(&ba, QIODevice::ReadOnly);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);

        RovHello rh;

        out >> rh.header;
        out >> rh.version;

        qint16 crc = 0;
        out >> crc;

        qint16 currentCrc = calculateCRC(ba.data(), ba.size() - 2);
        if (currentCrc == crc) {
            return rh.version;
        }

        return 0;
    }
};
Q_DECLARE_METATYPE(RovHello)
