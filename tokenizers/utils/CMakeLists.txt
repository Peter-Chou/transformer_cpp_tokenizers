add_library(utils OBJECT
    tokenizer_utils.h
    tokenizer_utils.cc
    unistr_utils.h
    )
target_link_libraries(utils PUBLIC
    ${ICU_LIBRARIES}
    tokenizers::lib::unilib
    )
# icu compile difinitions
set_target_properties(utils PROPERTIES
    COMPILE_DEFINITIONS "U_STATIC_IMPLEMENTATION;U_CHARSET_IS_UTF8=1;U_NO_DEFAULT_INCLUDE_UTF_HEADERS=1")
target_include_directories(utils PUBLIC
    "$<BUILD_INTERFACE:${TOKENIZERS_COMMON_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:include>
    )
add_library(tokenizers::utils ALIAS utils)
