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

#include "pluginspec.h"
#include "widgethandler.h"
#include "widgetsinterface_p.h"
#include "utils.h"

#include <QUuid>
#include <QDebug>
#include <QSettings>

WIDGETS_BEGIN_NAMESPACE

WidgetPluginSpec::WidgetPluginSpec(const PluginInfo &info)
    : m_plugin (info.plugin)
    , m_pluginId(info.id)
    , m_fileName(info.fileName)
{

}

WidgetPluginSpec::~WidgetPluginSpec()
{
    if (m_plugin) {
        m_plugin->deleteLater();
        m_plugin = nullptr;
    }
    if (m_dataStore) {
        m_dataStore->deleteLater();
        m_dataStore = nullptr;
    }
}

IWidget *WidgetPluginSpec::createWidget(const IWidget::Type &type)
{
    return createWidgetImpl(type, QUuid::createUuid().toString());
}

IWidget *WidgetPluginSpec::createWidget(const IWidget::Type &type, const InstanceId &key)
{
    return createWidgetImpl(type, key);
}

IWidget *WidgetPluginSpec::createWidgetForWidgetStore(const IWidget::Type &type)
{
    auto instance = createWidget(type);
    if (instance) {
        WidgetHandlerImpl::get(instance->handler())->m_isInstance = false;
    }
    return instance;
}

IWidget *WidgetPluginSpec::createWidgetImpl(const IWidget::Type &type, const InstanceId &key)
{
    if (!m_plugin->supportTypes().contains(type))
        return nullptr;

    auto instance = m_plugin->createWidget();
    if (!instance)
        return nullptr;

    auto handler = new WidgetHandlerImpl();
    instance->d->handler = handler;
    handler->m_type = type;
    handler->m_id = key;
    handler->m_pluginId = m_pluginId;
    handler->m_pluginType = m_plugin->type();
    handler->setDataStore(m_dataStore);
    qDebug(dwLog()) << "created widget." << m_pluginId << type << key;
    return instance;
}

QString WidgetPluginSpec::title() const
{
    return m_plugin->title();
}

QString WidgetPluginSpec::description() const
{
    return m_plugin->description();
}

IWidgetPlugin::Type WidgetPluginSpec::type() const
{
    return m_plugin->type();
}

QVector<IWidget::Type> WidgetPluginSpec::supportTypes() const
{
    return m_plugin->supportTypes();
}

void WidgetPluginSpec::setDataStore(DataStore *store)
{
    m_dataStore = store;
}

PluginId WidgetPluginSpec::id() const
{
    return m_pluginId;
}

WIDGETS_END_NAMESPACE
