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

#include "plugin.h"
#include "common/helper.hpp"
#include "accessible/accessible.h"
#include "clockview.h"
#include "settingsview.h"
#include "timezonemodel.h"

namespace dwclock {
QString WorldClockWidgetPlugin::title() const
{
    return tr("World Clock");
}

QString WorldClockWidgetPlugin::description() const
{
    return tr("Display clocks at different geographical locations");
}

IWidget *WorldClockWidgetPlugin::createWidget()
{
    return new WorldClockWidget();
}

QWidget *WorldClockWidget::view()
{
    return m_viewManager->clockView();
}

bool WorldClockWidget::initialize(const QStringList &arguments) {
    Q_UNUSED(arguments);
    static bool hasLoaded = false;
    if (!hasLoaded)
        hasLoaded = BuildinWidgetsHelper::instance()->loadTranslator("dde-widgets-worldclock_");

    m_viewManager = new ViewManager();
    auto clockView = m_viewManager->clockView();
    clockView->setWidgetHandler(handler());

    QObject::connect(m_viewManager->model(), &TimezoneModel::timezonesChanged, m_viewManager, [this]() {
        const QStringList &timezones = m_viewManager->model()->timezones();
        handler()->setValue("locations", timezones);
    });

    return true;
}

void WorldClockWidget::delayInitialize()
{
    QAccessible::installFactory(accessibleFactory);
}

void WorldClockWidget::typeChanged(const IWidget::Type type)
{
    auto clockView = m_viewManager->clockView();
    clockView->setFixedSize(handler()->size());

    if (type == IWidget::Small) {
        const QStringList &defaultLocations = TimezoneModel::defaultLocations();
        const auto data = handler()->value("locations", defaultLocations).toStringList();
        // Get the first location as Model.
        m_viewManager->updateModel({data.first()});
    } else if (type == IWidget::Middle) {
        const QStringList &defaultLocations = TimezoneModel::defaultLocations();
        auto data = handler()->value("locations", defaultLocations).toStringList();
        // Get the locations as Model, and the rest is added from DefaultLocations if possible.
        for (auto item : defaultLocations) {
            if (data.count() >= defaultLocations.count())
                break;
            if (data.contains(item))
                continue;

            data << item;
        }
        m_viewManager->updateModel(data);
    }
}

bool WorldClockWidget::enableSettings()
{
    return true;
}

void WorldClockWidget::settings()
{
    auto settingsView = m_viewManager->settingsView();
    auto pw = m_viewManager->clockView();

    settingsView->move(pw->mapToGlobal(pw->geometry().bottomLeft()));
    settingsView->exec();
}
}
