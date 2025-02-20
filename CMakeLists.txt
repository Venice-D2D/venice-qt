cmake_minimum_required(VERSION 3.16)
project(venice-qt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Bluetooth Core Network REQUIRED)
find_package(Protobuf REQUIRED)

add_definitions(-DQT_DEPRECATED_WARNINGS)

add_executable(${PROJECT_NAME}
    main.cpp
    datachannel.cpp
    errorstartingfiletransferserviceveniceexception.cpp
    external/protobuf/cpp_proto/venice.pb.cc
    filetransferservice.cpp
    mainwindow.cpp
    notavailableportfoundveniceexception.cpp
    notbluetoothadapterfoundveniceexception.cpp
    notsuitablewifiadapterfoundveniceexception.cpp
    venicebluetoothuuid.cpp
    veniceexception.cpp
    venicemessage.cpp
    veniceservice.cpp
    venicetimer.cpp
    wifidatachannel.cpp
)

# Headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets 
    Qt5::Bluetooth 
    Qt5::Core 
    Qt5::Network 
    ${Protobuf_LIBRARIES}
)
