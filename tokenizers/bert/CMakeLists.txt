add_library(bert_tokenizer OBJECT
    bert_tokenizer.h
    bert_tokenizer.cc
    )

target_link_libraries(bert_tokenizer PUBLIC
    fundamental_tokenizer
    )
