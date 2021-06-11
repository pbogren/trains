# General
cmake_minimum_required(VERSION 3.12)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/CompilerOptions.cmake")
project(project-dt060g-pabo1800 VERSION 0.1 LANGUAGES CXX)

#
# Global
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include what you use.
if (NOT MSVC)
	find_program(iwyu_path NAMES include-what-you-use iwyu)
	if(NOT iwyu_path)
		message(WARNING "Could not find the program include-what-you-use")
	else()
		set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
	endif()
endif()

if (MSVC)
    add_compile_options(/W4 /w14640 /permissive-)
elseif (GNU)
    add_compile_options(-Wall -Wextra -Wshadow -Wpedantic -Wnon-virtual-dtor)
elseif(Clang)
    add_compile_options(-Weverything -Wno-c++98-compat)
elseif(AppleClang)
endif()
set(include_path "${CMAKE_SOURCE_DIR}/include")

configure_file(
${CMAKE_SOURCE_DIR}/Trains.txt
${CMAKE_CURRENT_BINARY_DIR}/Trains.txt COPYONLY)
configure_file(
${CMAKE_SOURCE_DIR}/TrainStations.txt
${CMAKE_CURRENT_BINARY_DIR}/TrainStations.txt COPYONLY)
configure_file(
${CMAKE_SOURCE_DIR}/TrainMap.txt
${CMAKE_CURRENT_BINARY_DIR}/TrainMap.txt COPYONLY)

add_library(string_funcs
    src/string_funcs.cpp)
target_include_directories(string_funcs
    PUBLIC ${include_path})

add_library(path
    src/path.cpp)
target_include_directories(path
    PUBLIC ${include_path})
target_compile_features(path
    PRIVATE cxx_std_17)

add_library(vehicle_interface
    src/vehicle.cpp
    src/vehicle_type.cpp)
target_compile_features(vehicle_interface
    PUBLIC cxx_std_17)
target_include_directories(vehicle_interface
    PUBLIC ${include_path})

add_library(vehicles
    src/coach.cpp
    src/sleeping_car.cpp
    src/open_freight_car.cpp
    src/covered_freight_car.cpp
    src/electric_locomotive.cpp
    src/diesel_locomotive.cpp
    src/vehicle_factory.cpp)
target_compile_features(vehicles
    PUBLIC cxx_std_17)
target_link_libraries(vehicles
    PUBLIC vehicle_interface)
if (Clang OR GNU)
    target_link_libraries(vehicles
        PRIVATE -fsanitize=address,leak,undefined --coverage)
    target_compile_options(vehicles
        PRIVATE -O1 -fno-omit-frame-pointer --coverage)
endif()
target_include_directories(vehicles
    PUBLIC "${include_path}")

add_library(train
    src/train_connection.cpp
    src/train.cpp)
target_compile_features(train
    PUBLIC cxx_std_17)
target_link_libraries(train
    PUBLIC vehicle_interface)
if(Clang OR GNU)
target_link_libraries(train
    PRIVATE -fsanitize=address,leak,undefined --coverage)
target_compile_options(train
    PRIVATE -O1 -fno-omit-frame-pointer --coverage)
endif()
target_include_directories(train
    PUBLIC ${include_path})

add_library(time_point
    src/time_point.cpp)
target_compile_features(time_point
    PUBLIC cxx_std_17)
# target_link_libraries(time_point
#     PRIVATE --coverage)
# target_compile_options(time_point
#     PRIVATE --coverage)
target_include_directories(time_point
    PUBLIC ${include_path})

add_library(station
    src/station.cpp)
target_compile_features(station
    PUBLIC cxx_std_17)
target_link_libraries(station
    vehicles string_funcs)
target_include_directories(station
    PUBLIC ${include_path})

add_library(events
    src/events/event.cpp
    src/events/start_event.cpp
    src/events/assembly_event.cpp
    src/events/ready_event.cpp
    src/events/departure_event.cpp
    src/events/arrival_event.cpp
    src/events/disassembly_event.cpp)
target_compile_features(events
    PUBLIC cxx_std_17)
