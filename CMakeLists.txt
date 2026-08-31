cmake_minimum_required(VERSION 3.15)
project(MyProject LANGUAGES CXX)

# Force the compiler to use C++26 standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Use ccache if ccache is available. Uncomment to use it as a template.
#find_program(CCACHE_PROGRAM ccache)
#if(NOT CCACHE_PROGRAM)
  #message(STATUS "ccache not found on system. Installing via FetchContent...")
    
  #include(FetchContent)
   #FetchContent_Declare(
        #ccache
        #GIT_REPOSITORY https://github.com/ccache/ccache.git
        #GIT_TAG        v4.10.2 # Use a stable release version
        #SYSTEM                 # Prevents ccache warnings from flooding your build logs
        #SOURCE_SUBDIR  .  # <--- ADD THIS LINE HERE TO PREVENT CCACHE DIR ERRORS.
    #)
    
    # Hide ccache's internal build options from your main CMake GUI cache
    #set(BUILD_TESTING OFF CACHE INTERNAL "")
    #set(HIREDIS_WITH_SSL OFF CACHE INTERNAL "")
    
    #FetchContent_MakeAvailable(ccache)
    
    # Get the path to the newly built ccache executable target
    #set(CCACHE_PROGRAM $<TARGET_FILE:ccache>)
#endif()

#if(CCACHE_PROGRAM)
    #set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_PROGRAM}")
    #set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    #message(STATUS "Ninja + Ccache integration successful")
#endif()

#This is a template for including libraries:
 
#[[ target_include_directories(mylib PRIVATE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mylib>
 $<INSTALL_INTERFACE:include/mylib>
 ) ]]

#This is a template for using fetch content to a github repo.


#End template here

# Define a standard CMake variable for your target name
set(TARGET_NAME "my_app")

# Create the executable using the CMake variable syntax ${TARGET_NAME}. src/main.cpp
add_executable(${TARGET_NAME} src/main.cpp)

# Add compiler flags
target_compile_options(${TARGET_NAME} PRIVATE
  -Wall
  -Wextra
  -Wconversion
  -Wshadow
  -Wpedantic
  -Wsign-conversion
  -Wshadow
  -Wunused
  -mavx2
)

