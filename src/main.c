#include "raylib.h"
#include "jogador.h"
#include "obstaculo.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef enum GameScreen { 
    MENU, 
    GAMEPLAY,
    GAME_OVER,
    WIN 
} GameScreen;

#define OBSTACULO_INTERVALO_SPAWN 1.3f
#define PONTOS_VITORIA 999999
#define VELOCIDADE_BASE 300.0f

int CarregarHiScore()
{
    int hiScore = 0;
    FILE *arquivo = fopen("highscore.txt", "r");
    if (arquivo != NULL)
    {
        fscanf(arquivo, "%d", &hiScore);
        fclose(arquivo);
    }
    return hiScore;
}

void SalvarHiScore(int score)
{
    FILE *arquivo = fopen("highscore.txt", "w");
    if (arquivo != NULL)
    {
        fprintf(arquivo, "%d", score);
        fclose(arquivo);
    }
}

void ResetarJogo(Pikachu *player, NodoObstaculo **listaDeObstaculos, float *spawnTimer, float *score, float *velocidadeAtual)
{
    player->posicao = (Vector2){100, GetScreenHeight() * 0.7f};
    player->velocidadeVertical = 0;
    player->pulosRestantes = 2;
    
    atualizarColisao(player); 

    LimparObstaculos(listaDeObstaculos); 

    *spawnTimer = OBSTACULO_INTERVALO_SPAWN;
    *score = 0.0f;
    *velocidadeAtual = VELOCIDADE_BASE;
}


