/*
 * Copyright (C) 2017 ~ 2022 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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

#include "notifymodel.h"
#include "notification/persistence.h"
#include "notifylistview.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>

NotifyModel::NotifyModel(QObject *parent, AbstractPersistence *database, NotifyListView *view)
    : QAbstractListModel(parent)
    , m_view(view)
    , m_database(database)
    , m_freeTimer(new QTimer(this))
{
    m_freeTimer->setInterval(AnimationTime + 100);
    initData();
    initConnect();
}

ListItem NotifyModel::getAppData(QString appName) const
{

    foreach (auto item, m_notifications) {
        if (item.appName() == appName)
            return item;
    }
    Q_UNREACHABLE();
}

int NotifyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int count = 0;
    for (int i = 0; i < showCount(); i++) {
        count += 1 + m_notifications[i].showCount();
    }
    return count;
}

QVariant NotifyModel::data(const QModelIndex &index, int role) const
{
    //返回空值，不绘制check状态
    if (Qt::CheckStateRole == role)
        return QVariant();

    if (!index.isValid() || index.row() > (rowCount(QModelIndex()) - 1)) {
        return QVariant();
    }

    EntityPtr entity = getEntityByRow(index.row());
    if (entity) {
        entity->setCurrentIndex(index.row());
        return QVariant::fromValue(entity);
    }
    return QVariant();
}

Qt::ItemFlags NotifyModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (m_view != nullptr) m_view->openPersistentEditor(index);
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index);
}

int NotifyModel::remainNotificationCount() const
{
    // 剩余条数 = 显示项的隐藏条数 + 未显示项的所有条数
    int count = 0;
    int i = 0;
    while (i < showCount()) {
        count += m_notifications[i++].hideCount();
    }
    while (i < m_notifications.count()) {
        count += m_notifications[i++].count();
    }
    return count;
}

void NotifyModel::addNotify(EntityPtr entity)
{
    beginResetModel();
    addAppData(entity);
    endResetModel();
}

void NotifyModel::removeNotify(EntityPtr entity)
{
    if (m_notifications.isEmpty())
        return;

    beginResetModel();
    for (int i = 0; i < m_notifications.size(); i++) {
        ListItem &AppGroup = m_notifications[i];
        if (AppGroup.appName() == entity->appName()) {
            AppGroup.remove(entity);

            if (AppGroup.isEmpty()) {
                m_notifications.removeAt(i);
                break;
            }
        }
    }
    endResetModel();

    if (m_database != nullptr) {
        m_database->removeOne(QString::number(entity->id()));
    }
    Q_EMIT removedNotif();
}

void NotifyModel::removeAppGroup(QString appName)
{
    beginResetModel();
    if (m_notifications.isEmpty())
        return;
    QList<EntityPtr> removedEntities;
    for (int i = 0; i < m_notifications.size(); i++) {
        if (m_notifications[i].appName() == appName) {
            removedEntities = m_notifications[i].take();
            m_notifications.removeAt(i);
            // it's unique item with same appName in m_notifications, so we can break,
            // otherwise, it maybe error remove more than one item.
            break;
        }
    }
    endResetModel();
    // removeApp is not provided by `Notification`.
    for (auto entity : removedEntities) {
        m_database->removeOne(QString::number(entity->id()));
    }
}

void NotifyModel::removeAllData()
{
    beginResetModel();
    m_notifications.clear();
    endResetModel();
    m_database->removeAll();
}

void NotifyModel::expandDataByAppName(const QString &appName)
{
    beginResetModel();
    for (int i = 0; i < m_notifications.size(); i++) {
        ListItem &AppGroup = m_notifications[i];
        if (appName.isEmpty() || AppGroup.appName() == appName) {
            AppGroup.toggleFolding(false);
        }
    }
    endResetModel();
}

void NotifyModel::expandData()
{
    m_isCollapse = false;
    expandDataByAppName(QString());
}

void NotifyModel::collapseData()
{
    m_isCollapse = true;
    beginResetModel();
    for (int i = 0; i < m_notifications.size(); i++) {
        ListItem &AppGroup = m_notifications[i];
        AppGroup.toggleFolding(true);
    }
    endResetModel();
}

void NotifyModel::removeTimeOutNotify()
{
    QList<EntityPtr> notifyList;
    for (int i = m_notifications.size() - 1; i >= 0; i--) {
        const auto &data = m_notifications[i].data();
        // it's timer order, so we can check last item rough first.
        Q_ASSERT(!data.isEmpty());
        if (!checkTimeOut(data.last(), OVERLAPTIMEOUT_7_DAY)) {
            continue;
        }

        auto iter = std::find_if(data.begin(), data.end(), [this](const EntityPtr &entity) {
            return checkTimeOut(entity, OVERLAPTIMEOUT_7_DAY);
        });
        if (iter == data.end()) {
            continue;
        }
        const int index = iter - data.begin();

        beginResetModel();
        notifyList << m_notifications[i].take(index);
        if (index == 0) {
            m_notifications.removeAt(i);
        }
        endResetModel();
    }

    for (int j = 0; j < notifyList.size(); j++) {
        m_database->removeOne(QString::number(notifyList[j]->id()));
    }
}

void NotifyModel::cacheData(EntityPtr entity)
{
    if (m_view->isVisible()) {
        m_cacheList.push_front(entity);
        if (!m_freeTimer->isActive()) {
            m_freeTimer->start();
        }
    } else {
        addNotify(entity);
    }
}

void NotifyModel::freeData()
{
    if (!m_notifications.isEmpty() && m_notifications.first().appName() == m_cacheList.first()->appName()) {
        m_view->createAddedAnimation(m_cacheList.first(), getAppData(m_cacheList.first()->appName()));
    } else {
        addNotify(m_cacheList.first());
    }
    m_cacheList.removeFirst();
    if (m_cacheList.isEmpty()) {
        m_freeTimer->stop();
    }
}

void NotifyModel::initData()
{
    if (m_database == nullptr)  return;
    QList<EntityPtr> notifications = m_database->getAllNotify();

    std::sort(notifications.begin(), notifications.end(), [=](const EntityPtr& ptr1,const EntityPtr& ptr2) {
        return ptr1->ctime().toLongLong() < ptr2->ctime().toLongLong();
    });

    foreach (auto notify, notifications) {
        if(!checkTimeOut(notify, OVERLAPTIMEOUT_7_DAY)) {
            addAppData(notify);
        } else {
            m_database->removeOne(QString::number(notify->id()));
        }
    }
}

void NotifyModel::initConnect()
{
    connect(m_database, &AbstractPersistence::RecordAdded, this, &NotifyModel::cacheData);
    connect(m_freeTimer, &QTimer::timeout, this, &NotifyModel::freeData);
    connect(m_view, &NotifyListView::addedAniFinished, this, &NotifyModel::addNotify);
    connect(m_view, &NotifyListView::removeAniFinished, this, &NotifyModel::removeNotify);
    connect(m_view, &NotifyListView::expandAniFinished, this, &NotifyModel::expandDataByAppName);
    connect(m_view, &NotifyListView::refreshItemTime, this, &NotifyModel::removeTimeOutNotify);
}

void NotifyModel::addAppData(EntityPtr entity)
{
    if (contains(entity->appName())) {
        for (int i = 0; i < m_notifications.size(); i++) {
            ListItem &AppGroup = m_notifications[i];
            if (AppGroup.appName() == entity->appName()) {
                AppGroup.push(entity);
                m_notifications.move(i, 0);
                break;
            }
        }
    } else {
        ListItem AppGroup(entity->appName());
        AppGroup.push(entity);
        m_notifications.append(AppGroup);
    }

    std::sort(m_notifications.begin(), m_notifications.end(), [=](const ListItem & item1, const ListItem & item2) {
        return item1.title()->ctime().toLongLong() > item2.title()->ctime().toLongLong();
    });
}

EntityPtr NotifyModel::getEntityByRow(int row) const
{
    Q_ASSERT(row < rowCount(QModelIndex()));

    for (int i = 0, index = 0; i < showCount(); ++i) {
        const auto &item = m_notifications[i];
        const int nextIndex = index + 1 + item.showCount();
        if (nextIndex <= row) {
            index = nextIndex;
            continue;
        }

        if (index == row) {
            return item.title();
        }
        return item.showAt(row - index - 1);
    }
    Q_UNREACHABLE();
}

bool NotifyModel::checkTimeOut(EntityPtr ptr, int sec)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(ptr->ctime().toLongLong());
    return t.secsTo(QDateTime::currentDateTime()) > sec;
}

bool NotifyModel::contains(const QString &appName)
{
    for (auto item : m_notifications) {
        if (item.appName() == appName)
            return true;
    }
    return false;
}

int NotifyModel::showCount() const
{
    return m_isCollapse ? qMin(NotifyShowMaxCount, m_notifications.count()) : m_notifications.count();
}

ListItem::ListItem(const QString &appName)
{
    m_title = std::make_shared<NotificationEntity>(appName);
    m_title->setIsTitle(true);
}

void ListItem::push(const EntityPtr &entity)
{
    const qlonglong ctime = entity->ctime().toLongLong();
    auto iter = std::find_if(m_data.begin(), m_data.end(), [ctime](const EntityPtr &entity) {
        return entity->ctime().toLongLong() < ctime;
    });

    resetShowLastHideCount();
    if (iter == m_data.end()) {
        m_data.append(entity);
    } else {
        m_data.insert(iter, entity);
    }
    updateShowLastHideCount();
    updateShowTitleTime();
}

void ListItem::remove(const EntityPtr &entity)
{
    auto index = m_data.indexOf(entity);
    if (index >= 0 && index < showCount()) {
        resetShowLastHideCount();
        m_data.removeOne(entity);

        if (showCount() > 0) {
            updateShowLastHideCount();
            updateShowTitleTime();
        }
    }
}

QList<EntityPtr> ListItem::take()
{
    QList<EntityPtr> tmp;
    tmp.swap(m_data);
    return tmp;
}

QList<EntityPtr> ListItem::take(const int from)
{
    Q_ASSERT(0 <= from && from < count());

    QList<EntityPtr> tmp = m_data.mid(from);
    auto iter = m_data.begin() + from;
    while (iter != m_data.end()) {
        resetShowLastHideCount();
        iter = m_data.erase(iter);
        updateShowLastHideCount();
        updateShowTitleTime();
    }
    return tmp;
}

void ListItem::toggleFolding(const bool collapse)
{
    resetShowLastHideCount();
    m_isCollapse = collapse;
    if (m_isCollapse) {
        updateShowLastHideCount();
    } else {
        resetShowLastHideCount();
    }
}

void ListItem::resetShowLastHideCount()
{
    if (showCount() > 0) {
        showLast()->setHideCount(0);
    }
}

void ListItem::updateShowLastHideCount()
{
    if (showCount() > 0) {
        showLast()->setHideCount(qMin(2, hideCount()));
    }
}

void ListItem::updateShowTitleTime()
{
    if (showCount() > 0) {
        m_title->setTime(showFirst()->ctime());
    }
}
