kdevpgqt_generate(_kdevpgList css NAMESPACE Css DEBUG_VISITOR TOKEN_TEXT
    "${css_SOURCE_DIR}/parser/css.g"
    "${css_SOURCE_DIR}/parser/tokenizer.h"
)
set( parser_SRCS
    ${_kdevpgList}
)

# Copy tokenizer.ll to the builddir, so that flex doesn't write out
# absolute paths in the generated file when we pass them as arguments.
# In short, I don't want stuff like
# '#line 2 "/home/kde/build/.../tokenizer.cpp" in SVN.
add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.flex"
    COMMAND ${CMAKE_COMMAND}  ARGS -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.flex"
            "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
)
set_source_files_properties(
    "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    GENERATED
)
# Add command to generate the lexer.
add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    COMMAND ${FLEX_EXECUTABLE}
    ARGS --nounistd -o"tokenizer.cpp" "tokenizer.flex"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(parser_STAT_SRCS
    ${kdevcss_LOG_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp
    parsesession.cpp
    editorintegrator.cpp
    htmlparser.cpp
)

### next target
add_library(kdev4cssparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
generate_export_header(kdev4cssparser EXPORT_MACRO_NAME KDEVCSSPARSER_EXPORT
                                       EXPORT_FILE_NAME parserexport.h)
target_link_libraries( kdev4cssparser LINK_PRIVATE
    KDev::Language
)
install(TARGETS kdev4cssparser DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_TESTING)
    ### next target
    if(WIN32)
        add_executable(css-parser main.cpp)
    else()
        add_executable(css-parser ${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp main.cpp)
    endif()
    target_link_libraries(css-parser
        Qt5::Test
        KDev::Language
        KDev::Tests
        kdev4cssparser
    )

    ### next target
    set(parsertest_SRCS test/parsertest.cpp
      # TODO: extra library!
        ${kdevcss_LOG_SRCS}
      ../duchain/tests/duchaintestbase.cpp
      ../duchain/builders/contextbuilder.cpp
      ../duchain/builders/declarationbuilder.cpp)
    ecm_add_test(${parsertest_SRCS}
        TEST_NAME parsertest
        LINK_LIBRARIES
            Qt5::Test
            kdev4cssparser
            KDev::Language
            KDev::Tests
    )
endif()
