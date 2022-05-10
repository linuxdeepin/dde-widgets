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
#include "widgetmanager.h"
#include "instancemodel.h"

#include <DBlurEffectWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <dflowlayout.h>

class WidgetStore;
class EditModePanel;
class DisplayModePanel;
class AnimationViewContainer;
class GeometryHandler;
DWIDGET_USE_NAMESPACE

class MainView : public DBlurEffectWidget
{
    Q_OBJECT

public:
    explicit MainView (WidgetManager *manager, QWidget *parent = nullptr);
    virtual ~MainView() override;

    enum Mode{
        Display,
        Edit
    };

    void init();

    Mode displayMode() const;

public Q_SLOTS:
    void showView();
    void hideView();

    void updateGeometry(const QRect &rect);

    void switchToEditMode();
    void switchToDisplayMode();

Q_SIGNALS:
    void displayModeChanged();

private:
    int expectedWidth() const;
private:
    WidgetManager *m_manager = nullptr;
    WidgetStore *m_storeView;
    EditModePanel *m_editModeView;
    DisplayModePanel *m_displayModeView;
    InstanceModel *m_instanceModel = nullptr;
    QHBoxLayout *m_layout = nullptr;
    Mode m_mode;

    AnimationViewContainer *m_animationContainer;
    GeometryHandler *m_geometryHandler;
};
