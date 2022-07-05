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
#include <QPointer>
#include "mainview.h"

WIDGETS_USE_NAMESPACE
namespace dwclock {
class WorldClockWidget : public IWidget {
public:
    virtual QWidget *view() override;
    virtual ~WorldClockWidget() { }

    virtual bool initialize(const QStringList &arguments) override;
    virtual void delayInitialize() override;
    virtual void typeChanged(const IWidget::Type type) override;

    virtual bool enableSettings() override;
    virtual void settings() override;

private:

    QPointer<ViewManager> m_viewManager = nullptr;
};

class WorldClockWidgetPlugin : public IWidgetPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DdeWidgetsPlugin_iid FILE "plugin.json")
    Q_INTERFACES(WIDGETS_NAMESPACE::IWidgetPlugin)

public:
    QString title() const override;
    virtual QString description() const override;
    IWidget *createWidget() override;
    virtual QVector<IWidget::Type> supportTypes() const { return {IWidget::Middle, IWidget::Small};}

    virtual QIcon logo() const override;
    virtual QStringList contributors() const override;
};
}
