cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE SWIN_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

if(APPLE)
	add_library(SWin ${SWIN_HEADERS} SWin_Cocoa.m SWin_Utils.c)
	set(SWIN_LIBRARIES SWin PARENT_SCOPE)
elseif(MSVC)
	add_library(SWin ${SWIN_HEADERS}
			Win32/SWin_Win32_GL.c
			Win32/SWin_Win32_Thread.c
			Win32/SWin_Win32_VK.c
			Win32/SWin_Win32_Window.c
			Win32/SWin_Win32_Utils.c
			SWin_Utils.c)
	set(SWIN_LIBRARIES SWin PARENT_SCOPE)
else()
	add_library(SWin ${SWIN_HEADERS}
			X11/SWin_X11_GL.c
			X11/SWin_X11_Thread.c
			X11/SWin_X11_VK.c
			X11/SWin_X11_Window.c
			X11/SWin_X11_Utils.c
			SWin_Utils.c)

	set(SWIN_LIBRARIES SWin -lX11 -ldl -lpthread PARENT_SCOPE)
endif()

target_include_directories (SWin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)