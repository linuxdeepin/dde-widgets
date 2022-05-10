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
#include <widgetsinterface.h>

class WidgetManager;

WIDGETS_BEGIN_NAMESPACE

class WidgetPluginSpec {
public:
    explicit WidgetPluginSpec(const PluginId &id, IWidgetPlugin *plugin);
    ~WidgetPluginSpec();

    PluginId id() const;
    QString title() const;
    QString description() const;
    IWidgetPlugin::Type type() const;

    QVector<IWidget::Type> supportTypes() const;

    IWidget *createWidget(const IWidget::Type &type);
    IWidget *createWidget(const IWidget::Type &type, const InstanceId &key);
private:

    IWidget *createWidgetForWidgetStore(const IWidget::Type &type);

    IWidget *createWidgetImpl(const IWidget::Type &type, const InstanceId &key);
    void setDataStore(DataStore *store);

    IWidgetPlugin *m_plugin = nullptr;
    PluginId m_pluginId;
    DataStore *m_dataStore = nullptr;

    friend class ::WidgetManager;
};

WIDGETS_END_NAMESPACE
