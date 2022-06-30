/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "timezonemodel.h"
#include "utils/zoneinfo.h"
#include "utils/timezone.h"

#include <QDebug>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QEvent>

namespace dwclock {
static const QString TimedateService = QStringLiteral("com.deepin.daemon.Timedate");
static const QString TimedatePath = QStringLiteral("/com/deepin/daemon/Timedate");
static const QString TimedateInterface = QStringLiteral("com.deepin.daemon.Timedate");

static QStringList timezoneAndLocals()
{
    QStringList zones;
    for (const auto &zoneInfo : installer::GetZoneInfoList()) {
        auto timezone = zoneInfo.timezone;
        zones << timezone; // timezone as completion candidate.

        // localized timezone as completion candidate.
        const QString locale = QLocale::system().name();
        QString localizedTimezone = installer::GetLocalTimezoneName(timezone, locale);
        zones << localizedTimezone;
    }
    return zones;
}

TimezoneListModel::TimezoneListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int TimezoneListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_data.isEmpty())
        m_data = timezoneAndLocals();

    return m_data.count();
}

QVariant TimezoneListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto row = index.row();

    const bool isLocalZonetime = row % 2;
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        return m_data[row];
    } else if (role == TimezoneModel::CityName) {
        return isLocalZonetime ? m_data[row] : m_data[row + 1];
    } else if (role == TimezoneModel::ZoneName) {
        return isLocalZonetime ? m_data[row - 1] : m_data[row];
    }

    return QVariant();
}

static QString getTimezoneCity(const QString &timezone)
{
    const QString locale = QLocale::system().name();
    return installer::GetLocalTimezoneName(timezone, locale);
}

TimezoneModel::TimezoneModel(QObject *parent)
    : QStandardItemModel(parent)
{
    connect(this, &TimezoneModel::dataChanged, this, &TimezoneModel::emitTimezoneChanged);
}

TimezoneModel::~TimezoneModel()
{
    if (m_timezoneChangedTimer) {
        m_timezoneChangedTimer->stop();
        delete m_timezoneChangedTimer;
    }
}

void TimezoneModel::appendItems(const QStringList &timezones)
{
    QList<QStandardItem *> items;
    for (int i = 0; i < timezones.count(); ++i) {
        const auto &timezone = timezones[i];
        auto cityName = getTimezoneCity(timezone);
        if (cityName.isEmpty()) {
            qWarning() << "GetZoneInfoByZone() error for the zone:" << timezone;
            cityName = timezone;
        }

        auto itemModel = new QStandardItem();
        itemModel->setDragEnabled(true);
        itemModel->setDropEnabled(false);
        itemModel->setEditable(false);

        itemModel->setText(cityName);
        itemModel->setData(timezone, ZoneName);
        updateTimezoneOffset(itemModel, timezone);
        items << itemModel;

    }
    appendColumn(items);
}

void TimezoneModel::updateTimezone(const QModelIndex &index, const QString &timezone)
{
    auto itemModel = item(index.row());
    if (!itemModel)
        return;

    const auto &cityName = getTimezoneCity(timezone);
    itemModel->setText(cityName);
    itemModel->setData(timezone, ZoneName);
    updateTimezoneOffset(itemModel, timezone);
}

QStringList TimezoneModel::timezones() const
{
    QStringList result;
    for (int i = 0; i < rowCount(QModelIndex()); i++) {
        result << data(index(i, 0), ZoneName).toString();
    }
    return result;
}

QStringList TimezoneModel::defaultLocations()
{
    return QStringList{"Asia/Beijing", "Asia/Shanghai", "Asia/Wuhan", "Asia/Xian"};
}

void TimezoneModel::updateTimezoneOffset(QStandardItem *item, const QString &timezone)
{
    QDBusInterface interface (TimedateService, TimedatePath, TimedateInterface, QDBusConnection::sessionBus());
    auto reply = interface.asyncCall("GetZoneInfo", timezone);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this,
                     [this, timezone, item](QDBusPendingCallWatcher* call) {
        static bool Registered = false;
        if (!Registered)
            registerZoneInfoMetaType();

        QDBusPendingReply<ZoneInfo> reply = *call;
        if (reply.isError()) {
            qWarning() << "It's error for GetZoneInfo [" << timezone << "] :" << reply.error().message();
        } else {
            const auto info = reply.value();
            item->setData(info.getUTCOffset(), ZoneOffset);
        }
        call->deleteLater();
    });
}

void TimezoneModel::emitTimezoneChanged()
{
    if (m_timezoneChangedTimer && m_timezoneChangedTimer->isActive())
        return;

    if (!m_timezoneChangedTimer)
        m_timezoneChangedTimer = new QBasicTimer();

    m_timezoneChangedTimer->start(100, this);
}

void TimezoneModel::timerEvent(QTimerEvent *event)
{
    if (m_timezoneChangedTimer && m_timezoneChangedTimer->timerId() == event->timerId()) {
        Q_EMIT timezonesChanged();
        m_timezoneChangedTimer->stop();
    }
}
}
