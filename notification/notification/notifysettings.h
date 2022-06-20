/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
 *             yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOTIFYSETTINGS_H
#define NOTIFYSETTINGS_H

#include <com_deepin_dde_daemon_launcherd.h>
#include "types/launcheriteminfo.h"
#include "types/launcheriteminfolist.h"

#include <QObject>

class QGSettings;
class QTimer;

using LauncherInter = com::deepin::dde::daemon::Launcher;

class AbstractNotifySetting : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        APPNAME,
        APPICON,
        ENABELNOTIFICATION,
        ENABELPREVIEW,
        ENABELSOUND,
        SHOWINNOTIFICATIONCENTER,
        LOCKSCREENSHOWNOTIFICATION,
        SHOWNOTIFICATIONTOP
    } AppConfigurationItem;

    typedef enum {
        DNDMODE,
        LOCKSCREENOPENDNDMODE,
        OPENBYTIMEINTERVAL,
        STARTTIME,
        ENDTIME,
        SHOWICON
    } SystemConfigurationItem;

    explicit AbstractNotifySetting(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual void initAllSettings() = 0;
    virtual void setAppSetting(const QString &id, const AppConfigurationItem &item, const QVariant &var) = 0;
    virtual QVariant getAppSetting(const QString &id, const AppConfigurationItem &item) = 0;
    virtual void setSystemSetting(const SystemConfigurationItem &item, const QVariant &var) = 0;
    virtual QVariant getSystemSetting(const SystemConfigurationItem &item) = 0;
    virtual QStringList getAppLists() = 0;
    virtual void appAdded(const LauncherItemInfo &info) = 0;
    virtual void appRemoved(const QString &id) = 0;

signals:
    void appAddedSignal(const QString &id);
    void appRemovedSignal(const QString &id);
    void appSettingChanged(const QString &id, const uint &item, QVariant var);
    void systemSettingChanged(const uint &item, QVariant var);
};

class NotifySettingsObserver : public AbstractNotifySetting
{
    Q_OBJECT
public:
    explicit NotifySettingsObserver(QObject *parent = nullptr);
    void initAllSettings() override { Q_UNREACHABLE(); }
    void setAppSetting(const QString &id, const AppConfigurationItem &item, const QVariant &var) override;
    QVariant getAppSetting(const QString &id, const AppConfigurationItem &item) override;
    void setSystemSetting(const SystemConfigurationItem &, const QVariant &) override { Q_UNREACHABLE(); }
    QVariant getSystemSetting(const SystemConfigurationItem &item) override { Q_UNUSED(item); Q_UNREACHABLE(); return QVariant();}
    QStringList getAppLists() override { Q_UNREACHABLE(); return QStringList();}
    void appAdded(const LauncherItemInfo &info) override { Q_UNUSED(info); Q_UNREACHABLE(); }
    void appRemoved(const QString &id) override { Q_UNUSED(id); Q_UNREACHABLE(); }

private Q_SLOTS:
    void onReceivedAppInfoChanged(const QString &id, uint item, QDBusVariant var);

private:
    QDBusInterface *notifyObserver();

    QScopedPointer<QDBusInterface> m_notifyObserver;
};

#endif // NOTIFYSETTINGS_H
