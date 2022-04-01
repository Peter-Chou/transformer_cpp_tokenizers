package_add_test(test_bert_tokenizer
    test_bert_tokenizer.cc
    ../main.cc
    )
target_link_libraries(test_bert_tokenizer
    tokenizers::tokenizers)
