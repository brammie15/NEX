#include "imgui.h"
#include "imgui_impl_glfw_switch.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define GLM_FORCE_PURE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


#ifdef __SWITCH__
#include <switch.h>
#endif

#include <chrono>
#include <set>
#include <thread>

#include "Engine/DeltaTime.h"
#include "Scene/SceneManager.h"
#include "Managers/InputManager.h"
#include "Scene.h"
#include "Transform.h"
#include "GameObject.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/MeshRendererComponent.h"
#include "Engine/Components/CameraMoverComponent.h"

// Constants used for scaling as well as
// creating a window of the right size on PC
constexpr uint32_t WINDOW_WIDTH  = 1280;
constexpr uint32_t WINDOW_HEIGHT = 720;

#define DEFAULT_FPS 60

static void errorCallback(int errorCode, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", errorCode, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void initImgui(GLFWwindow* window) {
     // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    const char* glsl_version = "#version 430 core";

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

     // Load Fonts
    io.Fonts->AddFontDefault();
    {
        PlFontData standard, extended;
        static ImWchar extended_range[] = {0xe000, 0xe152};
        if (R_SUCCEEDED(plGetSharedFontByType(&standard,     PlSharedFontType_Standard)) &&
                R_SUCCEEDED(plGetSharedFontByType(&extended, PlSharedFontType_NintendoExt))) {
            std::uint8_t *px;
            int w, h, bpp;
            ImFontConfig font_cfg;

            font_cfg.FontDataOwnedByAtlas = false;
            io.Fonts->AddFontFromMemoryTTF(standard.address, standard.size, 20.0f, &font_cfg, io.Fonts->GetGlyphRangesDefault());
            font_cfg.MergeMode            = true;
            io.Fonts->AddFontFromMemoryTTF(extended.address, extended.size, 20.0f, &font_cfg, extended_range);

            io.Fonts->GetTexDataAsAlpha8(&px, &w, &h, &bpp);
            io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;
            io.Fonts->Build();
        }
    }
}

int main(int, char**)
{
    socketInitializeDefault();

    nxlinkStdio();

    printf("Hello world!\n");
    Result rc = romfsInit();
    if (R_FAILED(rc))
    {
        printf("romfsInit() failed: 0x%x\n", rc);
        return EXIT_FAILURE;
    }


    std::srand(std::time(nullptr));
    
    glfwSetErrorCallback(errorCallback);

    // glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);

    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "test", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    // Load OpenGL routines using glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    initImgui(window);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    PadState pad;
    padInitializeDefault(&pad);

    SceneManager::GetInstance().CreateScene("SampleScene");

    auto& currentScene = SceneManager::GetInstance().GetCurrentScene();
    
    auto testObj = std::make_shared<GameObject>("TestObject");
    currentScene.Add(testObj);

    auto cameraObj = std::make_shared<GameObject>("Camera");
    auto cameraComp = cameraObj->AddComponent<CameraComponent>();
    auto cameraMover = cameraObj->AddComponent<CameraMover>();
    currentScene.Add(cameraObj);   
    cameraObj->GetTransform().SetLocalPosition(glm::vec3(-5.0f, 1.0f, 0.0f));

    auto modelObj = std::make_shared<GameObject>("Model");
    currentScene.Add(modelObj);
    modelObj->AddComponent<MeshRendererComponent>("/switch/models/fire_hydrant/FireHydrantMesh.obj");

    currentScene.SetMainCamera(cameraComp);
    
    // glFrontFace(GL_CW);

    // Cull mode
    // glEnable(GL_CULL_FACE);
    // glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glCullFace(GL_BACK);


    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        auto& dt{ DeltaTime::GetInstance() };

        auto& sceneManager{ SceneManager::GetInstance() };
        
        dt.Update();

        glfwPollEvents();

        const bool shouldContinue = InputManager::GetInstance().ProcessInput();
        if (!shouldContinue) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }

        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break;

        sceneManager.Update();
        sceneManager.LateUpdate();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        SceneManager::GetInstance().RenderImgui();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SceneManager::GetInstance().Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        sceneManager.HandleScene();

        std::this_thread::sleep_for(dt.SleepDuration());
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    socketExit();

    return 0;
}
