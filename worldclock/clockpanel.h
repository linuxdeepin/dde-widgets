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
#include "clock.h"

class QVBoxLayout;
namespace dwclock {
class ClockDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ClockDelegate(QObject *parent = Q_NULLPTR);

public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString utcOffsetText(const int offset) const;
    QString promptOfUtcOffsetText(const int offset) const;

    mutable Clock m_clock;
};

class ClockView : public QListView
{
    Q_OBJECT
public:
    explicit ClockView (QWidget *parent = nullptr);
    virtual ~ClockView() override;

    bool isSmallType() const;
    void setSmallType(const bool small);

private:
    bool m_isSmallType = false;
};

class ClockPanel : public QWidget {
    Q_OBJECT
public:
    explicit ClockPanel(QWidget *parent = nullptr);
    ClockView *view() const;

    void setSmallType(const bool isSmallType);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    ClockView *m_view = nullptr;
    QVBoxLayout *m_layout = nullptr;
};
}