target_include_directories(events
    PUBLIC ${include_path}/events)
target_link_libraries(events
    PUBLIC time_point)
if (Clang OR GNU)
    target_link_libraries(events
        PRIVATE -fsanitize=address,leak,undefined --coverage)
    target_compile_options(events
        PRIVATE -O1 -fno-omit-frame-pointer --coverage)
endif()


add_library(simulator
    src/simulator.cpp)
target_compile_features(simulator
    PUBLIC cxx_std_17)
target_include_directories(simulator
    PUBLIC ${include_path})
target_link_libraries(simulator
    PUBLIC time_point events)
if (Clang OR GNU)
    target_link_libraries(simulator
        PRIVATE -fsanitize=address,leak,undefined --coverage)
    target_compile_options(simulator
        PRIVATE -O1 -fno-omit-frame-pointer --coverage)
endif()

add_library(dispatcher
    src/train_dispatcher.cpp)
target_compile_features(dispatcher
    PUBLIC cxx_std_17)
target_include_directories(dispatcher
    PUBLIC ${include_path})
target_link_libraries(dispatcher
    PUBLIC train station path vehicles)
if (Clang OR GNU)
    target_link_libraries(dispatcher
        PRIVATE -fsanitize=address,leak,undefined --coverage)
    target_compile_options(dispatcher
        PRIVATE -O1 -fno-omit-frame-pointer --coverage)
endif()

add_library(consoleIO
    src/console_IO.cpp)
target_compile_features(consoleIO
    PUBLIC cxx_std_17)
target_include_directories(consoleIO
    PUBLIC ${include_path})

add_library(menu
    src/ui_menu.cpp)
target_compile_features(menu
    PUBLIC cxx_std_17)
target_include_directories(menu
    PUBLIC ${include_path})

add_library(trainlog
    src/train_log.cpp)
target_compile_features(trainlog
    PUBLIC cxx_std_17)
target_include_directories(trainlog
    PUBLIC ${include_path})
target_link_libraries(trainlog
    PUBLIC train dispatcher time_point)

add_library(carlog
    src/car_log.cpp)
target_compile_features(carlog
    PUBLIC cxx_std_17)
target_include_directories(carlog
    PUBLIC ${include_path})
target_link_libraries(carlog
    PUBLIC dispatcher time_point)

add_library(printer
    src/printer.cpp)
target_compile_features(printer
    PUBLIC cxx_std_17)
target_include_directories(printer
    PUBLIC ${include_path})
target_link_libraries(printer
    PUBLIC trainlog carlog dispatcher)

add_library(app
    src/trains_app.cpp)
target_compile_features(app
    PUBLIC cxx_std_17)
target_include_directories(app
    PUBLIC ${include_path})
target_link_libraries(app
    PUBLIC consoleIO simulator dispatcher events trainlog carlog printer)

add_library(user_interface
    src/user_interface.cpp)
target_compile_features(user_interface
    PUBLIC cxx_std_17)
target_include_directories(user_interface
    PUBLIC ${include_path})
target_link_libraries(user_interface
    PUBLIC app menu)

# Executable
set(exec ${PROJECT_NAME})
add_executable(${exec}
    src/main.cpp)
target_link_libraries(${exec}
    PRIVATE user_interface)
target_compile_features(${exec}
    PUBLIC cxx_std_17)

# Tests
# include(CTest)
# enable_testing()
# add_subdirectory(tests)

set(zip-files
    include
    src
    rapport/rapport.pdf
    CMakeLists.txt
    CompilerOptions.cmake
    TrainMap.txt
    Trains.txt
    TrainStations.txt)
set(archive_file "${CMAKE_CURRENT_SOURCE_DIR}/zip/${PROJECT_NAME}.zip")
add_custom_command(
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${archive_file} --format=zip
        -- ${zip-files}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT "${archive_file}"
    DEPENDS ${zip-files}
    COMMENT "Zipping source files to ${archive_file}."
)

#[=[Target to create the archive.

    Triggering this target will call the custom command that OUTPUTs
    the file that this target DEPENDs on.
#]=]
add_custom_target(zip DEPENDS "${archive_file}")


