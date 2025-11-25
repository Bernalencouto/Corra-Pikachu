#include "raylib.h"
#include "jogador.h"
#include "obstaculo.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef enum GameMode {
    MODO_HISTORIA,
    MODO_INFINITO
} GameMode;

typedef enum GameScreen { 
    MENU, 
    GAMEPLAY,
    GAME_OVER,
    WIN 
} GameScreen;

#define OBSTACULO_INTERVALO_SPAWN 1.3f
#define PONTOS_VITORIA_HISTORIA 4000 
#define VELOCIDADE_BASE 300.0f
#define PONTOS_ESTAGIO_2 1000
#define PONTOS_ESTAGIO_3 3000

#define CADEIRA_NOVA_LARGURA 51
#define CADEIRA_NOVA_ALTURA 80
#define MESA_NOVA_LARGURA 106
#define MESA_NOVA_ALTURA 80

#define ASH_ALTURA_TELA 135

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

void ResetarJogo(Pikachu *player, NodoObstaculo **listaDeObstaculos, float *spawnTimer, float *score, float *velocidadeAtual, int *estagio)
{
    player->posicao = (Vector2){100, 300};
    player->velocidadeVertical = 0;
    player->pulosRestantes = 2;
    player->estaNaPlataforma = false;
    
    atualizarColisao(player); 
    LimparObstaculos(listaDeObstaculos); 
    *spawnTimer = OBSTACULO_INTERVALO_SPAWN;
    *score = 0.0f;
    *velocidadeAtual = VELOCIDADE_BASE;
    *estagio = 1;
}

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Corra, Pikachu!");
    InitAudioDevice(); 
    
    SetTargetFPS(60); 
    srand(time(NULL)); 

    GameScreen estadoAtual = MENU;
    int estagioAtual = 1;
    
    GameMode modoDeJogo = MODO_HISTORIA;
    int menuSelecao = 0; 

    Texture2D cenarioInicial = LoadTexture("resources/cenario_inicial.jpg");
    Texture2D cenarioEstagio1 = LoadTexture("resources/cenario_jogo2.png");
    Texture2D cenarioEstagio2 = LoadTexture("resources/cenario_jogo.png");
    Texture2D cenarioEstagio3 = LoadTexture("resources/cenario_jogo3.jpg");
    
    Image ashImage = LoadImage("resources/ash.png");
    int ashNovaAltura = ASH_ALTURA_TELA; 
    int ashNovaLargura = (int)(ashImage.width * ( (float)ashNovaAltura / ashImage.height));
    ImageResize(&ashImage, ashNovaLargura, ashNovaAltura);
    Texture2D ashTexture = LoadTextureFromImage(ashImage);
    UnloadImage(ashImage);
    
    float posicaoCenarioX = 0.0f;

    Texture2D pikachuTextura = LoadTexture("resources/pikachu_run_sheet.png");

    int totalFrames = 4; 
    int frameLargura = pikachuTextura.width / totalFrames; 
    int frameAltura = pikachuTextura.height;
    
    int pikachuLarguraTela = 90;
    int pikachuAlturaTela = 90;
    Pikachu player = criarPikachu(100, 300, pikachuLarguraTela, pikachuAlturaTela);

    int frameAtual = 0;
    float frameTimer = 0.0f;
    float frameDelay = 1.0f / 10.0f; 
    
    Rectangle frameRec = { 0.0f, 0.0f, (float)frameLargura, (float)frameAltura };

    Image pokeballImage = LoadImage("resources/pokeball.png");
    ImageResize(&pokeballImage, 30, 30);
    Texture2D pokeballTexture = LoadTextureFromImage(pokeballImage);
    UnloadImage(pokeballImage);
    Image cadeiraImage = LoadImage("resources/obstaculo_cadeira.png");
    ImageResize(&cadeiraImage, CADEIRA_NOVA_LARGURA, CADEIRA_NOVA_ALTURA);
    Texture2D cadeiraTexture = LoadTextureFromImage(cadeiraImage);
    UnloadImage(cadeiraImage);
    Image mesaImage = LoadImage("resources/obstaculo_mesa.png");
    ImageResize(&mesaImage, MESA_NOVA_LARGURA, MESA_NOVA_ALTURA);
    Texture2D mesaTexture = LoadTextureFromImage(mesaImage);
    UnloadImage(mesaImage);

    Music musicMenu = LoadMusicStream("resources/music1.mp3");
    Music musicGameplay = LoadMusicStream("resources/music2.mp3");

    NodoObstaculo *listaDeObstaculos = NULL; 
    float spawnTimer = 0.0f;
    float score = 0.0f;
    int hiScore = CarregarHiScore();
    float velocidadeAtual = VELOCIDADE_BASE;
    
    Vector2 posicaoEncontroPikachu = { 0.0f, 0.0f };

    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();

        switch(estadoAtual)
        {
            case MENU: UpdateMusicStream(musicMenu); break;
            case GAMEPLAY: UpdateMusicStream(musicGameplay); break;
            default: break;
        }

        switch(estadoAtual)
        {
            case MENU:
            {
                if (!IsMusicStreamPlaying(musicMenu))
                {
                    PlayMusicStream(musicMenu);
                }
                
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                {
                    menuSelecao = (menuSelecao + 1) % 2; 
                }
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                {
                    menuSelecao = (menuSelecao - 1 + 2) % 2; 
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    StopMusicStream(musicMenu);
                    PlayMusicStream(musicGameplay);
                    
                    if (menuSelecao == 0) modoDeJogo = MODO_HISTORIA;
                    else modoDeJogo = MODO_INFINITO;
                    
                    estadoAtual = GAMEPLAY; 
                    ResetarJogo(&player, &listaDeObstaculos, &spawnTimer, &score, &velocidadeAtual, &estagioAtual); 
                }
            } break;

            case GAMEPLAY:
            {
                if (estagioAtual == 1 && score > PONTOS_ESTAGIO_2) estagioAtual = 2;
                if (estagioAtual == 2 && score > PONTOS_ESTAGIO_3) estagioAtual = 3;

                Texture2D cenarioAtual;
                if (estagioAtual == 1) cenarioAtual = cenarioEstagio1;
                else if (estagioAtual == 2) cenarioAtual = cenarioEstagio2;
                else cenarioAtual = cenarioEstagio3;

                float cenarioScale = (float)GetScreenHeight() / cenarioAtual.height;
                float cenarioScaledWidth = cenarioAtual.width * cenarioScale;
                posicaoCenarioX -= (velocidadeAtual / 3.0f) * deltaTime;
                while (posicaoCenarioX <= -cenarioScaledWidth)
                {
                    posicaoCenarioX += cenarioScaledWidth;
                }

                if (ChecarColisaoObstaculos(listaDeObstaculos, &player))
                {
                    StopMusicStream(musicGameplay); 
                    estadoAtual = GAME_OVER;
                    if ((int)score > hiScore)
                    {
                        hiScore = (int)score;
                        SalvarHiScore(hiScore);
                    }
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
                    float chaoY = 400.0f;
                    if (estagioAtual == 1)
                    {
                        AdicionarObstaculo(&listaDeObstaculos, pokeballTexture, TIPO_POKEBOLA, GetRandomValue(150, 300), 30, 30);
                    }
                    else if (estagioAtual == 2)
                    {
                        if (GetRandomValue(0, 2) == 0)
                            AdicionarObstaculo(&listaDeObstaculos, cadeiraTexture, TIPO_CADEIRA, chaoY - CADEIRA_NOVA_ALTURA, CADEIRA_NOVA_LARGURA, CADEIRA_NOVA_ALTURA);
                        else
                            AdicionarObstaculo(&listaDeObstaculos, pokeballTexture, TIPO_POKEBOLA, GetRandomValue(150, 300), 30, 30);
                    }
                    else if (estagioAtual == 3)
                    {
                        int sorteio = GetRandomValue(0, 3);
                        if (sorteio == 0)
                            AdicionarObstaculo(&listaDeObstaculos, cadeiraTexture, TIPO_CADEIRA, chaoY - CADEIRA_NOVA_ALTURA, CADEIRA_NOVA_LARGURA, CADEIRA_NOVA_ALTURA);
                        else if (sorteio == 1)
                            AdicionarObstaculo(&listaDeObstaculos, mesaTexture, TIPO_MESA, chaoY - MESA_NOVA_ALTURA, MESA_NOVA_LARGURA, MESA_NOVA_ALTURA);
                        else
                            AdicionarObstaculo(&listaDeObstaculos, pokeballTexture, TIPO_POKEBOLA, GetRandomValue(150, 300), 30, 30);
                    }
                }

                int scoreAntigo = (int)score;
                score += 10.0f * deltaTime;
                int scoreNovo = (int)score;
                if ((scoreNovo / 100) > (scoreAntigo / 100))
                {
                    velocidadeAtual *= 1.01f;
                }
                AtualizarObstaculos(&listaDeObstaculos, deltaTime, velocidadeAtual);
                
                if (modoDeJogo == MODO_HISTORIA && score >= PONTOS_VITORIA_HISTORIA)
                {
                    StopMusicStream(musicGameplay); 
                    estadoAtual = WIN;
                    
                    posicaoEncontroPikachu = player.posicao; 
                    
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
            Texture2D cenarioDesenho;
            
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
                    DrawText("CORRA, PIKACHU!", (GetScreenWidth() - textWidth1) / 2, GetScreenHeight() / 2 - 100, 40, DARKGRAY);
                    
                    int textWidthModoH = MeasureText("Modo História", 20);
                    DrawText("Modo História", 
                             (GetScreenWidth() - textWidthModoH) / 2, 
                             GetScreenHeight() / 2 - 20, 20, 
                             (menuSelecao == 0) ? YELLOW : GRAY); 

                    int textWidthModoI = MeasureText("Modo Infinito", 20);
                    DrawText("Modo Infinito", 
                             (GetScreenWidth() - textWidthModoI) / 2, 
                             GetScreenHeight() / 2 + 10, 20, 
                             (menuSelecao == 1) ? YELLOW : GRAY); 
                             
                    int textWidthH = MeasureText(TextFormat("HI-SCORE: %06d", hiScore), 20);
                    DrawText(TextFormat("HI-SCORE: %06d", hiScore), (GetScreenWidth() - textWidthH) / 2, GetScreenHeight() / 2 + 50, 20, DARKGRAY);
                
                } break;

                case GAMEPLAY:
                {
                    if (estagioAtual == 1) cenarioDesenho = cenarioEstagio1;
                    else if (estagioAtual == 2) cenarioDesenho = cenarioEstagio2;
                    else cenarioDesenho = cenarioEstagio3;

                    scale = (float)GetScreenHeight() / cenarioDesenho.height;
                    scaledWidth = cenarioDesenho.width * scale;
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    Rectangle destRec = { player.posicao.x, player.posicao.y, (float)pikachuLarguraTela, (float)pikachuAlturaTela };

                    DrawTexturePro(
                        pikachuTextura,   
                        frameRec,     
                        destRec,      
                        (Vector2){0, 0},    
                        0.0f,         
                        WHITE         
                    );

                    DesenharObstaculos(listaDeObstaculos);

                    DrawText(TextFormat("HI-SCORE: %06d", hiScore), 20, 20, 20, WHITE);
                    DrawText(TextFormat("PONTOS: %06.0f", score), 20, 50, 20, WHITE);
                    DrawFPS(GetScreenWidth() - 100, 10);
                } break;
                
                case GAME_OVER:
                {
                    if (estagioAtual == 1) cenarioDesenho = cenarioEstagio1;
                    else if (estagioAtual == 2) cenarioDesenho = cenarioEstagio2;
                    else cenarioDesenho = cenarioEstagio3;
                    
                    scale = (float)GetScreenHeight() / cenarioDesenho.height;
                    scaledWidth = cenarioDesenho.width * scale;
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    Rectangle destRec = { player.posicao.x, player.posicao.y, (float)pikachuLarguraTela, (float)pikachuAlturaTela };
                    DrawTexturePro(pikachuTextura, frameRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                    
                    DesenharObstaculos(listaDeObstaculos);
                    
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
                    if (estagioAtual == 1) cenarioDesenho = cenarioEstagio1;
                    else if (estagioAtual == 2) cenarioDesenho = cenarioEstagio2;
                    else cenarioDesenho = cenarioEstagio3;
                    
                    scale = (float)GetScreenHeight() / cenarioDesenho.height;
                    scaledWidth = cenarioDesenho.width * scale;
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);
                    DrawTexturePro(cenarioDesenho, (Rectangle){0,0, (float)cenarioDesenho.width, (float)cenarioDesenho.height}, (Rectangle){posicaoCenarioX + scaledWidth, 0, scaledWidth, (float)GetScreenHeight()}, (Vector2){0,0}, 0.0f, WHITE);

                    Rectangle pikachuWinRec = { posicaoEncontroPikachu.x, posicaoEncontroPikachu.y, (float)pikachuLarguraTela, (float)pikachuAlturaTela };
                    DrawTexturePro(pikachuTextura, frameRec, pikachuWinRec, (Vector2){0, 0}, 0.0f, WHITE);
                    
                    DesenharObstaculos(listaDeObstaculos);
                    
                    float ashDesenhoX = posicaoEncontroPikachu.x + pikachuLarguraTela + 10;
                    float ashDesenhoY = (posicaoEncontroPikachu.y + pikachuAlturaTela) - ashTexture.height;
                    DrawTexture(ashTexture, (int)ashDesenhoX, (int)ashDesenhoY, WHITE);

                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.6f)); 

                    int tw1 = MeasureText("VOCE CONSEGUIU!", 40);
                    DrawText("VOCE CONSEGUIU!", (GetScreenWidth() - tw1) / 2, GetScreenHeight() / 2 - 80, 40, GREEN);
                    
                    int twS = MeasureText(TextFormat("Pontuacao Final: %06.0f", score), 20);
                    DrawText(TextFormat("Pontuacao Final: %06.0f", score), (GetScreenWidth() - twS) / 2, GetScreenHeight() / 2 - 20, 20, RAYWHITE);
                    
                    int twH = MeasureText(TextFormat("Hi-Score: %06d", hiScore), 20);
                    DrawText(TextFormat("Hi-Score: %06d", hiScore), (GetScreenWidth() - twH) / 2, GetScreenHeight() / 2 + 10, 20, LIGHTGRAY);
                    
                    int tw2 = MeasureText("Aperte ENTER para voltar ao Menu", 20);
                    DrawText("Aperte ENTER para voltar ao Menu", (GetScreenWidth() - tw2) / 2, GetScreenHeight() / 2 + 50, 20, RAYWHITE);
                } break;
            }

        EndDrawing();
    }

    LimparObstaculos(&listaDeObstaculos);
    UnloadTexture(pokeballTexture);
    UnloadTexture(cadeiraTexture);
    UnloadTexture(mesaTexture);
    UnloadTexture(pikachuTextura); 
    UnloadTexture(cenarioInicial);
    UnloadTexture(cenarioEstagio1);
    UnloadTexture(cenarioEstagio2);
    UnloadTexture(cenarioEstagio3);
    UnloadTexture(ashTexture);
    
    UnloadMusicStream(musicMenu);
    UnloadMusicStream(musicGameplay);
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}