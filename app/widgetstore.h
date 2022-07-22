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
class QScrollArea;
class QStackedLayout;
DWIDGET_BEGIN_NAMESPACE
class DButtonBox;
DWIDGET_END_NAMESPACE
WIDGETS_FRAME_BEGIN_NAMESPACE
class WidgetManager;
class WidgetStoreCell;
class PluginCell : public DBlurEffectWidget {
    Q_OBJECT
public:
    explicit PluginCell(QWidget *parent = nullptr);
    void setTitle(const QString &text);
    void setDescription(const QString &text);
    void addCell(WidgetStoreCell *cell);
    void setChecked(const int index, const bool checked = true);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel *m_title = nullptr;
    QLabel *m_description = nullptr;
    QStackedLayout *m_layout = nullptr;
    DButtonBox *m_typeBox = nullptr;
    QVector<WidgetStoreCell *> m_cells;
};

class WidgetStoreCell : public DragDropWidget {
    Q_OBJECT
public:
    explicit WidgetStoreCell(WidgetHandler *handler, QWidget *parent = nullptr);
    void setView(QWidget *view);
    QWidget *action() const;
    WidgetHandler *m_handler = nullptr;

Q_SIGNALS:
    void enterChanged(bool in);
    void addWidget(const PluginId &pluginId, int type);

protected:
    virtual void startDrag(const QPoint &pos) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    virtual void timerEvent(QTimerEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void updateViewPlaceholder();

    QWidget *m_view = nullptr;
    QLabel *m_viewPlaceholder = nullptr;
    QBasicTimer m_viewPlaceholderFresher;
    QWidget *m_action = nullptr;
};

class WidgetStore : public QWidget {
    Q_OBJECT
public:
    explicit WidgetStore (WidgetManager *manager, QWidget *parent = nullptr);
    void init();

    void addPlugin(const PluginId &pluginId);
    void removePlugin(const PluginId &pluginId);
    QScrollArea *scrollView();

private:
    void load();
    PluginCell *addPluginCell(const PluginId &pluginId);
Q_SIGNALS:
    void addWidget(const PluginId &pluginId, int type);
private:
    WidgetManager* m_manager = nullptr;
    QWidget *m_views = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QMap<PluginId, PluginCell *> m_pluginCells;
    QScrollArea *m_scrollView = nullptr;
};
WIDGETS_FRAME_END_NAMESPACE
