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

#include "clockview.h"
#include "clock.h"
#include "global.h"
#include "timezonemodel.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <DFontSizeManager>
#include <QTimer>
DWIDGET_USE_NAMESPACE

namespace dwclock {
static const QString WorldClockCity(u8"world-clock-city");
static const QString WorldClockOffset(u8"world-clock-offset");

ClockDelegate::ClockDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ClockDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    auto view = qobject_cast<const ClockView *>(option.widget);
    Q_ASSERT(view);

    QWidget *cell = new QWidget(parent);
    auto layout = new QVBoxLayout(cell);
    auto margins = UI::clock::defaultMargins;
    margins.setTop(UI::clock::topMargin);
    margins.setBottom(UI::clock::bottomMargin);
    layout->setContentsMargins(margins);
    layout->setSpacing(0);
    auto clock = new ClockWidget();
    clock->setObjectName(WorldClockOffset);
    {
        const auto cellSize = option.rect.size();
        const auto delta = view->isSmallType() ? UI::clock::smallAndMiddleDeltaSize : 0;
        const int minLength = qMin(cellSize.width() - delta, cellSize.height());
        clock->setFixedSize(QSize(minLength, minLength));
    }
    int utcOffset = index.data(TimezoneModel::ZoneOffset).toInt();
    clock->setUTCOffset(utcOffset);
    layout->addWidget(clock, 0, Qt::AlignHCenter | Qt::AlignTop);

    layout->addStretch();

    layout->addSpacing(UI::clock::spacingCityNameAndTimezoneOffset);

    auto cityLabel = new QLabel();
    cityLabel->setObjectName(WorldClockCity);
    cityLabel->setText(index.data().toString());
    QFont font = cityLabel->font();
    font.setBold(true);
    font.setWeight(QFont::Black);
    cityLabel->setFont(font);
    cityLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    cityLabel->setForegroundRole(QPalette::BrightText);
    DFontSizeManager::instance()->bind(cityLabel, DFontSizeManager::T8);
    layout->addWidget(cityLabel, 0, Qt::AlignHCenter | Qt::AlignBottom);

    if (!view->isSmallType()) {
        layout->addSpacing(UI::clock::spacingCityNameAndTimezoneOffset);

        auto offset = new QLabel();
        offset->setObjectName(WorldClockOffset);
        offset->setText(utcOffsetText(utcOffset));
        layout->addWidget(offset, 0, Qt::AlignHCenter | Qt::AlignBottom);
    }
    return cell;
}

QSize ClockDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto view = qobject_cast<const ClockView *>(option.widget);
    Q_ASSERT(view);

    int spacing = view->spacing();

    const auto &rect = option.rect;

    const auto cellSize = QSize((rect.width() - (index.model()->rowCount() - 1) * spacing) / index.model()->rowCount(), rect.height());
    const int minLength = qMin(cellSize.width(), cellSize.height());
    return QSize(minLength, minLength);
}

void ClockDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void ClockDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(painter);

    if (auto view = qobject_cast<const ClockView*>(option.widget)) {
        if (!view->isPersistentEditorOpen(index)) {
            const_cast<ClockView *>(view)->openPersistentEditor(index);
        }
    }
}

QString ClockDelegate::utcOffsetText(const int offset) const
{
    return QString("%1h").arg(offset / 3600);
}

void ClockDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (auto label = editor->findChild<QLabel *>(WorldClockCity))
        label->setText(index.data().toString());

    if (auto label = editor->findChild<QLabel *>(WorldClockOffset))
        label->setText(utcOffsetText(index.data(TimezoneModel::ZoneOffset).toInt()));

    if (auto widget = editor->findChild<ClockWidget *>(WorldClockOffset))
        widget->setUTCOffset(index.data(TimezoneModel::ZoneOffset).toInt());
}

ClockView::ClockView(QWidget *parent)
    : QListView (parent)
{
    setFrameShape(QFrame::NoFrame);
    setFlow(QListView::LeftToRight);
    setItemDelegate(new ClockDelegate(this));
    setSpacing(UI::clock::cellSpacing);
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPalette pt = palette();
    // QFrame doesn't draw Background using QPalette::Window.
    pt.setBrush(QPalette::Base, UI::clock::panelBackground);
    setPalette(pt);

    // Update per second.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){ update(); });
    timer->start(1000);
}

ClockView::~ClockView()
{
}

bool ClockView::isSmallType() const
{
    if (m_handler)
        return m_handler->type() == IWidget::Small;

    return false;
}

void ClockView::setWidgetHandler(WidgetHandler *handler)
{
    m_handler = handler;
}
}
