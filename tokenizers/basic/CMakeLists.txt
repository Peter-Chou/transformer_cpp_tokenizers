add_library(basic_tokenizers
    basic_tokenizer.h
    basic_tokenizer.cc
    wordpiece_tokenizer.h
    wordpiece_tokenizer.cc
    )
target_link_libraries(basic_tokenizers PUBLIC
    tokenizers::utils
    )
target_include_directories(basic_tokenizers PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS basic_tokenizers
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::basic_tokenizers ALIAS basic_tokenizers)

## test
tokenizers_cc_test(
    NAME
    basic_tokenizer_test
    SRCS
    "basic_tokenizer_test.cc"
    DEPS
    tokenizers::basic_tokenizers
    GTest::gmock_main
    )

tokenizers_cc_test(
    NAME
    wordpiece_tokenizer_test
    SRCS
    "wordpiece_tokenizer_test.cc"
    DEPS
    tokenizers::basic_tokenizers
    GTest::gmock_main
    )
