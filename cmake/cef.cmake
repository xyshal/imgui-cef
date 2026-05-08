# Compiled CEF attempt
# ====================
#set(CEF_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/code)
#set(CEF_OUT_DIR ${CEF_ROOT}/chromium_git/chromium/src/out/Release_GN_arm64)
#
#set(CEF_INCLUDE_PATH
#  ${CEF_ROOT}/chromium_git/cef
#  ${CEF_ROOT}/chromium_git/chromium/src
#  ${CEF_OUT_DIR}/gen/cef
#)
#
#set(CEF_LIBRARIES
#  ${CEF_OUT_DIR}/obj/cef/libcef_dll_wrapper.a
#)


# Prebuild CEF libcef_dll wrapper attempt
# =======================================
set(CEF_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cef)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CEF_ROOT}/cmake)
find_package(CEF REQUIRED)
add_subdirectory(${CEF_ROOT}/libcef_dll EXCLUDE_FROM_ALL)



# Prebuilt CEF attempt
# ====================

#set(CEF_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cef)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CEF_ROOT}/cmake)
#find_package(CEF REQUIRED)


#find_library(CEF_FRAMEWORK
#    NAMES "Chromium Embedded Framework"
#    PATHS "${CEF_ROOT}/Release"
#    NO_DEFAULT_PATH
#)
#
#
#set_target_properties(cef PROPERTIES
#    IMPORTED_LOCATION
#    "${CEF_ROOT}/Release/Chromium Embedded Framework.framework/Chromium Embedded Framework"
#    INTERFACE_INCLUDE_DIRECTORIES
#    "${CEF_ROOT}"
#)
