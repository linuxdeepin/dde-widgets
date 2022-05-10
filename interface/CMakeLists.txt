PROJECT(interface)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(REQUIRED_QT_VERSION 5.11.3)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOMOC_COMPILER_PREDEFINES ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

file(GLOB INTERFACES
    widgetsglobal.h
    widgetsinterface.h)

set(HEADERS
    ${INTERFACES}
    widgetsinterface_p.h
)
set(SOURCES
    widgetsinterface.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME dde-widgets)

set(COMMON_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${COMMON_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE})

install(FILES ${INTERFACES}
    DESTINATION include/dde-widgets)

install(FILES "DdeWidgetsConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DdeWidgets)

