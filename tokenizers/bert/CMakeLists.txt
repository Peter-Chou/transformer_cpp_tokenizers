add_library(bert_tokenizer STATIC
    bert_tokenizer.h
    bert_tokenizer.cc
    )

target_link_libraries(bert_tokenizer PUBLIC
    fundamental_tokenizer
    basic_tokenizers
    utils
    unilib
    )

target_include_directories(bert_tokenizer PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tokenizers>
    $<INSTALL_INTERFACE:include>)
