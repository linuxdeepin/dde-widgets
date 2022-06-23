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

#include <gtest/gtest.h>

#include "widgetmanager.h"
#include "pluginspec.h"
#include "helper.hpp"

WIDGETS_FRAME_USE_NAMESPACE
static PluginGuard pluginGuard;
class ut_WidgetManager : public ::testing::Test
{
public:
};

TEST_F(ut_WidgetManager, matchVersion)
{
    ASSERT_EQ(WidgetManager::currentVersion(), QString("1.0"));
    ASSERT_TRUE(WidgetManager::matchVersion("1.1"));
    ASSERT_FALSE(WidgetManager::matchVersion("2.1"));
}

TEST_F(ut_WidgetManager, loadPlugins)
{
    WidgetManager manager;
    manager.loadPlugins();
    const auto &plugins = manager.plugins();
    ASSERT_TRUE(plugins.contains(ExamplePluginId));
    ASSERT_GE(plugins.size(), 1);

    ASSERT_TRUE(manager.getPlugin(ExamplePluginId));
    const auto &store = manager.loadWidgetStoreInstances();

    ASSERT_TRUE(store.contains(ExamplePluginId));
    ASSERT_EQ(store.values(ExamplePluginId).size(), 3);
}

TEST_F(ut_WidgetManager, createWidget)
{
    WidgetManager manager;
    manager.loadPlugins();
    auto instance = manager.createWidget(ExamplePluginId, IWidget::Middle);
    ASSERT_TRUE(instance);
    const auto &instanceId = instance->handler()->id();
    ASSERT_FALSE(instanceId.isEmpty());
    auto replacedInstance = manager.createWidget(ExamplePluginId, IWidget::Large, instanceId);
    ASSERT_EQ(instanceId, replacedInstance->handler()->id());
}

TEST_F(ut_WidgetManager, removeWidget)
{
    WidgetManager manager;
    manager.loadPlugins();
    auto instance = manager.createWidget(ExamplePluginId, IWidget::Middle);
    const auto &instanceId = instance->handler()->id();
    manager.removeWidget(instanceId);

    ASSERT_FALSE(manager.getInstance(instanceId));
}

static WidgetManager gManager;
class ut_WidgetPluginSpec : public ::testing::Test
{
public:
    static void SetUpTestCase() {
        gManager.loadPlugins();
    }
    static void TearDownTestCase() {
        gManager.shutdown();
    }

    virtual void SetUp() override
    {
        plugin = gManager.getPlugin(ExamplePluginId);
    }
    virtual void TearDown() override
    {
        plugin = nullptr;
    }
    WidgetPlugin *plugin = nullptr;
};

TEST_F(ut_WidgetPluginSpec, baseInfo)
{
    ASSERT_EQ(plugin->id(), ExamplePluginId);
    ASSERT_EQ(plugin->type(), IWidgetPlugin::Normal);
    ASSERT_EQ(plugin->supportTypes().size(), 3);

    const InstanceId instanceId("example-instance-id");
    auto instance = plugin->createWidget(IWidget::Large, instanceId);
    ASSERT_EQ(instance->handler()->id(), instanceId);
    delete instance;
}
