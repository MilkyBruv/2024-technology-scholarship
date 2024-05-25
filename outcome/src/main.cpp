#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#ifdef _WIN32
#endif

#ifdef __unix__
#include "../include/raylib/linux/raylib.h"
#endif

#include "../include/rlImGui/rlImGui.h"
#include "../include/imgui/imgui.h"
#include "../include/ImGuiColorTextEdit/TextEditor.h"

#define PADDING 8

const void set_imgui_colours();
const void update_file(const char* fragData, const char* vertData);
const void show_text_editor();

int main(int argc, char const *argv[])
{
    static TextEditor fragEditor;
    static TextEditor vertEditor;

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    fragEditor.SetLanguageDefinition(lang);
    vertEditor.SetLanguageDefinition(lang);

    static const char* fragExampleCode = R"(#version 330 core

// Input from the vertex shader
in vec2 fragTexCoord;

// Output color
out vec4 finalColor;

// Uniforms
uniform sampler2D screenTexture; // Screen texture

void main()
{
	vec4 col = texture(screenTexture, fragTexCoord);
    // Sample the screen texture
    col.r = col.r;
    col.g = col.g;
    col.b = col.b;
    finalColor = col;
}
)";
    static const char* vertExampleCode = R"(#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 texCoord; // Vertex texture coordinate

// Output to the fragment shader
out vec2 fragTexCoord;

// Uniforms
uniform mat4 mvp; // Model-View-Projection matrix

void main()
{
    // Pass the texture coordinate to the fragment shader
    fragTexCoord = texCoord;

    // Transform the vertex position by the MVP matrix
    gl_Position = mvp * vec4(position, 1.0);
}
)";
    
    fragEditor.SetText(fragExampleCode);
    vertEditor.SetText(vertExampleCode);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "test window");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    ImVec4 sceneZone =      ImVec4();
    ImVec4 editor1Zone =    ImVec4();
    ImVec4 editor2Zone =    ImVec4();

    Texture2D image = LoadTexture("./res/image2.png");
    RenderTexture2D shaderRenderTexture = LoadRenderTexture(GetScreenWidth() / 2, GetScreenHeight());
    Shader shader = LoadShader("./src/vertex.glsl", "./src/fragment.glsl");

    while (!WindowShouldClose())
    {
        sceneZone =     ImVec4(0, 0, GetScreenWidth() / 2, GetScreenHeight());
        editor1Zone =   ImVec4(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight() / 2);
        editor2Zone =   ImVec4(GetScreenWidth() / 2, GetScreenHeight() / 2, GetScreenWidth() / 2, GetScreenHeight() / 2);
        
        BeginShaderMode(shader);
        {
            BeginTextureMode(shaderRenderTexture);
            {
                ClearBackground(BLACK);
                DrawTexture(image, 0, 0, WHITE);
                // DrawCircle(100, 256, 32.0f, RED);
            }
            EndTextureMode();
        }
        EndShaderMode();

        BeginDrawing();
        {
            ClearBackground(BLACK);

            // Imgui stuff
            rlImGuiBegin();
            {
                set_imgui_colours();

                // Main menu bar
                ImGui::BeginMainMenuBar();

                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Save", "Ctrl+S"))
                        {
                            update_file(fragEditor.GetText().c_str(), vertEditor.GetText().c_str());
                            shader = LoadShader("./src/vertex.glsl", "./src/fragment.glsl");
                        }

                        ImGui::EndMenu();
                    }

                ImGui::EndMainMenuBar();

                // Create static frame
                ImGui::SetNextWindowPos(ImVec2(sceneZone.x, sceneZone.y), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(sceneZone.z, sceneZone.w), ImGuiCond_Always);
                ImGui::Begin("Test container", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

                    ImGui::Text("this is some text bruv");

                ImGui::End();

                // Set font scale to be higher (original is 13px)
                float originalFontScale = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= 1.5;
                ImGui::PushFont(ImGui::GetFont());

                ImGui::SetNextWindowPos(ImVec2(editor1Zone.x, editor1Zone.y), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(editor1Zone.z, editor1Zone.w), ImGuiCond_Always);
                ImGui::Begin("Test container 2", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

                    vertEditor.Render("Vertex Shader Editor");

                ImGui::End();
                
                ImGui::SetNextWindowPos(ImVec2(editor2Zone.x, editor2Zone.y), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(editor2Zone.z, editor2Zone.w), ImGuiCond_Always);
                ImGui::Begin("Test container 3", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

                    fragEditor.Render("Fragment Shader Editor");

                ImGui::End();

                // Reset font size back to original
                ImGui::GetFont()->Scale = originalFontScale;
                ImGui::PopFont();

                ImGui::Render();
            }
            rlImGuiEnd();

            // raylib stuff here
            DrawTexturePro(shaderRenderTexture.texture, 
                {0, 0, (float) shaderRenderTexture.texture.width, (float) -shaderRenderTexture.texture.height},
                {0, 48, (float) GetScreenWidth() / 2, (float) GetScreenHeight()},
                {0, 0}, 0.0f, WHITE);
        }
        EndDrawing();
    }

    UnloadTexture(image);
    UnloadShader(shader);
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}

const void set_imgui_colours()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colours = style->Colors;

    colours[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colours[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colours[ImGuiCol_FrameBgHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colours[ImGuiCol_FrameBgActive] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colours[ImGuiCol_TitleBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colours[ImGuiCol_TitleBgActive] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
}

const void update_file(const char* fragData, const char* vertData)
{
	std::ofstream fragFile("./src/fragment.glsl", std::ios::trunc);
    fragFile << fragData;
    fragFile.close();

    std::ofstream vertFile("./src/vertex.glsl", std::ios::trunc);
    vertFile << vertData;
    vertFile.close();
}
