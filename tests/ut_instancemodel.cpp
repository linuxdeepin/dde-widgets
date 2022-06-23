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
#include "instancemodel.h"
#include "helper.hpp"
#include <QSignalSpy>
WIDGETS_FRAME_USE_NAMESPACE

static PluginGuard pluginGuard;
static WidgetManager manager;
class ut_InstanceModel : public ::testing::Test
{
public:
    static void SetUpTestCase() {
        manager.loadPlugins();
    }
    static void TearDownTestCase() {
        manager.shutdown();
    }

    virtual void SetUp() override
    {
        model = new InstanceModel(&manager);
    }
    virtual void TearDown() override
    {
        model->deleteLater();
        model = nullptr;
    }
    InstanceModel *model = nullptr;
};

TEST_F(ut_InstanceModel, instanceCRUD)
{
    auto instance = model->addInstance(ExamplePluginId, IWidget::Large);
    const auto &instanceId = instance->handler()->id();

    ASSERT_TRUE(instance);
    ASSERT_EQ(model->count(), 1);
    ASSERT_TRUE(model->getInstance(instance->handler()->id()));
    ASSERT_TRUE(model->existInstance(ExamplePluginId));

    ASSERT_TRUE(model->getInstance(0));

    model->replaceInstance(instanceId, IWidget::Middle);

    auto instance2 = model->addInstance(ExamplePluginId, IWidget::Middle);
    ASSERT_EQ(model->count(), 2);
    ASSERT_TRUE(model->instancePosition(instanceId) == 0);
    ASSERT_TRUE(model->instancePosition(instance2->handler()->id()) == 1);

    model->moveInstance(instance2->handler()->id(), 0);
    ASSERT_TRUE(model->instancePosition(instanceId) == 1);
    ASSERT_TRUE(model->instancePosition(instance2->handler()->id()) == 0);

    model->removeInstance(instanceId);
    ASSERT_EQ(model->count(), 1);

    model->removeInstance(instance2->handler()->id());
}

TEST_F(ut_InstanceModel, instanceSignals)
{
    InstanceId instanceId;
    qRegisterMetaType<InstancePos>("InstancePos");
    qRegisterMetaType<InstanceId>("InstanceId");
    ASSERT_EQ(model->count(), 0);
    {
        QSignalSpy spy(model, &InstanceModel::added);
        auto instance = model->addInstance(ExamplePluginId, IWidget::Large);
        instanceId = instance->handler()->id();
        ASSERT_EQ(spy.count(), 1);
    }
    {
        QSignalSpy spy(model, &InstanceModel::replaced);
        auto instance = model->replaceInstance(instanceId, IWidget::Middle);
        ASSERT_EQ(spy.count(), 1);
        ASSERT_EQ(instance->handler()->type(), IWidget::Middle);
    }
    {
        auto instance = model->addInstance(ExamplePluginId, IWidget::Middle);
        QSignalSpy spy(model, &InstanceModel::moved);
        model->moveInstance(instanceId, 1);
        ASSERT_EQ(spy.count(), 1);
        model->removeInstance(instance->handler()->id());
    }
    {
        QSignalSpy spy(model, &InstanceModel::removed);
        model->removeInstance(instanceId);
        ASSERT_EQ(spy.count(), 1);
    }
}
