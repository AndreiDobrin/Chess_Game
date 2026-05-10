add_executable(${PROJECT_NAME}
        Piece.h
        Piece.cpp
        Color.cpp
        Color.h
        Position.cpp
        Position.h
        King.cpp
        King.h
        Board.cpp
        Board.h
        Pawn.cpp
        Pawn.h
        Rook.cpp
        Rook.h
        Knight.cpp
        Knight.h
        Bishop.cpp
        Bishop.h
        BishopMovement.cpp
        BishopMovement.h
        RookMovement.cpp
        RookMovement.h
        Queen.cpp
        Queen.h
        GameDB.cpp
        GameDB.h
        gameEngine.cpp
        gameEngine.h
        Globals.cpp
        Globals.h
        UI.h
        myRectangle.h
        UI.cpp
)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

# Automatizeaza copierea DLL-urilor langa executabil pentru a preveni eroarea 0xC0000135
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND_EXPAND_LISTS
)
