// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
