/*
 *  Copyright © 2019-2022 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Optimus Manager Qt.
 *
 *  Optimus Manager Qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Optimus Manager Qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Optimus Manager Qt. If not, see <https://www.gnu.org/licenses/>.
 */

#include "optimussettings.h"

#include <QFile>
#include <QSettings>

const QMap<bool, QString> OptimusSettings::s_boolMap = {{false, QStringLiteral("no")},
                                                        {true, QStringLiteral("yes")}};
const QMap<OptimusSettings::Mode, QString> OptimusSettings::s_modeMap = {{Integrated, QStringLiteral("integrated")},
                                                                         {Nvidia, QStringLiteral("nvidia")},
                                                                         {Hybrid, QStringLiteral("hybrid")},
                                                                         {Auto, QStringLiteral("auto")}};
const QMap<OptimusSettings::SwitchingMethod, QString> OptimusSettings::s_switchingMethodMap = {{NoneMethod, QStringLiteral("none")},
                                                                                               {Nouveau, QStringLiteral("nouveau")},
                                                                                               {Bbswitch, QStringLiteral("bbswitch")},
                                                                                               {AcpiCall, QStringLiteral("acpi_call")},
                                                                                               {Custom, QStringLiteral("custom")}};
const QMap<OptimusSettings::PciReset, QString> OptimusSettings::s_pciResetMap = {{NoReset, s_boolMap[false]},
                                                                                 {FunctionLevelReset, QStringLiteral("function_level")},
                                                                                 {HotReset, QStringLiteral("hot_reset")}};
const QMap<OptimusSettings::IntelDriver, QString> OptimusSettings::s_intelDriverMap = {{IntelModesetting, QStringLiteral("modesetting")},
                                                                                       {Intel, QStringLiteral("intel")}};
const QMap<OptimusSettings::AmdDriver, QString> OptimusSettings::s_amdDriverMap = {{AmdModesetting, s_intelDriverMap[IntelModesetting]},
                                                                                   {Amd, QStringLiteral("amdgpu")}};
const QMap<OptimusSettings::AccelMethod, QString> OptimusSettings::s_accelMethodMap = {{DefaultMethod, {}},
                                                                                       {SNA, QStringLiteral("sna")},
                                                                                       {XNA, QStringLiteral("xna")},
                                                                                       {UXA, QStringLiteral("uxa")}};
const QMap<OptimusSettings::TearFree, QString> OptimusSettings::s_tearFreeMap = {{DefaultTearFree, {}},
                                                                                 {EnableTearFree, s_boolMap[true]},
                                                                                 {DisableTearFree, s_boolMap[false]}};
const QMap<OptimusSettings::NvidiaOption, QString> OptimusSettings::s_nvidiaOptionMap = {{Overclocking, QStringLiteral("overclocking")},
                                                                                         {TripleBuffer, QStringLiteral("triple_buffer")}};
const QMap<OptimusSettings::DynamicPowerManagement, QString> OptimusSettings::s_nvidiaDynamicPowerManagementMap = {{No, s_boolMap[false]},
                                                                                                                   {Coarse, QStringLiteral("coarse")},
                                                                                                                   {Fine, QStringLiteral("fine")}};

OptimusSettings::OptimusSettings(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(detectConfigPath().first, QSettings::IniFormat, this))
{
}

OptimusSettings::OptimusSettings(const QString &filename, QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(filename, QSettings::IniFormat, this))
{
}

OptimusSettings::SwitchingMethod OptimusSettings::switchingMethod() const
{
    const QString method = m_settings->value(QStringLiteral("optimus/switching")).toString();
    return s_switchingMethodMap.key(method, defaultSwitchingMethod());
}

void OptimusSettings::setSwitchingMethod(OptimusSettings::SwitchingMethod method)
{
    m_settings->setValue(QStringLiteral("optimus/switching"), s_switchingMethodMap[method]);
}

OptimusSettings::SwitchingMethod OptimusSettings::defaultSwitchingMethod()
{
    return NoneMethod;
}

bool OptimusSettings::isPciPowerControlEnabled() const
{
    const QString powerControlString = m_settings->value(QStringLiteral("optimus/pci_power_control")).toString();
    return s_boolMap.key(powerControlString, defaultPciPowerControlEnabled());
}

void OptimusSettings::setPciPowerControlEnabled(bool enable)
{
    m_settings->setValue(QStringLiteral("optimus/pci_power_control"), s_boolMap[enable]);
}

bool OptimusSettings::defaultPciPowerControlEnabled()
{
    return false;
}

