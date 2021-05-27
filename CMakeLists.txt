# Initialize the project configuration.
cmake_minimum_required(VERSION 3.17)
project(annotator)

# The project uses C++14.
set(CMAKE_CXX_STANDARD 14)

# Access the OpenCV libraries from the system.
find_package( OpenCV 4.1.2 REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

# Add a number of other miscellaneous include
# directories which happen to contain useful files.
include_directories(/usr/local/include/)
include_directories(/usr/local/Cellar)

# Add the primary executables.
add_executable(annotator annotator.cc system/paths.cc writer/textwriter.cc writer/writer.cc
               handler/handler.cc annotation/annotation.cc config/config.cc)

# Link the OpenCV libraries to the project.
target_link_libraries(annotator ${OpenCV_LIBS})