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

#include "widgetmanager.h"
#include "pluginspec.h"
#include "widgethandler.h"

#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QWidget>

static QPair<quint16, quint16> parseVersion(const QString &version)
{
    QPair<quint16, quint16> res{0, 0};
    const QStringList &items = version.split(".");
    if (items.size() != 2)
        return res;

    bool ok = false;
    const quint16 major = items.first().toUShort(&ok);
    if (!ok)
        return res;
    const quint16 minor = items.last().toUShort(&ok);
    if (!ok)
        return res;
    if (major <= 0 && minor <=0 )
        return res;

    res.first = major;
    res.second = minor;
    return res;
}

WidgetManager::WidgetManager()
{
}

WidgetManager::~WidgetManager()
{
    shutdown();
}

void WidgetManager::shutdown()
{
    aboutToShutdown(m_widgets.values().toVector());
    // it maybe exist dangling pointer if IWidget is released by QObject.
    qDeleteAll(m_widgets);
    m_widgets.clear();
    qDeleteAll(m_plugins);
    m_plugins.clear();
}

DataStore *WidgetManager::dataStore()
{
    return &m_dataStore;
}

QString WidgetManager::currentVersion()
{
    return QString("1.0");
}

bool WidgetManager::matchVersion(const QString &version)
{
    static const auto supportVersion = parseVersion(currentVersion());
    const auto curr = parseVersion(version);
    return ((curr.first > 0 || curr.second > 0) && (curr.first <= supportVersion.first));
}

void WidgetManager::loadPlugins()
{
    qDeleteAll(m_plugins);
    m_plugins.clear();

    // The same pluginid will be overwritten by later, `DDE_WIDGETS_PLUGIN_DIRS` > `./plugins` > `/usr`
    QStringList dirs;
    dirs << "/usr/lib/dde-widgets/plugins";
    dirs << QCoreApplication::applicationDirPath() + "/plugins";
    const QString &envPaths = qEnvironmentVariable("DDE_WIDGETS_PLUGIN_DIRS");
    if (!envPaths.isEmpty()) {
        QStringList list = envPaths.split(':');
        std::reverse(list.begin(), list.end());
        dirs << list;
    }

    qDebug(dwLog()) << "load plugins from those dir:" << dirs;
    QStringList pluginPaths;
    for (auto dir : qAsConst(dirs)) {
        auto pluginsDir = QDir(dir);
        if (!pluginsDir.exists())
            continue;

        const auto entryList = pluginsDir.entryList(QDir::Files);
        for (const QString &fileName : qAsConst(entryList)) {
            pluginPaths << pluginsDir.absoluteFilePath(fileName);
        }
    }

    for (QString fileName : qAsConst(pluginPaths)) {
        if (auto spec = loadPlugin(fileName)) {
            qDebug(dwLog()) << "load the plugin [" << spec->id() << "] successful." << fileName;

            auto store = new DataStore(dataStorePath(spec->id()), QSettings::NativeFormat);
            spec->setDataStore(store);
            m_plugins.insert(spec->id(), spec);
        }
    }
}

QHash<PluginId, WidgetPlugin *> WidgetManager::plugins() const
{
    return m_plugins;
}

QList<WidgetPlugin *> WidgetManager::plugins(const IWidgetPlugin::Type type) const
{
    QList<WidgetPlugin *> result;
    for (auto plugin : qAsConst(m_plugins)) {
        if (plugin->type() != type)
            continue;
        result.append(plugin);
    }
    return result;
}

QMultiMap<PluginId, Instance *> WidgetManager::loadWidgetStoreInstances()
{
    QMultiMap<PluginId, Instance *> instances;
    for (auto plugin : m_plugins) {
        if (plugin->type() != IWidgetPlugin::Normal) {
            continue;
        }

        static const QVector<IWidget::Type> Types{IWidget::Large, IWidget::Middle, IWidget::Small};
        for (auto type : Types) {
            auto instance = plugin->createWidgetForWidgetStore(type);
            if (!instance)
                continue;

            instances.insert(plugin->id(), instance);
        }
    }
    initialize(instances.values().toVector());
    return instances;
}

WidgetPluginSpec *WidgetManager::loadPlugin(const QString &fileName)
{
    QPluginLoader loader(fileName);
    const auto &meta = loader.metaData();

    QString id;
    IWidgetPlugin *plugin = nullptr;
    do {
        const auto iid = meta["IID"].toString();
        if (iid.isEmpty())
            break;

        if (iid != QString(qobject_interface_iid<IWidgetPlugin *>()))
            break;

        id = meta["MetaData"]["pluginId"].toString();
        if (id.isEmpty()) {
            qWarning(dwLog()) << "pluginId not existed in MetaData for the plugin." << fileName;
            break;
        }
        const auto version = meta["MetaData"]["version"].toString();
        if (!matchVersion(version)) {
            qWarning(dwLog()) << QString("plugin version [%1] is not matched by [%2].").arg(version).arg(currentVersion()) << fileName;
            break;
        }
        if (!loader.instance()) {
            qWarning(dwLog()) << "load the plugin error." << loader.errorString();
            break;
        }
        plugin = qobject_cast<IWidgetPlugin *>(loader.instance());
        if (!plugin) {
            qWarning(dwLog()) << "the plugin isn't a IWidgetPlugin." << fileName;
            break;
        }
    } while (false);

    if (plugin) {
        auto spec = new WidgetPluginSpec (id, plugin);

        return spec;
    }

    loader.unload();
    return nullptr;
}

