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

#include <DApplicationHelper>
#include <QWidget>
DWIDGET_USE_NAMESPACE

class MemoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(QWidget *parent = nullptr);
    virtual ~MemoryWidget() override;

    void updateMemoryInfo(const QString &memPercent,
                          const QString &swapPercent);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void changeTheme(DApplicationHelper::ColorType themeType);
    void changeFont(const QFont &font);

private:
    QColor summaryColor;
    QFont m_memTxtFont;

    QColor memoryBackgroundColor;
    QColor memoryColor {"#00C5C0"};
    QColor memoryForegroundColor {"#00C5C0"};
    QColor numberColor;
    QColor swapBackgroundColor;
    QColor swapColor {"#FEDF19"};
    QColor swapForegroundColor {"#FEDF19"};

    qreal memoryBackgroundOpacity = 0.1;
    qreal memoryForegroundOpacity = 1.0;
    qreal swapBackgroundOpacity = 0.1;
    qreal swapForegroundOpacity = 1.0;
    const int ringWidth = 6;

    QFont m_memPercentFont;

    QString m_memPercent;
    //交换内存
    QString m_swapPercent;
};
