PROJECT(frame)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(REQUIRED_QT_VERSION 5.11.3)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets DBus Concurrent)
find_package(DtkCore REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkWidget REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOMOC_COMPILER_PREDEFINES ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

include(src.cmake)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${HEADERS} ${SOURCES} ${DBUS_XML})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME dde-widgets)

set(COMMON_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::DBus
    Qt5::Concurrent
    ${DtkWidget_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${COMMON_LIBS})
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../interface)
target_link_libraries(${PROJECT_NAME} PRIVATE dde-widgets)

target_include_directories(${PROJECT_NAME} PUBLIC
    Qt5::Concurrent
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    ${DtkWidget_INCLUDE_DIRS}
    ${DFrameworkDBus_INCLUDE_DIRS}
)

install(FILES services/org.deepin.dde.Widgets.service DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dde-widgets/translations)
