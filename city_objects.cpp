#include "city_objects.hpp"
#include "rlgl.h"
#include <cmath>

Vector3 transmitterPos = { 0.0f, 0.5f, 0.0f };
Vector3 receiverPos = { 0.0f, 0.5f, 0.0f };
bool transmitterPlaced = false;
bool receiverPlaced = false;

void DrawRealisticHuman(Vector3 pos, Color col, float walkPhase) {
    float sY = pos.y + 0.1f;
    DrawCube((Vector3){ pos.x, sY + 1.2f, pos.z }, 0.4f, 0.8f, 0.4f, col);
    DrawSphere((Vector3){ pos.x, sY + 1.9f, pos.z }, 0.25f, (Color){ 225, 185, 150, 255 });
    float leftLeg = sin(walkPhase) * 0.4f;
    float rightLeg = -sin(walkPhase) * 0.4f;
    DrawCylinderEx((Vector3){ pos.x - 0.12f, sY + 0.8f, pos.z }, (Vector3){ pos.x - 0.12f, sY, pos.z + leftLeg }, 0.08f, 0.08f, 6, col);
    DrawCylinderEx((Vector3){ pos.x + 0.12f, sY + 0.8f, pos.z }, (Vector3){ pos.x + 0.12f, sY, pos.z + rightLeg }, 0.08f, 0.08f, 6, col);
}

void DrawOrientedSociety(Vector3 pos, float height, Color facadeColor, int floors, bool facingPositiveX, bool facingNegativeZ) {
    DrawCube(pos, 28.0f, height, 28.0f, facadeColor);
    DrawCubeWires(pos, 28.0f, height, 28.0f, (Color){ 30, 32, 35, 255 });
}

void DrawBillboardSignalIcon(Vector3 pos, float pathLossDb) {
    int bars = 4;
    Color iconColor = GREEN;
    if (pathLossDb > 55.0f) { iconColor = YELLOW; bars = 3; }
    if (pathLossDb > 70.0f) { iconColor = ORANGE; bars = 2; }
    if (pathLossDb > 85.0f) { iconColor = RED; bars = 1; }

    rlPushMatrix();
        rlTranslatef(pos.x, pos.y + 24.0f, pos.z);
        for (int i = 0; i < 4; i++) {
            Color bCol = (i < bars) ? iconColor : (Color){ 80, 80, 80, 255 };
            DrawCube((Vector3){ -3.0f + (i * 2.0f), (i * 1.5f) / 2.0f, 0.0f }, 1.2f, 2.0f + (i * 1.5f), 1.2f, bCol);
        }
    rlPopMatrix();
}

void InitializeCity(std::vector<Human>& pop, std::vector<Vehicle>& traffic, std::vector<Building>& architecture) {
    architecture.clear(); pop.clear(); traffic.clear();
    architecture.push_back({ (Vector3){ 0.0f, 40.0f, 0.0f }, (Vector3){ 40.0f, 80.0f, 40.0f }, (Color){ 44, 62, 80, 255 }, 0, 20 }); 
    architecture.push_back({ (Vector3){ -45.0f, 30.0f, -400.0f }, (Vector3){ 6.0f, 60.0f, 6.0f }, DARKGRAY, 4, 1 }); 
    architecture.push_back({ (Vector3){ -45.0f, 30.0f, -1200.0f }, (Vector3){ 6.0f, 60.0f, 6.0f }, DARKGRAY, 4, 1 }); 
    architecture.push_back({ (Vector3){ 400.0f, 30.0f, 45.0f }, (Vector3){ 6.0f, 60.0f, 6.0f }, DARKGRAY, 4, 1 });   
    architecture.push_back({ (Vector3){ 800.0f, 30.0f, 45.0f }, (Vector3){ 6.0f, 60.0f, 6.0f }, DARKGRAY, 4, 1 });   

    float currentZ = -40.0f;
    for (int i = 0; i < 5; i++) { architecture.push_back({ (Vector3){ -45.0f, 25.0f, currentZ }, (Vector3){ 28.0f, 50.0f, 28.0f }, (Color){ 218, 223, 225, 255 }, 1, 8 }); currentZ -= 35.0f; }
    currentZ -= 125.0f;
    for (int i = 0; i < 5; i++) { architecture.push_back({ (Vector3){ -45.0f, 25.0f, currentZ }, (Vector3){ 28.0f, 50.0f, 28.0f }, (Color){ 228, 233, 237, 255 }, 1, 8 }); currentZ -= 35.0f; }

    float currentX = 40.0f;
    for (int i = 0; i < 5; i++) { architecture.push_back({ (Vector3){ currentX, 25.0f, 45.0f }, (Vector3){ 28.0f, 50.0f, 28.0f }, (Color){ 218, 223, 225, 255 }, 1, 8 }); currentX += 35.0f; }
    currentX += 270.0f;
    for (int i = 0; i < 10; i++) { architecture.push_back({ (Vector3){ currentX, 35.0f, 45.0f }, (Vector3){ 28.0f, 70.0f, 28.0f }, (Color){ 189, 195, 199, 255 }, 1, 10 }); currentX += 40.0f; }

    for (int i = 0; i < 150; i++) { pop.push_back({ (Vector3){ -12.0f, 0.1f, -40.0f - (i * 12.0f) }, (Color){ 46, 204, 113, 255 }, (float)i, false, 0.0f }); }
    for (int i = 0; i < 6; i++) { traffic.push_back({ (Vector3){ 0.0f, 0.1f, -80.0f - (i * 250.0f) }, (Color){ 231, 76, 60, 255 }, false, 1.4f, true, 0.0f }); }
}

