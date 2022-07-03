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

#include "mainview.h"
#include "displaymodepanel.h"
#include "editmodepanel.h"
#include "widgetstore.h"
#include "widgetmanager.h"
#include "animationviewcontainer.h"
#include "geometryhandler.h"
#include "appearancehandler.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QScrollArea>
#include <DFontManager>
#include <DPlatformWindowHandle>
DGUI_USE_NAMESPACE
WIDGETS_FRAME_BEGIN_NAMESPACE
MainView::MainView( WidgetManager *manager, QWidget *parent)
    : DBlurEffectWidget (parent)
    , m_manager(manager)
    , m_animationContainer(new AnimationViewContainer(parent))
    , m_layout(new QHBoxLayout(this))
    , m_instanceModel(new InstanceModel(m_manager, this))
    , m_geometryHandler(new GeometryHandler())
    , m_appearancehandler(new Appearancehandler(this))
{
    setParent(m_animationContainer);
    m_appearancehandler->addTargetWidget(m_animationContainer);

    qApp->installEventFilter(new LongPressEventFilter(this));
    if (releaseMode()) {
        DPlatformWindowHandle handler(parentWidget());
        handler.setEnableSystemResize(false);
        handler.setEnableSystemMove(false);
    }

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    m_layout->setContentsMargins(UI::defaultMargins);
    m_layout->setSpacing(UI::EditMode::spacing);

    m_storeView = new WidgetStore(m_manager, this);
    m_storeView->setFixedWidth(UI::Store::panelWidth);
    m_storeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    m_layout->addWidget(m_storeView);

    m_editModeView = new EditModePanel(m_manager, this);
    m_editModeView->setFixedWidth(UI::Edit::panelWidth);

    connect(m_editModeView, &EditModePanel::editCompleted, this, [this] () {
        switchToDisplayMode();
        m_animationContainer->showView();
    });

    m_displayModeView = new DisplayModePanel(m_manager);
    m_displayModeView->setFixedWidth(UI::Display::panelWidth);
    m_displayModeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    connect(m_displayModeView, &DisplayModePanel::editClicked, this, [this] () {
        switchToEditMode();
        m_animationContainer->showView();
    });

    m_layout->addStretch();
}

MainView::~MainView()
{
}

void MainView::init()
{
    m_instanceModel->loadPrePanelInstances();

    connect(m_storeView, &WidgetStore::addWidget, this, [this](const PluginId &pluginId, int type){
        m_instanceModel->addInstance(pluginId, static_cast<IWidget::Type>(type));
    });

    m_storeView->init();

    m_editModeView->setModel(m_instanceModel);
    m_editModeView->init();

    m_displayModeView->setModel(m_instanceModel);
    m_displayModeView->init();

    connect(m_animationContainer, &AnimationViewContainer::outsideAreaReleased, this, &MainView::hideView);
}

MainView::Mode MainView::displayMode() const
{
    return m_mode;
}

void MainView::showView()
{
    qDebug(dwLog()) << "showView()";
    m_animationContainer->showView();
    m_manager->showAllWidgets();
}

void MainView::hideView()
{
    qDebug(dwLog()) << "hideView()";
    m_animationContainer->hideView();
    m_manager->hideAllWidgets();
}

void MainView::updateGeometry(const QRect &rect)
{
    m_animationContainer->updateGeometry(rect);
    resize(rect.size());
}

void MainView::switchToEditMode()
{
    m_mode = Edit;
    setContentsMargins(UI::EditMode::leftMargin, UI::topMargin, UI::EditMode::rightMargin, UI::bottomMargin);

    const auto targetRect = m_geometryHandler->getGeometry(expectedWidth());
    updateGeometry(targetRect);

    if (hasComposite()) {
        setBlurEnabled(true);
    }
    m_storeView->scrollView()->setVisible(true);
    m_layout->addWidget(m_storeView->scrollView());

    m_layout->removeWidget(m_displayModeView->scrollView());
    m_layout->addWidget(m_editModeView->scrollView());
    m_editModeView->setEnabledMode(true);
    m_displayModeView->setEnabledMode(false);

    Q_EMIT displayModeChanged();
}

void MainView::switchToDisplayMode()
{
    m_mode = Display;
    setContentsMargins(UI::DisMode::leftMargin, UI::topMargin, UI::DisMode::rightMargin, UI::bottomMargin);

    const auto targetRect = m_geometryHandler->getGeometry(expectedWidth());
    updateGeometry(targetRect);
    // TODO DBlurEffectWidget is black when changing BlurEnabled but none Composite, it's maybe a bug.
    if (hasComposite()) {
        setBlurEnabled(false);
    }
    m_storeView->scrollView()->setVisible(false);
    m_layout->removeWidget(m_storeView->scrollView());
    m_layout->removeWidget(m_editModeView->scrollView());
    m_layout->addWidget(m_displayModeView->scrollView());
    m_editModeView->setEnabledMode(false);
    m_displayModeView->setEnabledMode(true);

    Q_EMIT displayModeChanged();
}

void MainView::removePlugin(const PluginId &pluginId)
{
    m_instanceModel->removePlugin(pluginId);
    m_storeView->removePlugin(pluginId);

    m_manager->removePlugin(pluginId);
}

void MainView::addPlugin(const PluginPath &pluginPath)
{
    auto spec = m_manager->loadPlugin(pluginPath);

    m_storeView->addPlugin(spec->id());
}

int MainView::expectedWidth() const
{
    return m_mode == Edit ? UI::EditMode::width : UI::DisMode::width;
}
WIDGETS_FRAME_END_NAMESPACE
