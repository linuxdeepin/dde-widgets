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

#include "widgetsserver.h"
#include "widgetmanager.h"
#include "dbusserver_adaptor.h"
#include <QDebug>

#define DDE_WIDGETS_SERVICE "org.deepin.dde.Widgets"

WidgetsServer::WidgetsServer(QObject *parent)
    : QObject (parent)
    , m_manager (new WidgetManager())
{

}

WidgetsServer::~WidgetsServer()
{
    // WidgetManager need be destroyed before `View`, because IWidget may be released by QObject.
    delete m_manager;
    m_manager = nullptr;
    if (m_mainView) {
        m_mainView->deleteLater();
    }
}

bool WidgetsServer::registerService()
{
    (void) new DBusServer(this);
    auto bus = QDBusConnection::sessionBus();

    if (!bus.registerService(DDE_WIDGETS_SERVICE)) {
        qWarning() << QString("Can't register the %1 service, error:%2.").arg(DDE_WIDGETS_SERVICE).arg(bus.lastError().message());
        return false;
    }
    if (!bus.registerObject("/", this)) {
        qWarning() << QString("Can't register to the D-Bus object.");
        return false;
    }
    return true;
}

void WidgetsServer::start()
{
    m_manager->loadPlugins();
    Show();
}

void WidgetsServer::Toggle()
{
    if (m_mainView && m_mainView->isVisible()) {
        Hide();
    } else {
        Show();
    }
}

void WidgetsServer::Show()
{
    qDebug(dwLog()) << "Show";
    if (!m_mainView) {
        m_mainView = new MainView(m_manager);

        m_mainView->init();
    }

//    m_mainView->switchToDisplayMode();
    m_mainView->switchToEditMode();
    m_mainView->showView();
}

void WidgetsServer::Hide()
{
    qDebug(dwLog()) << "Hide";
    if (!m_mainView) {
        return;
    }

    m_mainView->hideView();
}

void WidgetsServer::SyncWidgets()
{
    qDebug(dwLog()) << "SyncWidgets";
    const auto removedPluginIds = m_manager->removingPlugins();
    for (auto pluginId : removedPluginIds) {
        m_mainView->removePlugin(pluginId);
    }

    const auto addedPluginIds = m_manager->addingPlugins();
    for (auto pluginId : addedPluginIds) {
        m_mainView->addPlugin(pluginId);
    }
    qDebug(dwLog()) << " removedPlugins:" << removedPluginIds
                    << "addedPlugins:" << addedPluginIds;
}
