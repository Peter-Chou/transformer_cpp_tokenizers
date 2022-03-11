# Enable modules to include each other's files
include_directories(.)

add_subdirectory(bert)

add_executable(main
    main.cc)

target_link_libraries(main PUBLIC
    bert_tokenizer
    ${ICU_LIBRARIES}
    )
target_include_directories(main PUBLIC ${ICU_INCLUDE_DIRS})
