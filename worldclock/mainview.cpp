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

#include "mainview.h"

#include "clockview.h"
#include "settingsview.h"
#include "timezonemodel.h"

namespace dwclock {
ViewManager::ViewManager(QObject *parent)
    : QObject (parent)
    , m_model(new TimezoneModel())
{
}

ViewManager::~ViewManager()
{
    m_model->deleteLater();
    m_model = nullptr;
}

ClockView *ViewManager::clockView() const
{
    if (!m_clockView) {
        m_clockView = new ClockView();
        m_clockView->setModel(m_model);
    }
    return m_clockView;
}

SettingsView *ViewManager::settingsView() const
{
    if (!m_settingsView) {
        m_settingsView = new SettingsView(m_model);
        m_settingsView->setAttribute(Qt::WA_DeleteOnClose, true);
        m_settingsView->selectItem();
    }
    return m_settingsView;
}

TimezoneModel *ViewManager::model() const
{
    return m_model;
}

void ViewManager::updateModel(const QStringList &timezones)
{
    m_model->clear();
    m_model->appendItems(timezones);
}
}
