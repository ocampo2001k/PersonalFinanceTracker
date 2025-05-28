cmake_minimum_required(VERSION 3.16)
project(PersonalFinanceTracker VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(wxWidgets CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# Define source files
set(SOURCES
    main.cpp
    Model/Transaction.cpp
    ViewModel/TransactionManager.cpp
    Database/DatabaseHandler.cpp
    View/MainWindow.cpp
)

# Define header files (for IDE support)
set(HEADERS
    Model/Transaction.h
    ViewModel/TransactionManager.h
    Database/DatabaseHandler.h
    View/MainWindow.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    wx::core wx::base wx::adv
    SQLite::SQLite3
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    # Set subsystem to Windows for GUI application
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy database to output directory (if it exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/finance_tracker.db")
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/finance_tracker.db"
        "${CMAKE_BINARY_DIR}/bin/finance_tracker.db"
        COPYONLY
    )
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "PersonalFinanceTracker")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Personal Finance Tracker - C++ MVVM Application")
set(CPACK_PACKAGE_VENDOR "Your Name")

include(CPack) 