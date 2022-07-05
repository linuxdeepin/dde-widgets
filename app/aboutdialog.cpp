
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

#include "aboutdialog.h"
#include <DLabel>
#include <QCoreApplication>

WIDGETS_FRAME_BEGIN_NAMESPACE

InstanceAboutDialog::InstanceAboutDialog(QWidget *parent)
    : DDialog(parent)
{
    setMinimumWidth(UI::About::width);
    setContentLayoutContentsMargins(UI::About::margins);

    m_logo = new DLabel();
    addContent(m_logo, Qt::AlignCenter);

    m_title = new DLabel();
    addSpacing(UI::About::titleSpacing);
    addContent(m_title, Qt::AlignCenter);

    m_version = new DLabel();
    addSpacing(UI::About::versionSpacing);
    addContent(m_version, Qt::AlignCenter);

    m_contributor = new DLabel();
    addSpacing(UI::About::contributorSpacing);
    addContent(m_contributor, Qt::AlignCenter);

    m_description = new DLabel();
    addSpacing(UI::About::descriptionSpacing);
    addContent(m_description, Qt::AlignCenter);
}

void InstanceAboutDialog::setLogo(const QIcon &logo)
{
    m_logo->setPixmap(logo.pixmap(UI::About::logoSize));
}

void InstanceAboutDialog::setVersion(const QString &version)
{
    m_version->setText(qApp->translate("InstanceAboutDialog", "version: %1").arg(version));
}

void InstanceAboutDialog::setTitle(const QString &title)
{
    m_title->setText(title);
}

void InstanceAboutDialog::setDescription(const QString &description)
{
    m_description->setText(description);
}

void InstanceAboutDialog::setContributor(const QString &contributor)
{
    m_contributor->setText(qApp->translate("InstanceAboutDialog", "contributor: %1").arg(contributor));
}
WIDGETS_FRAME_END_NAMESPACE
