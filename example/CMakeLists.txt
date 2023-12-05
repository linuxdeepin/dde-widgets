# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(dde-examplewidget-plugin)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(REQUIRED_QT_VERSION 6.0.0)
find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets)

find_package(DdeWidgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOMOC_COMPILER_PREDEFINES ON)

SET(LIBRARY_OUTPUT_PATH ../app/plugins)
add_library(${PROJECT_NAME} SHARED plugin.h plugin.cpp)

set(COMMON_LIBS
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${DdeWidgets_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${DdeWidgets_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${COMMON_LIBS})

install_dde_widgets_plugin(${PROJECT_NAME})
