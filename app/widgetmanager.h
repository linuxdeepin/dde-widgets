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
#include "pluginspec.h"
#include <widgetsinterface.h>
#include <QSettings>

WIDGETS_USE_NAMESPACE

WIDGETS_FRAME_BEGIN_NAMESPACE
class WidgetManager {
public:
    explicit WidgetManager();
    ~WidgetManager();

    void shutdown();
    DataStore *dataStore();
    static QString currentVersion();
    static bool matchVersion(const QString &version);

    QStringList pluginPaths() const;
    void loadPlugins();
    QHash<PluginId, WidgetPlugin *> plugins() const;
    QList<WidgetPlugin *> plugins(const IWidgetPlugin::Type type) const;
    WidgetPlugin *getPlugin(const PluginId &key) const;

    QMultiMap<PluginId, Instance *> loadWidgetStoreInstances();
    IWidget *createWidget(const PluginId &pluginId, const IWidget::Type &type);
    IWidget *createWidget(const PluginId &pluginId, const IWidget::Type &type, const InstanceId &id);
    void removeWidget(const InstanceId &instanceId);
    void typeChanged(const InstanceId &instanceId, const IWidget::Type &type);
    Instance *getInstance(const InstanceId &key);
    QList<Instance *> instances() const;
    void showAllWidgets();
    void hideAllWidgets();
    void showWidgets(const QVector<Instance *> &instances);
    void hideWidgets(const QVector<Instance *> &instances);
    QVector<Instance *> initialize(const QVector<Instance *> &instances);
    bool initialize(Instance *instance);
    void aboutToShutdown(const QVector<Instance *> &instances);
    void aboutToShutdown(Instance *instance);

    void clearDataStore(const PluginId &id);
    void clearDataStore(const PluginId &id, const InstanceId &instanceId);
    QString dataStorePath(const PluginId &pluginId) const;

    WidgetPluginSpec *loadPlugin(const PluginPath &pluginPath);
    PluginInfo parsePluginInfo(const QString &fileName) const;
    WidgetPluginSpec *loadPlugin(const PluginInfo &info);
    bool isPlugin(const QString &fileName) const;
    QList<PluginId> removingPlugins() const;
    QList<PluginPath> addingPlugins();
    void removePlugin(const PluginId &key);
    QList<Instance *> getInstances(const PluginId &key) const;
    QList<Instance *> createWidgetStoreInstances(const PluginId &key);

private:
    DataStore m_dataStore;
    // all plugin.
    QHash<PluginId, WidgetPlugin *> m_plugins;
    // all Instance of created.
    QHash<InstanceId, Instance *> m_widgets;
    QStringList m_arguments;
};
WIDGETS_FRAME_END_NAMESPACE
