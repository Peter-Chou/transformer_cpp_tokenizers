add_library(basic_tokenizers OBJECT
    basic_tokenizer.h
    basic_tokenizer.cc
    wordpiece_tokenizer.h
    wordpiece_tokenizer.cc
    )
target_link_libraries(basic_tokenizers PUBLIC
    tokenizers::utils
    # ${ICU_LIBRARIES}
    )
# icu compile difinitions
# set_target_properties(basic_tokenizers PROPERTIES
#     COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
target_include_directories(basic_tokenizers PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )
add_library(tokenizers::basic_tokenizers ALIAS basic_tokenizers)