bool OptimusSettings::isPciRemoveEnabled() const
{
    const QString pciRemoveString = m_settings->value(QStringLiteral("optimus/pci_remove")).toString();
    return s_boolMap.key(pciRemoveString, defaultPciRemoveEnabled());
}

void OptimusSettings::setPciRemoveEnabled(bool enable)
{
    m_settings->setValue(QStringLiteral("optimus/pci_remove"), s_boolMap[enable]);
}

bool OptimusSettings::defaultPciRemoveEnabled()
{
    return false;
}

OptimusSettings::PciReset OptimusSettings::pciReset() const
{
    const QString pciResetString = m_settings->value(QStringLiteral("optimus/pci_reset")).toString();
    return s_pciResetMap.key(pciResetString, defaultPciReset());
}

void OptimusSettings::setPciReset(PciReset type)
{
    m_settings->setValue(QStringLiteral("optimus/pci_reset"), s_pciResetMap[type]);
}

OptimusSettings::PciReset OptimusSettings::defaultPciReset()
{
    return NoReset;
}

bool OptimusSettings::isAutoLogoutEnabled() const
{
    const QString terminateSessionsString = m_settings->value(QStringLiteral("optimus/auto_logout")).toString();
    return s_boolMap.key(terminateSessionsString, defaultAutoLogoutEnabled());
}

void OptimusSettings::setAutoLogoutEnabled(bool enable)
{
    m_settings->setValue(QStringLiteral("optimus/auto_logout"), s_boolMap[enable]);
}

bool OptimusSettings::defaultAutoLogoutEnabled()
{
    return true;
}

OptimusSettings::Mode OptimusSettings::startupMode() const
{
    const QString modeString = m_settings->value(QStringLiteral("optimus/startup_mode")).toString();
    return s_modeMap.key(modeString, defaultStartupMode());
}

void OptimusSettings::setStartupMode(OptimusSettings::Mode gpu)
{
    m_settings->setValue(QStringLiteral("optimus/startup_mode"), s_modeMap[gpu]);
}

OptimusSettings::Mode OptimusSettings::defaultStartupMode()
{
    return Integrated;
}

OptimusSettings::Mode OptimusSettings::batteryStartupMode() const
{
    const QString modeString = m_settings->value(QStringLiteral("optimus/startup_auto_battery_mode")).toString();
    return s_modeMap.key(modeString, defaultBatteryStartupMode());
}

void OptimusSettings::setBatteryStartupMode(OptimusSettings::Mode gpu)
{
    m_settings->setValue(QStringLiteral("optimus/startup_auto_battery_mode"), s_modeMap[gpu]);
}

OptimusSettings::Mode OptimusSettings::defaultBatteryStartupMode()
{
    return Integrated;
}

OptimusSettings::Mode OptimusSettings::externalPowerStartupMode() const
{
    const QString gpuString = m_settings->value(QStringLiteral("optimus/startup_auto_extpower_mode")).toString();
    return s_modeMap.key(gpuString, defaultExternalPowerStartupMode());
}

void OptimusSettings::setExternalPowerStartupMode(OptimusSettings::Mode gpu)
{
    m_settings->setValue(QStringLiteral("optimus/startup_auto_extpower_mode"), s_modeMap[gpu]);
}

OptimusSettings::Mode OptimusSettings::defaultExternalPowerStartupMode()
{
    return Nvidia;
}

OptimusSettings::IntelDriver OptimusSettings::intelDriver() const
{
    const QString driverString = m_settings->value(QStringLiteral("intel/driver")).toString();
    return s_intelDriverMap.key(driverString, defaultIntelDriver());
}

void OptimusSettings::setIntelDriver(OptimusSettings::IntelDriver driver)
{
    m_settings->setValue(QStringLiteral("intel/driver"), s_intelDriverMap[driver]);
}

OptimusSettings::IntelDriver OptimusSettings::defaultIntelDriver()
{
    return IntelModesetting;
}

OptimusSettings::AccelMethod OptimusSettings::intelAccelMethod() const
{
    const QString accelMethodString = m_settings->value(QStringLiteral("intel/accel")).toString();
    return s_accelMethodMap.key(accelMethodString, defaultIntelAccelMethod());
}

void OptimusSettings::setIntelAccelMethod(OptimusSettings::AccelMethod method)
{
    m_settings->setValue(QStringLiteral("intel/accel"), s_accelMethodMap[method]);
}

OptimusSettings::AccelMethod OptimusSettings::defaultIntelAccelMethod()
{
    return DefaultMethod;
}

OptimusSettings::TearFree OptimusSettings::intelTearFree() const
{
    const QString tearFreeString = m_settings->value(QStringLiteral("intel/tearfree")).toString();
    return s_tearFreeMap.key(tearFreeString, defaultIntelTearFree());
}

