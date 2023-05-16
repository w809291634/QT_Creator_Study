#include "mybattery.h"
#include "mybatteryplugin.h"

#include <QtPlugin>

MyBatteryPlugin::MyBatteryPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void MyBatteryPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool MyBatteryPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *MyBatteryPlugin::createWidget(QWidget *parent)
{
    return new MyBattery(parent);
}

QString MyBatteryPlugin::name() const
{
    return QLatin1String("MyBattery");
}

QString MyBatteryPlugin::group() const
{
    return QLatin1String("");
}

QIcon MyBatteryPlugin::icon() const
{
    return QIcon();
}

QString MyBatteryPlugin::toolTip() const
{
    return QLatin1String("");
}

QString MyBatteryPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool MyBatteryPlugin::isContainer() const
{
    return false;
}

QString MyBatteryPlugin::domXml() const
{
    return QLatin1String("<widget class=\"MyBattery\" name=\"myBattery\">\n</widget>\n");
}

QString MyBatteryPlugin::includeFile() const
{
    return QLatin1String("mybattery.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mybatteryplugin, MyBatteryPlugin)
#endif // QT_VERSION < 0x050000
