#include "obstaculo.h"
#include <stdlib.h>

void AdicionarObstaculo(NodoObstaculo **lista, Texture2D textura)
{
    NodoObstaculo *novoNodo = (NodoObstaculo *)malloc(sizeof(NodoObstaculo));
    if (novoNodo == NULL) return; 

    novoNodo->obstaculo.rec.width = textura.width;
    novoNodo->obstaculo.rec.height = textura.height;
    novoNodo->obstaculo.rec.x = GetScreenWidth() + textura.width;

    int maxY = 400 - textura.height; 
    int minY = 100;
    
    int range = maxY - minY;
    int randomY = minY + (rand() % (range + 1));
    novoNodo->obstaculo.rec.y = (float)randomY;
    
    novoNodo->proximo = *lista; 
    *lista = novoNodo;      
}

void AtualizarObstaculos(NodoObstaculo **lista, float deltaTime, float velocidadeAtual)
{
    NodoObstaculo **atual = lista;

    while (*atual != NULL)
    {
        (*atual)->obstaculo.rec.x -= velocidadeAtual * deltaTime;

        if ((*atual)->obstaculo.rec.x < -(*atual)->obstaculo.rec.width)
        {
            NodoObstaculo *nodoParaDeletar = *atual; 
            *atual = (*atual)->proximo; 
            free(nodoParaDeletar);
        }
        else
        {
            atual = &((*atual)->proximo);
        }
    }
}

void DesenharObstaculos(NodoObstaculo *lista, Texture2D textura)
{
    NodoObstaculo *atual = lista; 
    while (atual != NULL)
    {
        DrawTexture(textura, (int)atual->obstaculo.rec.x, (int)atual->obstaculo.rec.y, WHITE);
        atual = atual->proximo; 
    }
}

bool ChecarColisaoObstaculos(NodoObstaculo *lista, Rectangle playerRect)
{
    NodoObstaculo *atual = lista;
    while (atual != NULL)
    {
        if (CheckCollisionRecs(playerRect, atual->obstaculo.rec))
        {
            return true;
        }
        atual = atual->proximo;
    }
    return false;
}

void LimparObstaculos(NodoObstaculo **lista)
{
    NodoObstaculo *atual = *lista;
    while (atual != NULL)
    {
        NodoObstaculo *proximo = atual->proximo; 
        free(atual); 
        atual = proximo; 
    }
    *lista = NULL; 
}