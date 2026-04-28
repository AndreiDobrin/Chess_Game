add_library(sqlite STATIC sqlite/sqlite3.c)
target_include_directories(sqlite PUBLIC sqlite)


add_subdirectory(SDL)
add_subdirectory(SDL_image)

set(SDLTTF_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
add_subdirectory(SDL_ttf)


add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
        SDL3::SDL3
        sqlite
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)