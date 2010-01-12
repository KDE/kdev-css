
if(KDEVPGQT_FOUND)
     kdevpgqt_generate(_kdevpgList css NAMESPACE Css DEBUG_VISITOR TOKEN_TEXT
        "${css_SOURCE_DIR}/parser/css.g"
        "${css_SOURCE_DIR}/parser/tokenizer.h"
    )
    set( parser_SRCS
       ${_kdevpgList}
   )
else(KDEVPGQT_FOUND)
   message(STATUS "Assuming existence of generated parser files")
    set(parser_SRCS
        generated/cssparser.cpp
        generated/cssvisitor.cpp
        generated/cssdefaultvisitor.cpp )
endif(KDEVPGQT_FOUND)


find_package(Flex)

if(FLEX_FOUND)
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
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/cssparser.h"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    -o"tokenizer.cpp"
                "tokenizer.flex"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
else(FLEX_FOUND)
    # If flex is not available, copy the pre-generated lexer from SVN.
    add_custom_command(
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/generated/tokenizer.cpp"
        COMMAND ${CMAKE_COMMAND}  ARGS -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/generated/tokenizer.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp"
        COMMENT "flex is not installed, using the pre-generated lexer from SVN."
    )
endif(FLEX_FOUND)


set(parser_STAT_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp
    parsesession.cpp
    editorintegrator.cpp
    )

### next target
kde4_add_library( kdev4cssparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries( kdev4cssparser
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
)
install(TARGETS kdev4cssparser DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
kde4_add_executable( css-parser main.cpp ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries(css-parser ${QT_QTCORE_LIBRARY} ${KDEVPLATFORM_LANGUAGE_LIBRARIES})

### next target
set(parsertest_SRCS test/parsertest.cpp)
kde4_add_unit_test(parsertest ${parsertest_SRCS})
target_link_libraries(parsertest kdev4cssparser ${QT_QTTEST_LIBRARY})


add_custom_target( copy-generated
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssparser.cpp" "${css_SOURCE_DIR}/parser/generated/cssparser.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssparser.h" "${css_SOURCE_DIR}/parser/generated/cssparser.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssast.h" "${css_SOURCE_DIR}/parser/generated/cssast.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssvisitor.h" "${css_SOURCE_DIR}/parser/generated/cssvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssvisitor.cpp" "${css_SOURCE_DIR}/parser/generated/cssvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssdefaultvisitor.h" "${css_SOURCE_DIR}/parser/generated/cssdefaultvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssdefaultvisitor.cpp" "${css_SOURCE_DIR}/parser/generated/cssdefaultvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/cssdebugvisitor.h" "${css_SOURCE_DIR}/parser/generated/cssdebugvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${css_BINARY_DIR}/parser/tokenizer.cpp" "${css_SOURCE_DIR}/parser/generated/tokenizer.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssparser.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssparser.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssvisitor.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssvisitor.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssdefaultvisitor.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssdefaultvisitor.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssast.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssdebugvisitor.h"
    DEPENDS "${css_BINARY_DIR}/parser/tokenizer.cpp"
    )
