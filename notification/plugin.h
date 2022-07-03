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

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include <QPushButton>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>

#include "notification/persistence.h"
#include "notifycenterwidget.h"

WIDGETS_USE_NAMESPACE

class NotificationWidget : public IWidget {
public:
    virtual QWidget *view() override {
        return m_view.get();
    }
    virtual ~NotificationWidget() override {
        if (m_persistence) {
            m_persistence->deleteLater();
            m_persistence = nullptr;
        }
    }
private:

    QScopedPointer<NotifyCenterWidget> m_view;
    AbstractPersistence* m_persistence = nullptr;

public:
    virtual bool initialize(const QStringList &arguments) override;

    virtual void delayInitialize() override;
};

class NotificationWidgetPlugin : public IWidgetPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DdeWidgetsPlugin_iid FILE "plugin.json")
    Q_INTERFACES(WIDGETS_NAMESPACE::IWidgetPlugin)

public:
    QString title() const override;
    virtual QString description() const override;
    virtual IWidget *createWidget() override;
    virtual QVector<IWidget::Type> supportTypes() const { return {IWidget::Custom};}
    virtual IWidgetPlugin::Type type() const override { return IWidgetPlugin::Alone; }
};
