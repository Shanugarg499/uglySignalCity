#include "raylib.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "city_objects.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

int main() {
    std::cout << "[SYSTEM] Coupling background MATLAB Engine context..." << std::endl;
    std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr = matlab::engine::startMATLAB();
    matlabPtr->eval(u"addpath('/Users/shanugarg499/Documents/MATLAB')");

    const int screenWidth = 1400;
    const int screenHeight = 1000;
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "3D RF Wave Topography Presentation Engine");

    Vector3 cameraTarget = { 0.0f, 4.0f, -120.0f }; 
    float cameraRadius = 500.0f;
    float cameraAngleH = 0.95f;
    float cameraAngleV = 0.55f;

    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 52.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    std::vector<Human> population;
    std::vector<Vehicle> traffic;
    std::vector<Building> architecture;

    InitializeCity(population, traffic, architecture);
    SetTargetFPS(60);
    
    float runTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        runTimer += dt;

        cameraRadius -= GetMouseWheelMove() * 45.0f;
        if (cameraRadius < 40.0f) cameraRadius = 40.0f;
        if (cameraRadius > 1500.0f) cameraRadius = 1500.0f;

        // Shift + Drag for camera orbit execution
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            Vector2 delta = GetMouseDelta();
            cameraAngleH -= delta.x * 0.007f;
            cameraAngleV += delta.y * 0.007f;
            if (cameraAngleV > 1.42f) cameraAngleV = 1.42f;
            if (cameraAngleV < 0.05f) cameraAngleV = 0.05f;
        }

        float pan = 15.0f;
        if (IsKeyDown(KEY_W)) cameraTarget.z -= pan;
        if (IsKeyDown(KEY_S)) cameraTarget.z += pan;
        if (IsKeyDown(KEY_A)) cameraTarget.x -= pan;
        if (IsKeyDown(KEY_D)) cameraTarget.x += pan;

        camera.position.x = cameraTarget.x + cameraRadius * cos(cameraAngleV) * sin(cameraAngleH);
        camera.position.y = cameraTarget.y + cameraRadius * sin(cameraAngleV);
        camera.position.z = cameraTarget.z + cameraRadius * cos(cameraAngleV) * cos(cameraAngleH);
        camera.target = cameraTarget;

        // --- KEYBOARD MODIFIER PLACEMENT MATRIX ---
        // Ray cast drops hardware directly based on keyboard state triggers
        if (IsKeyPressed(KEY_T) || IsKeyPressed(KEY_R)) {
            Ray ray = GetMouseRay(GetMousePosition(), camera);
            if (ray.direction.y != 0.0f) {
                float targetGround = -ray.position.y / ray.direction.y;
                Vector3 intersection = { ray.position.x + ray.direction.x * targetGround, 0.5f, ray.position.z + ray.direction.z * targetGround };

                if (IsKeyPressed(KEY_T)) {
                    transmitterPos = intersection;
                    transmitterPlaced = true;
                    std::cout << "[NODE] Transmitter placed at: " << intersection.x << ", " << intersection.z << std::endl;
                }
                if (IsKeyPressed(KEY_R)) {
                    receiverPos = intersection;
                    receiverPlaced = true;
                    std::cout << "[NODE] Receiver placed at: " << intersection.x << ", " << intersection.z << std::endl;
                }
            }
        }

        // Sim updates
        for (auto& h : population) {
            h.walkPhase += dt * 8.0f;
            h.position.z += dt * 10.0f; if (h.position.z > 0.0f) h.position.z = -1800.0f;
        }
        for (auto& v : traffic) {
            v.position.z += v.speed; if (v.position.z > 0.0f) v.position.z = -1900.0f;
        }

        float linkDistance = 0.0f;
        float pathLossDb = 0.0f;
        if (transmitterPlaced && receiverPlaced) {
            linkDistance = sqrt(pow(transmitterPos.x - receiverPos.x, 2) + pow(transmitterPos.z - receiverPos.z, 2));
            pathLossDb = 20.0f * log10(linkDistance + 1.0f) + 32.4f; 
        }

        BeginDrawing();
            ClearBackground((Color){ 200, 225, 245, 255 });
            BeginMode3D(camera);
                DrawInfrastructure(architecture, population, runTimer);
                DrawNetworkOverlay(architecture, runTimer); 
                DrawTraffic(traffic, runTimer);
            EndMode3D();

            // --- HUD INSTRUMENTATION CONTROLS PANEL ---
            DrawRectangle(20, 20, 780, 115, (Color){ 255, 255, 255, 240 });
            DrawFPS(45, 30);
            DrawText("3D TOPOGRAPHY PROPAGATION MATRIX | TARGET KEY PLACEMENT WORKING", 45, 52, 14, DARKGRAY);
            DrawText("Press [T] Over Street -> Drop Tx (Gold) | Press [R] Over Street -> Drop Rx (Purple)", 45, 72, 12, DARKGREEN);
            
            // THE DYNAMIC TELEMETRY BOX
            DrawRectangle(1140, 20, 240, 180, (Color){ 20, 25, 30, 245 });
            DrawRectangleLines(1140, 20, 240, 180, LIGHTGRAY);
            DrawText("SIGNAL METRICS BOX", 1155, 35, 13, LIGHTGRAY);

            if (transmitterPlaced && receiverPlaced) {
                char stats[64];
                snprintf(stats, sizeof(stats), "Range: %.1fm", linkDistance);
                DrawText(stats, 1155, 60, 14, WHITE);
                
                snprintf(stats, sizeof(stats), "Loss: -%.2f dB", pathLossDb);
                DrawText(stats, 1155, 80, 14, ORANGE);
                
                Color statusColor = GREEN;
                const char* statusText = "EXCELLENT";
                int bars = 4;

                if (pathLossDb > 55.0f) { statusColor = YELLOW; statusText = "STABLE"; bars = 3; }
                if (pathLossDb > 70.0f) { statusColor = ORANGE; statusText = "WEAK CELL"; bars = 2; }
                if (pathLossDb > 85.0f) { statusColor = RED; statusText = "DROPPED"; bars = 1; }

                DrawText(statusText, 1155, 105, 18, statusColor);

                for (int i = 0; i < 4; i++) {
                    Color barColor = (i < bars) ? statusColor : DARKGRAY;
                    DrawRectangle(1155 + (i * 14), 165 - (i * 8), 10, 12 + (i * 8), barColor);
                }
            } else {
                DrawText("STATUS: PENDING", 1155, 65, 16, YELLOW);
                DrawText("Hover mouse and press", 1155, 95, 13, GRAY);
                DrawText("[T] and [R] to calculate.", 1155, 115, 13, GRAY);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
