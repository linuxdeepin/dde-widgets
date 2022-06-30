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

#include <QWidget>
#include <QListView>
#include <QStyledItemDelegate>
#include "widgetsinterface.h"

namespace dwclock {
WIDGETS_USE_NAMESPACE
class ClockDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ClockDelegate(QObject *parent = Q_NULLPTR);

public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString utcOffsetText(const int offset) const;
};

class ClockView : public QListView
{
    Q_OBJECT
public:
    explicit ClockView (QWidget *parent = nullptr);
    virtual ~ClockView() override;

    bool isSmallType() const;
    void setWidgetHandler(WidgetHandler *handler);

private:
    WidgetHandler *m_handler = nullptr;
};
}
