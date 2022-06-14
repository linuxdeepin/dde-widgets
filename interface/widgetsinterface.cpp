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

#include "widgetsinterface.h"
#include "widgetsinterface_p.h"

#include <QCoreApplication>

WIDGETS_BEGIN_NAMESPACE

#ifndef QT_DEBUG
Q_LOGGING_CATEGORY(dwLog, "dde.widgets", QtInfoMsg)
#else
Q_LOGGING_CATEGORY(dwLog, "dde.widgets")
#endif

IWidget::IWidget()
    : d(new IWidget::Private())
{
}

IWidget::~IWidget()
{
    Q_ASSERT(d);
    delete d->handler;
    d->handler = nullptr;

    delete d;
    d = nullptr;
}

WidgetHandler *IWidget::handler() const
{
    Q_ASSERT(d->handler);
    return d->handler;
}

QString IWidget::userInterfaceLanguage()
{
    return qApp->property("dapp_locale").toString();
}

WIDGETS_END_NAMESPACE
