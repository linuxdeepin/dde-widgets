// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "global.h"
#include <QBitmap>
#include <QPointer>
#include <QWidget>
#include <widgetsinterface.h>
#include <DWidget>

DWIDGET_BEGIN_NAMESPACE
class DClipEffectWidget;
DWIDGET_END_NAMESPACE

WIDGETS_FRAME_BEGIN_NAMESPACE
WIDGETS_USE_NAMESPACE
class WidgetContainer : public QWidget {
    Q_OBJECT
public:
    explicit WidgetContainer(QWidget *view, bool isCustom, QWidget *parent = nullptr);
    virtual ~WidgetContainer() override;
    void setIsUserAreaInstance(const bool isUserAreaInstance);

    static QPainterPath clipPathOfRound(const QRect &rect, const bool isUserAreaInstance);
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    bool m_isUserAreaInstance = false;
    QPointer<QWidget> m_view = nullptr;
    DTK_WIDGET_NAMESPACE::DClipEffectWidget *m_clipView = nullptr;
};

class PlaceholderWidget : public QWidget {
    Q_OBJECT
public:
    PlaceholderWidget(QWidget *view, QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    QWidget *m_view = nullptr;
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
