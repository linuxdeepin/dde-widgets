// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DOCKRECT_H
#define DOCKRECT_H

#include <QRect>
#include <QDBusMetaType>

struct DockRect
{
public:
    DockRect();
    operator QRect() const;

    bool operator!=(const DockRect &other);
    bool operator==(const DockRect &other);

    friend QDebug operator<<(QDebug debug, const DockRect &rect);
    friend const QDBusArgument &operator>>(const QDBusArgument &arg, DockRect &rect);
    friend QDBusArgument &operator<<(QDBusArgument &arg, const DockRect &rect);

private:
    int x;
    int y;
    uint w;
    uint h;
};

Q_DECLARE_METATYPE(DockRect)

void registerDockRectMetaType();

#endif // DOCKRECT_H
