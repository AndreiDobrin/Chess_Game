add_executable(${PROJECT_NAME} main.cpp
        Window.h
        Rectangle.h)
target_sources(${PROJECT_NAME} PRIVATE ../vendor/sqlite/sqlite3.c)
target_include_directories(${PROJECT_NAME} PRIVATE ../vendor/sqlite)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)