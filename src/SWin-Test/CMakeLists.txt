cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE RAZTER_TEST_H *.h)
file(GLOB_RECURSE RAZTER_TEST_C *.c)

if(APPLE)
	add_executable(
		SWin-Test 
		MACOSX_BUNDLE 
		${RAZTER_TEST_H} 
		${RAZTER_TEST_C} 
	)
else()
	add_executable(SWin-Test ${RAZTER_TEST_H} ${RAZTER_TEST_C})
endif()


if(APPLE)
	find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa)
	find_library(COCOA_LIBRARY Cocoa)

	target_link_libraries (SWin-Test ${COCOA_LIBRARY})
endif()

target_include_directories (SWin-Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries (SWin-Test ${SWIN_LIBRARIES})
