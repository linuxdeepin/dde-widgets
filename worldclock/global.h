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
#include <QSize>

namespace dwclock {
namespace UI {
namespace clock {
    static const QMargins defaultMargins(0, 0, 0, 0);
    static const QSize settingsDialogSize(360, 320);
    static const int spacingCityNameAndTimezoneOffset = 4;
    static const int spacingCityNameAndClock = 15;
    static const int cellSpacing = 20;
    static const QColor panelBackground(247, 247, 247, 0.6 * 255);
    static const QColor middleClockBackground(0, 0, 0, 0.05 * 255);
    static const int spacingBackgroundAndClock = 8;
    static const int smallSpacing = 10;
    static const int middleSpacing = 8;
}
namespace edit {
    static const QSize chooseDialogSize(360, 500);
    static const QColor choosePanelBackground(247, 247, 247, 0.8 * 255);
    static const int spacingTitle = 10;
    static const int spacingDesc = 6;
    static const QSize itemSize(340, 36);
    static const int sectionTitleIndent = 10;
}
}
}
