
### Setup the Binaries folder in prep for the build

# message(STATUS "Creating Binaries Directories")

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/arm64-v8a/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/armeabi-v7a/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86_64/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/arm64-v8a/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/armeabi-v7a/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86_64/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/arm64-v8a/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/armeabi-v7a/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/android/x86_64/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/ios/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/ios/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/ios/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win32/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win32/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win32/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win64/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win64/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/win64/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux32/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux32/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux32/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux64/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux64/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/linux64/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx32/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx32/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx32/release/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx64/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx64/debug/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries/osx64/release/)
#file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Application/Debug/)
#file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Application/Release/)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/assets)
