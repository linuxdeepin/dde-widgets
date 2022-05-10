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

#include <QObject>
#include <QEvent>
#include <QWidget>
#include <QLayoutItem>
#include <QPropertyAnimation>

class DragDropFilter : public QObject {
    Q_OBJECT
public:
    explicit DragDropFilter(QObject *parent);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    bool pressFlag = false;
};

class DragDropWidget : public QWidget {
    Q_OBJECT
public:
    explicit DragDropWidget(QWidget *parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void startDrag(const QPoint &pos) { Q_UNUSED(pos);}
    virtual void gobackDrag(const QPixmap &pixmap, const QPoint &pos);

protected:
    QPoint m_startDrag;
private:
    bool m_pressFlag = false;
};
class AnimationWidgetItem : public QObject, public QWidgetItemV2 {
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE updateGeometry)
public:
    explicit AnimationWidgetItem(QWidget *widget);
    virtual void setGeometry(const QRect &rect) override;

public Q_SLOTS:
    void updateGeometry(const QRect &rect);

private:
    QPropertyAnimation *m_currentXAni = nullptr;
};

bool hasComposite();
