add_library(fundamental_tokenizer
    fundamental_tokenizer.h
    fundamental_tokenizer.cc
    )

target_link_libraries(fundamental_tokenizer PUBLIC
    tokenizers::utils
    )
set_target_properties(fundamental_tokenizer PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
target_include_directories(fundamental_tokenizer PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS fundamental_tokenizer
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::fundamental_tokenizer ALIAS fundamental_tokenizer)

## test
tokenizers_cc_test(
    NAME
    fundamental_tokenizer_test
    SRCS
    "fundamental_tokenizer_test.cc"
    DEPS
    tokenizers::fundamental_tokenizer
    GTest::gmock_main
    )
