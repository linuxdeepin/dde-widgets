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

#include <QWidget>
#include <DRegionMonitor>

class QPropertyAnimation;
DGUI_USE_NAMESPACE
class AnimationViewContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentX READ currentX WRITE setCurrentX)
public:
    explicit AnimationViewContainer (QWidget *parent = nullptr);
    virtual ~AnimationViewContainer() override;

    void showView();
    void hideView();
    void updateGeometry(const QRect &rect);

Q_SIGNALS:
    void outsideAreaReleased();

private Q_SLOTS:
    void regionMonitorHide(const QPoint & p);

private:
    int currentX() const;
    void setCurrentX(const int x);
    void registerRegion();
    void unRegisterRegion();

private:
    QRect m_currRect;
    QRect m_targetRect;
    QPropertyAnimation *m_currentXAni = nullptr;
    DRegionMonitor *m_regionMonitor = nullptr;
};
