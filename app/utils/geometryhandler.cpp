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

#include "geometryhandler.h"

#include <DDBusInterface>

#include <QDebug>
#include <QApplication>
#include <QScrollArea>
#include <QScreen>

WIDGETS_FRAME_BEGIN_NAMESPACE
using MonitorInter = org::deepin::dde::display1::Monitor;
static const QString DisplayDaemonDBusServie = "org.deepin.dde.Display1";
static const QString DisplayDaemonDBusPath = "/org/deepin/dde/Display1";
static const QString DockDaemonDBusServie = "org.deepin.dde.daemon.Dock1";
static const QString DockDaemonDBusPath = "/org/deepin/dde/daemon/Dock1";
namespace Geo
{
    static const int DockMargin = 5;

    enum DockPosition {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3
    };

    enum DockModel {
        Fashion = 0,
        Efficient = 1
    };
    static const int CenterMargin = 0;
}

GeometryHandler::GeometryHandler(QObject *parent)
    : QObject (parent)
{
    m_displayInter = new DisplayInter(DisplayDaemonDBusServie, DisplayDaemonDBusPath,
                                          QDBusConnection::sessionBus(), this);
    m_dockDeamonInter = new DockInter(DockDaemonDBusServie, DockDaemonDBusPath,
                                      QDBusConnection::sessionBus(), this);
}

GeometryHandler::~GeometryHandler()
{
    if (m_displayInter) {
        m_displayInter->deleteLater();
        m_displayInter = nullptr;
    }
    if (m_dockDeamonInter) {
        m_dockDeamonInter->deleteLater();
        m_dockDeamonInter = nullptr;
    }
}

QRect GeometryHandler::getGeometry(const int expectedWidth, const bool reduceDockHeight)
{
    // TODO value of returned `frontendWindowRect()` is invalid, it's maybe a bug in dtkcore.
//    QRect dockRect = m_dockDeamonInter->frontendWindowRect();
    DTK_CORE_NAMESPACE::DDBusInterface dockInterface(m_dockDeamonInter->service(), m_dockDeamonInter->path(),
                                 m_dockDeamonInter->interface());
    QRect dockRect = qdbus_cast<DockRect>(dockInterface.property("FrontendWindowRect"));

    auto displayRect = calcDisplayRect(dockRect);

    auto dockPos = static_cast<Geo::DockPosition>(m_dockDeamonInter->position());
    auto dockMode = m_dockDeamonInter->displayMode();

    int height = displayRect.height() - Geo::CenterMargin * 2;
    if (dockPos == Geo::DockPosition::Top || dockPos == Geo::DockPosition::Bottom) {
        if (reduceDockHeight)
            height = displayRect.height() - Geo::CenterMargin * 2 - dockRect.height();

        if(dockMode == Geo::DockModel::Fashion) {
            if (dockRect.height() != 0) {
                height -= Geo::DockMargin * 2;
            }
        }
    }

    int x = displayRect.x() + displayRect.width() - expectedWidth - Geo::CenterMargin;
    if (dockPos == Geo::DockPosition::Right) {
        if(dockMode == Geo::DockModel::Fashion) {
            x =  displayRect.x() + displayRect.width() - (expectedWidth + dockRect.width() + Geo::DockMargin * 2 + Geo::CenterMargin);
            if (dockRect.width() == 0) {
                x += Geo::DockMargin * 2;
            }
        } else {
            x =  displayRect.x() + displayRect.width() - (expectedWidth + dockRect.width() + Geo::CenterMargin);
        }
    }

    int y = displayRect.y() + Geo::CenterMargin;
    if (dockPos == Geo::DockPosition::Top) {
        if(dockMode == Geo::DockModel::Fashion) {
            y = displayRect.y() + Geo::CenterMargin + dockRect.height() + Geo::DockMargin * 2;
        } else {
            y = displayRect.y() + Geo::CenterMargin + dockRect.height();
        }
    }

    return QRect(x, y, expectedWidth, height);
}

QRect GeometryHandler::calcDisplayRect(const QRect &dockRect)
{
    qreal ratio = qApp->primaryScreen()->devicePixelRatio();
    QRect displayRect = m_displayInter->primaryRect();
    QList<QDBusObjectPath> screenList = m_displayInter->monitors();

    for (const auto &screen : screenList) {
        MonitorInter monitor("org.deepin.dde.Display1", screen.path(), QDBusConnection::sessionBus());
        QRect monitorRect(monitor.x(), monitor.y(), monitor.width(), monitor.height());
        if (monitor.enabled() && monitorRect.contains(dockRect.center())) {
            displayRect = QRect(monitorRect.x(), monitorRect.y(),
                                monitorRect.width() / ratio, monitorRect.height() / ratio);
            break;
        }
    }
    return displayRect;
}
WIDGETS_FRAME_END_NAMESPACE
