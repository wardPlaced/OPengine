#########################################
# MATH - LAYER 3
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
)

add_external_opifex_includes()

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Math/include" MATH_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Math/src" MATH_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	1_Core
	2_Data
)


#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(3_Math SHARED ${MATH_SRC})
else()
	add_library(3_Math STATIC ${MATH_SRC})
endif()
if(${OPIFEX_OS_IOS})
	set_xcode_property(3_Math GCC_GENERATE_DEBUGGING_SYMBOLS YES)
endif()
add_opifex_defines()

add_definitions(-DGLEW_STATIC)

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

target_link_libraries(3_Math ${ALL_LIBS})

set_target_properties(3_Math PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Math PROPERTIES OUTPUT_NAME "Math")

output_library(3_Math "Math")
