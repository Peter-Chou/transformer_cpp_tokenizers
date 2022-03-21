add_library(fundamental_tokenizer OBJECT
    fundamental_tokenizer.h
    fundamental_tokenizer.cc
    )

target_link_libraries(fundamental_tokenizer PUBLIC
    utils
    ${ICU_LIBRARIES}
    )
target_include_directories(fundamental_tokenizer PUBLIC ${ICU_INCLUDE_DIRS})

set_target_properties(fundamental_tokenizer PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
