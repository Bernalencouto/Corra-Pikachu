#include "raylib.h"
#include "jogador.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Corra, Pikachu!");
    SetTargetFPS(60);


    int totalFrames = 0;
    
    Image pikachuAnim = LoadImageAnim("resources/pikachu.gif", &totalFrames);

    Texture2D pikachuTextura = LoadTextureFromImage(pikachuAnim);

    int frameLargura = pikachuAnim.width;
    int frameAltura = pikachuAnim.height / totalFrames;

    Pikachu player = criarPikachu(100, 300, frameLargura, frameAltura);

    int frameAtual = 0;
    float frameTimer = 0.0f;
    float frameDelay = 1.0f / 10.0f;

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();

        atualizarPikachu(&player, deltaTime);
        if (IsKeyPressed(KEY_SPACE)) {
            pularPikachu(&player);
        }


        frameTimer += deltaTime;
        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            frameAtual++;

            if (frameAtual >= totalFrames) {
                frameAtual = 0;
            }

            int frameOffset = frameLargura * frameAltura * 4 * frameAtual;

            unsigned char *dataPtr = (unsigned char *)pikachuAnim.data;
            
            UpdateTexture(pikachuTextura, dataPtr + frameOffset);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureV(pikachuTextura, player.posicao, WHITE);
            
            DrawText("Pressione ESPAÇO para pular!", 10, 10, 20, BLACK);
            DrawFPS(screenWidth - 100, 10);

        EndDrawing();
    }

    UnloadTexture(pikachuTextura);
    UnloadImage(pikachuAnim);
    CloseWindow();
    return 0;
}