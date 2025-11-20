#include "obstaculo.h"
#include <stdlib.h>

void AdicionarObstaculo(NodoObstaculo **lista, Texture2D textura, TipoObstaculo tipo, float posY)
{
    NodoObstaculo *novoNodo = (NodoObstaculo *)malloc(sizeof(NodoObstaculo));
    if (novoNodo == NULL) return; 

    novoNodo->obstaculo.textura = textura;
    novoNodo->obstaculo.tipo = tipo;
    
    novoNodo->obstaculo.rec.width = (float)textura.width;
    novoNodo->obstaculo.rec.height = (float)textura.height;
    novoNodo->obstaculo.rec.x = GetScreenWidth() + textura.width;
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
            if (atual->obstaculo.tipo == TIPO_POKEBOLA)
            {
                return true;
            }
            else if (atual->obstaculo.tipo == TIPO_MESA || atual->obstaculo.tipo == TIPO_CADEIRA)
            {
                float margemErro = 10.0f;
                bool estaCaindo = player->velocidadeVertical > 0;
                float topoDoPeDoPikachu = player->colisao.y + player->colisao.height;
                float topoDoObstaculo = atual->obstaculo.rec.y;

                if (estaCaindo && (topoDoPeDoPikachu < topoDoObstaculo + margemErro))
                {
                    player->estaNaPlataforma = true;
                    player->posicao.y = atual->obstaculo.rec.y - player->colisao.height - 10;
                    player->velocidadeVertical = 0;
                    player->pulosRestantes = 2;
                }
                else
                {
                    player->posicao.x = atual->obstaculo.rec.x - player->colisao.width - 10;
                    if (player->posicao.x < 0) player->posicao.x = 0;
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