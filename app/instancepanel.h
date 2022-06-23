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
#include <dflowlayout.h>
#include "utils.h"

DWIDGET_USE_NAMESPACE

class QScrollArea;
WIDGETS_FRAME_BEGIN_NAMESPACE
class InstanceModel;
class WidgetManager;
class InstancePanelCell : public DragDropWidget {
    Q_OBJECT
public:
    explicit InstancePanelCell(Instance *instance, QWidget *parent = nullptr);

    void setInstance(Instance *instance);
    QWidget *view() const;
    InstanceId id() const;
    virtual void setView() = 0;
    bool isFixted() const;
    bool isCustom() const;

protected:
    virtual void startDrag(const QPoint &pos) override;

protected:
    Instance *m_instance;
};

class InstancePanel : public QWidget {
    Q_OBJECT
public:
    explicit InstancePanel (WidgetManager *manager, QWidget *parent = nullptr);
    virtual ~InstancePanel() override;
    void setModel(InstanceModel *model);
    QScrollArea *scrollView();

    bool isEnabledMode() const { return m_mode;}
    void setEnabledMode(bool mode);

    virtual InstancePanelCell *createWidget(Instance *instance) = 0;
public Q_SLOTS:
    void addWidget(const InstanceId &key, InstancePos pos);
    void moveWidget(const InstancePos &source, InstancePos target);
    void removeWidget(const InstanceId &id);
    void replaceWidget(const InstanceId &id, InstancePos pos);

protected:
    int positionCell(const QPoint &pos) const;
    int positionCell(const QPoint &pos, const QSize &size, const QPoint &hotSpot) const;
    void setView();

private Q_SLOTS:
    void onMenuRequested(const InstanceId &id);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

    bool canDragDrop(const InstancePos pos) const;
private:
    bool canDragDrop(QDropEvent *event) const;

protected:
    WidgetManager *m_manager = nullptr;
    QWidget *m_views = nullptr;
    InstanceModel *m_model = nullptr;
private:
    DFlowLayout *m_layout = nullptr;
    bool m_mode = false;
    QScrollArea *m_scrollView = nullptr;
};
WIDGETS_FRAME_END_NAMESPACE
