project(css)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 9)
set(VERSION_PATCH 60)
# KDevplatform dependency version
set( KDEVPLATFORM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

cmake_minimum_required(VERSION 2.8.12)
find_package (ECM 1.0.0 REQUIRED NO_MODULE)
message(STATUS "${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
message(STATUS "${CMAKE_MODULE_PATH}")

include(ECMAddTests)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 REQUIRED Core Widgets Test WebKitWidgets)
find_package(KF5 REQUIRED COMPONENTS KDELibs4Support ConfigWidgets KIO I18n KCMUtils TextEditor ThreadWeaver )
find_package(KDevPlatform ${KDEVPLATFORM_VERSION} REQUIRED)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

find_package(KDevelop-PG-Qt REQUIRED)
find_package(FLEX REQUIRED)

include_directories(
    ${CMAKE_BINARY_DIR}/parser
    ${KDEVPGQT_INCLUDE_DIR}
)

add_subdirectory(parser)

set(kdevcss_PART_SRCS
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

qt5_add_resources(kdevcss_PART_SRCS kdevcssresources.qrc)

add_library(kdevcsssupport MODULE ${kdevcss_PART_SRCS})

target_link_libraries(kdevcsssupport
    kdev4cssparser

    KDev::Interfaces
    KDev::Language

    KF5::I18n
    KF5::TextEditor

    # TODO: remove once KDevPlatform moves away from KDELibs4Support.
    KF5::KDELibs4Support
)

install(TARGETS kdevcsssupport DESTINATION ${PLUGIN_INSTALL_DIR}/kdevplatform/${KDEV_PLUGIN_VERSION})

configure_file(kdevcsssupport.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/kdevcsssupport.desktop)
kcoreaddons_desktop_to_json(kdevcsssupport ${CMAKE_CURRENT_BINARY_DIR}/kdevcsssupport.desktop)

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

qt5_add_resources(completionmodeltest_SRCS kdevcssresources.qrc)

ecm_add_test(${completionmodeltest_SRCS}
    TEST_NAME completionmodeltest
    LINK_LIBRARIES
        Qt5::Test
        kdev4cssparser
        KDev::Interfaces KDev::Language KDev::Tests
        KF5::I18n KF5::TextEditor
        # TODO: remove once KDevPlatform moves away from KDELibs4Support.
        KF5::KDELibs4Support
)

### next target
set(duchaintest_SRCS
    duchain/tests/duchain.cpp
    duchain/tests/duchaintestbase.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
)
ecm_add_test(${duchaintest_SRCS}
    TEST_NAME duchaintest
    LINK_LIBRARIES
        kdev4cssparser
        Qt5::Test
        KDev::Interfaces KDev::Language KDev::Tests
        KF5::I18n KF5::TextEditor
        # TODO: remove once KDevPlatform moves away from KDELibs4Support.
        KF5::KDELibs4Support
)

### next target
set(parsejobtest_SRCS
    duchain/tests/parsejobtest.cpp
)
ecm_add_test(${parsejobtest_SRCS}
    TEST_NAME parsejobtest
    LINK_LIBRARIES
        Qt5::Test
        KDev::Interfaces KDev::Language KDev::Tests
        KF5::I18n KF5::TextEditor
        # TODO: remove once KDevPlatform moves away from KDELibs4Support.
        KF5::KDELibs4Support
)
