#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"

#define GRAVIDADE 1200.0f
#define PULO_JOGADOR -650.0f

// Offset para diminuir a hitbox (metade do valor em cada lado)
// Aumentado para 15.0f para diminuir mais a hitbox
#define HITBOX_OFFSET_X 15.0f 
#define HITBOX_OFFSET_Y 15.0f 

typedef struct {
    Vector2 posicao;
    float velocidadeVertical;
    Rectangle colisao;
    int pulosRestantes;
    float paddingX;
    float paddingY;
    bool estaNaPlataforma;
    float largura; // Adicionado para gerenciar o tamanho visual
    float altura;  // Adicionado para gerenciar o tamanho visual
} Pikachu;

Pikachu criarPikachu(int x, int y, int largura, int altura);
void atualizarPikachu(Pikachu *p, float deltaTime);
void pularPikachu(Pikachu *p);
void atualizarColisao(Pikachu *p);

#endif