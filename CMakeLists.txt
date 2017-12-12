cmake_minimum_required(VERSION 3.0)
project(css)

set(VERSION_MAJOR 4)
set(VERSION_MINOR 90)
set(VERSION_PATCH 90)

find_package (ECM "5.14.0" REQUIRED NO_MODULE)
message(STATUS "${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
message(STATUS "${CMAKE_MODULE_PATH}")

include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(GenerateExportHeader)
include(FeatureSummary)

set(QT_MIN_VERSION "5.5.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets Test)
set(KF5_DEP_VERSION "5.15.0")
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS
    I18n
    ItemModels # needed because missing in KDevPlatformConfig.cmake, remove once dep on kdevplatform >=5.2.2
    TextEditor
)
set(KDEVPLATFORM_DEP_VERSION "5.1.0")
find_package(KDevPlatform ${KDEVPLATFORM_DEP_VERSION} CONFIG)
set_package_properties(KDevPlatform PROPERTIES
    TYPE REQUIRED
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

find_package(KDevelop-PG-Qt CONFIG)
set_package_properties(KDevelop-PG-Qt PROPERTIES
    TYPE REQUIRED
)

find_package(FLEX)
set_package_properties(FLEX PROPERTIES
    TYPE REQUIRED
)

add_definitions(-DTRANSLATION_DOMAIN=\"kdevcss\")

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} # debug.h
    ${CMAKE_CURRENT_BINARY_DIR}/parser
    ${FLEX_INCLUDE_DIRS}
    ${KDEVPGQT_INCLUDE_DIR}
)

ecm_qt_declare_logging_category(kdevcss_LOG_SRCS
    HEADER debug.h
    IDENTIFIER KDEV_CSS
    CATEGORY_NAME "kdevelop.languages.css"
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
    ${kdevcss_LOG_SRCS}
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
)

# kdebugsettings file
install(FILES kdevcsssupport.categories DESTINATION ${KDE_INSTALL_CONFDIR})

### next target
set(completionmodeltest_SRCS
    completion/test/modeltest.cpp
    completion/model.cpp
    completion/contentassistdata.cpp
    ##TODO: put this into a shared lib
    duchain/tests/duchaintestbase.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
    ${kdevcss_LOG_SRCS}
)

qt5_add_resources(completionmodeltest_SRCS kdevcssresources.qrc)

ecm_add_test(${completionmodeltest_SRCS}
    TEST_NAME completionmodeltest
    LINK_LIBRARIES
        Qt5::Test
        kdev4cssparser
        KDev::Interfaces KDev::Language KDev::Tests
        KF5::I18n KF5::TextEditor
)

### next target
set(duchaintest_SRCS
    duchain/tests/duchain.cpp
    duchain/tests/duchaintestbase.cpp
    duchain/builders/contextbuilder.cpp
    duchain/builders/declarationbuilder.cpp
    ${kdevcss_LOG_SRCS}
)
ecm_add_test(${duchaintest_SRCS}
    TEST_NAME duchaintest
    LINK_LIBRARIES
        kdev4cssparser
        Qt5::Test
        KDev::Interfaces KDev::Language KDev::Tests
        KF5::I18n KF5::TextEditor
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
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
