/*
 * Copyright (C) 2016 ~ 2022 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *             yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "persistence.h"

#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMapIterator>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingReply>

#include "notificationentity.h"

static const QString DDENotifyDBusServer = "com.deepin.dde.Notification";
static const QString DDENotifyDBusInterface = "com.deepin.dde.Notification";
static const QString DDENotifyDBusPath = "/com/deepin/dde/Notification";

PersistenceObserver::PersistenceObserver(QObject *parent)
    : AbstractPersistence(parent)
{
}

void PersistenceObserver::removeOne(const QString &id)
{
    qDebug() << "removeOne() RemoveRecord id:" << id;
    notifyObserver()->asyncCall("RemoveRecord", id);
}

void PersistenceObserver::removeAll()
{
    qDebug() << "removeAll() ClearRecords";
    notifyObserver()->asyncCall("ClearRecords");
}

QList<EntityPtr> PersistenceObserver::getAllNotify()
{
    qDebug() << "getAllNotify() GetAllRecords";
    QDBusPendingCall call = notifyObserver()->asyncCall("GetAllRecords");
    call.waitForFinished();
    QDBusPendingReply<QString> reply(call);
    const QString &data = reply.value();

    QList<EntityPtr> result;
    QJsonArray notifys = QJsonDocument::fromJson(data.toLocal8Bit()).array();
    foreach (auto notify, notifys) {
        QJsonObject obj = notify.toObject();
        auto entity = json2Entity(obj);
        if (!entity) {
            qWarning() << "getAllNotify() entity is invalid" << obj;
            continue;
        }
        result << entity;
    }

    return result;
}

void PersistenceObserver::onReceivedRecord(const QString &id)
{
    qDebug() << "onReceivedRecord() RecordAdded id" << id;
    // TODO multiple RecordAdded requests can be merged into one.
    QDBusPendingCall call = notifyObserver()->asyncCall("GetRecordById", id);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished,
                     this, [this] (QDBusPendingCallWatcher *call) {
        QDBusPendingReply<QString> reply = *call;
        if (reply.isError()) {
            qWarning() << "onReceivedRecord() GetRecordById error:" << reply.error();
        } else {
            const QString &data = reply.value();
            if (auto entity = json2Entity(data)) {
                emit RecordAdded(entity);
            }
        }
        call->deleteLater();
    });
}

static QVariantMap ConvertStringToMap(const QString &text)
{
    QVariantMap map;

    QStringList keyValueList = text.split(HINT_SEGMENT);
    foreach (QString text, keyValueList) {
        QStringList list = text.split(KEYVALUE_SEGMENT);
        if (list.size() != 2)
            continue;
        QString key = list[0];
        QVariant value = QVariant::fromValue(list[1]);

        map.insert(key, value);
    }

    return map;
}

EntityPtr PersistenceObserver::json2Entity(const QString &data)
{
    const auto array = QJsonDocument::fromJson(data.toLocal8Bit()).array();
    if (array.isEmpty())
        return nullptr;

    return json2Entity(array.first().toObject());
}

EntityPtr PersistenceObserver::json2Entity(const QJsonObject &obj)
{
    QStringList actions = obj.value("action").toString().split(ACTION_SEGMENT);
    auto notification = std::make_shared<NotificationEntity>(obj.value("name").toString(),
                                                             obj.value("id").toString(),
                                                             obj.value("icon").toString(),
                                                             obj.value("summary").toString(),
                                                             obj.value("body").toString(),
                                                             actions,
                                                             ConvertStringToMap(obj.value("hint").toString()),
                                                             obj.value("time").toString(),
                                                             obj.value("replacesid").toString(),
                                                             obj.value("timeout").toString());
    return notification;
}

QDBusInterface * PersistenceObserver::notifyObserver()
{
    if (!m_notifyObserver) {
        m_notifyObserver.reset(new QDBusInterface(DDENotifyDBusServer, DDENotifyDBusPath, DDENotifyDBusInterface));

        if (!m_notifyObserver->isValid()) {
            qWarning() << "notifyObserver() NotifyInterface is invalid, and can't send operator.";
        }

        auto connection = QDBusConnection::sessionBus();
        bool valid = connection.connect(DDENotifyDBusServer, DDENotifyDBusPath, DDENotifyDBusInterface,
                           "RecordAdded", this, SLOT(onReceivedRecord(const QString &)));
        if (!valid) {
            qWarning() << "notifyObserver() NotifyConnection is invalid, and can't receive RecordAdded signal.";
        }

    }
    return m_notifyObserver.get();
}
