set(Qt_VERSION "5.15.0")

if (${UNIX})
    set(INNER_Qt_PLATFORM gcc_64)
elseif(${MINGW})
    set(INNER_Qt_PLATFORM mingw81_64)
else()
    message(FATAL_ERROR "Not using a supported Qt platform")
endif()

set(Qt_PLATFORM ${INNER_Qt_PLATFORM} CACHE STRING "Platform used in Qt installation")
set(Qt5_INSTALLATION ${DEPS_DIR}/Qt)
set(Qt5_BIN_DIR ${Qt5_INSTALLATION}/${Qt_VERSION}/${Qt_PLATFORM}/bin)
set(Qt5_DIR ${Qt5_INSTALLATION}/${Qt_VERSION}/${Qt_PLATFORM}/lib/cmake/Qt5)

if (NOT IS_DIRECTORY ${Qt5_DIR})
    message(FATAL_ERROR "Required CMake files for Qt could not be found on: ${Qt5_DIR}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia Test REQUIRED)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
set(QT_TEST_INCLUDES ${Qt5Test_INCLUDE_DIRS})

set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia)
set(QT_TEST_LIB Qt5::Test)
