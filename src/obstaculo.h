#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "raylib.h"
#include "jogador.h"

typedef enum {
    TIPO_POKEBOLA,
    TIPO_CADEIRA,
    TIPO_MESA
} TipoObstaculo;

typedef struct Obstaculo {
    Rectangle rec;
    Texture2D textura;
    TipoObstaculo tipo;
} Obstaculo;

typedef struct NodoObstaculo {
    Obstaculo obstaculo;
    struct NodoObstaculo *proximo;
} NodoObstaculo;

void AdicionarObstaculo(NodoObstaculo **lista, Texture2D textura, TipoObstaculo tipo, float posY);
void AtualizarObstaculos(NodoObstaculo **lista, float deltaTime, float velocidadeAtual);
void DesenharObstaculos(NodoObstaculo *lista);
bool ChecarColisaoObstaculos(NodoObstaculo *lista, Pikachu *player);
void LimparObstaculos(NodoObstaculo **lista);

#endif