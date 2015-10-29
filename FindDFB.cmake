# try to find directfb.h

set(HEADER_SEARCH_PATHS "/usr/local/include/directfb" "/usr/local/include/directfb-internal")

find_path(
	DFB_INCLUDE_DIRS directfb.h
	PATHS ${HEADER_SEARCH_PATHS}
	NO_DEFAULT_PATH
)
if(NOT EXISTS ${DFB_INCLUDE_DIRS})
	find_path(DFB_INCLUDE_DIRS directfb.h)
endif()

# try to find the DFB library
find_library(
	DFB_LIBRARIES NAMES directfb
	PATHS ${LIBRARY_SEARCH_PATHS}
	NO_DEFAULT_PATH
)
if(NOT DFB_LIBRARIES)
	find_library(DFB_LIBRARIES directfb)
endif()

if(EXISTS ${DFB_INCLUDE_DIRS})
	set(DFB_FOUND true)
else()
	set(DFB_FOUND false)
endif()
