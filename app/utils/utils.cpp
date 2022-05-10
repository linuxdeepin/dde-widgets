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

#include "utils.h"
#include <QDebug>

#include <QMoveEvent>
#include <QWidget>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QPropertyAnimation>
#include <QLabel>
#include <DWindowManagerHelper>
DGUI_USE_NAMESPACE

DragDropFilter::DragDropFilter(QObject *parent)
    : QObject(parent)
{
}

bool DragDropFilter::eventFilter(QObject *obj, QEvent *event) {
    if (auto mouseEvent = dynamic_cast<QMouseEvent *>(event)) {
        if (mouseEvent->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress) {
            pressFlag = true;
        } else if (mouseEvent->type() == QEvent::MouseMove) {
            if (pressFlag) {
                pressFlag = false;
                auto pre = dynamic_cast<QMouseEvent *>(event);
                auto widget = qobject_cast<QWidget *>(obj);
                auto pos = widget->mapToParent(pre->localPos().toPoint());
                auto e = new QMouseEvent(QEvent::MouseMove, QPointF(pos.x(), pos.y()), pre->button(), pre->buttons(), pre->modifiers());
                qApp->postEvent(parent(), e);
                return true;
            }
        } else {
            pressFlag = false;
        }
    }
    return QObject::eventFilter(obj, event);
}

DragDropWidget::DragDropWidget(QWidget *parent)
    : QWidget (parent)
{
}

void DragDropWidget::mousePressEvent(QMouseEvent *event)
{
    m_pressFlag = (event->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress);

    QWidget::mousePressEvent(event);
}

void DragDropWidget::mouseMoveEvent(QMouseEvent *event)
{
    do {
        // drag event.
        if (!m_pressFlag) {
            break;
        }
        m_pressFlag = false;
        startDrag(event->pos());
    } while (false);

    QWidget::mouseMoveEvent(event);
}

void DragDropWidget::gobackDrag(const QPixmap &pixmap, const QPoint &pos)
{
    qDebug() << "gobackDrag" << pos << m_startDrag;
    QLabel *widget = new QLabel();
    widget->setAttribute(Qt::WA_TranslucentBackground);
    widget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    widget->setFixedSize(pixmap.size());
    widget->setPixmap(pixmap);
    widget->move(pos);
    widget->show();

    auto currentXAni = new QPropertyAnimation(widget, "pos");
    const int AnimationTime = 250;
    currentXAni->setEasingCurve(QEasingCurve::OutCubic);
    currentXAni->setDuration(AnimationTime);
    currentXAni->setStartValue(pos);
    currentXAni->setEndValue(m_startDrag);
    currentXAni->setDirection(QAbstractAnimation::Forward);
    currentXAni->start();
    connect(currentXAni, &QPropertyAnimation::finished, currentXAni, &QWidget::deleteLater);
    connect(currentXAni, &QPropertyAnimation::finished, widget, &QWidget::deleteLater);
}

AnimationWidgetItem::AnimationWidgetItem(QWidget *widget)
    : QWidgetItemV2(widget)
    , QObject(widget)
    , m_currentXAni (new QPropertyAnimation(this, "geometry"))
{
    const int AnimationTime = 300;
    m_currentXAni->setEasingCurve(QEasingCurve::Linear);
    m_currentXAni->setDirection(QAbstractAnimation::Forward);
    m_currentXAni->setDuration(AnimationTime);
}

void AnimationWidgetItem::setGeometry(const QRect &rect)
{
    m_currentXAni->setStartValue(geometry());
    m_currentXAni->setEndValue(rect);
    m_currentXAni->start();
}

void AnimationWidgetItem::updateGeometry(const QRect &rect)
{
    QWidgetItemV2::setGeometry(rect);
}

bool hasComposite()
{
    return DWindowManagerHelper::instance()->hasComposite();
}
