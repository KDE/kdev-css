PROJECT(css)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${css_SOURCE_DIR}/cmake)

enable_testing()

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#Don't error out if the FindXXX are missing for these two
find_package(KDevelop-PG-Qt QUIET)

if(KDEVPGQT_FOUND)
    include_directories(
        ${CMAKE_BINARY_DIR}/parser
        ${KDEVPGQT_INCLUDE_DIR}
    )
else(KDEVPGQT_FOUND)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated/kdevelop-pg-qt
    )
endif(KDEVPGQT_FOUND)

add_subdirectory(parser)

set(kdevcsslanguagesupport_PART_SRCS
    csslanguagesupport.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
)

kde4_add_plugin(kdevcsslanguagesupport ${kdevcsslanguagesupport_PART_SRCS})

target_link_libraries(kdevcsslanguagesupport
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    kdev4cssparser
)

install(TARGETS kdevcsslanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES kdevcsssupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES completion/CSS.xml DESTINATION ${DATA_INSTALL_DIR}/kdevcsssupport/completion)


### next target
set(completionmodeltest_SRCS
    completion/test/modeltest.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
)
kde4_add_unit_test(completionmodeltest ${completionmodeltest_SRCS})
target_link_libraries(completionmodeltest
    kdev4cssparser
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
)
