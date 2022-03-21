add_library(basic_tokenizers OBJECT
    basic_tokenizer.h
    basic_tokenizer.cc
    wordpiece_tokenizer.h
    wordpiece_tokenizer.cc
    )

target_link_libraries(basic_tokenizers PUBLIC
    utils
    ${ICU_LIBRARIES}
    )
target_include_directories(basic_tokenizers PUBLIC ${ICU_INCLUDE_DIRS})
