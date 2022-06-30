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

#include <QWidget>

namespace dwclock {
class ClockWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ClockWidget(QWidget *parent = 0);
    virtual ~ClockWidget();

    void setUTCOffset(const int utcOffset);
protected:
    void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void updateClockPointPixmap();
    void updateClockPlatPixmap(const bool isNightMode);
    QPixmap getPixmap(const QString &name, const QSize &size);

    bool m_isBlack = false;
    int m_utcOffset = 0;
    QPixmap m_plat;
    QPixmap m_hour;
    QPixmap m_min;
    QPixmap m_sec;
    QSize m_clockPlatSize;
    QSize m_clockPointSize;
};
}
