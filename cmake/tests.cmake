function(add_qtest)
    #yet again: AAndaluz, you rock!!!!!

    set(options OPTIONAL "")
    set(oneValueArgs ID SRC_FOLDER)
    set(multiValueArgs SOURCES TARGET_LINK)

    cmake_parse_arguments(QTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(TRANSFORM QTEST_SOURCES PREPEND ${CMAKE_SOURCE_DIR}/${QTEST_SRC_FOLDER}/)

    set(TARGET_NAME ${QTEST_ID})

    add_executable(${TARGET_NAME} "${QTEST_SOURCES}")

    target_include_directories(${TARGET_NAME} PUBLIC
            ${ROOT_INCLUDE_DIR}
            ${QT_TEST_INCLUDES}
            ${QT_INCLUDES})

    target_link_libraries(${TARGET_NAME}
            mainlib
            ${QT_LIBS}
            ${QT_TEST_LIB})

    add_test(NAME ${TARGET_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${TARGET_NAME}")
endfunction()