PROJECT(dde-datewidget-plugin)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(REQUIRED_QT_VERSION 5.11.3)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets)
find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkWidget REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOMOC_COMPILER_PREDEFINES ON)

SET(LIBRARY_OUTPUT_PATH ../app/plugins)
add_library(${PROJECT_NAME} SHARED dateplugin.h dateplugin.cpp)

set(COMMON_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${DtkWidget_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${COMMON_LIBS})

# user don't need the code
include_directories(../interface/)

# user need to replace the code with `install_dde_widgets_plugin(${PROJECT_NAME})`
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib/dde-widgets/plugins)
# find_package(DdeWidgets REQUIRED)
# install_dde_widgets_plugin(${PROJECT_NAME})
