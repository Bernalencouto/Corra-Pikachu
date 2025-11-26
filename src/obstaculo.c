#include "obstaculo.h"
#include <stdlib.h>

void AdicionarObstaculo(NodoObstaculo **lista, Texture2D textura, TipoObstaculo tipo, float posY, float largura, float altura)
{
    NodoObstaculo *novoNodo = (NodoObstaculo *)malloc(sizeof(NodoObstaculo));
    if (novoNodo == NULL) return; 

    novoNodo->obstaculo.textura = textura;
    novoNodo->obstaculo.tipo = tipo;
    
    novoNodo->obstaculo.rec.width = largura;
    novoNodo->obstaculo.rec.height = altura;
    novoNodo->obstaculo.rec.x = GetScreenWidth() + largura;
    novoNodo->obstaculo.rec.y = posY;
    
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

void DesenharObstaculos(NodoObstaculo *lista)
{
    NodoObstaculo *atual = lista; 
    while (atual != NULL)
    {
        DrawTexture(atual->obstaculo.textura, (int)atual->obstaculo.rec.x, (int)atual->obstaculo.rec.y, WHITE);
        atual = atual->proximo; 
    }
}

bool ChecarColisaoObstaculos(NodoObstaculo *lista, Pikachu *player)
{
    NodoObstaculo *atual = lista;
    
    player->estaNaPlataforma = false;

    while (atual != NULL)
    {
        if (CheckCollisionRecs(player->colisao, atual->obstaculo.rec))
        {
            if (atual->obstaculo.tipo == TIPO_POKEBOLA || atual->obstaculo.tipo == TIPO_CADEIRA)
            {
                return true; 
            }
            else if (atual->obstaculo.tipo == TIPO_MESA)
            {
                float pePikachu = player->colisao.y + player->colisao.height;
                float topoMesa = atual->obstaculo.rec.y;
                
                float tolerancia = 25.0f; 

                if (player->velocidadeVertical >= 0 && (pePikachu <= topoMesa + tolerancia))
                {
                    player->estaNaPlataforma = true;
                    player->posicao.y = topoMesa - player->altura + player->paddingY;
                    player->velocidadeVertical = 0;
                    player->pulosRestantes = 2;
                }
                else
                {
                    player->posicao.x = atual->obstaculo.rec.x - player->largura + player->paddingX;
                    atualizarColisao(player);
                }
            }
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