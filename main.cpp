#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "raylib.h"

enum ElementName = {
    None         = 0,
    Hydrogen     = 1,
    Helium       = 2,
    Lithium      = 3,
    Beryllium    = 4,
    Boron        = 5,
    Carbon       = 6,
    Nitrogen     = 7,
    Oxygen       = 8,
    Fluorine     = 9,
    Neon         = 10,
    Sodium       = 11,
    Magnesium    = 12,
    Aluminum     = 13,
    Silicon      = 14,
    Phosphorus   = 15,
    Sulfur       = 16,
    Chlorine     = 17,
    Argon        = 18,
    Potassium    = 19,
    Calcium      = 20
};

class Element {
public: // Make public for simplicity in this project
    ElementName name;
    std::string symbol;
    int atomicNumber;
    Rectangle bounds;
    std::vector<int> shells; // Store electrons per shell: e.g. {2, 8, 1}

    Element(ElementName n, std::string s, int num, Vector2 pos, std::vector<int> sh) 
        : name(n), symbol(s), atomicNumber(num), shells(sh) {
        bounds = { pos.x, pos.y, 60, 70 }; 
    }

    void DrawTile() {
        DrawRectangleRec(bounds, DARKBLUE);
        DrawText(symbol.c_str(), bounds.x + 15, bounds.y + 20, 20, WHITE);
    }

    void DrawVisualizer(float time) {
        Vector2 center = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        DrawCircleV(center, 20, RED); // Nucleus

        for (size_t i = 0; i < shells.size(); i++) {
            float radius = 80.0f + (i * 40.0f);
            DrawCircleLinesV(center, radius, LIGHTGRAY);
            
            for (int j = 0; j < shells[i]; j++) {
                float angle = time + (j * (2 * PI / shells[i]));
                float x = center.x + cos(angle) * radius;
                float y = center.y + sin(angle) * radius;
                DrawCircle(x, y, 6, GOLD);
            }
        }
        DrawText("Press 'ESC' to go back", 20, 20, 20, DARKGRAY);
    }

    bool isClicked(Vector2 mousePos) {
        return CheckCollisionPointRec(mousePos, bounds);
    }
};


class Table {
	std::vector<Element> grid;
    Table() : {}
    
    void InitTable() {
    	for (Element &e : elems) {
        	grid.push_back(Element(e.name, e.symbol, e.atomicNumber));
           }
    }
    
    void DrawTable() {
    	for (Element &e : elems) {
        	e.DrawTile();
        }
    }
};


int main() {
    InitWindow(1200, 800, "Periodic Table");
    
    // Setup first few elements
    std::vector<Element> table;
    table.push_back(Element(ElementName::Hydrogen, "H", 1, {100, 100}, {1}));
    table.push_back(Element(ElementName::Helium, "He", 2, {1000, 100}, {2}));
    table.push_back(Element(ElementName::Lithium, "Li", 3, {100, 180}, {2, 1}));

    Element* selected = nullptr;

    while (!WindowShouldClose()) {
        // Update
        if (selected) {
            if (IsKeyPressed(KEY_ESCAPE)) selected = nullptr;
        } else {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (auto& e : table) {
                    if (e.isClicked(GetMousePosition())) selected = &e;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (selected) {
            selected->DrawVisualizer(GetTime());
        } else {
            for (auto& e : table) e.DrawTile();
        }

        EndDrawing();
    }
    CloseWindow();
}


