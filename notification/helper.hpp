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

#include <QDebug>
#include <QCoreApplication>
#include <QDBusInterface>

namespace notify {
class Helper {
public:
    inline static Helper *instance()
    {
        static Helper *g_helper = nullptr;
        if (!g_helper)
            g_helper = new Helper();
        return g_helper;
    }

    inline void showNotificationModuleOfControlCenter();
};

void Helper::showNotificationModuleOfControlCenter()
{
    QDBusMessage message = QDBusMessage::createMethodCall("org.deepin.dde.ControlCenter1", "/org/deepin/dde/ControlCenter1",
                                                          "org.deepin.dde.ControlCenter1","ShowPage");
    message << "notification";
    QDBusConnection::sessionBus().asyncCall(message);
}
}
using namespace notify;
