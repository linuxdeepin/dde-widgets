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

#include "animationviewcontainer.h"
#include "utils.h"
#include <widgetsinterface.h>

#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QDBusInterface>
#include <QPropertyAnimation>

WIDGETS_USE_NAMESPACE
DGUI_USE_NAMESPACE
WIDGETS_FRAME_BEGIN_NAMESPACE
AnimationViewContainer::AnimationViewContainer(QWidget *parent)
    : DBlurEffectWidget (parent)
{
    setBlurEnabled(false);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedWidth(0);
    setWindowFlags(Qt::Tool);
}

AnimationViewContainer::~AnimationViewContainer()
{
}

void AnimationViewContainer::registerRegion()
{
    if (!m_regionMonitor) {
        QDBusInterface interface("org.deepin.api.XEventMonitor1", "/org/deepin/api/XEventMonitor1",
                                 "org.deepin.api.XEventMonitor1",
                                 QDBusConnection::sessionBus());
        if (!interface.isValid()) {
            qWarning(dwLog()) << "registerRegion error." << interface.lastError().message();
            return;
        }
        m_regionMonitor = new DRegionMonitor(this);
        m_regionMonitor->setCoordinateType(DRegionMonitor::Original);
        connect(m_regionMonitor, &DRegionMonitor::buttonRelease, this, &AnimationViewContainer::regionMonitorHide, Qt::UniqueConnection);
    }

    if (!m_regionMonitor->registered()) {
        m_regionMonitor->registerRegion();
    }
}

void AnimationViewContainer::unRegisterRegion()
{
    if (nullptr == m_regionMonitor)
        return;
    if (m_regionMonitor->registered()) {
        m_regionMonitor->unregisterRegion();
    }
}

void AnimationViewContainer::regionMonitorHide(const QPoint &p)
{
    // avoid to being hiden when app still is active.
    if (qApp->activeWindow())
        return;

    auto m_scale = qApp->primaryScreen()->devicePixelRatio();
    QPoint pScale(int(qreal(p.x() / m_scale)), int(qreal(p.y() / m_scale)));
    if (!geometry().contains(pScale)){
        if (releaseMode()) {
            Q_EMIT outsideAreaReleased();
        }
    }
}

void AnimationViewContainer::showView()
{
    if (m_currentXAni && (m_currentXAni->state() == QAbstractAnimation::Running))
        return;

    if (!m_currentXAni) {
        m_currentXAni = new QPropertyAnimation(this, "currentX");
        const int AnimationTime = 300;
        m_currentXAni->setEasingCurve(QEasingCurve::Linear);
        m_currentXAni->setDuration(AnimationTime);
    }

    const auto &rect = m_targetRect;
    qDebug(dwLog()) << "show view:" << rect;
    show();

    registerRegion();
    m_currentXAni->setStartValue(rect.left());
    m_currentXAni->setEndValue(rect.right());
    m_currentXAni->setDirection(QAbstractAnimation::Backward);
    m_currentXAni->start();
    disconnect(m_currentXAni, &QPropertyAnimation::finished, this, &AnimationViewContainer::hide);
}

void AnimationViewContainer::hideView()
{
    Q_ASSERT(m_currentXAni);
    if (m_currentXAni->state() == QAbstractAnimation::Running)
        return;

    const auto &rect = m_targetRect;
    qDebug(dwLog()) << "hide view" << rect;

    unRegisterRegion();
    m_currentXAni->setStartValue(rect.left());
    m_currentXAni->setEndValue(rect.right());
    m_currentXAni->setDirection(QAbstractAnimation::Forward);
    m_currentXAni->start();
    connect(m_currentXAni, &QPropertyAnimation::finished, this, &AnimationViewContainer::hide, Qt::UniqueConnection);
}

void AnimationViewContainer::updateGeometry(const QRect &rect)
{
    m_currRect = geometry();
    m_targetRect = rect;
    qDebug() << "updateGeometry:" << m_currRect << m_targetRect;
}

int AnimationViewContainer::currentX() const
{
    return QWidget::x();
}

void AnimationViewContainer::setCurrentX(const int x)
{
    auto rect = m_targetRect;
    rect.setLeft(x);
    rect.setWidth(m_targetRect.right() - x);
    setFixedWidth(rect.width());
    setGeometry(rect);
}
WIDGETS_FRAME_END_NAMESPACE
