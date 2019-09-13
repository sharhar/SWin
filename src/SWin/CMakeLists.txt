cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE SWIN_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

if(APPLE)
	add_library(SWin ${SWIN_HEADERS} 
			Cocoa/SWin_Cocoa_GL.m 
			Cocoa/SWin_Cocoa_Utils.m 
			Cocoa/SWin_Cocoa_Window.m 
			POSIX/SWin_POSIX_Thread.c
			SWin_Utils.c)
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

	FIND_PACKAGE(PkgConfig QUIET)
	PKG_CHECK_MODULES(GTK3 QUIET gtk+-3.0)

	if (GTK3_FOUND)
		INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
		LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

		ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

		add_library(SWin ${SWIN_HEADERS}
				GLX/SWin_GLX.c
				GTK/SWin_GTK_VK.c
				GTK/SWin_GTK_Window.c
				GTK/SWin_GTK_Utils.c
				POSIX/SWin_POSIX_Thread.c
				SWin_Utils.c)

		set(SWIN_LIBRARIES SWin -lX11 -ldl -lpthread ${GTK3_LIBRARIES} PARENT_SCOPE)

	else()
		add_library(SWin ${SWIN_HEADERS}
				GLX/SWin_GLX.c
				X11/SWin_X11_VK.c
				X11/SWin_X11_Window.c
				X11/SWin_X11_Utils.c
				POSIX/SWin_POSIX_Thread.c
				SWin_Utils.c)

		set(SWIN_LIBRARIES SWin -lX11 -ldl -lpthread PARENT_SCOPE)

	endif()
endif()

target_include_directories (SWin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)