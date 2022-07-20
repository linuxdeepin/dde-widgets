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

class TimezoneModel : public QStandardItemModel {
    Q_OBJECT
public:
    enum Timezone {
        CityName = Qt::UserRole + 1,
        ZoneName,
        ZoneOffset
    };

    TimezoneModel(QObject *parent = nullptr);
    virtual ~TimezoneModel();
    void appendItems(const QStringList &timezones);
    void updateTimezone(const QModelIndex &index, const QString &timezone);
    void updateModel(const QStringList &timezones);
    QStringList timezones() const;

    static QStringList defaultLocations();

Q_SIGNALS:
    void timezonesChanged();
    void modifyLocationClicked(const QModelIndex &index);

private Q_SLOTS:
    void emitTimezoneChanged();
    void onModifyLocationActionTriggered();

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private:
    void updateTimezone(QStandardItem *item, const QString &timezone);
    void updateTimezoneOffset(QStandardItem *item, const QString &timezone);

    QBasicTimer *m_timezoneChangedTimer = nullptr;
};
}
