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
#include "utils.h"
#include <widgetsinterface.h>

#include <DBlurEffectWidget>
DWIDGET_USE_NAMESPACE
WIDGETS_USE_NAMESPACE

class QLabel;
class QVBoxLayout;
class QStackedLayout;
class WidgetManager;
class WidgetStoreCell;
DWIDGET_BEGIN_NAMESPACE
class DButtonBox;
DWIDGET_END_NAMESPACE
class PluginCell : public DBlurEffectWidget {
    Q_OBJECT
public:
    explicit PluginCell(QWidget *parent = nullptr);
    void setTitle(const QString &text);
    void setDescription(const QString &text);
    void addCell(WidgetStoreCell *cell);
    void setChecked(const int index, const bool checked = true);

private:
    QLabel *m_title = nullptr;
    QLabel *m_description = nullptr;
    QStackedLayout *m_layout = nullptr;
    DButtonBox *m_typeBox = nullptr;
};

class WidgetStoreCell : public DragDropWidget {
    Q_OBJECT
public:
    explicit WidgetStoreCell(WidgetHandler *handler, QWidget *parent = nullptr);
    void setView(QWidget *view);
    WidgetHandler *m_handler = nullptr;

Q_SIGNALS:
    void enterChanged(bool in);
    void addWidget(const PluginId &pluginId, int type);

protected:
    void startDrag(const QPoint &pos) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QWidget *m_view = nullptr;
};

class WidgetStore : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStore (WidgetManager *manager, QWidget *parent = nullptr);
    void init();

private:
    void load();
Q_SIGNALS:
    void addWidget(const PluginId &pluginId, int type);
private:
    WidgetManager* m_manager = nullptr;
    QWidget *m_views = nullptr;
    QVBoxLayout *m_layout = nullptr;
};
