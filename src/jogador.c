#include "jogador.h"

Pikachu criarPikachu(int x, int y, int largura, int altura) {
    Pikachu p;
    p.posicao = (Vector2){ (float)x, (float)y };
    p.velocidadeVertical = 0.0f;
    p.colisao = (Rectangle){ (float)x, (float)y, (float)largura, (float)altura };
    p.pulosRestantes = 2;
    return p;
}

void atualizarPikachu(Pikachu *p, float deltaTime) {
    p->velocidadeVertical += GRAVIDADE * deltaTime;
    p->posicao.y += p->velocidadeVertical * deltaTime;

    if (p->posicao.y > 400) {
        p->posicao.y = 400;
        p->velocidadeVertical = 0;
        p->pulosRestantes = 2;
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
    }
}
void atualizarColisao(Pikachu *p) {
    p->colisao.x = p->posicao.x;
    p->colisao.y = p->posicao.y;
}