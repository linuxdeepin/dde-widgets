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
#include "instancepanel.h"

DWIDGET_USE_NAMESPACE

class WidgetManager;
class EditModePanelCell : public InstancePanelCell {
    Q_OBJECT
public:
    explicit EditModePanelCell(Instance *instance, QWidget *parent = nullptr);
    void init(const QString &title);
    virtual void setView() override;

Q_SIGNALS:
    void removeWidget(const InstanceId &id);
};

class EditModePanel : public InstancePanel {
    Q_OBJECT
public:
    explicit EditModePanel (WidgetManager *manager, QWidget *parent = nullptr);
    void init();

    virtual InstancePanelCell *createWidget(Instance *instance) override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

Q_SIGNALS:
    void editCompleted();
};
