
function(find_binary OPIFEX_LIBRARY OPIFEX_NAME NOT_STATIC)
	populate_binary_directory()

	if(${OPIFEX_OS_ANDROID})

		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")
		SET(${OPIFEX_LIBRARY} "${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${OPIFEX_LIBRARY_NAME}")

		message(STATUS "SETTING '${OPIFEX_LIBRARY}' to '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${OPIFEX_LIBRARY_NAME}'")

		message(STATUS "	RESULT '${${OPIFEX_LIBRARY}}'")

	else()
		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")
		message(STATUS "Is it shared?? ${OPIFEX_OPTION_SHARED}")

        #if(${OPIFEX_IOS})
		    SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}")
        #endif()

		if(${OPIFEX_OPTION_SHARED})
			if(${NOT_STATIC})
				SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.dylib")
			endif()
		endif()

		if(${OPIFEX_OS_WINDOWS})
			SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}.lib")
		endif()

		if(${OPIFEX_OS_ANDROID})
			SET(BINARY_RELEASE_MODE "")
		else()
			message(STATUS "Find Library in release? ${BINARY_RELEASE_MODE}")
			if(${OPIFEX_OPTION_RELEASE})
				SET(BINARY_RELEASE_MODE "release/")
			else()
				SET(BINARY_RELEASE_MODE "debug/")
			endif()
		endif()


		find_library( ${OPIFEX_LIBRARY} NAMES ${OPIFEX_LIBRARY_NAME} PATHS
		"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}")

		message(STATUS "Looking for '${OPIFEX_LIBRARY_NAME}' in '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}'")
		message(STATUS "		Found: '${${OPIFEX_LIBRARY}}'")

	endif()
endfunction(find_binary)
