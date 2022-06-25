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
#include <QObject>
#include <DBlurEffectWidget>

#include <com_deepin_daemon_appearance.h>
WIDGETS_FRAME_BEGIN_NAMESPACE
DWIDGET_USE_NAMESPACE
using Appearance = com::deepin::daemon::Appearance;

class Appearancehandler : public QObject {
    Q_OBJECT
public:
    explicit Appearancehandler(QObject *parent = nullptr);
    virtual ~Appearancehandler() override;

    void addTargetWidget(DBlurEffectWidget *target);

    int aplpha() const;

private:
    Appearance *m_appearance = nullptr;
    QList<QPointer<QWidget>> m_targets;
};
WIDGETS_FRAME_END_NAMESPACE