void OptimusSettings::setIntelTearFree(OptimusSettings::TearFree tearFree)
{
    m_settings->setValue(QStringLiteral("intel/tearfree"), s_tearFreeMap[tearFree]);
}

OptimusSettings::TearFree OptimusSettings::defaultIntelTearFree()
{
    return DefaultTearFree;
}

OptimusSettings::DRI OptimusSettings::intelDri() const
{
    return m_settings->value(QStringLiteral("intel/DRI"), defaultIntelDri()).value<DRI>();
}

void OptimusSettings::setIntelDri(OptimusSettings::DRI dri)
{
    m_settings->setValue(QStringLiteral("intel/DRI"), dri);
}

OptimusSettings::DRI OptimusSettings::defaultIntelDri()
{
    return DRI3;
}

bool OptimusSettings::isIntelModesetEnabled() const
{
    const QString modesetString = m_settings->value(QStringLiteral("intel/modeset")).toString();
    return s_boolMap.key(modesetString, defaultIntelModesetEnabled());
}

void OptimusSettings::setIntelModesetEnabled(bool enabled)
{
    m_settings->setValue(QStringLiteral("intel/modeset"), s_boolMap[enabled]);
}

bool OptimusSettings::defaultIntelModesetEnabled()
{
    return true;
}

OptimusSettings::AmdDriver OptimusSettings::amdDriver() const
{
    const QString driverString = m_settings->value(QStringLiteral("amd/driver")).toString();
    return s_amdDriverMap.key(driverString, defaultAmdDriver());
}

void OptimusSettings::setAmdDriver(AmdDriver driver)
{
    m_settings->setValue(QStringLiteral("amd/driver"), s_amdDriverMap[driver]);
}

OptimusSettings::AmdDriver OptimusSettings::defaultAmdDriver()
{
    return AmdModesetting;
}

OptimusSettings::TearFree OptimusSettings::amdTearFree() const
{
    const QString tearFreeString = m_settings->value(QStringLiteral("amd/tearfree")).toString();
    return s_tearFreeMap.key(tearFreeString, defaultAmdTearFree());
}

void OptimusSettings::setAmdTearFree(TearFree tearFree)
{
    m_settings->setValue(QStringLiteral("amd/tearfree"), s_tearFreeMap[tearFree]);
}

OptimusSettings::TearFree OptimusSettings::defaultAmdTearFree()
{
    return DefaultTearFree;
}

OptimusSettings::DRI OptimusSettings::amdDri() const
{
    return m_settings->value(QStringLiteral("amd/DRI"), defaultIntelDri()).value<DRI>();
}

void OptimusSettings::setAmdDri(DRI dri)
{
    m_settings->setValue(QStringLiteral("amd/DRI"), dri);
}

OptimusSettings::DRI OptimusSettings::defaultAmdDri()
{
    return DRI3;
}

bool OptimusSettings::isNvidiaModesetEnabled() const
{
    const QString modesetString = m_settings->value(QStringLiteral("nvidia/modeset")).toString();
    return s_boolMap.key(modesetString, defaultNvidiaModesetEnabled());
}

void OptimusSettings::setNvidiaModesetEnabled(bool enabled)
{
    m_settings->setValue(QStringLiteral("nvidia/modeset"), s_boolMap[enabled]);
}

bool OptimusSettings::defaultNvidiaModesetEnabled()
{
    return true;
}

bool OptimusSettings::isNvidiaPatEnabled() const
{
    const QString patString = m_settings->value(QStringLiteral("nvidia/PAT")).toString();
    return s_boolMap.key(patString, defaultNvidiaPatEnabled());
}

void OptimusSettings::setNvidiaPatEnabled(bool enabled)
{
    m_settings->setValue(QStringLiteral("nvidia/PAT"), s_boolMap[enabled]);
}

bool OptimusSettings::defaultNvidiaPatEnabled()
{
    return true;
}

int OptimusSettings::nvidiaDpi() const
{
    return m_settings->value(QStringLiteral("nvidia/DPI"), defaultNvidiaDpi()).toInt();
}

void OptimusSettings::setNvidiaDpi(int dpi)
{
    if (dpi == 0)
        m_settings->setValue(QStringLiteral("nvidia/DPI"), QString());
    else
        m_settings->setValue(QStringLiteral("nvidia/DPI"), dpi);
}

int OptimusSettings::defaultNvidiaDpi()
{
    return 96;
}

bool OptimusSettings::isNvidiaIgnoreAbi() const
{
    const QString ignoreAbiString = m_settings->value(QStringLiteral("nvidia/ignore_abi")).toString();
    return s_boolMap.key(ignoreAbiString, defaultNvidiaIgnoreAbi());
}

