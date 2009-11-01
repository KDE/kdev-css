PROJECT(css)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${css_SOURCE_DIR}/cmake)

enable_testing()

find_package(KDE4 REQUIRED)

find_package(KDevPlatform REQUIRED)
include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(kdevcsslanguagesupport_PART_SRCS
    csslanguagesupport.cpp
    completion/model.cpp
)

kde4_add_plugin(kdevcsslanguagesupport ${kdevcsslanguagesupport_PART_SRCS})

target_link_libraries(kdevcsslanguagesupport
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
)

install(TARGETS kdevcsslanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES kdevcsssupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})
