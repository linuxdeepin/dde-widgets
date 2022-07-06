/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "notifycenterwidget.h"
#include "notification/persistence.h"
#include "notification/constants.h"
#include "notification/iconbutton.h"
#include "style.h"
#include "helper.hpp"

#include <QDesktopWidget>
#include <QBoxLayout>
#include <QDBusInterface>
#include <QPalette>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QMenu>

#include <DLabel>
#include <DFontSizeManager>
#include <DGuiApplicationHelper>
#include <QPushButton>

DWIDGET_USE_NAMESPACE

NotifyCenterWidget::NotifyCenterWidget(AbstractPersistence *database, QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_notifyWidget(new NotifyWidget(this, database))
    , m_wmHelper(DWindowManagerHelper::instance())
{
    setBlurEnabled(false);
    initUI();
    initConnections();

    CompositeChanged();
}

void NotifyCenterWidget::initUI()
{
    m_notifyWidget->setAccessibleName("NotifyWidget");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);

    m_headWidget = new QWidget;
    m_headWidget->setAccessibleName("HeadWidget");
    m_headWidget->setFixedSize(Notify::CenterWidth, Notify::CenterTitleHeight);

    title_label = new DLabel;
    title_label->setAccessibleName("TitleLabel");
    QFont font = title_label->font();
    font.setBold(true);
    font.setWeight(QFont::Black);
    title_label->setFont(font);
    title_label->setText(tr("Notification Center"));
    title_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    title_label->setForegroundRole(QPalette::BrightText);
    DFontSizeManager::instance()->bind(title_label, DFontSizeManager::T3);

    m_toggleNotificationFolding = new CicleIconButton(nullptr);
    m_toggleNotificationFolding->setAccessibleName("ToggleNotificationFolding");
    m_toggleNotificationFolding->setFixedSize(UI::Panel::buttonSize);
    connect(m_toggleNotificationFolding, &CicleIconButton::clicked, this, &NotifyCenterWidget::toggleNotificationFolding);

    m_settingBtn = new CicleIconButton(nullptr);
    m_settingBtn->setIcon(DDciIcon::fromTheme("notify_more"));
    m_settingBtn->setAccessibleName("SettingButton");
    m_settingBtn->setFixedSize(UI::Panel::buttonSize);
    connect(m_settingBtn, &CicleIconButton::clicked, this, &NotifyCenterWidget::showSettingMenu);

    m_clearButton = new CicleIconButton;
    m_clearButton->setAccessibleName("ClearButton");
    m_clearButton->setIcon(DDciIcon::fromTheme("notify_clear"));
    m_clearButton->setFixedSize(UI::Panel::buttonSize);
//    m_clearButton->setFocusPolicy(Qt::StrongFocus);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->setMargin(0);
    head_Layout->addWidget(title_label, 0, Qt::AlignLeft | Qt::AlignBottom);
    head_Layout->addStretch();
    head_Layout->addWidget(m_toggleNotificationFolding, 0, Qt::AlignRight | Qt::AlignTop);
    head_Layout->addWidget(m_settingBtn, 0, Qt::AlignRight | Qt::AlignTop);
    head_Layout->addWidget(m_clearButton, 0, Qt::AlignRight | Qt::AlignTop);
    m_headWidget->setLayout(head_Layout);

    m_expandRemaining = new QPushButton();
    m_expandRemaining->setFlat(true);
    m_expandRemaining->setAccessibleName("ExpandRemainingButton");
    connect(m_expandRemaining, &QPushButton::clicked, this, &NotifyCenterWidget::expandNotificationFolding);
    connect(m_notifyWidget->model(), &NotifyModel::modelReset, this, &NotifyCenterWidget::updateDisplayOfRemainingNotification);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(Notify::CenterMargin, Notify::CenterMargin, 0, 0);
    mainLayout->addWidget(m_headWidget);
    mainLayout->addWidget(m_notifyWidget);
    mainLayout->addWidget(m_expandRemaining);

    setLayout(mainLayout);

    connect(m_clearButton, &CicleIconButton::clicked, this, [this]() {
        m_notifyWidget->model()->removeAllData();
    });

    connect(m_notifyWidget->model(), &NotifyModel::appCountChanged, this, &NotifyCenterWidget::updateDisplayOfRemainingNotification);

    refreshTheme();

    collapesNotificationFolding();

    updateDisplayOfRemainingNotification();
}

void NotifyCenterWidget::initConnections()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &NotifyCenterWidget::refreshTheme);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &NotifyCenterWidget::CompositeChanged, Qt::QueuedConnection);

    connect(m_notifyWidget, &NotifyWidget::focusOnButton, this, [this] {
        qDebug() << "set Focus on clearButton";
        m_clearButton->setFocus();
        m_clearButton->update();
    });
}

void NotifyCenterWidget::refreshTheme()
{
    QPalette pa = title_label->palette();
    pa.setBrush(QPalette::WindowText, pa.brightText());
    title_label->setPalette(pa);
}

void NotifyCenterWidget::CompositeChanged()
{
    m_hasComposite = m_wmHelper->hasComposite();
}

void NotifyCenterWidget::updateDisplayOfRemainingNotification()
{
    const bool hasAppNotification = m_notifyWidget->model()->rowCount() > 0;
    if (!hasAppNotification) {
        m_expandRemaining->setText(tr("No new notifications"));
    } else {
        const int rowCount = m_notifyWidget->model()->remainNotificationCount();
        m_expandRemaining->setText(tr("%1 more notifications").arg(QString::number(rowCount)));
    }
}

void NotifyCenterWidget::expandNotificationFolding()
{
    m_isCollapesNotificationFolding = false;
    m_notifyWidget->model()->expandData();
    m_toggleNotificationFolding->setIcon(DDciIcon::fromTheme("arrow_ordinary_up"));
    Q_EMIT notificationFoldingChanged(m_isCollapesNotificationFolding);
    m_expandRemaining->hide();
    m_toggleNotificationFolding->show();
}

void NotifyCenterWidget::collapesNotificationFolding()
{
    m_isCollapesNotificationFolding = true;
    m_notifyWidget->model()->collapseData();
    m_toggleNotificationFolding->setIcon(DDciIcon::fromTheme("arrow_ordinary_down"));
    Q_EMIT notificationFoldingChanged(m_isCollapesNotificationFolding);
    m_expandRemaining->show();
    m_toggleNotificationFolding->hide();
}

void NotifyCenterWidget::toggleNotificationFolding()
{
    if (m_isCollapesNotificationFolding) {
        expandNotificationFolding();
    } else {
        collapesNotificationFolding();
    }
}

void NotifyCenterWidget::showNotificationModuleOfControlCenter()
{
    Helper::instance()->showNotificationModuleOfControlCenter();
}

void NotifyCenterWidget::showSettingMenu()
{
    QMenu *menu = new QMenu(this);
    do {
        QAction *action = menu->addAction(tr("Notification settings"));
        action->setCheckable(true);

        connect(action, &QAction::triggered, this, &NotifyCenterWidget::showNotificationModuleOfControlCenter);
    } while (false);

    menu->exec(QCursor::pos());
    menu->deleteLater();
}
