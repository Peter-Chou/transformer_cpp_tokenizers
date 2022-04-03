add_library(bert_tokenizer STATIC
    bert_tokenizer.h
    bert_tokenizer.cc
    )
target_link_libraries(bert_tokenizer PUBLIC
    tokenizers::fundamental_tokenizer
    tokenizers::basic_tokenizers
    tokenizers::lib::unilib
    tokenizers::utils
    )
target_include_directories(bert_tokenizer PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )
add_library(tokenizers::bert_tokenizer ALIAS bert_tokenizer)

tokenizers_cc_test(
    NAME
    bert_tokenizer_test
    SRCS
    "bert_tokenizer_test.cc"
    DEPS
    tokenizers::bert_tokenizer
    GTest::gmock_main
    )
