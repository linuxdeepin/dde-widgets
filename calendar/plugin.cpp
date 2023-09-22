// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "plugin.h"
#include "common/helper.hpp"

#include <QMouseEvent>
#include <QVBoxLayout>

#include <DClipEffectWidget>
#include <DDBusSender>
#include <DFrame>

DWIDGET_USE_NAMESPACE

namespace dwclock {
QString CalendarWidgetPlugin::title() const
{
    return tr("Calendar");
}

QString CalendarWidgetPlugin::description() const
{
    return tr("Display calendar");
}

QString CalendarWidgetPlugin::aboutDescription() const
{
    return tr("Viewing calendar");
}

IWidget *CalendarWidgetPlugin::createWidget()
{
    return new CalendarWidget();
}

QWidget *CalendarWidget::view()
{
    DFrame *frame = new DFrame();
    frame->setFixedSize(handler()->size());
    frame->setFrameRounded(true);
    auto layout = new QVBoxLayout(frame);
    layout->setMargin(0);
    DCalendarWidget *calendar = new DCalendarWidget();
    layout->addWidget(calendar, 0, Qt::AlignCenter);
    frame->installEventFilter(this);
    m_calendar = calendar;
    return frame;
}

bool CalendarWidget::initialize(const QStringList &arguments) {
    Q_UNUSED(arguments);
    static bool hasLoaded = false;
    if (!hasLoaded)
        hasLoaded = BuildinWidgetsHelper::instance()->loadTranslator("dde-widgets-calendar_");

    return true;
}

void CalendarWidget::showWidgets()
{
    if (m_calendar) {
        m_calendar->setSelectedDate(QDate::currentDate());
    }
}

bool CalendarWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (qobject_cast<DFrame *>(watched) &&
        event->type() == QEvent::MouseButtonDblClick)
    {
        showDDECalendar();
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void CalendarWidget::showDDECalendar()
{
    qDebug(dwLog()) << "showDDECalendar()";
    DDBusSender().service("com.deepin.Calendar")
        .interface("com.deepin.Calendar")
        .path("/com/deepin/Calendar")
        .method("RaiseWindow")
        .call();
}

QIcon CalendarWidgetPlugin::logo() const
{
    return QIcon::fromTheme("dde-calendar");
}

QStringList CalendarWidgetPlugin::contributors() const
{
    return {BuildinWidgetsHelper::instance()->contributor()};
}
}
