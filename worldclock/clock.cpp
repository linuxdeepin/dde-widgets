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

#include "clock.h"

#include <QPainter>
#include <QTimeZone>
#include <QIcon>
#include <QDebug>
#include <QResizeEvent>

namespace dwclock {
static const QSize baseClockSize = QSize(224, 224);
static const QSize basePointSize = QSize(145, 15);

ClockWidget::ClockWidget(QWidget *parent)
    : QWidget(parent)
{
}

ClockWidget::~ClockWidget()
{

}

QPixmap ClockWidget::getPixmap(const QString &name, const QSize &size)
{
    const QIcon &icon = QIcon::fromTheme(name);
    const qreal ratio = devicePixelRatioF();
    QPixmap pixmap = icon.pixmap(size * ratio).scaled(size * ratio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPainter p(&pixmap);
    p.setRenderHints(QPainter::Antialiasing);
    p.drawPixmap(0, 0, pixmap);
    pixmap.setDevicePixelRatio(ratio);
    return pixmap;
}

void ClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!m_clockPlatSize.isValid() || !m_clockPointSize.isValid())
        return;

    QDateTime datetime(QDateTime::currentDateTime());
    datetime = datetime.addSecs(datetime.offsetFromUtc() - m_utcOffset);
    const QTime time(datetime.time());

    updateClockPointPixmap();
    const bool nightMode = !(time.hour() >= 6 && time.hour() < 18);
    updateClockPlatPixmap(nightMode);

    const auto center = rect().center();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // draw plate
    QRect platRect = m_plat.rect();
    platRect.moveCenter(center);
    painter.drawPixmap(platRect, m_plat);

    int nHour = (time.hour() >= 12) ? (time.hour() - 12) : time.hour();
    int nStartAngle = 90; //The image from 0 start , but the clock need from -90 start

    const QPointF clockPointPosition(-m_clockPointSize.width() / 2.0, -m_clockPointSize.height() / 2.0);
    // draw hour hand
    const qreal hourAngle = qreal(nHour * 30 + time.minute() * 30 / 60 + time.second() * 30 / 60 / 60 - nStartAngle);
    painter.save();
    painter.translate(center);
    painter.rotate(hourAngle);
    painter.drawPixmap(clockPointPosition, m_hour);
    painter.restore();

    // draw minute hand
    const qreal minuteAngle = qreal(time.minute() * 6 + time.second() * 6 / 60 - nStartAngle);
    painter.save();
    painter.translate(center);
    painter.rotate(minuteAngle);
    painter.drawPixmap(clockPointPosition, m_min);
    painter.restore();

    // draw second hand
    const qreal secondAngle = qreal(time.second() * 6 - nStartAngle);
    painter.save();
    painter.translate(center);
    painter.rotate(secondAngle);
    painter.drawPixmap(clockPointPosition, m_sec);
    painter.restore();
}

void ClockWidget::setUTCOffset(const int utcOffset)
{
    if (m_utcOffset == utcOffset)
        return;

    m_utcOffset = utcOffset;
    update();
}

void ClockWidget::resizeEvent(QResizeEvent *event)
{
    if (event->size() != event->oldSize()) {
        m_clockPlatSize = event->size();
        const auto ratio = m_clockPlatSize.width() * 1.0 / baseClockSize.width();
        m_clockPointSize = basePointSize * ratio;
    }

    return QWidget::resizeEvent(event);
}

void ClockWidget::updateClockPointPixmap()
{
    if (m_hour.size() == m_clockPointSize)
        return;

    m_hour = getPixmap("dcc_noun_hour", m_clockPointSize);
    m_min = getPixmap("dcc_noun_minute", m_clockPointSize);
    m_sec = getPixmap("dcc_noun_second", m_clockPointSize);
}

void ClockWidget::updateClockPlatPixmap(const bool isNightMode)
{
    if (isNightMode != m_isBlack || m_plat.size() != m_clockPlatSize) {
        m_plat = getPixmap(isNightMode ? "dcc_clock_black" : "dcc_clock_white", m_clockPlatSize);
        m_isBlack = !m_isBlack;
    }
}
}
