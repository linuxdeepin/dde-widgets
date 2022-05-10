find_package(Qt5 COMPONENTS Core Gui Widgets)

include_directories(/usr/include/dde-widgets)

set(COMMON_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
link_libraries(dde-widgets ${COMMON_LIBS})

function(install_dde_widgets_plugin plugin)
    install(TARGETS ${plugin} DESTINATION /usr/lib/dde-widgets/plugins)
endfunction()
