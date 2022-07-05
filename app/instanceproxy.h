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

#include "global.h"
#include <QBitmap>
#include <QPointer>
#include <QWidget>
#include <widgetsinterface.h>

WIDGETS_FRAME_BEGIN_NAMESPACE
WIDGETS_USE_NAMESPACE
class WidgetContainer : public QWidget {
    Q_OBJECT
public:
    explicit WidgetContainer(QWidget *view, QWidget *parent = nullptr);
    virtual ~WidgetContainer() override;
    void setIsUserAreaInstance(const bool isUserAreaInstance);

    static QBitmap bitmapOfMask(const QSize &size, const bool isUserAreaInstance);
    static QBitmap bitmapOfMask(const QSize &size, const qreal radius);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    bool m_isUserAreaInstance = false;
    QPointer<QWidget> m_view = nullptr;
};

class InstanceProxy : public QObject {
public:
    explicit InstanceProxy(IWidget *impl);
    ~InstanceProxy();

    QWidget *view() const;
    WidgetHandler *handler() const;

    bool initialize(const QStringList &arguments);
    void delayInitialize();
    void typeChanged(const IWidget::Type &type);
    void showWidgets();
    void hideWidgets();
    void aboutToShutdown();
    void settings();
    bool enableSettings();

    bool isUserAreaInstance() const;

private:
    QScopedPointer<IWidget> m_impl;
    mutable QPointer<WidgetContainer> m_containerView;
};
WIDGETS_FRAME_END_NAMESPACE
