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

#include <widgetsinterface.h>
#include <QTranslator>
#include <DStandardPaths>
#include <QCoreApplication>

WIDGETS_BEGIN_NAMESPACE
class BuildinWidgetsHelper {
public:
    inline static BuildinWidgetsHelper *instance()
    {
        static BuildinWidgetsHelper *g_helper = nullptr;
        if (!g_helper)
            g_helper = new BuildinWidgetsHelper();
        return g_helper;
    }

    inline bool loadTranslator(const QString &fileNamePrefix);

    inline QString contributor() const
    {
        return qApp->translate("BuildinWidgetsHelper", "https://www.chinauos.com");
    }
};

bool BuildinWidgetsHelper::loadTranslator(const QString &fileNamePrefix)
{
    const auto &locale = IWidget::userInterfaceLanguage();
    if (!locale.isEmpty()) {
        const QString fileName(fileNamePrefix + locale);
        // translations dirs.
        QStringList dirs { "./translations/" };
        DCORE_USE_NAMESPACE;
        const auto &genDatas = DStandardPaths::standardLocations(
                    QStandardPaths::GenericDataLocation);
        for (const auto & path : qAsConst(genDatas)) {
            dirs << path + "/dde-widgets/translations/";
        }
        auto qtl = new QTranslator();
        for (auto dir : dirs) {
            if (qtl->load(fileName, dir)) {
                qApp->installTranslator(qtl);
                qInfo(dwLog()) << QString("load translation [%1] successful.").arg(fileName);
                return true;
            }
        }
        if (qtl->isEmpty()) {
            qWarning(dwLog()) << QString("load translation [%1] error from those dirs.").arg(fileName)
                              << dirs;
            qtl->deleteLater();
        }
    }
    return false;
}

WIDGETS_END_NAMESPACE
