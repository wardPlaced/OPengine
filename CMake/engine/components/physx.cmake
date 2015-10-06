
macro(add_opifex_physx_set_source)

	# Look for an External Path to the PhysX folder
	SET(_PHYSX_SOURCE 0)
	if(NOT "${PHYSX_PATH}" STREQUAL "")
		message(STATUS "PhysX PATH WAS SET ${PHYSX_PATH}")
		SET(_PHYSX_SOURCE 1)
		SET(_PHYSX_PATH "${PHYSX_PATH}")
	else()
		message(STATUS "PhysX PATH WAS NOT SET ${PHYSX_PATH}")
		SET(_PHYSX_PATH "${PROJECT_SOURCE_DIR}/External/PhysX")
	endif()

endmacro(add_opifex_physx_set_source)


macro(add_opifex_physx_include)

	add_opifex_physx_set_source()
	include_directories(${_PHYSX_PATH}/Include/)
	message(STATUS "${_PHYSX_PATH}/Include/")

endmacro(add_opifex_physx_include)


function(add_opifex_physx APPLICATION_TARGET )

	if(NOT ${OPIFEX_OPTION_PHYSX})
		RETURN()
	endif()

	add_opifex_physx_set_source()

	message(STATUS "Link Physics")

    if(${OPIFEX_OS_WINDOWS})
		add_opifex_physx_windows(${APPLICATION_TARGET})
    else()
		add_opifex_physx_osx(${APPLICATION_TARGET})
	endif()

endfunction(add_opifex_physx)


macro(add_opifex_physx_osx APPLICATION_TARGET)



	if(${OPIFEX_OS_OSX64})

		SET(_PHYSX_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/PhysX/lib/debug/osx64/")
		if(_PHYSX_SOURCE)
			SET(_PHYSX_BINARY_LOCATION "${PHYSX_PATH}/Lib/osx64/")
		endif()

		message("${PHYSX_PATH} ${_PHYSX_SOURCE}")

		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3DEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysXProfileSDKDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libLowLevelDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libLowLevelClothDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3CharacterKinematicDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3CommonDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3CookingDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3ExtensionsDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysX3VehicleDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPhysXVisualDebuggerSDKDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPxTaskDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libPvdRuntimeDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libSceneQueryDEBUG.a)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}libSimulationControllerDEBUG.a)

		link_from_binaries(${APPLICATION_TARGET}
			libPhysX3DEBUG.a
			libPhysXProfileSDKDEBUG.a
			libLowLevelDEBUG.a
			libLowLevelClothDEBUG.a
			libPhysX3CharacterKinematicDEBUG.a
			libPhysX3CommonDEBUG.a
			libPhysX3CookingDEBUG.a
			libPhysX3ExtensionsDEBUG.a
			libPhysX3VehicleDEBUG.a
			libPhysXVisualDebuggerSDKDEBUG.a
			libPxTaskDEBUG.a
			libPvdRuntimeDEBUG.a
			libSceneQueryDEBUG.a
			libSimulationControllerDEBUG.a)

	elseif(${OPIFEX_OS_OSX32})

		SET(_PHYSX_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/PhysX/lib/debug/osx64/")
		if(_PHYSX_SOURCE)
			SET(_PHYSX_BINARY_LOCATION "${PHYSX_PATH}/out/x64.debug/")
		endif()

		message("${PHYSX_PATH} ${_PHYSX_SOURCE}")

		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3DEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysXProfileSDKDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libLowLevelDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libLowLevelClothDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CharacterKinematicDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CommonDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CookingDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3ExtensionsDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3VehicleDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysXVisualDebuggerSDKDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPxTaskDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPvdRuntimeDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libSceneQueryDEBUG.a)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libSimulationControllerDEBUG.a)

		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3DEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysXProfileSDKDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libLowLevelDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libLowLevelClothDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CharacterKinematicDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CommonDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CookingDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3ExtensionsDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3VehicleDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysXVisualDebuggerSDKDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPxTaskDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libPvdRuntimeDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libSceneQueryDEBUG.a)
		copy_file_to_binaries(/External/PhysX/lib/osx32/libSimulationControllerDEBUG.a)
	endif()

endmacro(add_opifex_physx_osx)


