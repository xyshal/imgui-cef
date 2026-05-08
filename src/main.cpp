// STL
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <X11/Xlib.h>
#endif

// imgui
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

// CEF
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/cef_browser.h"

namespace
{

GLFWwindow* Initialize()
{

  // Setup window
  if (!glfwInit())
    return nullptr;

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  GLFWwindow* window = glfwCreateWindow(1280, 720, "whatever", NULL, NULL);
  if (window == NULL) return nullptr;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwMaximizeWindow(window);

  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  return window;
}

void PreRender()
{
  glfwPollEvents();

  CefDoMessageLoopWork();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void PostRender(GLFWwindow* window)
{
  int w = 0;
  int h = 0;
  glfwGetFramebufferSize(window, &w, &h);
  glViewport(0, 0, w, h);
  glClear(GL_COLOR_BUFFER_BIT);
  constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
}

void Shutdown(GLFWwindow* window)
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}


} // end anonymous namespace


class ImGuiRenderHandler : public CefRenderHandler
{
public:
    ImGuiRenderHandler(int w, int h) : vWidth(w), vHeight(h) {
        glGenTextures(1, &vTexture);
        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vWidth, vHeight,
                     0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
    }

    ~ImGuiRenderHandler() {
        glDeleteTextures(1, &vTexture);
    }

    void GetViewRect(CefRefPtr<CefBrowser>, CefRect& rect) override {
        rect = CefRect(0, 0, vWidth, vHeight);
    }

    void OnPaint(CefRefPtr<CefBrowser>,
                 PaintElementType type,
                 const RectList&,
                 const void* buffer,
                 int width,
                 int height) override {

        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0, 0,
                        width,
                        height,
                        GL_BGRA,
                        GL_UNSIGNED_INT_8_8_8_8_REV,
                        buffer);
    }

    GLuint GetTexture() const { return vTexture; }

private:
    int vWidth = 0;
    int vHeight = 0;
    GLuint vTexture;

    IMPLEMENT_REFCOUNTING(ImGuiRenderHandler);
};

class Client : public CefClient
{
public:
    Client(CefRefPtr<ImGuiRenderHandler> handler)
        : vRenderHandler(handler) {}

    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return vRenderHandler;
    }

    void SetBrowser(CefRefPtr<CefBrowser> b) { vBrowser = b; }
    CefRefPtr<CefBrowser> GetBrowser() { return vBrowser; }

private:
    CefRefPtr<ImGuiRenderHandler> vRenderHandler;
    CefRefPtr<CefBrowser> vBrowser;

    IMPLEMENT_REFCOUNTING(Client);
};


class TestApp : public CefApp
{
  IMPLEMENT_REFCOUNTING(TestApp);
};


int main(int argc, char** argv)
{
  // The working imgui render was dropped here to investigate the CEF
  // integration failure.
  //GLFWwindow* window = ::Initialize();
  //if (window == nullptr) return 1;
  GLFWwindow* window = nullptr;

  // CEF initialization
  CefMainArgs main_args(argc, argv);

  const int rv = CefExecuteProcess(main_args, nullptr, nullptr);
  if (rv >= 0) {
    return rv;
  }

  CefRefPtr<TestApp> app(new TestApp());

  CefSettings settings;
  settings.no_sandbox = true;
  settings.windowless_rendering_enabled = true;
  settings.multi_threaded_message_loop = false;
  CefString(&settings.root_cache_path).FromString("/tmp/cef_user_data");
  CefString(&settings.resources_dir_path).FromString(std::filesystem::current_path());
  CefString(&settings.locales_dir_path).FromString(std::filesystem::current_path());

  if (!CefInitialize(main_args, settings, app, nullptr)) {
    std::cerr << "Failed to initialize\n";
    return 1;
  }

  // TODO: Here's where we're at, integration-wise.  Can we get all the
  // relevant processes running CefInitialize() properly?
  return 0;

  // ---------------- Create browser ----------------
  CefWindowInfo windowInfo;
  windowInfo.SetAsWindowless(0);

  CefBrowserSettings browserSettings;

  CefRefPtr<ImGuiRenderHandler> renderHandler = new ImGuiRenderHandler(1280, 720);
  CefRefPtr<Client> client = new Client(renderHandler);

  CefBrowserHost::CreateBrowser(windowInfo,
                                client.get(),
                                "https://slashdot.org",
                                browserSettings,
                                nullptr,
                                nullptr);

  while (!glfwWindowShouldClose(window)) {
    ::PreRender();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::Begin("ImGui Window");
    {
      ImGui::Text("This should be a browser, I guess!");
      GLuint tex = renderHandler->GetTexture();
      ImGui::Image((void*)(intptr_t)tex, ImVec2(1280, 720));

    }
    ImGui::End();

    ImGui::Render();

    ::PostRender(window);
  }

  ::Shutdown(window);

  return 0;
}

