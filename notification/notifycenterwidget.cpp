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

#include <QDesktopWidget>
#include <QBoxLayout>
#include <QDBusInterface>
#include <QPalette>
#include <QDebug>
#include <QTimer>
#include <QScreen>

#include <DIconButton>
#include <DLabel>
#include <DFontSizeManager>
#include <DGuiApplicationHelper>

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
    m_headWidget->setFixedSize(Notify::CenterWidth - 2 * Notify::CenterMargin, Notify::CenterTitleHeight);

    DIconButton *bell_notify = new DIconButton(m_headWidget);
    bell_notify->setAccessibleName("BellNotifyBtn");
    bell_notify->setFlat(true);
    bell_notify->setIconSize(QSize(32, 32));
    bell_notify->setFixedSize(32, 32);
    const auto ratio = devicePixelRatioF();
    QIcon icon_pix = QIcon::fromTheme("://icons/notifications.svg").pixmap(bell_notify->iconSize() * ratio);
    bell_notify->setIcon(icon_pix);
    bell_notify->setFocusPolicy(Qt::NoFocus);

    title_label = new DLabel;
    title_label->setAccessibleName("TitleLabel");
    QFont font = title_label->font();
    font.setBold(true);
    font.setWeight(QFont::DemiBold);
    title_label->setFont(font);
    title_label->setText(tr("Notification Center"));
    title_label->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
    title_label->setForegroundRole(QPalette::BrightText);
    DFontSizeManager::instance()->bind(title_label, DFontSizeManager::T5);

    m_clearButton = new IconButton;
    m_clearButton->setAccessibleName("ClearButton");
    m_clearButton->setOpacity(IconButton::RELEASE, 255 * 0.0);
    m_clearButton->setRadius(Notify::CenterTitleHeight / 2);
    m_clearButton->setFixedSize(Notify::CenterTitleHeight, Notify::CenterTitleHeight);
    m_clearButton->setFocusPolicy(Qt::StrongFocus);

    QHBoxLayout *head_Layout = new QHBoxLayout;
    head_Layout->addWidget(bell_notify, Qt::AlignLeft | Qt::AlignTop);
    head_Layout->setMargin(0);
    head_Layout->addStretch();
    head_Layout->addWidget(title_label, Qt::AlignCenter | Qt::AlignBottom);
    head_Layout->addStretch();
    head_Layout->addWidget(m_clearButton, Qt::AlignRight | Qt::AlignTop);
    m_headWidget->setLayout(head_Layout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(Notify::CenterMargin, Notify::CenterMargin, 0, 0);
    mainLayout->addWidget(m_headWidget);
    mainLayout->addWidget(m_notifyWidget);

    setLayout(mainLayout);

    connect(m_clearButton, &IconButton::clicked, this, [ = ]() {
        m_notifyWidget->model()->removeAllData();
    });

    refreshTheme();
}

void NotifyCenterWidget::initConnections()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &NotifyCenterWidget::refreshTheme);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &NotifyCenterWidget::CompositeChanged, Qt::QueuedConnection);

    connect(m_notifyWidget, &NotifyWidget::focusOnButton, this, [=] {
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
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        m_clearButton->setIcon("://icons/list_icon_clear.svg");
        m_clearButton->setOpacity(IconButton::HOVER, 255 * 0.2);
        m_clearButton->setOpacity(IconButton::PRESS, 255 * 0.3);
    } else {
        m_clearButton->setIcon("://icons/list_icon_clear_dark.svg");
        m_clearButton->setOpacity(IconButton::HOVER, 255 * 0.2);
        m_clearButton->setOpacity(IconButton::PRESS, 255 * 0.1);
    }
}


void NotifyCenterWidget::CompositeChanged()
{
    m_hasComposite = m_wmHelper->hasComposite();
}
