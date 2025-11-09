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
    player->posicao = (Vector2){100, GetScreenHeight() * 0.7f};
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

    Texture2D cenarioInicial = LoadTexture("resources/cenario_inicial.jpg");
    Texture2D cenarioJogo = LoadTexture("resources/cenario_jogo2.jpg");
    float posicaoCenarioX = 0.0f;

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
    
    player.posicao = (Vector2){100, GetScreenHeight() * 0.7f};
    player.colisao.height = (float)frameAltura;
    player.colisao.width = (float)frameLargura;

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
                float cenarioScale = (float)GetScreenHeight() / cenarioJogo.height;
                float cenarioScaledWidth = cenarioJogo.width * cenarioScale;
                
                posicaoCenarioX -= 100.0f * deltaTime;
                if (posicaoCenarioX <= -cenarioScaledWidth)
                {
                    posicaoCenarioX = 0;
                }

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
            
            float scale, scaledWidth;
            
            switch(estadoAtual)
            {
                case MENU:
                {
                    DrawTexturePro(
                        cenarioInicial, 
                        (Rectangle){ 0.0f, 0.0f, (float)cenarioInicial.width, (float)cenarioInicial.height }, 
                        (Rectangle){ 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() }, 
                        (Vector2){ 0, 0 }, 0.0f, WHITE
                    );
                    
                    int textWidth1 = MeasureText("CORRA, PIKACHU!", 40);
                    DrawText("CORRA, PIKACHU!", (GetScreenWidth() - textWidth1) / 2, GetScreenHeight() / 2 - 60, 40, DARKGRAY);
                    
                    int textWidth2 = MeasureText("Aperte ENTER para começar", 20);
                    DrawText("Aperte ENTER para começar", (GetScreenWidth() - textWidth2) / 2, GetScreenHeight() / 2, 20, GRAY);
                } break;

                case GAMEPLAY:
                {
                    scale = (float)GetScreenHeight() / cenarioJogo.height;
                    scaledWidth = cenarioJogo.width * scale;
                    
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    
                    DrawTextureRec(pikachuTextura, frameRec, player.posicao, WHITE);
                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);
                    
                    DrawFPS(GetScreenWidth() - 100, 10);
                } break;
                
                case GAME_OVER:
                {
                    scale = (float)GetScreenHeight() / cenarioJogo.height;
                    scaledWidth = cenarioJogo.width * scale;
                    
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    DrawTextureRec(pikachuTextura, frameRec, player.posicao, WHITE);
                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);

                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.6f));

                    int textWidthGO1 = MeasureText("GAME OVER", 40);
                    DrawText("GAME OVER", (GetScreenWidth() - textWidthGO1) / 2, GetScreenHeight() / 2 - 60, 40, RED);
                    
                    int textWidthGO2 = MeasureText("Aperte ENTER para voltar ao Menu", 20);
                    DrawText("Aperte ENTER para voltar ao Menu", (GetScreenWidth() - textWidthGO2) / 2, GetScreenHeight() / 2, 20, RAYWHITE);
                    
                } break;
            }

        EndDrawing();
    }

    LimparObstaculos(&listaDeObstaculos);
    UnloadTexture(pokeballTexture);
    UnloadTexture(pikachuTextura);
    UnloadImage(pikachuAnim);
    
    UnloadTexture(cenarioInicial);
    UnloadTexture(cenarioJogo);
    
    CloseWindow();
    
    return 0;
}