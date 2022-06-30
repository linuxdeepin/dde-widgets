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

#include "settingsview.h"
#include "utils/searchinput.h"
#include "timezonemodel.h"

#include <DBlurEffectWidget>
#include <DListView>
#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>

DWIDGET_USE_NAMESPACE
namespace dwclock {
ZoneSearch::ZoneSearch(QWidget *parent)
    : QWidget(parent)
    , m_searchInput(new SearchInput())
{
    auto layout = new QHBoxLayout(this);
    m_searchInput->setSearchText(tr("zonetime name"));
    layout->addWidget(m_searchInput);

    auto completions = new TimezoneListModel();
    m_completer = new QCompleter(completions, m_searchInput);
    m_completer->popup()->setAttribute(Qt::WA_InputMethodEnabled);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->popup()->setSelectionMode(QAbstractItemView::SingleSelection);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains);

    m_searchInput->setCompleter(m_completer);

    auto completerPopup = m_completer->popup();

    DBlurEffectWidget *blurEffect = new DBlurEffectWidget();
    blurEffect->setAccessibleName("blurEffect");
    blurEffect->setMaskColor(Qt::white);

    QHBoxLayout *popuLayout = new QHBoxLayout(completerPopup);
    popuLayout->setSpacing(0);
    popuLayout->setMargin(0);
    popuLayout->addWidget(blurEffect);
    blurEffect->lower();

    connect(m_searchInput, &SearchInput::returnPressed, this, &ZoneSearch::onSelectedChanged);
}

void ZoneSearch::onSelectedChanged()
{
    const auto popup = m_completer->popup();
    QModelIndex index = popup->selectionModel()->currentIndex();
    if (index.isValid()) {
        const QString &timezone = index.data(TimezoneModel::ZoneName).toString();
        Q_EMIT timezoneChanged(timezone);
        popup->close();
    }
}

SettingsView::SettingsView(TimezoneModel *model, QWidget *parent)
    : DDialog(parent)
    , m_clockView(new DListView())
    , m_searchInput(new ZoneSearch())
    , m_model(model)
{
    m_clockView->setDragDropMode(QListView::InternalMove);
    m_clockView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_clockView->setModel(m_model);
    addContent(m_searchInput);
    addContent(m_clockView);
    connect(m_searchInput, &ZoneSearch::timezoneChanged, this, &SettingsView::onTimezoneChanged);
}

void SettingsView::selectItem(const int row)
{
    m_clockView->setCurrentIndex(m_model->index(row, 0));
}

void SettingsView::onTimezoneChanged(const QString &timezone)
{
    const auto index = m_clockView->currentIndex();
    if (!index.isValid() || index.row() >= m_model->rowCount())
        return;

    m_model->updateTimezone(index, timezone);
}
}
