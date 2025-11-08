#include "raylib.h"
#include "jogador.h"
#include <stdlib.h>
#include <time.h> 

typedef enum GameScreen { 
    MENU, 
    GAMEPLAY
} GameScreen;

#define MAX_OBSTACULOS 50
#define OBSTACULO_VELOCIDADE 300.0f
#define OBSTACULO_INTERVALO_SPAWN 1.5f

typedef struct Obstaculo {
    Rectangle rec;
    Color cor;
    bool active;
} Obstaculo;

void ResetarJogo(Pikachu *player, Obstaculo obstaculos[], float *spawnTimer)
{
    player->posicao = (Vector2){100, 300};
    player->velocidadeVertical = 0;
    player->pulosRestantes = 2;
    
    for (int i = 0; i < MAX_OBSTACULOS; i++)
    {
        obstaculos[i].active = false;
    }
    
    *spawnTimer = OBSTACULO_INTERVALO_SPAWN;
}


int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Corra, Pikachu!");
    SetTargetFPS(60); 
    srand(time(NULL)); 

    GameScreen estadoAtual = MENU;

    int totalFrames = 0;
    Image pikachuAnim = LoadImageAnim("resources/pikachu.gif", &totalFrames);
    int frameLarguraOriginal = pikachuAnim.width;
    int frameAlturaOriginal = pikachuAnim.height / totalFrames;
    int novaAlturaFrame = 60; 
    float escala = (float)novaAlturaFrame / frameAlturaOriginal;
    int novaLarguraFrame = (int)(frameLarguraOriginal * escala);

    ImageResize(&pikachuAnim, novaLarguraFrame, novaAlturaFrame * totalFrames);
    
    Texture2D pikachuTextura = LoadTextureFromImage(pikachuAnim);
    
    int frameLargura = pikachuAnim.width;
    int frameAltura = pikachuAnim.height / totalFrames;
    
    Pikachu player = criarPikachu(100, 300, frameLargura, frameAltura);

    int frameAtual = 0;
    float frameTimer = 0.0f;
    float frameDelay = 1.0f / 10.0f;
    
    Rectangle frameRec = { 0.0f, 0.0f, (float)frameLargura, (float)frameAltura };


    Obstaculo obstaculos[MAX_OBSTACULOS] = {0}; 
    float spawnTimer = 0.0f; 
    for (int i = 0; i < MAX_OBSTACULOS; i++)
    {
        obstaculos[i].rec.width = 30;
        obstaculos[i].rec.height = 30;
        obstaculos[i].cor = RED;
        obstaculos[i].active = false;
    }

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();

        switch(estadoAtual)
        {
            case MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoAtual = GAMEPLAY; 
                    ResetarJogo(&player, obstaculos, &spawnTimer); 
                }
            } break;

            case GAMEPLAY:
            {

                atualizarPikachu(&player, deltaTime);
                if (IsKeyPressed(KEY_SPACE)) {
                    pularPikachu(&player);
                }
                
                frameTimer += deltaTime;
                if (frameTimer >= frameDelay) {
                    frameTimer = 0.0f;
                    frameAtual++;
                    if (frameAtual >= totalFrames) frameAtual = 0;
                    
                    frameRec.y = (float)frameAtual * frameAltura;

                    
                }
                
                spawnTimer += deltaTime;
                if (spawnTimer >= OBSTACULO_INTERVALO_SPAWN)
                {
                    spawnTimer = 0.0f;
                    for (int i = 0; i < MAX_OBSTACULOS; i++)
                    {
                        if (!obstaculos[i].active)
                        {
                            obstaculos[i].active = true;
                            obstaculos[i].rec.x = screenWidth + obstaculos[i].rec.width;
                            obstaculos[i].rec.y = 300 + (rand() % 100); 
                            break; 
                        }
                    }
                }

                for (int i = 0; i < MAX_OBSTACULOS; i++)
                {
                    if (obstaculos[i].active)
                    {
                        obstaculos[i].rec.x -= OBSTACULO_VELOCIDADE * deltaTime;
                        if (obstaculos[i].rec.x < -obstaculos[i].rec.width)
                        {
                            obstaculos[i].active = false;
                        }
                    }
                }
                
            } break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE); 
            
            switch(estadoAtual)
            {
                case MENU:
                {
                    DrawText("CORRA, PIKACHU!", 240, 150, 40, DARKGRAY);
                    DrawText("Aperte ENTER para começar", 250, 220, 20, GRAY);
                } break;

                case GAMEPLAY:
                {

                    DrawTextureRec(pikachuTextura, frameRec, player.posicao, WHITE);
                    

                    for (int i = 0; i < MAX_OBSTACULOS; i++)
                    {
                        if (obstaculos[i].active)
                        {
                            DrawRectangleRec(obstaculos[i].rec, obstaculos[i].cor);
                        }
                    }
                    
                    DrawFPS(screenWidth - 100, 10);
                } break;
            }

        EndDrawing();
    }

    UnloadTexture(pikachuTextura);
    UnloadImage(pikachuAnim);
    CloseWindow();
    return 0;
}