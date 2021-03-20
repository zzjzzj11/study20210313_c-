cmake_minimum_required(VERSION 3.4.1)
 
link_directories(
	/usr/lib
	)
 project(sqlite3_demo)
 add_executable(student student.cpp)
 target_link_libraries(student sqlite3)

