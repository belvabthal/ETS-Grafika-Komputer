#include "screen_simulation.h"
#include "coords.h"
#include "screen_type.h"
#include "src/algo/bresenham.h" 
#include "src/objects/env.h"    
#include "src/objects/car.h"
#include "src/objects/tent.h"
#include "src/objects/person.h"
#include "src/objects/bear.h"

#include <stdio.h>
#include <math.h>

// --- STATE ANIMASI & WAKTU ---
static float simTimer = 0.0f; 
static bool isPlaying = false; 
static float timeScale = 1.0f; 

// --- STATE KAMERA ---
static int originalOriginX = 0;
static int originalOriginY = 0;
static int originalTickStep = 0;
static bool isCameraInitialized = false;

static float panOffsetX = 0.0f; 
static float panOffsetY = 0.0f;
static float targetZoom = 30.0f; 
static float currentZoomFloat = 30.0f; 

// --- Challange ---
static bool isBluePrintMode = false;
static int weatherState = 0;

static Color LerpColor(Color a, Color b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (Color){
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        255
    };
}

static float Lerp(float a, float b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return a + (b - a) * t;
}

void DrawSimulationScreen(UIButton btnBack, int* currentScreen) {
    // INISIALISASI KAMERA & STATE
    if (!isCameraInitialized) {
        originalOriginX = G_OriginX;
        originalOriginY = G_OriginY;
        originalTickStep = G_TickStep;
        
        targetZoom = 20.0f; 
        currentZoomFloat = 20.0f;
        panOffsetX = 0.0f;
        panOffsetY = 0.0f;
        
        simTimer = 0.0f;
        isPlaying = false; 
        timeScale = 1.0f; 
        
        isCameraInitialized = true;
    }

    bool isSequenceDone = (simTimer >= 90.0f);

    if (isPlaying) {
        simTimer += GetFrameTime() * timeScale; 
    }

    // TIMING OBJEK UTAMA
    float carX;
    float carScale = 1.2f;
    float groundLevel = -1.0f;
    float carSpeed = 3.0f;
    
    if (simTimer < 25.0f) {
        carX = 93.0f - (simTimer * carSpeed);
    } else {
        carX = 18.0f; 
    }

    // 3. MATEMATIKA KAMERA DINAMIS
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        targetZoom += wheel * 10.0f; 
        if (targetZoom < 10.0f) targetZoom = 10.0f;   
        if (targetZoom > 40.0f) targetZoom = 40.0f; 
    }
    
    if (isPlaying && simTimer > 5.0f && simTimer < 20.0f && wheel == 0.0f && targetZoom == 20.0f) {
        targetZoom = Lerp(20.0f, 38.0f, (simTimer - 5.0f) / 15.0f);
    }

    currentZoomFloat = Lerp(currentZoomFloat, targetZoom, 10.0f * GetFrameTime());
    G_TickStep = (int)roundf(currentZoomFloat);

    float focusX;
    if (simTimer < 25.0f) {
        focusX = carX; 
    } else {
        float shiftPhase = (simTimer - 25.0f) / 4.0f; 
        focusX = Lerp(18.0f, 4.5f, shiftPhase); 
    }

    float baseOriginX = (SCREEN_W * 0.5f) - (focusX * G_TickStep);
    float baseOriginY = (SCREEN_H * 0.7f) - (groundLevel * G_TickStep);

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        panOffsetX += delta.x;
        panOffsetY += delta.y;
    } else {
        panOffsetX = Lerp(panOffsetX, 0.0f, 5.0f * GetFrameTime());
        panOffsetY = Lerp(panOffsetY, 0.0f, 5.0f * GetFrameTime());
    }

    // Kunci Sumbu X (Kiri & Kanan)
    float worldMinX = -50.0f; // Batas mentok kiri (Posisi bintang/pohon terakhir)
    float worldMaxX = 100.0f; // Batas mentok kanan (Titik awal mobil)

    float maxOriginX = -(worldMinX * G_TickStep);
    float minOriginX = SCREEN_W - (worldMaxX * G_TickStep);

    // Kunci nilai pergeseran jika melewati batas matematika
    if (baseOriginX + panOffsetX > maxOriginX) panOffsetX = maxOriginX - baseOriginX;
    if (baseOriginX + panOffsetX < minOriginX) panOffsetX = minOriginX - baseOriginX;

    // Kunci Sumbu Y (Atas & Bawah)
    float worldMaxY = 95.0f; // Batas mentok atas (Sedikit di atas ruang bintang tertinggi Y=90)
    float worldMinY = -15.0f; // Batas mentok bawah (Agar tidak melihat tanah terlalu dalam)

    float maxOriginY = worldMaxY * G_TickStep;
    float minOriginY = SCREEN_H + (worldMinY * G_TickStep);

    // Kunci nilai pergeseran jika melewati batas angkasa atau kedalaman tanah
    if (baseOriginY + panOffsetY > maxOriginY) panOffsetY = maxOriginY - baseOriginY;
    if (baseOriginY + panOffsetY < minOriginY) panOffsetY = minOriginY - baseOriginY;

    G_OriginX = (int)roundf(baseOriginX + panOffsetX);
    G_OriginY = (int)roundf(baseOriginY + panOffsetY);

    // WARNA LANGIT & TANAH
    Color skyDay   = (Color){135, 206, 235, 255}; 
    Color skyDusk  = (Color){255, 140,  0, 255};  
    Color skyNight = (Color){ 15,  20,  40, 255}; 

    Color currentSky;
    if (simTimer < 8.0f) currentSky = skyDay; 
    else if (simTimer < 16.0f) currentSky = LerpColor(skyDay, skyDusk, (simTimer - 8.0f) / 8.0f); 
    else if (simTimer < 24.0f) currentSky = LerpColor(skyDusk, skyNight, (simTimer - 16.0f) / 8.0f);
    else if (simTimer < 75.0f) currentSky = skyNight; 
    else if (simTimer < 82.0f) currentSky = LerpColor(skyNight, skyDusk, (simTimer - 75.0f) / 7.0f); 
    else if (simTimer < 90.0f) currentSky = LerpColor(skyDusk, skyDay, (simTimer - 82.0f) / 8.0f); 
    else currentSky = skyDay;

    ClearBackground(currentSky);

    if (IsKeyPressed(KEY_O)) {
        isBluePrintMode = !isBluePrintMode;
    }

    int groundY = MAP_Y(groundLevel);
    Color groundCol;
    if (simTimer < 24.0f) {
        groundCol = LerpColor((Color){50, 100, 40, 255}, (Color){15, 25, 15, 255}, simTimer/24.0f);
    } else if (simTimer < 75.0f) {
        groundCol = (Color){15, 25, 15, 255};
    } else {
        groundCol = LerpColor((Color){15, 25, 15, 255}, (Color){50, 100, 40, 255}, (simTimer - 75.0f)/15.0f);
    }
    
    DrawRectangle(0, groundY, SCREEN_W, 2000, groundCol);
    Bres_ThickLine(0, groundY, SCREEN_W, groundY, 4, DARKBROWN);

    // BINTANG MUNCUL DAN MEMUDAR
    if (simTimer > 24.0f && simTimer < 85.0f) { 
        float starAlpha = 1.0f;
        if (simTimer < 26.0f) starAlpha = (simTimer - 24.0f) / 2.0f; // Fade In (Malam)
        else if (simTimer > 80.0f) starAlpha = 1.0f - ((simTimer - 80.0f) / 5.0f); // Fade Out (Pagi)

        if (starAlpha > 1.0f) starAlpha = 1.0f; 
        if (starAlpha < 0.0f) starAlpha = 0.0f;

        float camMinX = (0 - G_OriginX) / (float)G_TickStep;
        float camMaxX = (SCREEN_W - G_OriginX) / (float)G_TickStep;

        DrawStarField(camMinX - 1.0f, camMaxX + 1.0f, simTimer, starAlpha, isBluePrintMode);
    }

    // VIEW FRUSTUM CULLING (HUTAN)
    float camMinX = (0 - G_OriginX) / (float)G_TickStep;
    float camMaxX = (SCREEN_W - G_OriginX) / (float)G_TickStep;

    for (float tx = 100.0f; tx >= -50.0f; tx -= 3.5f) {
        if (tx >= camMinX - 3.0f && tx <= camMaxX + 3.0f) {
            if (tx < -5.0f || tx > 20.0f) { 
                float randomScale = 1.5f + (fabs(sinf(tx)) * 0.8f); 
                DrawComplexTree(tx, groundLevel + randomScale, randomScale, isBluePrintMode);
            }
        }
    }

    // RENDER KAMP & OBJEK UTAMA
    // 1. TENDA 
    DrawComplexTent(-4.0f, groundLevel, 1.6f, isBluePrintMode);

    // 2. BERUANG MUNCUL (Jam 10 Malam / simTimer = 40.0 sampai 45.0)
    // 2. BERUANG MUNCUL (Jam 10 Malam / simTimer = 40.0 sampai 45.0)
    if (simTimer > 40.0f) {
        float bearX = -15.0f; // Mulai dari luar layar kiri
        float bearAnim = 0.0f;
        float bearScale = 1.4f; // Ukuran beruang

        if (simTimer < 45.0f) { 
            // Fase 1: Jalan mendekati tenda (sampai X = -8.0)
            // Jarak tempuh 7 unit dalam 5 detik -> kecepatan 1.4
            bearX = -15.0f + ((simTimer - 40.0f) * 1.4f); 
            bearAnim = simTimer;
        } else if (simTimer < 47.0f) { 
            // Fase 2: Berhenti dan menatap orang (2 detik ketegangan di X = -8.0)
            bearX = -8.0f;
            bearAnim = 0.0f; // Mode Idle (Bernapas/Diam)
        } else if (simTimer < 52.0f) { 
            // Fase 3: Orang kabur, beruang maju ke tempat api unggun (Target X = -1.0)
            // Baru mulai jalan di detik 49 agar memberi waktu orangnya lari duluan
            if (simTimer > 49.0f) {
                // Jarak tempuh 7 unit (-8 ke -1) dalam 3 detik (52 - 49)
                bearX = -8.0f + ((simTimer - 49.0f) * (7.0f / 3.0f)); 
                bearAnim = simTimer;
            } else {
                bearX = -8.0f; // Masih diam melihat orang lari
            }
        } else if (simTimer < 70.0f) { 
            // Fase 4: Menikmati kehangatan api unggun di X = -1.0
            bearX = -1.0f;
            bearAnim = 0.0f; 
        } else { 
            // Fase 5: Api mati, beruang pergi ke kanan hutan (Mulai dari X = -1.0)
            bearX = -1.0f + ((simTimer - 70.0f) * 2.0f);
            bearAnim = simTimer;
        }
        
        // Render beruang SEBELUM mobil agar bisa tertutup bodi mobil saat lewat
        DrawComplexBear(bearX, groundLevel + bearScale, bearScale, isBluePrintMode, bearAnim);
    }

    // 3. LOGIKA ORANG & API UNGGUN
    if (simTimer > 25.5f) {
        float p2Time = simTimer - 25.5f; 
        float personScale = 1.1f;
        float personX = 1.5f; // Posisi santai di api
        float personWalkAnim = 0.0f;
        bool isHiddenInTent = false;

        if (p2Time < 5.0f) { 
            // Jalan dari mobil ke api
            personX = 17.0f - (p2Time * 3.1f); 
            personWalkAnim = p2Time; 
        } else if (simTimer < 47.0f) { 
            // Santai di api (sampai detik 47, yaitu 2 detik setelah beruang muncul)
            personX = 1.5f; 
            personWalkAnim = 5.1f + (p2Time - 5.0f); 
        } else if (simTimer < 49.0f) { 
            // PANIK KABUR KE TENDA! (Berlari ke X = -4.0)
            personX = 1.5f - ((simTimer - 47.0f) * 3.5f); 
            personWalkAnim = 0.1f + ((simTimer - 47.0f) * 2.5f);
        } else {
            isHiddenInTent = true; 
        }

        if (!isHiddenInTent) {
            DrawComplexPerson(personX, groundLevel + personScale, personScale, isBluePrintMode, personWalkAnim);
        }

        // LOGIKA API UNGGUN
        float fireAnimState = -1.0f; 
        if (p2Time > 5.5f && simTimer < 70.0f) {
            fireAnimState = p2Time - 5.5f; 
        }
        DrawComplexCampfire(4.5f, groundLevel, 1.0f, isBluePrintMode, fireAnimState);
    }

    // 4. MOBIL (Digambar paling terakhir agar menutupi beruang yang lewat di belakangnya)
    float wheelAngle = carX / carScale; 
    DrawComplexCar(carX, groundLevel + carScale, carScale, isBluePrintMode, wheelAngle);

    // --- Cuaca ---
    if (weatherState == 1) {
         DrawRainWeather(SCREEN_W, SCREEN_H, simTimer, isBluePrintMode);
    }

    // UI MEDIA CONTROLS 
    if (IsKeyPressed(KEY_W)) weatherState = (weatherState == 0) ? 1 : 0;

    // Tombol Kembali ke Menu
    if (GuiButton(btnBack)) {
        G_OriginX = originalOriginX;
        G_OriginY = originalOriginY;
        G_TickStep = originalTickStep;
        isCameraInitialized = false;
        *currentScreen = SCREEN_MENU;
    }

    // --- KOTAK PANEL UTAMA ---
    int panelW = 700; 
    int panelH = 70;
    int panelX = (SCREEN_W / 2) - (panelW / 2);
    int panelY = SCREEN_H - 90;
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.6f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, colLizard);

    const char* txtOutline = isBluePrintMode ? "OUTLINE: ON" : "OUTLINE: OFF";
    Color colOutBg = isBluePrintMode ? colBrightGold : colGermanDark;
    Color colOutHov = isBluePrintMode ? colAcorn : colFlatGreen;
    UIButton btnSimOutline = {{panelX + 20, panelY + 15, 140, 40}, txtOutline, colOutBg, colOutHov, colWhiteText};

    const char* txtWeather = (weatherState == 1) ? "CUACA: HUJAN" : "CUACA: CERAH";
    Color colWeaBg = (weatherState == 1) ? colLizard : colGermanDark;
    Color colWeaHov = (weatherState == 1) ? colFlatGreen : colFlatGreen;
    UIButton btnSimWeather = {{panelX + 170, panelY + 15, 140, 40}, txtWeather, colWeaBg, colWeaHov, colWhiteText};

    UIButton btnSpeedDown = {{panelX + 330, panelY + 15, 50, 40}, "<<", colFlatGreen, colLizard, colWhiteText};
    
    const char* playText = isSequenceDone ? "RESTART" : (isPlaying ? "PAUSE" : "PLAY");
    Color playBgCol = isSequenceDone ? colAcorn : (isPlaying ? colGermanDark : colFlatGreen);
    Color playHovCol = isSequenceDone ? colBrightGold : (isPlaying ? colFlatGreen : colLizard);
    UIButton btnPlay = {{panelX + 390, panelY + 15, 120, 40}, playText, playBgCol, playHovCol, colWhiteText};
    
    UIButton btnSpeedUp = {{panelX + 520, panelY + 15, 50, 40}, ">>", colFlatGreen, colLizard, colWhiteText};

    if (GuiButton(btnSimOutline)) isBluePrintMode = !isBluePrintMode;
    if (GuiButton(btnSimWeather)) weatherState = (weatherState == 0) ? 1 : 0;

    if (GuiButton(btnSpeedDown)) {
        timeScale -= 0.5f; 
        if (timeScale < 0.5f) timeScale = 0.5f; 
    }
    
    if (GuiButton(btnPlay)) {
        if (isSequenceDone) { 
            simTimer = 0.0f;
            timeScale = 1.0f;
            isPlaying = true;
        } else { 
            isPlaying = !isPlaying;
        }
    }

    if (GuiButton(btnSpeedUp)) {
        timeScale += 0.5f; 
        if (timeScale > 5.0f) timeScale = 5.0f; 
    }

    // --- TEKS INDIKATOR KECEPATAN (UJUNG KANAN) ---
    char timeTxt[30]; 
    snprintf(timeTxt, 30, "%.1fx | %02ds", timeScale, (int)simTimer);
    DrawText(timeTxt, panelX + 590, panelY + 27, 16, colBrightGold);

    
    float gameHour = 7.0f; 
    if (simTimer <= 31.0f) {
        gameHour = 7.0f + (simTimer / 31.0f) * 12.0f;
    } else if (simTimer <= 90.0f) {
        gameHour = 19.0f + ((simTimer - 31.0f) / 59.0f) * 12.0f;
    }

    int displayHour = ((int)gameHour) % 24;
    int displayMin = (int)((gameHour - (int)gameHour) * 60);

    DrawRectangle(20, 15, 170, 45, Fade(BLACK, 0.6f));
    DrawRectangleLines(20, 15, 170, 45, colBrightGold);
    DrawText(TextFormat("%02d : %02d", displayHour, displayMin), 35, 22, 30, RAYWHITE);
    
    const char* periodText = (displayHour >= 6 && displayHour < 18) ? "DAY" : "NIGHT";
    Color periodCol = (displayHour >= 6 && displayHour < 18) ? SKYBLUE : colFlatGreen;
    DrawText(periodText, 135, 28, 16, periodCol);

    int currentFPS = GetFPS();
    
    Color fpsCol = (currentFPS >= 50) ? colLizard : ((currentFPS >= 30) ? colBrightGold : RED);
    
    DrawRectangle(SCREEN_W - 130, 15, 110, 35, Fade(BLACK, 0.5f));
    DrawRectangleLines(SCREEN_W - 130, 15, 110, 35, fpsCol);
    
    // Tampilkan teks FPS menggunakan TextFormat bawaan Raylib
    DrawText(TextFormat("FPS: %d", currentFPS), SCREEN_W - 110, 23, 20, fpsCol);
}
