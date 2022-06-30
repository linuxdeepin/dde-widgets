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

#include "accessibledefine.h"

#include "clockview.h"
#include "settingsview.h"
#include "clock.h"

// 添加accessible
namespace dwclock {
SET_FORM_ACCESSIBLE(ClockView, "ClockView")
SET_FORM_ACCESSIBLE(SettingsView, "SettingsView")
SET_FORM_ACCESSIBLE(ZoneSearch, "ZoneSearch")
SET_FORM_ACCESSIBLE(ClockWidget, "ClockWidget")

QAccessibleInterface *accessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *interface = nullptr;

    const QString NamespaceString("dwclock::");
    USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ClockView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), SettingsView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ZoneSearch)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ClockWidget)

    return interface;
}
}