void DrawNetworkOverlay(const std::vector<Building>& architecture, float time) {
    std::vector<Vector3> sources;
    for (const auto& b : architecture) { if (b.type == 4) sources.push_back((Vector3){ b.position.x, 60.0f, b.position.z }); }
    if (transmitterPlaced) sources.push_back(transmitterPos);

    // 3D Topographic Wave Columns Rendering
    float meshStep = 20.0f;
    for (float x = -150.0f; x < 1000.0f; x += meshStep) {
        for (float z = -1600.0f; z < 150.0f; z += meshStep) {
            if (x > 40.0f && z > 40.0f) continue;
            float netAmplitude = 0.0f; int waveIdx = 0;
            for (const auto& s : sources) {
                float dist = sqrt(pow(x - s.x, 2) + pow(z - s.z, 2));
                float phase = dist * 0.25f - time * 6.0f;
                float strength = 30000.0f / (dist * dist + 800.0f);
                netAmplitude += sin(phase + waveIdx) * strength; waveIdx++;
            }
            float heightGraphY = 0.5f + fabs(netAmplitude) * 12.0f;
            if (heightGraphY > 50.0f) heightGraphY = 50.0f;

            Color graphColor = (Color){ 0, 120, 255, 45 };
            if (heightGraphY > 8.0f)  graphColor = (Color){ 0, 230, 100, 60 };
            if (heightGraphY > 22.0f) graphColor = (Color){ 255, 69, 0, 85 };

            DrawCube((Vector3){ x, heightGraphY / 2.0f + 0.5f, z }, meshStep - 1.5f, heightGraphY, meshStep - 1.5f, graphColor);
        }
    }

    // High contrast structural labels rendering inside the 3D viewport canvas
    if (transmitterPlaced) {
        DrawCube(transmitterPos, 5.0f, 25.0f, 5.0f, GOLD);
        DrawCubeWires(transmitterPos, 5.1f, 25.1f, 5.1f, BLACK);
        DrawCube((Vector3){ transmitterPos.x, transmitterPos.y + 26.0f, transmitterPos.z }, 8.0f, 2.0f, 2.0f, ORANGE); // Text plate proxy
    }
    
    if (receiverPlaced) {
        float bounceY = receiverPos.y + 6.0f + sin(time * 5.0f) * 1.5f;
        rlPushMatrix();
            rlTranslatef(receiverPos.x, bounceY, receiverPos.z);
            rlRotatef(time * 90.0f, 0.0f, 1.0f, 0.0f);
            DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 6.0f, 6.0f, 6.0f, MAGENTA);
            DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, 6.1f, 6.1f, 6.1f, WHITE);
        rlPopMatrix();
    }

    if (transmitterPlaced && receiverPlaced) {
        float linkDist = sqrt(pow(transmitterPos.x - receiverPos.x, 2) + pow(transmitterPos.z - receiverPos.z, 2));
        float pLoss = 20.0f * log10(linkDist + 1.0f) + 32.4f;
        Color trackingRayColor = (pLoss > 75.0f) ? RED : GREEN;
        DrawLine3D((Vector3){ transmitterPos.x, 15.0f, transmitterPos.z }, (Vector3){ receiverPos.x, 6.0f, receiverPos.z }, trackingRayColor);
        DrawBillboardSignalIcon(receiverPos, pLoss);
    }
}

void DrawInfrastructure(const std::vector<Building>& architecture, const std::vector<Human>& pop, float time) {
    DrawCube((Vector3){ 400.0f, -0.2f, -400.0f }, 2500.0f, 0.4f, 2500.0f, (Color){ 236, 240, 241, 255 });
    DrawCube((Vector3){ 550.0f, -0.6f, -500.0f }, 850.0f, 2.0f, 850.0f, (Color){ 52, 152, 219, 255 });
    DrawCube((Vector3){ 0.0f, 0.1f, -1000.0f }, 16.0f, 0.2f, 2000.0f, (Color){ 52, 73, 94, 255 }); 
    DrawCube((Vector3){ 550.0f, 0.1f, 0.0f }, 1100.0f, 0.2f, 16.0f, (Color){ 52, 73, 94, 255 }); 

    for (const auto& b : architecture) {
        bool isVertical = (b.position.x < -10.0f);
        if (b.type == 0) {
            DrawCube(b.position, b.size.x, b.size.y, b.size.z, b.color);
        } else if (b.type == 1) {
            DrawOrientedSociety(b.position, b.size.y, b.color, b.floors, isVertical, !isVertical);
        } else if (b.type == 4) {
            DrawCube(b.position, b.size.x, b.size.y, b.size.z, DARKGRAY);
            Color beacon = (sin(time * 8.0f) > 0) ? RED : (Color){ 80, 20, 20, 255 };
            DrawSphere((Vector3){ b.position.x, b.position.y + b.size.y/2.0f, b.position.z }, 2.5f, beacon);
        }
    }
    for (const auto& human : pop) { DrawRealisticHuman(human.position, human.color, human.walkPhase); }
}

void DrawTraffic(const std::vector<Vehicle>& traffic, float time) {
    for (const auto& v : traffic) {
        float yaw = v.movingVertical ? 0.0f : 90.0f;
        rlPushMatrix();
            rlTranslatef(v.position.x, v.position.y, v.position.z);
            rlRotatef(yaw, 0.0f, 1.0f, 0.0f); 
            DrawCube((Vector3){ 0.0f, 1.0f, 0.0f }, 4.6f, 1.8f, 8.5f, v.color);
            float r = 1.1f; float w = 0.8f; float zF = 2.4f; float xOff = 2.4f; float yC = 0.6f;
            DrawCylinderEx((Vector3){ -xOff, yC, zF }, (Vector3){ -xOff + w, yC, zF }, r, r, 8, BLACK);
        rlPopMatrix();
    }
}
