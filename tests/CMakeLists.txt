add_executable(test_ecs test_ecs.cpp)
target_link_libraries(test_ecs PRIVATE BerkEngine)

add_test(NAME ECSTests COMMAND test_ecs)
