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

#ifndef OPTIMUSSETTINGS_H
#define OPTIMUSSETTINGS_H

#include <QFlags>
#include <QObject>

class QSettings;

class OptimusSettings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(OptimusSettings)

public:
    enum ConfigType {
        Permanent,
        Temporary
    };
    enum SwitchingMethod {
        NoneMethod,
        Nouveau,
        Bbswitch,
        AcpiCall,
        Custom,
    };
    enum PciReset {
        NoReset,
        FunctionLevelReset,
        HotReset
    };
    enum IntelDriver {
        IntelModesetting,
        Intel
    };
    enum AmdDriver {
        AmdModesetting,
        Amd
    };
    enum AccelMethod {
        DefaultMethod,
        SNA,
        XNA,
        UXA
    };
    enum TearFree {
        DefaultTearFree,
        EnableTearFree,
        DisableTearFree
    };
    enum Mode {
        Integrated,
        Nvidia,
        Hybrid,
        Auto
    };
    Q_ENUM(Mode)
    enum DRI {
        DRI2 = 2,
        DRI3 = 3,
    };
    Q_ENUM(DRI)
    enum NvidiaOption {
        Overclocking = 1,
        TripleBuffer = 2
    };
    Q_DECLARE_FLAGS(NvidiaOptions, NvidiaOption)
    enum DynamicPowerManagement {
        No,
        Coarse,
        Fine
    };
    Q_ENUM(DynamicPowerManagement)

    explicit OptimusSettings(QObject *parent = nullptr);
    explicit OptimusSettings(const QString &filename, QObject *parent = nullptr);

    // Optimus
    SwitchingMethod switchingMethod() const;
    void setSwitchingMethod(SwitchingMethod method);
    static SwitchingMethod defaultSwitchingMethod();

    bool isPciPowerControlEnabled() const;
    void setPciPowerControlEnabled(bool enable);
    static bool defaultPciPowerControlEnabled();

    bool isPciRemoveEnabled() const;
    void setPciRemoveEnabled(bool enable);
    static bool defaultPciRemoveEnabled();

    PciReset pciReset() const;
    void setPciReset(PciReset type);
    static PciReset defaultPciReset();

    bool isAutoLogoutEnabled() const;
    void setAutoLogoutEnabled(bool enable);
    static bool defaultAutoLogoutEnabled();

    Mode startupMode() const;
    void setStartupMode(Mode gpu);
    static Mode defaultStartupMode();

    Mode batteryStartupMode() const;
    void setBatteryStartupMode(Mode gpu);
    static Mode defaultBatteryStartupMode();

    Mode externalPowerStartupMode() const;
    void setExternalPowerStartupMode(Mode gpu);
    static Mode defaultExternalPowerStartupMode();

    // Intel
    IntelDriver intelDriver() const;
    void setIntelDriver(IntelDriver driver);
    static IntelDriver defaultIntelDriver();

    AccelMethod intelAccelMethod() const;
    void setIntelAccelMethod(AccelMethod method);
    static AccelMethod defaultIntelAccelMethod();

    TearFree intelTearFree() const;
    void setIntelTearFree(TearFree tearFree);
    static TearFree defaultIntelTearFree();

    DRI intelDri() const;
    void setIntelDri(DRI dri);
    static DRI defaultIntelDri();

    bool isIntelModesetEnabled() const;
    void setIntelModesetEnabled(bool enabled);
    static bool defaultIntelModesetEnabled();

    // Amd
    AmdDriver amdDriver() const;
    void setAmdDriver(AmdDriver driver);
    static AmdDriver defaultAmdDriver();

    TearFree amdTearFree() const;
    void setAmdTearFree(TearFree tearFree);
    static TearFree defaultAmdTearFree();

    DRI amdDri() const;
    void setAmdDri(DRI dri);
    static DRI defaultAmdDri();

    // Nvidia
    bool isNvidiaModesetEnabled() const;
    void setNvidiaModesetEnabled(bool enabled);
    static bool defaultNvidiaModesetEnabled();

    bool isNvidiaPatEnabled() const;
    void setNvidiaPatEnabled(bool enabled);
    static bool defaultNvidiaPatEnabled();

    int nvidiaDpi() const;
    void setNvidiaDpi(int dpi);
    static int defaultNvidiaDpi();

    bool isNvidiaIgnoreAbi() const;
    void setNvidiaIgnoreAbi(bool ignore);
    static bool defaultNvidiaIgnoreAbi();

    bool isNvidiaAllowExternalGpus() const;
    void setNvidiaAllowExternalGpus(bool allow);
    static bool defaultNvidiaAllowExternalGpus();

    NvidiaOptions nvidiaOptions() const;
    void setNvidiaOptions(NvidiaOptions options);
    static NvidiaOptions defaultNvidiaOptions();

    DynamicPowerManagement nvidiaDynamicPowerManagement() const;
    void setNvidiaDynamicPowerManagement(DynamicPowerManagement type);
    static DynamicPowerManagement defaultNvidiaDynamicPowerManagement();

    int nvidiaDynamicPowerManagementThreshold() const;
    void setNvidiaDynamicPowerManagementThreshold(int threshold);
    static int defaultNvidiaDynamicPowerManagementThreshold();

    static QString permanentConfigPath();
    static QPair<QString, ConfigType> detectConfigPath();
    static ConfigType defaultConfigType();

    static QString modeString(Mode gpu);

private:
    static QStringList nvidiaOptionsToStrings(NvidiaOptions options);
    static NvidiaOptions stringToNvidiaOptions(const QStringList &optionStrings);

    QSettings *m_settings;

    // Convert enum values into Optimus Manager strings (no, yes, none etc).
    static const QMap<bool, QString> s_boolMap;
    static const QMap<Mode, QString> s_modeMap;
    static const QMap<SwitchingMethod, QString> s_switchingMethodMap;
    static const QMap<PciReset, QString> s_pciResetMap;
    static const QMap<IntelDriver, QString> s_intelDriverMap;
    static const QMap<AmdDriver, QString> s_amdDriverMap;
    static const QMap<AccelMethod, QString> s_accelMethodMap;
    static const QMap<TearFree, QString> s_tearFreeMap;
    static const QMap<NvidiaOption, QString> s_nvidiaOptionMap;
    static const QMap<DynamicPowerManagement, QString> s_nvidiaDynamicPowerManagementMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(OptimusSettings::NvidiaOptions)

#endif // OPTIMUSSETTINGS_H
