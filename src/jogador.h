#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

#define GRAVIDADE 1200.0f
#define PULO_JOGADOR -500.0f

typedef struct {
    Vector2 posicao;
    float velocidadeVertical;
    Rectangle colisao;
    int pulosRestantes;
    float paddingX;
    float paddingY;
} Pikachu;

Pikachu criarPikachu(int x, int y, int largura, int altura);

void atualizarPikachu(Pikachu *p, float deltaTime);

void pularPikachu(Pikachu *p);

void atualizarColisao(Pikachu *p);

#endif