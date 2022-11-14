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

#include "appearancehandler.h"
#include <QDebug>

#include <QApplication>
#include <QScrollArea>
#include <QScreen>

WIDGETS_FRAME_BEGIN_NAMESPACE
static const QString DBusServie = "org.deepin.dde.Appearance1";
static const QString DBusPath = "/org/deepin/dde/Appearance1";

Appearancehandler::Appearancehandler(QObject *parent)
    : QObject (parent)
{
    m_appearance = new Appearance(DBusServie, DBusPath, QDBusConnection::sessionBus(), this);
}

Appearancehandler::~Appearancehandler()
{
    m_targets.clear();
    if (m_appearance) {
        m_appearance->deleteLater();
        m_appearance = nullptr;
    }
}

void Appearancehandler::addTargetWidget(DBlurEffectWidget *target)
{
    target->setMaskAlpha(aplpha());
    connect(m_appearance, &Appearance::OpacityChanged, target, [this, target](double  value) {
        target->setMaskAlpha(static_cast<quint8>(value * 255));
    });
    m_targets.append(target);
}

int Appearancehandler::aplpha() const
{
    const double value = m_appearance->opacity();
    return static_cast<quint8>(value * 255);
}
WIDGETS_FRAME_END_NAMESPACE
