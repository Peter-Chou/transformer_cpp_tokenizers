add_library(bert_tokenizer
    bert_tokenizer.h
    bert_tokenizer.cc
    )
target_link_libraries(bert_tokenizer PUBLIC
    fundamental_tokenizer
    basic_tokenizers
    unilib
    utils
    )
target_include_directories(bert_tokenizer PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

if(TOKENIZERS_ENABLE_INSTALL)
    install(TARGETS bert_tokenizer
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

add_library(tokenizers::bert_tokenizer ALIAS bert_tokenizer)

## test
tokenizers_cc_test(
    NAME
    bert_tokenizer_test
    SRCS
    "bert_tokenizer_test.cc"
    DEPS
    tokenizers::bert_tokenizer
    GTest::gmock_main
    )
