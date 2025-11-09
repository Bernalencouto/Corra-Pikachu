#include "raylib.h"
#include "jogador.h"
#include "obstaculo.h"
#include <stdlib.h>
#include <time.h>

typedef enum GameScreen { 
    MENU, 
    GAMEPLAY,
    GAME_OVER
} GameScreen;

#define OBSTACULO_INTERVALO_SPAWN 1.5f

void ResetarJogo(Pikachu *player, NodoObstaculo **listaDeObstaculos, float *spawnTimer)
{
    player->posicao = (Vector2){100, 300};
    player->velocidadeVertical = 0;
    player->pulosRestantes = 2;

    LimparObstaculos(listaDeObstaculos); 

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

    Image pokeballImage = LoadImage("resources/pokeball.png");
    ImageResize(&pokeballImage, 30, 30);
    Texture2D pokeballTexture = LoadTextureFromImage(pokeballImage);
    UnloadImage(pokeballImage);

    NodoObstaculo *listaDeObstaculos = NULL; 
    float spawnTimer = 0.0f; 

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();

        switch(estadoAtual)
        {
            case MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoAtual = GAMEPLAY; 
                    ResetarJogo(&player, &listaDeObstaculos, &spawnTimer); 
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
                    AdicionarObstaculo(&listaDeObstaculos, pokeballTexture);
                }

                AtualizarObstaculos(&listaDeObstaculos, deltaTime);

                if (ChecarColisaoObstaculos(listaDeObstaculos, player.colisao))
                {
                    estadoAtual = GAME_OVER;
                }
                
            } break;
            
            case GAME_OVER:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoAtual = MENU;
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

                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);
                    
                    DrawFPS(screenWidth - 100, 10);
                } break;
                
                case GAME_OVER:
                {
                    DrawTextureRec(pikachuTextura, frameRec, player.posicao, WHITE);
                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);

                    DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(BLACK, 0.6f));

                    DrawText("GAME OVER", 280, 150, 40, RED);
                    DrawText("Aperte ENTER para voltar ao Menu", 230, 220, 20, RAYWHITE);
                    
                } break;
            }

        EndDrawing();
    }

    LimparObstaculos(&listaDeObstaculos);
    UnloadTexture(pokeballTexture);
    UnloadTexture(pikachuTextura);
    UnloadImage(pikachuAnim);
    CloseWindow();
    
    return 0;
}