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

#ifndef NotifyCenterWidget_H
#define NotifyCenterWidget_H

#include "notifywidget.h"
#include "notification/constants.h"

#include <DBlurEffectWidget>
#include <DLabel>
#include <DWindowManagerHelper>
#include <DRegionMonitor>

#include <QTime>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class CicleIconButton;
class TransparentButton;
class AbstractPersistence;
class QVariantAnimation;
class QPropertyAnimation;
class QSequentialAnimationGroup;
class IconButton;
class QPushButton;
/*!
 * \~chinese \class NotifyCenterWidget
 * \~chinese \brief 继承于DBlurEffectWidget,DBlurEffectWidget是DTK库中的类,继承自QWidget.
 * \~chinese 通知中心主窗口类
 */
class NotifyCenterWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit NotifyCenterWidget(AbstractPersistence *database, QWidget* parent = nullptr);

private:
    void initUI();              //初始化主界面
    void initConnections();     //初始化信号槽连接
    void refreshTheme();        //系统主题改变时,刷新主题

Q_SIGNALS:
    void notificationFoldingChanged(bool isExpand);

private Q_SLOTS:
    void CompositeChanged();   //用来设置是否开启窗口特效
    void updateDisplayOfRemainingNotification();
    void updateTabFocus();
    void expandNotificationFolding();
    void collapesNotificationFolding();
    void toggleNotificationFolding();
    void showSettingMenu();
    void showNotificationModuleOfControlCenter();

private:
    QWidget *m_headWidget;
    NotifyWidget *m_notifyWidget;
    DLabel *title_label = nullptr;
    CicleIconButton *m_clearButton = nullptr;
    TransparentButton *m_expandRemaining = nullptr;
    CicleIconButton *m_settingBtn = nullptr;
    CicleIconButton *m_toggleNotificationFolding = nullptr;
    bool m_isCollapesNotificationFolding = true;
    QRect m_notifyRect;
    QRect m_dockRect;
    DWindowManagerHelper *m_wmHelper;
    bool m_hasComposite = false;
    QTimer *m_refreshTimer = nullptr;
    QTime m_tickTime;
    qreal m_scale = 1;
};

#endif // NotifyCenterWidget_H
