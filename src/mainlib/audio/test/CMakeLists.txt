set(TARGET_NAME SineGenerator_UTest)

add_executable(${TARGET_NAME} SineGenerator_UTest.cpp SineGenerator_UTest.h)

target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_INCLUDE_DIR} ${QT_TEST_INCLUDES} ${QT_INCLUDES})

target_link_libraries(${TARGET_NAME}
        mainlib
        ${QT_LIBS}
        ${QT_TEST_LIB})

add_test(${TARGET_NAME} ${TARGET_NAME})