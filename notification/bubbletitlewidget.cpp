/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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

#include "bubbletitlewidget.h"
#include "notification/bubbletool.h"
#include "notifylistview.h"

#include <QKeyEvent>
#include <QBoxLayout>
#include <QScroller>

#include <DFontSizeManager>
#include <DSysInfo>

BubbleTitleWidget::BubbleTitleWidget(NotifyModel *model, EntityPtr entity, QWidget *parent)
    : DWidget(parent),
      m_titleLabel(nullptr),
      m_model(model)
{
    m_appName = entity->appName();
    this->setObjectName(entity->appName());
    m_titleLabel = new DLabel;
    m_titleLabel->setForegroundRole(QPalette::BrightText);
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QFont font = m_titleLabel->font();
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    m_titleLabel->setFont(font);
    m_titleLabel->setText(BubbleTool::getDeepinAppName(entity->appName()));
    DFontSizeManager::instance()->bind(m_titleLabel, DFontSizeManager::T4);

    m_toggleNotificationFolding = new DIconButton(nullptr);
    m_toggleNotificationFolding->setObjectName(m_appName + "-ToggleNotificationFolding");
    m_toggleNotificationFolding->setFlat(true);
    m_toggleNotificationFolding->setCheckable(true);
    m_toggleNotificationFolding->setFixedSize(Notify::GroupButtonSize - 4, Notify::GroupButtonSize - 4);
    m_toggleNotificationFolding->setVisible(false);
    connect(m_toggleNotificationFolding, &DIconButton::clicked, this, &BubbleTitleWidget::toggleNotificationFolding);

    m_gotopBtn = new DIconButton(nullptr);
    m_gotopBtn->setObjectName(entity->appName() + "-GoTop");
    m_gotopBtn->setFlat(true);
    m_gotopBtn->setCheckable(true);
    m_gotopBtn->setVisible(false);
    connect(m_gotopBtn, &DIconButton::clicked, this, &BubbleTitleWidget::toggleAppTopping);

    m_closeButton = new DIconButton(DStyle::SP_CloseButton);
    m_closeButton->setObjectName(entity->appName() + "-CloseButton");
    m_closeButton->setFlat(true);
    m_closeButton->setIconSize(QSize(Notify::GroupButtonSize, Notify::GroupButtonSize));
    m_closeButton->setFixedSize(Notify::GroupButtonSize - 4, Notify::GroupButtonSize - 4);
    m_closeButton->setVisible(false);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setContentsMargins(10, 0, 0, 0);
    head_Layout->addWidget(m_titleLabel, Qt::AlignLeft);
    head_Layout->addStretch();
    head_Layout->addWidget(m_toggleNotificationFolding, 0, Qt::AlignRight);
    head_Layout->addWidget(m_gotopBtn, 0, Qt::AlignRight);
    head_Layout->addWidget(m_closeButton, 0, Qt::AlignRight);
    setLayout(head_Layout);

    connect(m_closeButton, &DIconButton::clicked, this, [ = ] {
        m_model->removeAppGroup(entity->appName());
    });

    updateNotificationFoldingStatus();
    updateAppToppingStatus();
}

void BubbleTitleWidget::setIndexRow(int row)
{
    m_indexRow = row;
}

void BubbleTitleWidget::enterEvent(QEvent *event)
{
    // QScroller::hasScroller用于判断listview是否处于滑动状态，滑动状态不触发paint相关操作，否则滑动动画异常
    // 欧拉此函数返回异常，且无触屏场景,不需要此判断
    if (Dtk::Core::DSysInfo::uosEditionType() == Dtk::Core::DSysInfo::UosEuler) {
        m_closeButton->setVisible(true);
        m_toggleNotificationFolding->setVisible(true);
        m_gotopBtn->setVisible(true);
    }
    if (!QScroller::hasScroller(m_view)) {
        m_closeButton->setVisible(true);
        m_toggleNotificationFolding->setVisible(true);
        m_gotopBtn->setVisible(true);
        updateAppToppingStatus();
    }
    QWidget::enterEvent(event);
}

void BubbleTitleWidget::leaveEvent(QEvent *event)
{
    if (Dtk::Core::DSysInfo::uosEditionType() == Dtk::Core::DSysInfo::UosEuler) {
        m_closeButton->setVisible(false);
        m_toggleNotificationFolding->setVisible(false);
        m_gotopBtn->setVisible(false);
    }
    bool hasScroller = QScroller::hasScroller(m_view);
    if (!hasScroller) {
        m_closeButton->setVisible(false);
        m_toggleNotificationFolding->setVisible(false);
        m_gotopBtn->setVisible(false);
    } else {
        // 滚动结束,处理hover变化
        connect(QScroller::scroller(m_view), &QScroller::stateChanged, this, [this](const QScroller::State state){
            if (state == QScroller::Inactive) {
                m_closeButton->setVisible(false);
                m_toggleNotificationFolding->setVisible(false);
                m_gotopBtn->setVisible(false);
            }
        });
    }

    QWidget::enterEvent(event);
}

void BubbleTitleWidget::focusInEvent(QFocusEvent *event)
{
    m_closeButton->setVisible(true);
    m_toggleNotificationFolding->setVisible(true);
    m_gotopBtn->setVisible(true);
    QWidget::focusInEvent(event);
}

void BubbleTitleWidget::focusOutEvent(QFocusEvent *event)
{
    m_closeButton->setVisible(false);
    m_toggleNotificationFolding->setVisible(false);
    m_gotopBtn->setVisible(false);
    QWidget::focusOutEvent(event);
}

QList<QPointer<QWidget> > BubbleTitleWidget::bubbleElements()
{
    QList<QPointer<QWidget>> bubble_elements;
    bubble_elements.append(m_closeButton);
    return bubble_elements;
}

void BubbleTitleWidget::setParentView(NotifyListView *view)
{
    m_view = view;
}

int BubbleTitleWidget::bubbleTitleWidgetHeight()
{
    return qMax(QFontMetrics(DFontSizeManager::instance()->t4()).height(), BubbleTitleHeight);
}

void BubbleTitleWidget::updateNotificationFoldingStatus()
{
    const ListItemPtr item = m_model->getAppData(m_appName);
    const bool isCollapse = item->isCollapse();

    m_toggleNotificationFolding->setIcon(isCollapse ? QIcon::fromTheme("go-down") :
                                                      QIcon::fromTheme("go-up"));
}

void BubbleTitleWidget::toggleNotificationFolding()
{
    const ListItemPtr item = m_model->getAppData(m_appName);
    const bool isCollapse = item->isCollapse();

    if (isCollapse) {
        m_model->expandDataByAppName(m_appName);
    } else {
        m_model->collapseDataByAppName(m_appName);
    }
    updateNotificationFoldingStatus();
}

void BubbleTitleWidget::updateAppToppingStatus()
{
    const bool isTopping = m_model->isAppTopping(m_appName);
    m_gotopBtn->setIcon(isTopping ? QIcon::fromTheme("stop") :
                                    QIcon::fromTheme("go-top"));

}

void BubbleTitleWidget::toggleAppTopping()
{
    const bool isTopping = m_model->isAppTopping(m_appName);
    m_model->setAppTopping(m_appName, !isTopping);

    m_model->refreshAppTopping();

    updateAppToppingStatus();
}
