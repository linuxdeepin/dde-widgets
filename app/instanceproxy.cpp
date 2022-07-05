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

#include "instanceproxy.h"

#include "widgethandler.h"
#include <QBitmap>
#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QWidget>

WIDGETS_FRAME_BEGIN_NAMESPACE

InstanceProxy::InstanceProxy(IWidget *impl)
    : QObject()
    , m_impl(impl)
{
}

InstanceProxy::~InstanceProxy()
{
    if (m_containerView)
        m_containerView->deleteLater();
}

QWidget *InstanceProxy::view() const
{
    if (!m_containerView) {
        m_containerView = new WidgetContainer(m_impl->view());
        m_containerView->setIsUserAreaInstance(isUserAreaInstance());
    }

    return m_containerView;
}

WidgetHandler *InstanceProxy::handler() const
{
    return m_impl->handler();
}

void InstanceProxy::typeChanged(const IWidget::Type &type)
{
    return m_impl->typeChanged(type);
}

bool InstanceProxy::initialize(const QStringList &arguments)
{
    return m_impl->initialize(arguments);
}

void InstanceProxy::delayInitialize()
{
    return m_impl->delayInitialize();
}

void InstanceProxy::showWidgets()
{
    return m_impl->showWidgets();
}

void InstanceProxy::hideWidgets()
{
    return m_impl->hideWidgets();
}

void InstanceProxy::aboutToShutdown()
{
    return m_impl->aboutToShutdown();
}

void InstanceProxy::settings()
{
    return m_impl->settings();
}

bool InstanceProxy::enableSettings()
{
    return m_impl->enableSettings();
}

bool InstanceProxy::isUserAreaInstance() const
{
    return WidgetHandlerImpl::get(m_impl->handler())->m_isUserAreaInstance;
}

WidgetContainer::WidgetContainer(QWidget *view, QWidget *parent)
    : QWidget(parent)
    , m_view(view)
{
    Q_ASSERT(m_view);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(UI::defaultMargins);
    layout->addWidget(m_view);
}

WidgetContainer::~WidgetContainer()
{
    if (m_view) {
        // avoid to being deleted by QWidget, which uses `delete` to release it's child,
        // it would be double free when `m_view` is InstanceProxy's `m_impl`.
        m_view->setParent(nullptr);
        m_view->deleteLater();
    }
}

void WidgetContainer::setIsUserAreaInstance(const bool isUserAreaInstance)
{
    m_isUserAreaInstance = isUserAreaInstance;
}

QBitmap WidgetContainer::bitmapOfMask(const QSize &size, const bool isUserAreaInstance)
{
    const qreal radius = isUserAreaInstance ? UI::RoundedRectRadius : UI::DataStoreRoundedRectRadius;
    return bitmapOfMask(size, radius);
}

QBitmap WidgetContainer::bitmapOfMask(const QSize &size, const qreal radius)
{
    QBitmap bitMap(size);
    bitMap.fill(Qt::color0);

    QPainter painter(&bitMap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(bitMap.rect(), radius, radius);

    return bitMap;
}

void WidgetContainer::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize() == event->size())
        return QWidget::resizeEvent(event);

    setMask(bitmapOfMask(event->size(), m_isUserAreaInstance));

    return QWidget::resizeEvent(event);
}
WIDGETS_FRAME_END_NAMESPACE
