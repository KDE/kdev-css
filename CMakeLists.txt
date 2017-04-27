cmake_minimum_required(VERSION 2.8.12)
project(css)

set(VERSION_MAJOR 4)
set(VERSION_MINOR 90)
set(VERSION_PATCH 90)
# KDevplatform dependency version
set( KDEVPLATFORM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

find_package (ECM 1.0.0 REQUIRED NO_MODULE)
message(STATUS "${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
message(STATUS "${CMAKE_MODULE_PATH}")

include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(KDEInstallDirs)
include(KDECMakeSettings)

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
    ${CMAKE_CURRENT_SOURCE_DIR} # debug.h
    ${CMAKE_CURRENT_BINARY_DIR}/parser
    ${KDEVPGQT_INCLUDE_DIR}
)

add_subdirectory(parser)

set(kdevcss_PART_SRCS
    csslanguagesupport.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
    debug.cpp
    parsejob.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
    navigation/fieldnavigationcontext.cpp
    navigation/navigationwidget.cpp
    navigation/valuenavigationcontext.cpp
    navigation/colornavigationcontext.cpp
)

qt5_add_resources(kdevcss_PART_SRCS kdevcssresources.qrc)

kdevplatform_add_plugin(kdevcsssupport JSON kdevcsssupport.json
    SOURCES ${kdevcss_PART_SRCS})
target_link_libraries(kdevcsssupport
    kdev4cssparser

    KDev::Interfaces
    KDev::Language

    KF5::I18n
    KF5::TextEditor

    # TODO: remove once KDevPlatform moves away from KDELibs4Support.
    KF5::KDELibs4Support
)

### next target
set(completionmodeltest_SRCS
    debug.cpp
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
    debug.cpp
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
