project(prototype-application)

if (APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET CACHE STRING "26.0")
endif()

set(PROJECT_SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CEF_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  imgui
  ${OPENGL_LIBRARIES}
  libcef_dll_wrapper
)

if (APPLE)
  target_link_libraries(${PROJECT_NAME} PUBLIC
    "$<LINK_LIBRARY:FRAMEWORK,CoreFoundation>"
    "$<LINK_LIBRARY:FRAMEWORK,Chromium Embedded Framework>"
  )
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-F${CEF_ROOT}/Release")
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE ${CEF_ROOT}/Debug/libcef.so)
endif()

if (APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ln -s ${CEF_ROOT}/Release ${CMAKE_CURRENT_BINARY_DIR}/../Frameworks)
else()
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CEF_ROOT}/Resources/locales
            ${CMAKE_CURRENT_BINARY_DIR}/locales
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CEF_ROOT}/Resources/chrome_100_percent.pak
            ${CEF_ROOT}/Resources/chrome_200_percent.pak
            ${CEF_ROOT}/Resources/resources.pak
            ${CEF_ROOT}/Resources/icudtl.dat
            ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

