PROJECT(css)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${css_SOURCE_DIR}/cmake)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform 1.6.60 REQUIRED)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 60)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

enable_testing()

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(KDevelop-PG-Qt REQUIRED)
find_package(Flex REQUIRED)

include_directories(
    ${CMAKE_BINARY_DIR}/parser
    ${KDEVPGQT_INCLUDE_DIR}
)

add_subdirectory(parser)

set(kdevcsslanguagesupport_PART_SRCS
    csslanguagesupport.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
    parsejob.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
    navigation/fieldnavigationcontext.cpp
    navigation/navigationwidget.cpp
    navigation/valuenavigationcontext.cpp
    navigation/colornavigationcontext.cpp
)

kde4_add_plugin(kdevcsslanguagesupport ${kdevcsslanguagesupport_PART_SRCS})

target_link_libraries(kdevcsslanguagesupport
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_THREADWEAVER_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_THREADWEAVER_LIBRARIES}
    kdev4cssparser
)

install(TARGETS kdevcsslanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

configure_file(kdevcsssupport.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/kdevcsssupport.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdevcsssupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES completion/CSS.xml DESTINATION ${DATA_INSTALL_DIR}/kdevcsssupport/completion)


### next target
set(completionmodeltest_SRCS
    completion/test/modeltest.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
    ##TODO: put this into a shared lib
    duchain/tests/duchaintestbase.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
)
kde4_add_unit_test(completionmodeltest ${completionmodeltest_SRCS})
target_link_libraries(completionmodeltest
    kdev4cssparser
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_TESTS_LIBRARIES}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)


### next target
set(duchaintest_SRCS
    duchain/tests/duchain.cpp
    duchain/tests/duchaintestbase.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
)
kde4_add_unit_test(duchaintest ${duchaintest_SRCS})
target_link_libraries(duchaintest
    kdev4cssparser
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDEVPLATFORM_TESTS_LIBRARIES}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)


### next target
set(parsejobtest_SRCS
    duchain/tests/parsejobtest.cpp
)
kde4_add_unit_test(parsejobtest ${parsejobtest_SRCS})
target_link_libraries(parsejobtest
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_TESTS_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${QT_QTTEST_LIBRARY}
)
