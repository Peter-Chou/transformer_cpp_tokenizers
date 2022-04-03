add_library(fundamental_tokenizer OBJECT
    fundamental_tokenizer.h
    fundamental_tokenizer.cc
    )

target_link_libraries(fundamental_tokenizer PUBLIC
    tokenizers::utils
    # ${ICU_LIBRARIES}
    )
set_target_properties(fundamental_tokenizer PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
target_include_directories(fundamental_tokenizer PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )
add_library(tokenizers::fundamental_tokenizer ALIAS fundamental_tokenizer)
