set(Qt5_VERSION "5.15.0")
set(Qt5_DIR ${DEPS_DIR}/Qt/${Qt5_VERSION}/gcc_64/lib/cmake/Qt5)
if (NOT IS_DIRECTORY ${Qt5_DIR})
    message(FATAL_ERROR "A valid Qt 5 version could not be found under: ${DEPS_DIR}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia Test REQUIRED)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
set(QT_TEST_INCLUDES ${Qt5Test_INCLUDE_DIRS})

set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia)
set(QT_TEST_LIB Qt5::Test)
