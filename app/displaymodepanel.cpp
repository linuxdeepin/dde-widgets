
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

#include "displaymodepanel.h"
#include "widgethandler.h"
#include "widgetmanager.h"
#include "utils.h"

#include <QScrollArea>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>

#include <DIconButton>

DisplayModePanelCell::DisplayModePanelCell(Instance *instance, QWidget *parent)
    : InstancePanelCell(instance, parent)
{
    setLayout(new QVBoxLayout());
    layout()->setContentsMargins(0, 0, 0, 0);
}

void DisplayModePanelCell::setView()
{
    layout()->addWidget(view());
}

DisplayModePanel::DisplayModePanel(WidgetManager *manager, QWidget *parent)
    : InstancePanel (manager, parent)
{
    setContentsMargins(UI::Display::leftMargin, UI::Display::topMargin, UI::Display::rightMargin, UI::Display::bottomMargin);
}

void DisplayModePanel::init()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(UI::defaultMargins);
    layout->setSpacing(0);
    m_views->setFixedWidth(width());

    layout->addWidget(m_views);

    auto button = new QPushButton();
    button->setText(tr("edit"));
    button->setFixedSize(UI::Display::EditSize);
    connect(button, &QPushButton::clicked, this, &DisplayModePanel::editClicked);
    layout->addSpacing(UI::Ins::button2FlowPanelTopMargin);
    layout->addWidget(button, 0, Qt::AlignHCenter);

    layout->addStretch();
}

DisplayModePanelCell *DisplayModePanel::createWidget(Instance *instance)
{
    auto cell = new DisplayModePanelCell(instance, m_views);

    return cell;
}
