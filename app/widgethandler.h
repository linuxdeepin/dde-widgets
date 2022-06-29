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
class WidgetHandlerImpl : public WidgetHandler {
public:
    explicit WidgetHandlerImpl();
    virtual ~WidgetHandlerImpl() override;

    virtual void setValue(const QString &key, const QVariant &value) override;
    virtual QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const override;
    virtual void resetValue(const QString &key);
    virtual void removeValue(const QString &key) override;
    virtual bool containsValue(const QString &key) override;
    virtual QString id() const override;
    virtual QString pluginId() const override { return m_pluginId;}
    virtual IWidget::Type type() const override { return m_type;}
    virtual QSize size() const override;
    static QSize size(const IWidget::Type type, const bool instance = true);
    QString typeString() const;
    static QString typeString(const Widgets::IWidget::Type type);
    void clear();
    bool unavailableDS() const;

    static WidgetHandlerImpl *get(WidgetHandler *handler);

    IWidgetPlugin::Type pluginType() const;
    bool isFixted() const;
    bool isCustom() const;
    void setDataStore(DataStore *store);

    bool m_isUserAreaInstance = true;
    InstanceId m_id;
    PluginId m_pluginId;
    IWidget::Type m_type;
    IWidgetPlugin::Type m_pluginType;
    DataStore *m_dataStore = nullptr;
};
WIDGETS_FRAME_END_NAMESPACE
