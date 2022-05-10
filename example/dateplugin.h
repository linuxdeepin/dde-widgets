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

WIDGETS_USE_NAMESPACE

class DateWidget : public IWidget {
public:
    virtual QWidget *view() override {
        return m_view;
    }
    virtual ~DateWidget() { }
private:
     QPushButton *m_view = nullptr;

public:
    virtual bool initialize(const QStringList &arguments) override {
         Q_UNUSED(arguments);
         m_view = new QPushButton();
        m_view->setText(QString::number(handler()->value("content").toInt()));
        QObject::connect(m_view, &QPushButton::clicked, m_view, [this]() {
            auto content = handler()->value("content").toInt();
            handler()->setValue("content", ++content);
            m_view->setText(QString::number(handler()->value("content").toInt()));
        });
        return true;
    }

     virtual void typeChanged(const IWidget::Type type) override
     {
         qDebug() << Q_FUNC_INFO << type << handler()->size();
         m_view->setFixedSize(handler()->size());
         if (type == IWidget::Small) {
             m_view->setIcon(QIcon::fromTheme("dde-file-manager"));
         } else if (type == IWidget::Middle) {
             m_view->setIcon(QIcon::fromTheme("dde-printer"));
         } else if (type == IWidget::Large) {
             m_view->setIcon(QIcon::fromTheme("deepin-album"));
         }
     }

     virtual bool enableSettings() override
     {
         qDebug(dwLog()) << Q_FUNC_INFO << handler()->pluginId() << handler()->id();
         return true;
     }

     virtual void settings() override
     {
         qDebug(dwLog()) << Q_FUNC_INFO << handler()->pluginId() << handler()->id();
     }
};

class DateWidgetPlugin : public IWidgetPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DdeWidgetsPlugin_iid FILE "dateplugin.json")
    Q_INTERFACES(WIDGETS_NAMESPACE::IWidgetPlugin)

public:
    QString title() const override;
    virtual QString description() const override;
    IWidget *createWidget() override;
    virtual QVector<IWidget::Type> supportTypes() const { return {IWidget::Small, IWidget::Middle, IWidget::Large};}
};
