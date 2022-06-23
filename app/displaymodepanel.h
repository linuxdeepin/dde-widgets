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

WIDGETS_FRAME_BEGIN_NAMESPACE
class DisplayModePanelCell : public InstancePanelCell {
    Q_OBJECT
public:
    DisplayModePanelCell(Instance *instance, QWidget *parent = nullptr);

    virtual void setView() override;
};

class DisplayModePanel : public InstancePanel {
    Q_OBJECT
public:
    DisplayModePanel (WidgetManager *manager, QWidget *parent = nullptr);
    void init();

    virtual DisplayModePanelCell *createWidget(Instance *instance) override;

Q_SIGNALS:
    void editClicked();
};
WIDGETS_FRAME_END_NAMESPACE
