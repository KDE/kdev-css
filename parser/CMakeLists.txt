
set(parser_STAT_SRCS
    tokenizer.cpp
    parsesession.cpp
    )

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


### next target
kde4_add_library( kdev4cssparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries( kdev4cssparser
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
)
install(TARGETS kdev4cssparser DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
kde4_add_executable( css-parser main.cpp )
target_link_libraries(css-parser ${QT_QTCORE_LIBRARY} kdev4cssparser)


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
    DEPENDS "${css_BINARY_DIR}/parser/cssparser.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssparser.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssvisitor.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssvisitor.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssdefaultvisitor.cpp"
    DEPENDS "${css_BINARY_DIR}/parser/cssdefaultvisitor.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssast.h"
    DEPENDS "${css_BINARY_DIR}/parser/cssdebugvisitor.h"
    )
