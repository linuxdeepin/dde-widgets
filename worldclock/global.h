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

#include <QMargins>
#include <QColor>

namespace dwclock {
namespace UI {
namespace clock {
    static const int topMargin = 25 - 22; // plat's background has margin with 22px.
    static const int bottomMargin = 22;
    static const QMargins defaultMargins(0, 0, 0, 0);
    static const int spacingCityNameAndTimezoneOffset = 3;
    static const int cellSpacing = 0;
    static const QColor panelBackground(247, 247, 247, 0.6 * 255);
    static const int smallAndMiddleDeltaSize = 40;
}
}
}