void OptimusSettings::setNvidiaIgnoreAbi(bool ignore)
{
    m_settings->setValue(QStringLiteral("nvidia/ignore_abi"), s_boolMap[ignore]);
}

bool OptimusSettings::defaultNvidiaIgnoreAbi()
{
    return false;
}

bool OptimusSettings::isNvidiaAllowExternalGpus() const
{
    const QString allowExternalGpuString = m_settings->value(QStringLiteral("nvidia/allow_external_gpus")).toString();
    return s_boolMap.key(allowExternalGpuString, defaultNvidiaAllowExternalGpus());
}

void OptimusSettings::setNvidiaAllowExternalGpus(bool allow)
{
    m_settings->setValue(QStringLiteral("nvidia/allow_external_gpus"), s_boolMap[allow]);
}

bool OptimusSettings::defaultNvidiaAllowExternalGpus()
{
    return false;
}

OptimusSettings::NvidiaOptions OptimusSettings::nvidiaOptions() const
{
    const QStringList optionStrings = m_settings->value(QStringLiteral("nvidia/options"), nvidiaOptionsToStrings(defaultNvidiaOptions())).toStringList();
    return stringToNvidiaOptions(optionStrings);
}

void OptimusSettings::setNvidiaOptions(NvidiaOptions options)
{
    m_settings->setValue(QStringLiteral("nvidia/options"), nvidiaOptionsToStrings(options));
}

OptimusSettings::NvidiaOptions OptimusSettings::defaultNvidiaOptions()
{
    return Overclocking;
}

OptimusSettings::DynamicPowerManagement OptimusSettings::nvidiaDynamicPowerManagement() const
{
    const QString dynamicPowerManagementString = m_settings->value(QStringLiteral("nvidia/dynamic_power_management")).toString();
    return s_nvidiaDynamicPowerManagementMap.key(dynamicPowerManagementString, defaultNvidiaDynamicPowerManagement());
}

void OptimusSettings::setNvidiaDynamicPowerManagement(DynamicPowerManagement type)
{
    m_settings->setValue(QStringLiteral("nvidia/dynamic_power_management"), s_nvidiaDynamicPowerManagementMap[type]);
}

OptimusSettings::DynamicPowerManagement OptimusSettings::defaultNvidiaDynamicPowerManagement()
{
    return No;
}

int OptimusSettings::nvidiaDynamicPowerManagementThreshold() const
{
    return m_settings->value(QStringLiteral("nvidia/dynamic_power_management_memory_threshold"), defaultNvidiaDynamicPowerManagementThreshold()).toInt();
}

void OptimusSettings::setNvidiaDynamicPowerManagementThreshold(int threshold)
{
    if (threshold == -1)
        m_settings->remove(QStringLiteral("nvidia/dynamic_power_management_memory_threshold"));
    else
        m_settings->setValue(QStringLiteral("nvidia/dynamic_power_management_memory_threshold"), threshold);
}

int OptimusSettings::defaultNvidiaDynamicPowerManagementThreshold()
{
    return -1;
}

QString OptimusSettings::permanentConfigPath()
{
    return QStringLiteral("/etc/optimus-manager/optimus-manager.conf");
}

QPair<QString, OptimusSettings::ConfigType> OptimusSettings::detectConfigPath()
{
    QFile tempPath(QStringLiteral("/var/lib/optimus-manager/temp_conf_path"));
    if (tempPath.open(QIODevice::ReadOnly))
        return {tempPath.readAll(), Temporary};

    return {permanentConfigPath(), Permanent};
}

OptimusSettings::ConfigType OptimusSettings::defaultConfigType()
{
    return Permanent;
}

QString OptimusSettings::modeString(OptimusSettings::Mode gpu)
{
    return s_modeMap[gpu];
}

QStringList OptimusSettings::nvidiaOptionsToStrings(NvidiaOptions options)
{
    QStringList optionStrings;
    for (auto it = s_nvidiaOptionMap.cbegin(); it != s_nvidiaOptionMap.cend(); ++it) {
        if (options.testFlag(it.key()))
            optionStrings.append(it.value());
    }

    // Set to an empty string, to avoid @Invalid() in configuration file
    if (optionStrings.empty())
        optionStrings.append(QString());

    return optionStrings;
}

OptimusSettings::NvidiaOptions OptimusSettings::stringToNvidiaOptions(const QStringList &optionStrings)
{
    NvidiaOptions options;
    for (auto it = s_nvidiaOptionMap.cbegin(); it != s_nvidiaOptionMap.cend(); ++it) {
        if (optionStrings.contains(it.value()))
            options |= it.key();
    }

    return options;
}
