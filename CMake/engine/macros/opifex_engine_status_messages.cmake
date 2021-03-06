
macro(opifex_engine_status_messages)

	add_marketplace_assets(OPIFEX_ASSETS_ADDITIONAL)
	set_opifex_assets()

	message(STATUS "\nOPengine CMake Success\n===================================\n
		OPIFEX_REPOSITORY: ${OPIFEX_REPOSITORY}
        RENDERER: ${RENDER_TARGET}
		IS DESKTOP: ${OPIFEX_OS_DESKTOP}
		IS MOBILE: ${OPIFEX_OS_MOBILE}
		IS ANDROID: ${OPIFEX_OS_ANDROID}
		OPIFEX_ENGINE_REPOSITORY: ${OPIFEX_ENGINE_REPOSITORY}
		OPIFEX_BINARIES: ${OPIFEX_BINARIES}
		OPIFEX_ASSETS: ${OPIFEX_ASSETS}
		OPIFEX_OS: ${OPIFEX_OS}
		PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}
		PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}
		OPIFEX_OPTION_RELEASE: ${OPIFEX_OPTION_RELEASE}
		BINARY_TARGET_DIRECTORY: ${BINARY_TARGET_DIRECTORY}\n")

endmacro(opifex_engine_status_messages)
