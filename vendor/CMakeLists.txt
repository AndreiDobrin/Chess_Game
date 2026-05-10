add_library(sqlite STATIC sqlite/sqlite3.c)
target_include_directories(sqlite PUBLIC sqlite)

# Force static libraries for SDL to avoid DLL missing errors (0xC0000135)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(SDL_IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL_IMAGE_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_IMAGE_STATIC ON CACHE BOOL "" FORCE)

set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
set(SDLTTF_PLUTOSVG OFF CACHE BOOL "" FORCE)
set(SDL_TTF_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_TTF_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)


add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
        SDL3::SDL3-static
        sqlite
        SDL3_image::SDL3_image-static
        SDL3_ttf::SDL3_ttf-static
)