#include "jogador.h"

Pikachu criarPikachu(int x, int y, int largura, int altura) {
    Pikachu p;
    p.posicao = (Vector2){ (float)x, (float)y };
    p.velocidadeVertical = 0.0f;
    p.pulosRestantes = 2;
    p.estaNaPlataforma = false;

    p.paddingX = 10.0f; 
    p.paddingY = 10.0f; 
    p.largura = (float)largura;
    p.altura = (float)altura;

    p.colisao = (Rectangle){ 
        (float)x + HITBOX_OFFSET_X, 
        (float)y + HITBOX_OFFSET_Y, 
        (float)largura - (HITBOX_OFFSET_X * 2), 
        (float)altura - (HITBOX_OFFSET_Y * 2)
    };
    return p;
}

void atualizarPikachu(Pikachu *p, float deltaTime) {
    
    if (!p->estaNaPlataforma)
    {
        p->velocidadeVertical += GRAVIDADE * deltaTime;
    }
    p->posicao.y += p->velocidadeVertical * deltaTime;

    if (p->posicao.y > 700) { 
        p->posicao.y = 700;
        p->velocidadeVertical = 0;
        p->pulosRestantes = 2;
        p->estaNaPlataforma = false;
    }

    if (p->posicao.y < 0) {
        p->posicao.y = 0;
        p->velocidadeVertical = 0;
    }

    atualizarColisao(p);
}

void pularPikachu(Pikachu *p) {
    if (p->pulosRestantes > 0) { 
        p->pulosRestantes--;
        p->velocidadeVertical = PULO_JOGADOR;
        p->estaNaPlataforma = false;
    }
}

void atualizarColisao(Pikachu *p) {
    p->colisao.x = p->posicao.x + HITBOX_OFFSET_X;
    p->colisao.y = p->posicao.y + HITBOX_OFFSET_Y;
    p->colisao.width = p->largura - (HITBOX_OFFSET_X * 2);
    p->colisao.height = p->altura - (HITBOX_OFFSET_Y * 2);
}