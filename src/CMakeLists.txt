set(ROOT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(mainlib)

set(TARGET_NAME alsaPlayground)

add_executable(${TARGET_NAME} main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_INCLUDE_DIR} ${QT_INCLUDES})

target_link_libraries(${TARGET_NAME}
        mainlib
        asound
        ${QT_LIBS})

