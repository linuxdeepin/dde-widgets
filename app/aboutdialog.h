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

#include <DDialog>

DWIDGET_BEGIN_NAMESPACE
class DLabel;
class DTipLabel;
DWIDGET_END_NAMESPACE
DWIDGET_USE_NAMESPACE

WIDGETS_FRAME_BEGIN_NAMESPACE
class InstanceAboutDialog : public DDialog {
    Q_OBJECT
public:
    explicit InstanceAboutDialog(QWidget *parent = nullptr);

    void setLogo(const QIcon &logo);
    void setVersion(const QString &version);
    void setTitle(const QString &title);
    void setDescription(const QString &description);
    void setContributor(const QString &contributor);

private:
    DLabel *m_logo = nullptr;
    DLabel *m_version = nullptr;
    DLabel *m_title = nullptr;
    DTipLabel *m_description = nullptr;
    DLabel *m_contributor = nullptr;
};

WIDGETS_FRAME_END_NAMESPACE
