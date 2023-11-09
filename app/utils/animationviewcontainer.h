// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "global.h"
#include <QWidget>
#include <DRegionMonitor>
#include <DBlurEffectWidget>

class QPropertyAnimation;
DGUI_USE_NAMESPACE
WIDGETS_FRAME_BEGIN_NAMESPACE
DWIDGET_USE_NAMESPACE
class AnimationViewContainer : public DBlurEffectWidget
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
    void regionMonitorHide(const QPoint & p, const int flag);

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
WIDGETS_FRAME_END_NAMESPACE
