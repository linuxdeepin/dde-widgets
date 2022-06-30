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

#pragma once

#include <QBasicTimer>
#include <QStandardItemModel>

namespace dwclock {
class TimezoneListModel : public QAbstractListModel {
    Q_OBJECT
public:
    TimezoneListModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

private:
    mutable QStringList m_data;
};

class TimezoneModel : public QStandardItemModel {
    Q_OBJECT
public:
    enum {
        CityName = Qt::UserRole + 1,
        ZoneName,
        ZoneOffset
    } Timezone;

    TimezoneModel(QObject *parent = nullptr);
    virtual ~TimezoneModel();
    void appendItems(const QStringList &timezones);
    void updateTimezone(const QModelIndex &index, const QString &timezone);
    QStringList timezones() const;

    static QStringList defaultLocations();

Q_SIGNALS:
    void timezonesChanged();

private Q_SLOTS:
    void emitTimezoneChanged();

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private:
    void updateTimezone(QStandardItem *item, const QString &timezone);
    void updateTimezoneOffset(QStandardItem *item, const QString &timezone);

    QBasicTimer *m_timezoneChangedTimer = nullptr;
};
}
