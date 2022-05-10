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

#include <QtGlobal>

static const char *ExamplePluginId = "org.deepin.dde.widgets.ExampleCommonDate";

class EnvGuard {
public:
    void set(const char *name, const QByteArray &value)
    {
        m_name = name;
        m_originValue = qgetenv(m_name);
        qputenv(m_name, value);
    }
    void restore()
    {
        qputenv(m_name, m_originValue);
    }
    QString value()
    {
        return qgetenv(m_name);
    }
private:
    QByteArray m_originValue;
    const char* m_name = nullptr;
};

struct PluginGuard {
    PluginGuard() {
        static const char *ExamplePluginPath = "../app/plugins";
        guard.set("DDE_WIDGETS_PLUGIN_DIRS", ExamplePluginPath);
    }
    ~PluginGuard() {
        guard.restore();
    }
    EnvGuard guard;
};
