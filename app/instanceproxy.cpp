// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "instanceproxy.h"

#include "widgethandler.h"
#include <QBitmap>
#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QWidget>
#include <DClipEffectWidget>

DWIDGET_USE_NAMESPACE
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
        const bool isCustom = WidgetHandlerImpl::get(m_impl->handler())->isCustom();
        m_containerView = new WidgetContainer(m_impl->view(), isCustom);
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

WidgetContainer::WidgetContainer(QWidget *view,  bool isCustom, QWidget *parent)
    : QWidget(parent)
    , m_view(view)
{
    Q_ASSERT(m_view);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(UI::defaultMargins);
    if (!isCustom)
        m_clipView = new DClipEffectWidget(m_view);

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

QPainterPath WidgetContainer::clipPathOfRound(const QRect &rect, const bool isUserAreaInstance)
{
    QPainterPath path;
    const qreal radius = isUserAreaInstance ? UI::RoundedRectRadius : UI::DataStoreRoundedRectRadius;
    path.addRoundedRect(rect, radius, radius);
    return path;
}

void WidgetContainer::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize() == event->size())
        return QWidget::resizeEvent(event);

    if (m_clipView)
        m_clipView->setClipPath(clipPathOfRound(rect(), m_isUserAreaInstance));

    return QWidget::resizeEvent(event);
}

PlaceholderWidget::PlaceholderWidget(QWidget *view, QWidget *parent)
    : QWidget(parent)
    , m_view(view)
{
    Q_ASSERT(m_view);
}

void PlaceholderWidget::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap = m_view->grab();
    pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setClipPath(WidgetContainer::clipPathOfRound(rect(), false));
    painter.drawPixmap(rect(), pixmap);

    return QWidget::paintEvent(event);
}
WIDGETS_FRAME_END_NAMESPACE
