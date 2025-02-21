cmake_minimum_required(VERSION 3.29)
project(venice-qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Widgets Bluetooth Core Network)

# Find Protobuf
find_package(Protobuf REQUIRED)

add_definitions(-DQT_DEPRECATED_WARNINGS)



# Source files
set(SOURCES
    src/main.cpp
    src/channel/datachannel.cpp
    src/exception/errorstartingfiletransferserviceveniceexception.cpp
    external/protobuf/cpp_proto/venice.pb.cc
    src/service/filetransferservice.cpp
    src/ui/mainwindow.cpp
    src/exception/notavailableportfoundveniceexception.cpp
    src/exception/notbluetoothadapterfoundveniceexception.cpp
    src/exception/notsuitablewifiadapterfoundveniceexception.cpp
    src/service/venicebluetoothuuid.cpp
    src/exception/veniceexception.cpp
    src/network/venicemessage.cpp
    src/service/veniceservice.cpp
    src/event/venicetimer.cpp
    src/channel/wifidatachannel.cpp
)

set(HEADERS
    include/ui/mainwindow.h
    include/service/veniceservice.h
)

qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

#set(FORMS
#    /home/daniel/Documents/git-repository/d2d/venice_qt/venice-qt/ui/mainwindow.ui
#)

# Generate H files for UI file by indicating their path
#qt5_wrap_ui(UI_HEADERS ${FORMS})
#qt_add_ui_files(UI_HEADERS ${FORMS})
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)


# Add the executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${MOC_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# Link with Qt and Protobuf
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Bluetooth
    Qt5::Core
    Qt5::Network
    ${Protobuf_LIBRARIES}
)
