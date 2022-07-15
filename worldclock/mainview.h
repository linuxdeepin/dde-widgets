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

#include <QPointer>
#include <QWidget>

namespace dwclock {
class ClockPanel;
class SettingsView;
class TimezoneModel;
class ViewManager : public QObject
{
    Q_OBJECT
public:
    explicit ViewManager (QObject *parent = nullptr);
    virtual ~ViewManager() override;
    ClockPanel *clockPanel() const;
    SettingsView *settingsView() const;
    TimezoneModel *model() const;
    void updateModel(const QStringList &timezones);

private:
    TimezoneModel *m_model = nullptr;
    mutable ClockPanel *m_clockPanel = nullptr;
    mutable QPointer<SettingsView> m_settingsView = nullptr;
};
}
