cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE SWIN_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

if(APPLE)
	add_library(SWin ${SWIN_HEADERS} SWin_Cocoa.m SWin_Utils.c)
elseif(MSVC)
	add_library(SWin ${SWIN_HEADERS} Win32/SWin_Win32_Utils.c Win32/SWin_Win32_Window.c Win32/SWin_Win32_Thread.c Win32/SWin_Win32_GL.c SWin_Utils.c)
else()
	add_library(SWin ${SWIN_HEADERS} SWin_Utils.c)
endif()


target_include_directories (SWin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)