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

#include "common/accessibledefine.h"

#include "clockpanel.h"
#include "settingsview.h"
#include "zonechooseview.h"
#include "clock.h"

// 添加accessible
namespace dwclock {
SET_FORM_ACCESSIBLE(ClockView, "ClockView")
SET_FORM_ACCESSIBLE(SettingsView, "SettingsView")
SET_FORM_ACCESSIBLE(ZoneSectionView, "ZoneSectionView")
SET_FORM_ACCESSIBLE(ZoneSection, "ZoneSection")
SET_FORM_ACCESSIBLE(ZoneChooseView, "ZoneChooseView")
SET_FORM_ACCESSIBLE(ClockPanel, "ClockPanel")

QAccessibleInterface *accessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *interface = nullptr;

    const QString NamespaceString("dwclock::");
    USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ClockView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), SettingsView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ZoneSectionView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ZoneSection)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ZoneChooseView)
            ELSE_USE_ACCESSIBLE(QString(classname).replace(NamespaceString, ""), ClockPanel)

    return interface;
}
}