WidgetPlugin *WidgetManager::getPlugin(const PluginId &key) const
{
    return m_plugins.value(key);
}

IWidget *WidgetManager::createWidget(const PluginId &pluginId, const IWidget::Type &type)
{
    if (auto plugin = getPlugin(pluginId)) {
        auto instance = plugin->createWidget(type);
        Q_ASSERT(instance);
        if (initialize(instance)) {
            return instance;
        }
        delete instance;
    }
    return nullptr;
}

IWidget *WidgetManager::createWidget(const PluginId &pluginId, const IWidget::Type &type, const InstanceId &id)
{
    if (auto plugin = getPlugin(pluginId)) {
        auto instance = plugin->createWidget(type, id);
        Q_ASSERT(instance);
        if (initialize(instance)) {
            return instance;
        }
        delete instance;
    }
    return nullptr;
}

void WidgetManager::removeWidget(const InstanceId &instanceId)
{
    if (auto instance = m_widgets.take(instanceId)) {
        aboutToShutdown(instance);
        if (auto view = instance->view()) {
            view->deleteLater();
        }
        delete instance;
    }
}

void WidgetManager::typeChanged(const InstanceId &instanceId, const IWidget::Type &type)
{
    if (auto instance = m_widgets.value(instanceId)) {
        WidgetHandlerImpl::get(instance->handler())->m_type = type;
        instance->typeChanged(type);
    }
}

Instance *WidgetManager::getInstance(const InstanceId &key)
{
    return m_widgets.value(key);
}

void WidgetManager::showWidgets(const QVector<Instance *> &instances)
{
    for (auto instance : qAsConst(instances)) {
        QtConcurrent::run(instance, &IWidget::showWidgets);
    }
}

void WidgetManager::hideWidgets(const QVector<Instance *> &instances)
{
    for (auto instance : qAsConst(instances)) {
        QtConcurrent::run(instance, &IWidget::hideWidgets);
    }
}

QVector<Instance *> WidgetManager::initialize(const QVector<Instance *> &instances)
{
    QVector<Instance *> failed;
    QVector<QFuture<void>> futures;
    futures.reserve(instances.size());
    for (auto instance : qAsConst(instances)) {
        qDebug(dwLog()) << "initialize widget." << instance->handler()->pluginId() << instance->handler()->id();
        if (!instance->initialize(m_arguments)) {
            failed.append(instance);
            continue;
        }

        m_widgets[instance->handler()->id()] = instance;

        instance->typeChanged(instance->handler()->type());

        qDebug(dwLog()) << "delayInitialize widget." << instance->handler()->pluginId() << instance->handler()->id();
        futures << QtConcurrent::run(instance, &IWidget::delayInitialize);
    }
    // TODO delay those blocking.
    for (auto item : futures) {
        item.waitForFinished();
    }
    return failed;
}

bool WidgetManager::initialize(Instance *instance)
{
    return initialize(QVector<Instance *>{instance}).isEmpty();
}

void WidgetManager::aboutToShutdown(const QVector<Instance *> &instances)
{
    for (auto instance : qAsConst(instances)) {
        qDebug(dwLog()) << "aboutToShutdown widget." << instance->handler()->pluginId() << instance->handler()->id();
        instance->aboutToShutdown();
    }
}

void WidgetManager::aboutToShutdown(Instance *instance)
{
    aboutToShutdown(QVector<Instance *>{instance});
}

void WidgetManager::clearDataStore(const PluginId &id)
{
    QFile::remove(dataStorePath(id));
}

void WidgetManager::clearDataStore(const PluginId &id, const InstanceId &instanceId)
{
    const auto &path = dataStorePath(id);
    if (!QFile::exists(path))
        return;

    DataStore dataStore(path, QSettings::NativeFormat);
    dataStore.beginGroup(instanceId);
    dataStore.remove("");
    dataStore.endGroup();
}

QString WidgetManager::dataStorePath(const PluginId &pluginId) const
{
    QFileInfo fileInfo(m_dataStore.fileName());
    const QDir dir(fileInfo.absoluteDir());
    const QString &baseName = fileInfo.baseName();
    return dir.absoluteFilePath(QString("%1-%2.json").arg(baseName).arg(pluginId));
}
