add_library(utils
    tokenizer_utils.h
    tokenizer_utils.cc
    unistr_utils.h
    )
target_link_libraries(utils PUBLIC
    ${ICU_LIBRARIES}
    tokenizers::lib::unilib
    )
# icu compile difinitions
set_target_properties(utils PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
target_include_directories(utils PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS utils
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::utils ALIAS utils)

## test
tokenizers_cc_test(
    NAME
    tokenizer_utils_test
    SRCS
    "tokenizer_utils_test.cc"
    DEPS
    tokenizers::utils
    GTest::gmock_main
    )
