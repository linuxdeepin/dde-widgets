/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wubw <wubowen_cm@deepin.com>
 *
 * Maintainer: wubw <wubowen_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QFont>
#include <QPixmap>

namespace dwclock {
class Clock
{
public:
    explicit Clock();
    virtual ~Clock();

    void setUTCOffset(const int utcOffset);
    void setLocation(const QString &location);

    void paint(QPainter *painter, const QRect &rect);

private:
    void updateClockPixmap(const bool isNightMode);
    void updateLocationFont();
    QPixmap getPixmap(const QString &name, const QSize &size);
    QPixmap getPixmap(const QString &name, const int size, const bool isDark);

    bool m_isBlack = false;
    int m_utcOffset = 0;
    QPixmap m_plat;
    QPixmap m_hour;
    QPixmap m_min;
    QPixmap m_sec;
    QSize m_clockPlatSize;
    QSize m_clockPointSize;
    QString m_location;
    QFont m_locationTxtFont;
};
}
