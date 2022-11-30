#include "RovTelemetryWidget.hpp"
#include "RovSingleton.hpp"

#include <QVBoxLayout>

RovTelemetryWidget::RovTelemetryWidget(QWidget* parent)
    : QWidget(parent)
    , m_pitch(new QLabel("Pitch: -", this))
    , m_roll(new QLabel("Roll: -", this))
    , m_yaw(new QLabel("Yaw: -", this))
    , m_depth(new QLabel("Depth: -", this))
    , m_temperature(new QLabel("Temperature: -", this))
    , m_regulatorsFeedback(new QLabel("Regulators: -", this))
    , m_ammeter(new QLabel("Current: - ", this))
    , m_voltmeter(new QLabel("Voltage: -", this))
{
    createConnections();
    createLayout();
}

void RovTelemetryWidget::createConnections()
{
    connect(RovSingleton::instance(), &RovSingleton::telemetryUpdated, this,
        &RovTelemetryWidget::onTelemetryUpdate);
}

void RovTelemetryWidget::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    QWidget* spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->addWidget(m_pitch.data());
    layout->addWidget(m_roll.data());
    layout->addWidget(m_yaw.data());
    layout->addWidget(m_depth.data());
    layout->addWidget(m_temperature.data());
    layout->addWidget(m_regulatorsFeedback.data());
    layout->addWidget(m_ammeter.data());
    layout->addWidget(m_voltmeter.data());

    QFont monospace("Consolas");
    m_pitch.data()->setFont(monospace);
    m_yaw.data()->setFont(monospace);
    m_roll.data()->setFont(monospace);
    m_regulatorsFeedback.data()->setFont(monospace);
    m_depth.data()->setFont(monospace);
    m_temperature.data()->setFont(monospace);
    m_ammeter.data()->setFont(monospace);
    m_voltmeter.data()->setFont(monospace);

    layout->addWidget(spacer);

    setLayout(layout);
}

void RovTelemetryWidget::onTelemetryUpdate()
{
    RovTelemetry& rt = RovSingleton::instance()->telemetry();

    m_pitch.data()->setText(QString(tr("Pitch: %1").arg(QString::number(rt.pitch, 'f', 2))));

    m_yaw.data()->setText(QString(tr("Yaw: %1").arg(QString::number(rt.yaw, 'f', 2))));

    m_roll.data()->setText(QString(tr("Roll: %1").arg(QString::number(rt.roll, 'f', 2))));

    m_regulatorsFeedback.data()->setText(
        QString(tr("Regulators: %1").arg(rt.regulatorsFeedback)));

    m_depth.data()->setText(QString(tr("Depth: %1").arg(QString::number(rt.depth, 'f', 2))));

    m_temperature.data()->setText(QString(tr("Temperature: %1").arg(QString::number(rt.temperature, 'f', 2))));

    m_ammeter.data()->setText(QString(tr("Current: %1 А").arg(QString::number(rt.ammeter, 'f', 2))));

    m_voltmeter.data()->setText(QString(tr("Voltage: %1 В").arg(QString::number(rt.voltmeter, 'f', 2))));
}
