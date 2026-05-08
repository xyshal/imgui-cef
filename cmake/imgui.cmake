set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imgui)

add_definitions(-DIMGUI_ENABLE_FREETYPE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
                 
add_library(imgui STATIC 
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_internal.h
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/misc/freetype/imgui_freetype.cpp
)

target_include_directories(imgui PUBLIC
  ${IMGUI_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
)
    
target_link_libraries(imgui PRIVATE 
  freetype
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
)
