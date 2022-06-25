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

#include "plugin.h"
#include "accessible/accessible.h"

#include "common/utils.h"
#include "handler/mem.h"

#include <DStandardPaths>
#include <QAccessible>
#include <QDBusInterface>
#include <QTranslator>
#include <QtConcurrent>

QString MemoryMonitorWidgetPlugin::title() const
{
    return tr("MemoryMonitor");
}

QString MemoryMonitorWidgetPlugin::description() const
{
    return tr("Memory Monitor");
}

IWidget *MemoryMonitorWidgetPlugin::createWidget()
{
    return new MemoryMonitorWidget();
}

bool MemoryMonitorWidget::loadTranslator(const QString &fileNamePrefix)
{
    const auto &locale = userInterfaceLanguage();
    if (!locale.isEmpty()) {
        const QString fileName(fileNamePrefix + locale);
        // translations dirs.
        QStringList dirs { "./translations/" };
        DCORE_USE_NAMESPACE;
        const auto &genDatas = DStandardPaths::standardLocations(
                    QStandardPaths::GenericDataLocation);
        for (const auto & path : qAsConst(genDatas)) {
            dirs << path + "/dde-widgets/translations/";
        }
        auto qtl = new QTranslator();
        for (auto dir : dirs) {
            if (qtl->load(fileName, dir)) {
                qApp->installTranslator(qtl);
                qInfo(dwLog()) << QString("load translation [%1] successful.").arg(fileName);
                return true;
            }
        }
        if (qtl->isEmpty()) {
            qWarning(dwLog()) << QString("load translation [%1] error from those dirs.").arg(fileName)
                              << dirs;
            qtl->deleteLater();
        }
    }
    return false;
}

void MemoryMonitorWidget::updateMemory()
{
    core::system::MemInfo info;
    info.readMemInfo();

    using namespace Utils;
    const auto &memPercent = QString::number((info.memTotal() - info.memAvailable()) * 1. / info.memTotal() * 100, 'f', 1);

    const auto &swapUsage = formatUnit((info.swapTotal() - info.swapFree()) << 10, B, 1);
    auto swapPercent = QString::number((info.swapTotal() - info.swapFree()) * 1. / info.swapTotal() * 100, 'f', 1);
    // This process is the same as `deepin-system-monitor-plugin`
    if (swapUsage.split(" ").size() != 2)
        swapPercent = QString();

    if (m_view) {
        m_view->updateMemoryInfo(memPercent, swapPercent);
        m_view->update();
    }
}

bool MemoryMonitorWidget::initialize(const QStringList &arguments) {
    Q_UNUSED(arguments);
    static bool hasLoaded = false;
    if (!hasLoaded)
        hasLoaded = loadTranslator("dde-widgets-memorymonitor_");

    m_view = new MemoryWidget();
    m_timer.reset(new QBasicTimer());
    m_view->installEventFilter(this);

    return true;
}

void MemoryMonitorWidget::delayInitialize()
{
    // enable accessible
    QAccessible::installFactory(memoryMonitorAccessibleFactory);
}

void MemoryMonitorWidget::typeChanged(const IWidget::Type type)
{
    Q_UNUSED(type)
    m_view->setFixedSize(handler()->size());
}

void MemoryMonitorWidget::timerEvent(QTimerEvent *event)
{
//    if (event->timerId() == m_timer->timerId())
//        QtConcurrent::run(QThreadPool::globalInstance(), this, &MemoryMonitorWidget::updateMemory);
    if (event->timerId() == m_timer->timerId())
        updateMemory();

    return QObject::timerEvent(event);
}

void MemoryMonitorWidget::showWidgets()
{
    updateMemory();

    const int Interval = 1000;
    m_timer->start(Interval, this);
}

void MemoryMonitorWidget::hideWidgets()
{
    m_timer->stop();
}

bool MemoryMonitorWidget::eventFilter(QObject *watched, QEvent *event)
{
    do {
        if (watched != m_view)
            break;
        if (event->type() != QEvent::MouseButtonPress &&
                event->type() != QEvent::MouseButtonRelease)
            break;
        auto me = dynamic_cast<QMouseEvent *>(event);
        if (me->button() != Qt::LeftButton)
            break;

        if (event->type() == QEvent::MouseButtonPress) {
            m_isPressed = true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            if (m_isPressed) {
                showSystemMonitorDetail();
                m_isPressed = false;
            }
        }
    } while (false);

    return QObject::eventFilter(watched, event);
}

void MemoryMonitorWidget::showSystemMonitorDetail()
{
    QDBusInterface interface("com.deepin.SystemMonitorMain", "/com/deepin/SystemMonitorMain",
                              "com.deepin.SystemMonitorMain");
     if (!interface.isValid()) {
         qWarning() << "Get com.deepin.SystemMonitorMain interface error." << interface.lastError().message();
         return;
     }
     qDebug() << "showSystemMonitorDetail()";
     const QString MsgCodeName("MSG_PROCESS");
     interface.call("slotJumpProcessWidget", MsgCodeName);
}
