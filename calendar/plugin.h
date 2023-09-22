// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <widgetsinterface.h>

WIDGETS_USE_NAMESPACE
namespace dwclock {
class CalendarWidget : public QObject, public IWidget {
public:
    virtual QWidget *view() override;
    virtual bool initialize(const QStringList &arguments) override;

public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    static void showDDECalendar();
};

class CalendarWidgetPlugin : public IWidgetPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DdeWidgetsPlugin_iid FILE "plugin.json")
    Q_INTERFACES(WIDGETS_NAMESPACE::IWidgetPlugin)

public:
    QString title() const override;
    virtual QString description() const override;
    virtual QString aboutDescription() const override;
    IWidget *createWidget() override;
    virtual QVector<IWidget::Type> supportTypes() const { return {IWidget::Large};}

    virtual QIcon logo() const override;
    virtual QStringList contributors() const override;
};
}
