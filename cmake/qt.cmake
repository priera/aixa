set(Qt5_DIR /home/pedro/Qt/5.12.4/gcc_64/lib/cmake/Qt5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)

set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
set(QT_TEST_INCLUDES ${Qt5Test_INCLUDE_DIRS})

set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
set(QT_TEST_LIB Qt5::Test)
