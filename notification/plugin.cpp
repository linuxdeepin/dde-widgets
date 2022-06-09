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
#include "accessible/accessible.h"

#include <QAccessible>

QString NotificationWidgetPlugin::title() const
{
    return tr("Notification");
}

QString NotificationWidgetPlugin::description() const
{
    return tr("Notification Center");
}

IWidget *NotificationWidgetPlugin::createWidget()
{
    return new NotificationWidget();
}

bool NotificationWidget::initialize(const QStringList &arguments) {
    Q_UNUSED(arguments);

    m_persistence = new PersistenceObserver();
    m_view = new NotifyCenterWidget(m_persistence);
    m_view->setFixedWidth(handler()->size().width());
    return true;
}

void NotificationWidget::delayInitialize()
{
    // enable accessible
    QAccessible::installFactory(notificationAccessibleFactory);
}
