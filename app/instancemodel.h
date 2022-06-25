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

WIDGETS_USE_NAMESPACE

WIDGETS_FRAME_BEGIN_NAMESPACE
class WidgetManager;
class InstanceModel : public QObject{
    Q_OBJECT
public:
    explicit InstanceModel(WidgetManager *manager, QObject *parent = nullptr);
    virtual ~InstanceModel() override;

    void loadPrePanelInstances();
    QVector<Instance *> instances() const;
    InstancePos instancePosition(const InstanceId &key);

    Instance *addInstance(const PluginId &pluginId, const IWidget::Type &type, InstancePos index = -1);
    void moveInstance(const InstanceId &source, InstancePos index = -1);
    Instance *replaceInstance(const InstanceId &key, const IWidget::Type &type);
    Instance *getInstance(const InstancePos pos) const;
    Instance *getInstance(const InstanceId &key) const;
    bool existInstanceByInstanceId(const InstanceId &key) const;
    bool existInstance(const PluginId &pluginId);

    QVector<IWidget::Type> pluginTypes(const PluginId &pluginId) const;
    void removePlugin(const PluginId &pluginId);

    int count() const;

public Q_SLOTS:

    void removeInstance(const InstanceId &key);

Q_SIGNALS:
    void added(const InstanceId &key, InstancePos pos);
    void removed(const InstanceId &key, InstancePos pos);
    void moved(const InstancePos &source, InstancePos target);
    void replaced(const InstanceId &key, InstancePos target);

private:
    void removeMapItem(const QString &dataStoreKey, const InstanceId &key);
    void addMapItem(const QString &dataStoreKey, const InstanceId &key, const QVariant &value);
    void updateContentItem(const InstanceId &key, const QString &contentKey, const QVariant &value);
    void addInstance(Instance *instance, InstancePos index = -1);

    void updatePositions();

    void loadOrCreateResidentInstance();
    void loadOrCreateAloneInstance();
    bool existInstanceInDataStore(const PluginId &pluginId);

private:
    // current instances in Panel.
    QVector<Instance *> m_instances;
    WidgetManager *m_manager;
    DataStore *m_dataStore;
};
WIDGETS_FRAME_END_NAMESPACE