macro(add_opifex_physx_windows APPLICATION_TARGET)

	if(${OPIFEX_OS_WIN64})

		if(${OPIFEX_OPTION_RELEASE})

			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXProfileSDK.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevel.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelCloth.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CharacterKinematic_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Common_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Cooking_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Extensions.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Vehicle.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXVisualDebuggerSDK.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PxTask.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PvdRuntime.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SceneQuery.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SimulationController.lib)

			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysXProfileSDK.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/LowLevel.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelCloth.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CharacterKinematic_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Common_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Cooking_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Extensions.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Vehicle.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysXVisualDebuggerSDK.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PxTask.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PvdRuntime.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/SceneQuery.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/SimulationController.lib)

			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CharacterKinematic_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Common_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Cooking_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Gpu_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysXDevice64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "nvToolsExt64_1.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})

		else()

			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3DEBUG_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXProfileSDKDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelClothDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CharacterKinematicDEBUG_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CommonDEBUG_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CookingDEBUG_x64.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3ExtensionsDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3VehicleDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXVisualDebuggerSDKDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PxTaskDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PvdRuntimeDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SceneQueryDEBUG.lib)
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SimulationControllerDEBUG.lib)

			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3DEBUG_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysXProfileSDKDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelClothDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CharacterKinematicDEBUG_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CommonDEBUG_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CookingDEBUG_x64.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3ExtensionsDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3VehicleDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PhysXVisualDebuggerSDKDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PxTaskDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/PvdRuntimeDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/SceneQueryDEBUG.lib)
			copy_file_to_binaries(/External/PhysX/lib/win64/SimulationControllerDEBUG.lib)

			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CharacterKinematicDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CommonDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CookingDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3DEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3GpuDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysXDevice64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "nvToolsExt64_1.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})

		endif()

	else()

		SET(_PHYSX_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/PhysX/lib/debug/win32/")
		SET(_PHYSX_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/PhysX/lib/debug/win32/")
		if(_PHYSX_SOURCE)
			SET(_PHYSX_BINARY_LOCATION "${_PHYSX_PATH}\\Lib\\vc12win32\\")
			SET(_PHYSX_DLL_LOCATION "${_PHYSX_PATH}\\Bin\\vc12win32\\")
		endif()

		message("${PHYSX_PATH} ${_PHYSX_SOURCE}")

		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3DEBUG_x86.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysXProfileSDKDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}LowLevelDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}LowLevelClothDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3CharacterKinematicDEBUG_x86.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3CommonDEBUG_x86.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3CookingDEBUG_x86.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3ExtensionsDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysX3VehicleDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PhysXVisualDebuggerSDKDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PxTaskDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}PvdRuntimeDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}SceneQueryDEBUG.lib)
		copy_to_binaries(${_PHYSX_BINARY_LOCATION}SimulationControllerDEBUG.lib)

		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysX3CharacterKinematicDEBUG_x86.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysX3CommonDEBUG_x86.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysX3CookingDEBUG_x86.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysX3DEBUG_x86.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysX3GpuDEBUG_x86.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}PhysXDevice.dll)
		copy_to_binaries(${_PHYSX_DLL_LOCATION}nvToolsExt32_1.dll)

		link_from_binaries(${APPLICATION_TARGET}
			PhysX3DEBUG_x86.lib
			PhysXProfileSDKDEBUG.lib
			LowLevelDEBUG.lib
			LowLevelClothDEBUG.lib
			PhysX3CharacterKinematicDEBUG_x86.lib
			PhysX3CommonDEBUG_x86.lib
			PhysX3CookingDEBUG_x86.lib
			PhysX3ExtensionsDEBUG.lib
			PhysX3VehicleDEBUG.lib
			PhysXVisualDebuggerSDKDEBUG.lib
			PvdRuntimeDEBUG.lib
			SceneQueryDEBUG.lib
			SimulationControllerDEBUG.lib
			)

		copy_from_binaries(${APPLICATION_TARGET} "PhysX3CharacterKinematicDEBUG_x86.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "PhysX3CommonDEBUG_x86.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "PhysX3CookingDEBUG_x86.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "PhysX3DEBUG_x86.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "PhysX3GpuDEBUG_x86.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "PhysXDevice.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "nvToolsExt32_1.dll" "/Application/Debug")

	endif()

endmacro(add_opifex_physx_windows)