int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Corra, Pikachu!");
    InitAudioDevice(); 
    
    SetTargetFPS(60); 
    srand(time(NULL)); 

    GameScreen estadoAtual = MENU;

    Texture2D cenarioInicial = LoadTexture("resources/cenario_inicial.jpg");
    Texture2D cenarioJogo = LoadTexture("resources/cenario_jogo2.jpg");
    Texture2D ashTexture = LoadTexture("resources/ash.png");
    float posicaoCenarioX = 0.0f;

    Texture2D pikachuTextura = LoadTexture("resources/pikachu_run_sheet.png");

    int totalFrames = 4; 
    int frameLargura = pikachuTextura.width / totalFrames; 
    int frameAltura = pikachuTextura.height;

    int novaAlturaFrame = 90;
    int novaLarguraFrame = 90;
    
    Pikachu player = criarPikachu(100, 300, novaLarguraFrame, novaLarguraFrame);

    int frameAtual = 0;
    float frameTimer = 0.0f;
    float frameDelay = 1.0f / 10.0f; 
    
    Rectangle frameRec = { 0.0f, 0.0f, (float)frameLargura, (float)frameAltura };

    Image pokeballImage = LoadImage("resources/pokeball.png");
    ImageResize(&pokeballImage, 30, 30);
    Texture2D pokeballTexture = LoadTextureFromImage(pokeballImage);
    UnloadImage(pokeballImage);

    Music musicMenu = LoadMusicStream("resources/music1.mp3");
    Music musicGameplay = LoadMusicStream("resources/music2.mp3");

    NodoObstaculo *listaDeObstaculos = NULL; 
    float spawnTimer = 0.0f;
    float score = 0.0f;
    int hiScore = CarregarHiScore();
    float velocidadeAtual = VELOCIDADE_BASE;

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();

        switch(estadoAtual)
        {
            case MENU:
                UpdateMusicStream(musicMenu);
                break;
            case GAMEPLAY:
                UpdateMusicStream(musicGameplay);
                break;
            default:
                break;
        }


        switch(estadoAtual)
        {
            case MENU:
            {
                if (!IsMusicStreamPlaying(musicMenu))
                {
                    PlayMusicStream(musicMenu);
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    StopMusicStream(musicMenu);
                    PlayMusicStream(musicGameplay);
                    
                    estadoAtual = GAMEPLAY; 
                    ResetarJogo(&player, &listaDeObstaculos, &spawnTimer, &score, &velocidadeAtual); 
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
                    frameRec.x = (float)frameAtual * frameLargura;
                }

                spawnTimer += deltaTime;
                if (spawnTimer >= OBSTACULO_INTERVALO_SPAWN)
                {
                    spawnTimer = 0.0f;
                    AdicionarObstaculo(&listaDeObstaculos, pokeballTexture);
                }

                int scoreAntigo = (int)score;
                
                score += 10.0f * deltaTime;
                
                int scoreNovo = (int)score;

                if ((scoreNovo / 100) > (scoreAntigo / 100))
                {
                    velocidadeAtual *= 1.01f;
                }

                AtualizarObstaculos(&listaDeObstaculos, deltaTime, velocidadeAtual);

                if (ChecarColisaoObstaculos(listaDeObstaculos, player.colisao))
                {
                    StopMusicStream(musicGameplay); 
                    estadoAtual = GAME_OVER;
                    if ((int)score > hiScore)
                    {
                        hiScore = (int)score;
                        SalvarHiScore(hiScore);
                    }
                }
                
                if (score >= PONTOS_VITORIA)
                {
                    StopMusicStream(musicGameplay); 
                    estadoAtual = WIN;
                    if ((int)score > hiScore)
                    {
                        hiScore = (int)score;
                        SalvarHiScore(hiScore);
                    }
                }
                
            } break;
            
            case GAME_OVER:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoAtual = MENU;
                    hiScore = CarregarHiScore();
                }
            } break;

            case WIN:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoAtual = MENU;
                    hiScore = CarregarHiScore();
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

                    int textWidthH = MeasureText(TextFormat("HI-SCORE: %06d", hiScore), 20);
                    DrawText(TextFormat("HI-SCORE: %06d", hiScore), (GetScreenWidth() - textWidthH) / 2, GetScreenHeight() / 2 + 40, 20, DARKGRAY);
                
                } break;

                case GAMEPLAY:
                {
                    scale = (float)GetScreenHeight() / cenarioJogo.height;
                    scaledWidth = cenarioJogo.width * scale;
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    DrawTexturePro(
                        pikachuTextura,   
                        frameRec,         
                        player.colisao,   
                        (Vector2){0, 0},    
                        0.0f,             
                        WHITE             
                    );

                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);

                    DrawText(TextFormat("HI-SCORE: %06d", hiScore), 20, 20, 20, WHITE);
                    DrawText(TextFormat("PONTOS: %06.0f", score), 20, 50, 20, WHITE);
                    DrawFPS(GetScreenWidth() - 100, 10);
                } break;
                
                case GAME_OVER:
                {
                    scale = (float)GetScreenHeight() / cenarioJogo.height;
                    scaledWidth = cenarioJogo.width * scale;
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioJogo, (Rectangle){0,0, (float)cenarioJogo.width, (float)cenarioJogo.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    DrawTexturePro(pikachuTextura, frameRec, player.colisao, (Vector2){0, 0}, 0.0f, WHITE);
                    DesenharObstaculos(listaDeObstaculos, pokeballTexture);
                    
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.6f));

                    int textWidthGO1 = MeasureText("GAME OVER", 40);
                    DrawText("GAME OVER", (GetScreenWidth() - textWidthGO1) / 2, GetScreenHeight() / 2 - 80, 40, RED);
                    
                    int textWidthS = MeasureText(TextFormat("Pontuação: %06.0f", score), 20);
                    DrawText(TextFormat("Pontuação: %06.0f", score), (GetScreenWidth() - textWidthS) / 2, GetScreenHeight() / 2 - 20, 20, RAYWHITE);
                    
                    int textWidthH = MeasureText(TextFormat("Hi-Score: %06d", hiScore), 20);
                    DrawText(TextFormat("Hi-Score: %06d", hiScore), (GetScreenWidth() - textWidthH) / 2, GetScreenHeight() / 2 + 10, 20, LIGHTGRAY);

                    int textWidthGO2 = MeasureText("Aperte ENTER para voltar ao Menu", 20);
                    DrawText("Aperte ENTER para voltar ao Menu", (GetScreenWidth() - textWidthGO2) / 2, GetScreenHeight() / 2 + 50, 20, RAYWHITE);
                } break;
                
                case WIN:
                {
                    DrawTexturePro(
                        cenarioInicial, 
                        (Rectangle){ 0.0f, 0.0f, (float)cenarioInicial.width, (float)cenarioInicial.height }, 
                        (Rectangle){ 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() }, 
                        (Vector2){ 0, 0 }, 0.0f, WHITE
                    );
                    DrawTexture(ashTexture, GetScreenWidth() / 2 - ashTexture.width / 2, 150, WHITE);
                    int tw1 = MeasureText("VOCÊ VENCEU!", 40);
                    DrawText("VOCÊ VENCEU!", (GetScreenWidth() - tw1) / 2, 50, 40, DARKGREEN);
                    int tw2 = MeasureText("Aperte ENTER para voltar ao Menu", 20);
                    DrawText("Aperte ENTER para voltar ao Menu", (GetScreenWidth() - tw2) / 2, GetScreenHeight() - 50, 20, GRAY);
                } break;
            }

        EndDrawing();
    }

    LimparObstaculos(&listaDeObstaculos);
    UnloadTexture(pokeballTexture);
    UnloadTexture(pikachuTextura); 
    UnloadTexture(cenarioInicial);
    UnloadTexture(cenarioJogo);
    UnloadTexture(ashTexture);
    
    UnloadMusicStream(musicMenu);
    UnloadMusicStream(musicGameplay);
    
    CloseAudioDevice();
    
    CloseWindow();
    
    return 0;
}