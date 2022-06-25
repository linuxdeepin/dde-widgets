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

#include <widgetsinterface.h>
#include "memorywidget.h"

#include <QObject>
#include <QBasicTimer>
#include <QPointer>

WIDGETS_USE_NAMESPACE

class MemoryMonitorWidget : public QObject, public IWidget {
    Q_OBJECT
public:
    virtual QWidget *view() override {
        return m_view.data();
    }
    virtual ~MemoryMonitorWidget() override {
        if (m_timer)
            m_timer->stop();
    }
private:
    bool loadTranslator(const QString &fileNamePrefix);

    void updateMemory();

    QPointer<MemoryWidget> m_view;
    QScopedPointer<QBasicTimer> m_timer;
    bool m_isPressed = false;

public:
    virtual bool initialize(const QStringList &arguments) override;

    virtual void delayInitialize() override;

    virtual void typeChanged(const IWidget::Type type) override;

    virtual void showWidgets() override;
    virtual void hideWidgets() override;

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private Q_SLOTS:
    void showSystemMonitorDetail();

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

class MemoryMonitorWidgetPlugin : public IWidgetPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DdeWidgetsPlugin_iid FILE "plugin.json")
    Q_INTERFACES(WIDGETS_NAMESPACE::IWidgetPlugin)

public:
    QString title() const override;
    virtual QString description() const override;
    virtual IWidget *createWidget() override;
    virtual QVector<IWidget::Type> supportTypes() const { return {IWidget::Small};}
    virtual IWidgetPlugin::Type type() const override { return IWidgetPlugin::Normal; }
};
