#pragma once
#include "raylib.h"
#include <vector>

struct Human {
    Vector3 position;
    Color color;
    float walkPhase;
    bool isOnCall;
    float signalRadius;
};

struct Vehicle {
    Vector3 position;
    Color color;
    bool isAmbulance;
    float speed;
    bool movingVertical;
    float wheelRot;
};

struct Building {
    Vector3 position;
    Vector3 size;
    Color color;
    int type; // 0: Monolith, 1: Society, 2: House, 3: Hospital, 4: Tower
    int floors;
};

// Global Interactive Node Tracking Parameters
extern Vector3 transmitterPos;
extern Vector3 receiverPos;
extern bool transmitterPlaced;
extern bool receiverPlaced;

void InitializeCity(std::vector<Human>& pop, std::vector<Vehicle>& traffic, std::vector<Building>& architecture);
void DrawInfrastructure(const std::vector<Building>& architecture, const std::vector<Human>& pop, float time);
void DrawTraffic(const std::vector<Vehicle>& traffic, float time);
void DrawNetworkOverlay(const std::vector<Building>& architecture, float time);
void DrawBillboardSignalIcon(Vector3 pos, float pathLossDb);
