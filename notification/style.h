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

#include <QSize>
#include <QColor>
#include <QMargins>

namespace Notify {
namespace UI {

namespace Panel{
    static const QColor buttonBackground(0, 0, 0, 0.1 * 255);
    static const QSize buttonSize(24, 24);
    static const QSize settingsIconSize(12, 12);
    static const QSize clearIconSize(settingsIconSize);
    static const QSize collapseIconSize(settingsIconSize);
    static const int noNotifyLabelFontAlpha(0.6 * 255);
    static const int settingSpacingBetweenMenu(4);
}
namespace Bubble {
    static const QSize buttonSize(24, 24);
    static const QSize settingsIconSize(10, 10);
    static const QSize clearIconSize(settingsIconSize);
    static const QSize collapseIconSize(settingsIconSize);
}
}
}
using namespace Notify;
