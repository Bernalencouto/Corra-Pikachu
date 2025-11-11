#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"

typedef struct Obstaculo {
    Rectangle rec;
} Obstaculo;

typedef struct NodoObstaculo {
    Obstaculo obstaculo;
    struct NodoObstaculo *proximo;
} NodoObstaculo;

void AdicionarObstaculo(NodoObstaculo **lista, Texture2D textura);

void AtualizarObstaculos(NodoObstaculo **lista, float deltaTime, float velocidadeAtual);

void DesenharObstaculos(NodoObstaculo *lista, Texture2D textura);

bool ChecarColisaoObstaculos(NodoObstaculo *lista, Rectangle playerRect);

void LimparObstaculos(NodoObstaculo **lista);

#endif