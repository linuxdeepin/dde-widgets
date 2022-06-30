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

#include <DDialog>
#include <QListView>
#include <QCompleter>
DWIDGET_USE_NAMESPACE

namespace dwclock {
class TimezoneModel;
class SearchInput;
class ZoneSearch : public QWidget {
    Q_OBJECT
public:
    ZoneSearch(QWidget *parent = nullptr);

Q_SIGNALS:
    void timezoneChanged(const QString &timezone);

private Q_SLOTS:
    void onSelectedChanged();

private:
    SearchInput *m_searchInput = nullptr;
    QCompleter *m_completer = nullptr;
};

class SettingsView : public DDialog {
    Q_OBJECT
public:
    SettingsView(TimezoneModel *model, QWidget *parent = nullptr);

    void selectItem(const int row = 0);

private Q_SLOTS:
    void onTimezoneChanged(const QString &timezone);

public:
    QListView *m_clockView = nullptr;
    TimezoneModel *m_model = nullptr;
    ZoneSearch *m_searchInput = nullptr;
};
}
