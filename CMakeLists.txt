cmake_minimum_required(VERSION 3.27)

project(venice-qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Widgets Bluetooth Core Network)

# Find Protobuf
find_package(Protobuf REQUIRED)

add_definitions(-DQT_DEPRECATED_WARNINGS)



# Source files
set(SOURCES
    src/main.cpp
    src/channel/channel.cpp
    src/channel/datachannel.cpp
    src/exception/errorstartingfiletransferserviceveniceexception.cpp
    external/protobuf/cpp_proto/venice.pb.cc
    src/sender/filetransferserviceprovider.cpp
    src/sender/filesender.cpp
    src/ui/mainwindow.cpp
    src/exception/notavailableportfoundveniceexception.cpp
    src/exception/notbluetoothadapterfoundveniceexception.cpp
    src/exception/notsuitablewifiadapterfoundveniceexception.cpp
    src/sender/venicebluetoothuuid.cpp
    src/exception/veniceexception.cpp
    src/network/venicemessage.cpp
    src/event/venicetimer.cpp
    src/channel/wifidatachannel.cpp
    src/channel/blebootstrapchannel.cpp
    src/channel/bootstrapchannel.cpp
    src/tool/terminallanguagemanager.cpp
    src/tool/networkinformationmanager.cpp
    src/receiver/filetransferservicediscoverer.cpp
    src/receiver/filereceiver.cpp
    src/metadata/channelmetadata.cpp
    src/metadata/filemetadata.cpp
)

set(HEADERS
    include/ui/mainwindow.h
    include/sender/filetransferserviceprovider.h
)

qt6_wrap_cpp(MOC_SOURCES ${HEADERS})

# Generate H files for UI file by indicating their path
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
    Qt6::Widgets
    Qt6::Bluetooth
    Qt6::Core
    Qt6::Network
    # QT6::Protobuf
    ${Protobuf_LIBRARIES}
)
