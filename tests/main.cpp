/*
 * Copyright (C) 2021 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <QApplication>
#include <QTimer>
/**
  添加Qt事件循环,兼容gtest.
 */
int runTest(QCoreApplication &app)
{
    int ret = 0;
    QTimer::singleShot(0, &app, [&app, &ret](){
        ret = RUN_ALL_TESTS();
        app.quit();
    });
    app.exec();
    return ret;
}

int main(int argc, char *argv[])
{
    // gerrit编译时没有显示器，需要指定环境变量
    qputenv("QT_QPA_PLATFORM", "offscreen");

    QApplication app(argc, argv);
    testing::InitGoogleTest(&argc, argv);

    return runTest(app);
}
