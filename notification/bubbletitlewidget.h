/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BUBBLETITLEWIDGET_H
#define BUBBLETITLEWIDGET_H
#include "notification/constants.h"
#include "notifymodel.h"
#include "bubbleitem.h"
#include <DWidget>
#include <DIconButton>
#include <DLabel>

DWIDGET_USE_NAMESPACE

class NotifyListView;
class CicleIconButton;

class BubbleTitleWidget : public BubbleBase
{
    Q_OBJECT
public:
    BubbleTitleWidget(NotifyModel *model = nullptr, EntityPtr entity = nullptr, QWidget *parent = nullptr);

public:
    void setIndexRow(int row);
    int indexRow() { return m_indexRow; }
    QList<QPointer<QWidget>> bubbleElements() const override;
    static int bubbleTitleWidgetHeight();

private Q_SLOTS:
    void toggleNotificationFolding();

private:
    void updateNotificationFoldingStatus();

    DLabel *m_titleLabel;
    CicleIconButton *m_toggleNotificationFolding = nullptr;
    int m_indexRow = 0;
};

#endif // BUBBLETITLEWIDGET_H
