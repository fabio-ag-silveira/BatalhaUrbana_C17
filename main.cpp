/**************************************************
  Universidade Federal da Grande Dourados - UFGD
  Trabalho Computação Gráfica - Batalha Urbana
  Bianca Andréia, Fabio Amaral Godoy da Silveira
**************************************************/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <time.h>
#define PI 3.14159265
#define G 9.81

void texto();
void tiro_1();
void tiro_2();
void trajetoria_balistica_1();
void trajetoria_balistica_2();
void nova_rodada();

char buf_1[100] = {0};

float tam = 3.5f, Tempo = 0,
      r, g, b,                   // Para a cor do fundo.
      c1_cor = 1,                // Para a cor do Carrinho 1.
      c2_cor = 1,                // Para a cor do Carrinho 2.
      c1_1, c1_2, c2_1, c2_2;    // Coordenadas de spawn dos carrinhos.

int n1, n2, jogada = 0, rodada = 0, numero_aleatorio, ponto1 = 0, ponto2 = 0, ponto_1 = 0, ponto_2 = 0,
            Velocidade_1 = 30, Velocidade_2 = 30, // Velocidade inicial dos projeteis dos canhoes.
            angulo_1 = 85, angulo_2 = 85; // Angulo inicial dos canhoes.

bool bola1 = false, bola2 = false, colisao1 = false, colisao2 = false, vez1 = false, vez2 = false;

static void Atualiza_tamanho(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-40.0,40.0,-40.0,40.0);
}

// -------------- Cenario --------------
void predio_maior()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.5f*4,-tam*4);
    glVertex2f(-1.5f*4,tam*4);
    glVertex2f(1.5f*4,tam*4);
    glVertex2f(1.5f*4,-tam*4);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.5f*4,-tam*4);
    glVertex2f(-1.5f*4,tam*4);
    glVertex2f(1.5f*4,tam*4);
    glVertex2f(1.5f*4,-tam*4);
    glEnd();
}

void predio_menor()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.5f*4,-tam*4);
    glVertex2f(-1.5f*4,tam*3);
    glVertex2f(1.5f*4,tam*3);
    glVertex2f(1.5f*4,-tam*4);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.5f*4,-tam*4);
    glVertex2f(-1.5f*4,tam*3);
    glVertex2f(1.5f*4,tam*3);
    glVertex2f(1.5f*4,-tam*4);
    glEnd();
}

void janela()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.5f,-1.0f);
    glVertex2f(-1.5f,1.0f);
    glVertex2f(1.5f,1.0f);
    glVertex2f(1.5f,-1.0f);
    glEnd();
}

void porta()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.0f,-tam/3);
    glVertex2f(-1.0f,tam/3);
    glVertex2f(1.0f,tam/3);
    glVertex2f(1.0f,-tam/3);
    glEnd();
}

// -------------- Objetos --------------
void base(void)
{
    glBegin(GL_QUADS);
    glVertex2f(-tam,-1.5f);
    glVertex2f(-tam,1.5f);
    glVertex2f(tam,1.5f);
    glVertex2f(tam,-1.5f);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-tam,-1.5f);
    glVertex2f(-tam,1.5f);
    glVertex2f(tam,1.5f);
    glVertex2f(tam,-1.5f);
    glEnd();
}

void cano()
{
    glBegin(GL_QUADS);
    glVertex2f(-0.7f,-3.0f);
    glVertex2f(-0.7f,3.0f);
    glVertex2f(0.7f,3.0f);
    glVertex2f(0.7f,-3.0f);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.7f,-3.0f);
    glVertex2f(-0.7f,3.0f);
    glVertex2f(0.7f,3.0f);
    glVertex2f(0.7f,-3.0f);
    glEnd();
}

void quadrado()
{
    glBegin(GL_QUADS);
    glVertex2f(-2.0f,-2.0f);
    glVertex2f(-2.0f,2.0f);
    glVertex2f(2.0f,2.0f);
    glVertex2f(2.0f,-2.0f);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-2.0f,-2.0f);
    glVertex2f(-2.0f,2.0f);
    glVertex2f(2.0f,2.0f);
    glVertex2f(2.0f,-2.0f);
    glEnd();
}

void triang()
{
    glBegin(GL_QUADS);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glEnd();
}

// Circulo
void eixo()
{
    float teta = 0.0f, teta2 = 0.1, raio = 1.0;

    glBegin(GL_POLYGON);
    while(teta < 360.0)
    {
        glVertex2f(1.5*cos(teta/180.0 * PI),1.5*sin(teta/180.0 * PI));
        teta += 0.1;
    }
    glEnd();

    glBegin(GL_LINES);
    while(teta2 < 360.0)
    {
        raio = 1.5;
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f((cos(teta2) * raio),(sin(teta2) * raio));
        raio = 1.2;

        glVertex2f((cos(teta2) * raio),(sin(teta2) * raio));
        teta2 +=1.0;
    }
    glEnd();
}

// -------------- Desenhos --------------
void desenha_predio_maior()
{
    glColor3f(0.5,0.5,0.5);
    predio_maior();

    // Primeiro par de janelas.
    glTranslatef(3,11.0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    glTranslatef(-6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Segundo par de janelas.
    glTranslatef(0,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Terceiro par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    // Quarto par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    // Quinto par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Sexto par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    // Porta
    glTranslatef(-3,-3.5,0.0);
    glColor3f(1.0,1.0,0.0);
    porta();
}

void desenha_predio_menor()
{
    glColor3f(0.0,0.0,0.0);
    predio_menor();

    // Primeiro par de janelas.
    glTranslatef(3,7.5,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(-6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Segundo par de janelas.
    glTranslatef(0,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Terceiro par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    // Quarto par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    // Quinto par de janelas.
    glTranslatef(-6,-4.0,0.0);
    glColor3f(1.0,1.0,1.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,1.0,0.0);
    janela();

    // Porta
    glTranslatef(-3,-3.5,0.0);
    glColor3f(1.0,1.0,0.0);
    porta();
}

void desenha_torre()
{
    glColor3f(0.0,0.0,0.0);
    predio_maior();

    // Relogio.
    glPushMatrix();
    glTranslatef(0,9.0,0.0);
    glScalef(2.5,2.5,0);
    glColor3f(1.0,1.0,0.0);
    eixo();
    glPopMatrix();

    // Teto
    glTranslatef(0,14.0,0.0);
    glColor3f(0.0,0.0,0.0);
    triang();

    // Porta
    glTranslatef(0,-26.5,0.0);
    glColor3f(1.0,1.0,0.0);
    porta();
}

void desenha_citadel()
{
    glPushMatrix();
    glScalef(1,0.5,0.0);
    glTranslatef(0,14.0,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,0.321,0.0);
    glTranslatef(0,-29.6,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.69,0.32,0.0);
    glTranslatef(-2.7,-5.6,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.2,0.1,0.0);
    glTranslatef(31.7,2,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.03,0.12,0.0);
    glTranslatef(225,-8,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glTranslatef(-25,5,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6,11.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(0,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(1.0,0.0,0.0);
    glPopMatrix();

//------------- Meio -------------
    glTranslatef(-1,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);

    glPushMatrix();
    glTranslatef(-0.5,0,0.0);
    glScalef(0.7,1.0,0.0);
    glColor3f(0.0,0.5,0.5);
    glPopMatrix();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.5,0.5);

    glTranslatef(-5,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

//--------------------------
    glPushMatrix();
    glTranslatef(-5,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(-6,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(6,0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();

    glTranslatef(0,-1.0,0.0);
    glColor3f(0.0,0.0,0.0);
    janela();
    glPopMatrix();

    glTranslatef(-10,20.0,0.0);
    glColor3f(0.0,0.0,0.0);
    glPushMatrix();
    glScalef(0.8,0.6,0.0);

    // Triangulo do topo.
    glBegin(GL_QUADS);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,3.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glEnd();
    glPopMatrix();

    // Antenas
    glPushMatrix();
    glScalef(0.03,0.2,0.0);
    glTranslatef(-70,0.0,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glTranslatef(400,7.0,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glTranslatef(35,-15.0,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glTranslatef(-105,-2.0,0.0);
    glColor3f(0.0,0.0,0.0);
    predio_maior();
    glPopMatrix();
}

void desenha_cidade()
{
//------------- Construcoes do lado esquerdo -------------
    glPushMatrix();
    glTranslatef(-30,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-45,-2,0.0);
    glScalef(1,1.25,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-60,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-75,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-105,-7,0.0);
    glScalef(1,0.8,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-135,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-165,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-180,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-195,-5,0.0);
    desenha_torre();
    glPopMatrix();

//------------- Construcoes do lado direito -------------
    glPushMatrix();
    glTranslatef(+15,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+30,-7,0.0);
    glScalef(1,0.85,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+45,-5,0.0);
    desenha_torre();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+60,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+75,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+90,-5,0.0);
    desenha_torre();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+105,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+135,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+150,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+165,-5,0.0);
    desenha_predio_maior();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+180,-5,0.0);
    desenha_predio_menor();
    glPopMatrix();
}

void desenha_carrinho_1()
{
    // Base
    glTranslatef(0.0,0.0,0.0);
    glColor3f(1.0*c1_cor,1.0*c1_cor,1.0*c1_cor);
    base();

    // Rodas
    glColor3f(0.0f*c1_cor, 1.0f*c1_cor, 1.0f*c1_cor);
    glPushMatrix();
    glTranslatef(-tam,-0.5, 0.0);
    eixo();
    glPopMatrix();
    glColor3f(0.0f*c1_cor, 1.0f*c1_cor, 1.0f*c1_cor);
    glPushMatrix();
    glTranslatef(tam,-0.5, 0.0);
    eixo();
    glPopMatrix();

    // Quadrado
    glTranslatef(0.0,10 + (-2*tam), 0.0);
    glColor3f(1.0*c1_cor,1.0*c1_cor,1.0*c1_cor);
    quadrado();

    // Cano do canhao
    glPushMatrix();
    glRotatef(angulo_1,0,0,1);
    glTranslatef(0.0,tam,0.0);
    glColor3f(0.0f, 1.0f, 1.0f);
    cano();

    glTranslatef(0.0,-tam,0.0);
    glColor3f(0.0f, 1.0f, 1.0f);
    eixo();
    glTranslatef(0.0,tam,0.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-tam/2,0.0);
    glColor3f(0.0f, 1.0f, 1.0f);

    //trajetoria_balistica_1();

    if(bola1 == true)
    {
        tiro_1();
    }
    glPopMatrix();
}

void desenha_carrinho_2()
{
    // Base
    glTranslatef(0.0,0.0,0.0);
    glColor3f(1.0*c2_cor,0.5*c2_cor,0.5*c2_cor);
    base();

    // Rodas
    glColor3f(1.0f*c2_cor, 0.0f*c2_cor, 0.0f*c2_cor);
    glPushMatrix();
    glTranslatef(-tam,-0.5, 0.0);
    eixo();
    glPopMatrix();
    glColor3f(1.0f*c2_cor, 0.0f*c2_cor, 0.0f*c2_cor);
    glPushMatrix();
    glTranslatef(tam,-0.5, 0.0);
    eixo();
    glPopMatrix();

    // Quadrado
    glTranslatef(0.0,10 + (-2*tam), 0.0);
    glColor3f(1.0*c2_cor,0.5*c2_cor,0.5*c2_cor);
    quadrado();

    // Cano do canhao
    glPushMatrix();
    glRotatef(angulo_2,0,0,1);
    glTranslatef(0.0,tam,0.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    cano();

    glTranslatef(0.0,-tam,0.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    eixo();
    glTranslatef(0.0,tam,0.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-tam/2,0.0);
    glColor3f(1,0,0);

    //trajetoria_balistica_2();

    if(bola2 == true)
    {
        tiro_2();
    }
    glPopMatrix();
}

void spawn_carrinho_1()
{
    glPushMatrix();

    switch(n1)
    {
    case 0:
        // Posicao 1.
        glTranslatef(-123,-16,0.0);
        c1_1 = -123;
        c1_2 = -16;
        desenha_carrinho_1();
        break;

    case 1:
        // Posicao 2.
        glTranslatef(-90,7,0.0);
        c1_1 = -90;
        c1_2 = 7;
        desenha_carrinho_1();
        break;

    case 2:
        // Posicao 3.
        glTranslatef(0,-16,0.0);
        c1_1 = 0;
        c1_2 = -16;
        desenha_carrinho_1();
        break;

    case 3:
        // Posicao 4.
        glTranslatef(150,11,0.0);
        c1_1 = 150;
        c1_2 = 11;
        desenha_carrinho_1();
        break;
    }
    glPopMatrix();
}

void spawn_carrinho_2()
{
    glPushMatrix();

    switch(n2)
    {
    case 0:
        // Posicao 1.
        glTranslatef(-180,7.7,0.0);
        c2_1 = -180;
        c2_2 = 7.7;
        desenha_carrinho_2();
        break;

    case 1:
        // Posicao 2.
        glTranslatef(60,7.7,0.0);
        c2_1 = 60;
        c2_2 = 7.7;
        desenha_carrinho_2();
        break;

    case 2:
        // Posicao 3.
        glTranslatef(120,-16,0.0);
        c2_1 = 120;
        c2_2 = -16;
        desenha_carrinho_2();
        break;

    case 3:
        // Posicao 4.
        glTranslatef(180,7.7,0.0);
        c2_1 = 180;
        c2_2 = 7.7;
        desenha_carrinho_2();
        break;
    }
    glPopMatrix();
}

void colisoes()
{

     glPushMatrix();
    glTranslatef(-30,-5,0.0);

    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-45,-3,0.0);
    //glScalef(1,1.25,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-60,-7.5,0.0);
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-75,-5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-90,-7.5,0.0);
        glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-105,-12,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();
    //glScalef(1,0.8,0.0);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-135,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150,-7.5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-165,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-180,-7.5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-195,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-195,8.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glEnd();

    glPopMatrix();

//------------- Construcoes do lado direito -------------
    glPushMatrix();
    glTranslatef(+15,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+30,-8.5,0.0);
        glColor3f(1,1,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();
    //glScalef(1,0.85,0.0);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+45,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(45,8.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+60,-7.5,0.0);
        glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+75,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+90,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

        glPushMatrix();
    glTranslatef(90,8.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+105,-7.5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+135,-7.5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+150,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+165,-5,0.0);
        glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(+180,-7.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(+205,-7.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-1400);
    glVertex2f(-6,1400);
    glVertex2f(6,1400);
    glVertex2f(6,-1400);
    glEnd();
    glPopMatrix();

        glPushMatrix();
    glTranslatef(-205,-7.5,0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-6,-1400);
    glVertex2f(-6,1400);
    glVertex2f(6,1400);
    glVertex2f(6,-1400);
    glEnd();
    glPopMatrix();


    /*
    glTranslatef(-30,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -30-6          && posicao_x+c1_1 <= -30+6         && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(-45,-3,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -45-6          && posicao_x+c1_1 <= -45+6         && posicao_y+c1_2 >= -3-14         && posicao_y+c1_2 <= -3+14)     ||


    glTranslatef(-60,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -60-6          && posicao_x+c1_1 <= -60+6         && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(-75,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -75-6          && posicao_x+c1_1 <= -75+6         && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(-90,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -90-6          && posicao_x+c1_1 <= -90+6         && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(-105,-12,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -105-6         && posicao_x+c1_1 <= -105+6        && posicao_y+c1_2 >= -12-14        && posicao_y+c1_2 <= -12+14)    ||


    glTranslatef(-135,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -135-6         && posicao_x+c1_1 <= -135+6        && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||

    glTranslatef(-150,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -150-6         && posicao_x+c1_1 <= -140+6        && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(-165,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -165-6         && posicao_x+c1_1 <= -165+6        && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(-180,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -180-6         && posicao_x+c1_1 <= -180+6        && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(-195,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= -195-6         && posicao_x+c1_1 <= -195+6        && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(-195,8.5,0.0);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);

    // TRIANGULO
    (posicao_x+c1_1 >= -195-6         && posicao_x+c1_1 <= -195+6        && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+6)     ||


//------------- Construcoes do lado direito -------------

    glTranslatef(+15,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +15-6          && posicao_x+c1_1 <= +15+6         && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(+30,-8.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +30-6          && posicao_x+c1_1 <= +30+6         && posicao_y+c1_2 >= -8.5-14       && posicao_y+c1_2 <= -8.5+14)   ||


    glTranslatef(+45,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +45-6          && posicao_x+c1_1 <= +45+6         && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(45,8.5,0.0);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);

    // TRIANGULO
    (posicao_x+c1_1 >= 45-6           && posicao_x+c1_1 <= 45+6          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+6)     ||


    glTranslatef(+60,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +60-6          && posicao_x+c1_1 <= +60+6         && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(+75,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +75-6          && posicao_x+c1_1 <= +75+6         && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(+90,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= 90-6           && posicao_x+c1_1 <= 90+6          && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(90,8.5,0.0);
    glVertex2f(-6.0f,0.0f);
    glVertex2f(0.0f,6.0f);
    glVertex2f(6.0f,0.0f);
    glVertex2f(0.0f,0.0f);

    // TRIANGULO
    (posicao_x+c1_1 >= 90-6           && posicao_x+c1_1 <= 90+6          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+6)     ||


    glTranslatef(+105,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +105-6         && posicao_x+c1_1 <= +105+6        && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(+135,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +135-6         && posicao_x+c1_1 <= +135+6        && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(+150,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +150-6         && posicao_x+c1_1 <= +150+6        && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(+165,-5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +165-6         && posicao_x+c1_1 <= +165+6        && posicao_y+c1_2 >= -5-14         && posicao_y+c1_2 <= -5+14)     ||


    glTranslatef(+180,-7.5,0.0);
    glVertex2f(-6,-14);
    glVertex2f(-6,14);
    glVertex2f(6,14);
    glVertex2f(6,-14);
    (posicao_x+c1_1 >= +180-6         && posicao_x+c1_1 <= +180+6        && posicao_y+c1_2 >= -7.5-14       && posicao_y+c1_2 <= -7.5+14)   ||


    glTranslatef(+205,-7.5,0.0);
    glVertex2f(-6,-1400);
    glVertex2f(-6,1400);
    glVertex2f(6,1400);
    glVertex2f(6,-1400);
    (posicao_x+c1_1 >= +205-6         && posicao_x+c1_1 <= +205+6        && posicao_y+c1_2 >= -7.5-1400     && posicao_y+c1_2 <= -7.5+1400) ||

    glTranslatef(-205,-7.5,0.0);
    glVertex2f(-6,-1400);
    glVertex2f(-6,1400);
    glVertex2f(6,1400);
    glVertex2f(6,-1400);
    (posicao_x+c1_1 >= -205-6         && posicao_x+c1_1 <= -205+6        && posicao_y+c1_2 >= -7.5-1400     && posicao_y+c1_2 <= -7.5+1400) ||
    */
}

static void Atualiza_desenho(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();// Inicializa matriz com a matriz identidade.
    glClear(GL_COLOR_BUFFER_BIT);// Limpa janela.

    texto();

    glPushMatrix();
    glTranslatef(0,-5,0.0);
    glScalef(0.5,2.5,0.0);
    desenha_citadel();  // Desenha a Citadel (construcao gigante).
    glPopMatrix();

    glScalef(0.2,0.3,0.0);
    glTranslatef(0,-115,0.0);
    desenha_cidade();   // Desenha a cidade (predios em ordem).

    // Desenha os carrinhos em uma das quatro posicoes de cada.
    spawn_carrinho_1();
    spawn_carrinho_2();

    //colisoes();

    glFlush();
}

void nova_rodada()
{
    srand(time(NULL));
    // Gera números aleatorios de 0 a 3, para definir as novas posicoes dos carrinhos.
    n1 = rand()%4;
    n2 = rand()%4;

    ponto1 = 0;
    ponto2 = 0;
    jogada = 0;

    numero_aleatorio = rand()%3;

    if(numero_aleatorio == 0)   // Se o numero e 0, o fundo e verde.
    {
        r = 0.0;
        g = 0.5;
        b = 0.5;
    }
    if(numero_aleatorio == 1)   // Se o número e 1, o fundo e roxo.
    {
        r = 0.5;
        g = 0.5;
        b = 1.0;
    }
    if(numero_aleatorio == 2)   // Se o numero e 2, o fundo e amarelo.
    {
        r = 0.5;
        g = 0.5;
        b = 0.0;
    }
    glClearColor(r, g, b, 0);

    c1_cor = 1;
    c2_cor = 1;

    vez1 = false;
    vez2 = false;

    rodada++;
}

void vez_jogada()
{
    jogada++;   // Incrementa o número da jogada.
}

void tempo(int value)
{
    Tempo += 0.1;   // O tempo para utilizar na equacao de lancamento oblíquo.
    glutPostRedisplay();
}

void LeTeclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
    case'q':
        // Sai do jogo.
        exit(0);
        break;

    case 'b':
        // Novo jogo.
        nova_rodada();
        ponto_1 = 0;
        ponto_2 = 0;
        rodada = 0;
        glLoadIdentity();
        glutPostRedisplay();
    }

    if(jogada%2 == 0)   // Se a jogada for par, e a vez do Player 1.
    {
        switch (tecla)
        {

//########################## CONTROLES PLAYER 1 ##########################
        // Movimenta o cano do canhao 1.
        case 'a':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 o angulo se nao estiver no limite.
                if(angulo_1 <= 87) angulo_1 = angulo_1 + 1;
                glutPostRedisplay();
            }

            break;

        case 'd':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                 // Decrementa em 1 o angulo se nao estiver no limite.
                if(angulo_1 >= -87) angulo_1 = angulo_1 - 1;
                glutPostRedisplay();
            }
            break;

        case 'w':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 a velocidade do canhao 1.
                if(Velocidade_1 >= 1) Velocidade_1 = Velocidade_1 + 1;
                glutPostRedisplay();
            }
            break;

        case 's':
            if(vez1 == false)   // Condicao para bloquear os controles do carrinho 1 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 a velocidade do canhao 1.
                if(Velocidade_1 >= 1) Velocidade_1 = Velocidade_1 - 1;
                glutPostRedisplay();
            }
            break;

        case 'f':
            // Atira com o canhao 1.
            vez1 = true;
            vez2 = false;
            bola1 = true;
            if(bola1)
            {
                glutPostRedisplay();
            }
            break;
        }
    }

    if(jogada%2 != 0)   // Se a jogada for impar, e a vez do Player 2.
    {
        switch (tecla)
        {
//########################## CONTROLES PLAYER 2 ##########################
        // Movimenta o cano do canhao 2.
        case 'j':
            if(vez2 == false) // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 o angulo se nao estiver no limite.
                if(angulo_2 <= 87) angulo_2 = angulo_2 + 1;
                glutPostRedisplay();
            }
            break;

        case 'l':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 o angulo se nao estiver no limite.
                if(angulo_2 >= -87) angulo_2 = angulo_2 - 1;
                glutPostRedisplay();
            }
            break;

        case 'i':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Incrementa em 1 a velocidade do canhao 2.
                if(Velocidade_2 >= 1) Velocidade_2 = Velocidade_2 + 1;
                glutPostRedisplay();
            }
            break;

        case 'k':
            if(vez2 == false)   // Condicao para bloquear os controles do carrinho 2 enquanto o projetil estiver no ar.
            {
                // Decrementa em 1 a velocidade do canhao 2.
                if(Velocidade_2 >= 1) Velocidade_2 = Velocidade_2 - 1;
                glutPostRedisplay();
            }
            break;

        case 'h':
            // Atira com o canhao 2.
            vez1 = false;
            vez2 = true;
            bola2 = true;
            if(bola2)
            {
                glutPostRedisplay();
            }
            break;
        }
    }
}

void tiro_1()
{
    float Voy = Velocidade_1 * sin((angulo_1 + 90)/180.0 * PI),
          Vox = Velocidade_1 * cos((angulo_1 + 90)/180.0 * PI),

          x = Vox * Tempo,
          y = Voy * Tempo - G * Tempo * Tempo / 2,

          posicao_x = 0.0 + x,
          posicao_y = 2.0 + y; // Posicao (x,y) do projetil.

    // Mostra a trajetoria do projetil durante o tiro.
    if(bola1 == true)
    {
        trajetoria_balistica_1();
    }

    // Limites para +x, -x e -y para a posicao do projetil.
    if(posicao_x >= -400.00 && posicao_y >= -50.00 && posicao_x <= 400.00)
    {
        glTranslatef(posicao_x, posicao_y, 0.0);
        eixo();
        glutTimerFunc(30, tempo, 0);

        // Verifica se acertou o carrinho 2.
        if(posicao_x+c1_1 >= c2_1-4 && posicao_x+c1_1 <= c2_1+4 && posicao_y+c1_2 >= c2_2-2 && posicao_y+c1_2 <= c2_2+6)
        {
            colisao1 = true;
            ponto1++;   // Pontuacao para alterar a cor do carrinho.
            ponto_1++;  // Pontuacao para mostrar na tela.
            bola1 = false;  // Sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();   // Alterna jogada.

            if(ponto1 == 1)
            {
                c2_cor = 0.5; // O Carrinho 2 muda de cor ao ser atingido.
            }

            if(ponto1 == 2)
            {
                c2_cor = 0.0; // O Carrinho 2 muda de cor ao ser atingido.
            }

            if(ponto1 == 3)   // No terceiro ponto, depois de um tempo, inicia uma nova partida.
            {
                glutTimerFunc(1000, tempo, 0.1);
                glutTimerFunc(1000, tempo, 0.1);
                nova_rodada();
                jogada = 0;
            }
        }

        if( (posicao_x+c1_1 >= -30-7          && posicao_x+c1_1 <= -30+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= -45-7          && posicao_x+c1_1 <= -45+7         && posicao_y+c1_2 >= -3-15         && posicao_y+c1_2 <= -3+15)     ||
            (posicao_x+c1_1 >= -70-7          && posicao_x+c1_1 <= -70+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= -75-7          && posicao_x+c1_1 <= -75+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= -90-7          && posicao_x+c1_1 <= -90+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= -105-7         && posicao_x+c1_1 <= -105+7        && posicao_y+c1_2 >= -12-15        && posicao_y+c1_2 <= -12+15)    ||
            (posicao_x+c1_1 >= -135-7         && posicao_x+c1_1 <= -135+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= -150-7         && posicao_x+c1_1 <= -150+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= -175-7         && posicao_x+c1_1 <= -175+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= -180-7         && posicao_x+c1_1 <= -180+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= -195-7         && posicao_x+c1_1 <= -195+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= -195-7         && posicao_x+c1_1 <= -195+7        && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
            (posicao_x+c1_1 >= +15-7          && posicao_x+c1_1 <= +15+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= +30-7          && posicao_x+c1_1 <= +30+7         && posicao_y+c1_2 >= -8.5-15       && posicao_y+c1_2 <= -8.5+15)   ||
            (posicao_x+c1_1 >= +45-7          && posicao_x+c1_1 <= +45+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= 45-7           && posicao_x+c1_1 <= 45+7          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
            (posicao_x+c1_1 >= 60-7           && posicao_x+c1_1 <= 60+7          && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= -60-7          && posicao_x+c1_1 <= -60+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= +70-7          && posicao_x+c1_1 <= +70+7         && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= +75-7          && posicao_x+c1_1 <= +75+7         && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= 90-7           && posicao_x+c1_1 <= 90+7          && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= 90-7           && posicao_x+c1_1 <= 90+7          && posicao_y+c1_2 >= 8.5-0         && posicao_y+c1_2 <= 8.5+7)     ||
            (posicao_x+c1_1 >= +105-7         && posicao_x+c1_1 <= +105+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= +135-7         && posicao_x+c1_1 <= +135+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= +150-7         && posicao_x+c1_1 <= +150+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= +175-7         && posicao_x+c1_1 <= +175+7        && posicao_y+c1_2 >= -5-15         && posicao_y+c1_2 <= -5+15)     ||
            (posicao_x+c1_1 >= +180-7         && posicao_x+c1_1 <= +180+7        && posicao_y+c1_2 >= -7.5-15       && posicao_y+c1_2 <= -7.5+15)   ||
            (posicao_x+c1_1 >= +205-7         && posicao_x+c1_1 <= +205+7        && posicao_y+c1_2 >= -7.5-1500     && posicao_y+c1_2 <= -7.5+1500) ||
            (posicao_x+c1_1 >= -205-7         && posicao_x+c1_1 <= -205+7        && posicao_y+c1_2 >= -7.5-1500     && posicao_y+c1_2 <= -7.5+1500) ||
            (posicao_x+c1_1 >= +0-1000        && posicao_x+c1_1 <= +0+1000       && posicao_y+c1_2 >= -40-7         && posicao_y+c1_2 <= -40+24))
        {
            bola1 = false;  // Colidiu nos predios ou nas bordas laterais/inferior, entao sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();   // Alterna jogada.
        }
    }
    else
    {
        bola1 = false;  // Sai da condicao de tiro.
        Tempo = 0;
        vez_jogada();   // Alterna jogada.
    }
}

void tiro_2()
{
    float Voy = Velocidade_2 * sin((angulo_2 + 90)/180.0 * PI),
          Vox = Velocidade_2 * cos((angulo_2 + 90)/180.0 * PI),

          x = Vox * Tempo,
          y = Voy * Tempo - G * Tempo * Tempo / 2,

          posicao_x = 0.0 + x,
          posicao_y = 2.0 + y; // Posicao (x,y) do projetil.

    if(bola2 == true)
    {
        // Mostra a trajetoria do projetil durante o tiro.
        trajetoria_balistica_2();
    }

    // Limites para +x, -x e -y para a posicao do projetil.
    if(posicao_x >= -400.00 && posicao_y >= -50.00 && posicao_x <= 400.00)
    {
        glTranslatef(posicao_x, posicao_y, 0.0);
        eixo();
        glutTimerFunc(30, tempo, 0);

        if(posicao_x+c2_1 >= c1_1-4 && posicao_x+c2_1 <= c1_1+4 && posicao_y+c2_2 >= c1_2-2 && posicao_y+c2_2 <= c1_2+6)
        {
            colisao2 = true;
            ponto2++;   // Pontuacao para alterar a cor do carrinho.
            ponto_2++;  // Pontuacao para mostrar na tela.
            bola2 = false;  // Sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();

            if(ponto2 == 1)
            {
                c1_cor = 0.5; // O Carrinho 1 muda de cor ao ser atingido.
            }

            if(ponto2 == 2)
            {
                c1_cor = 0.0; // O Carrinho 1 muda de cor ao ser atingido.
            }

            if(ponto2 == 3)   // No terceiro ponto, depois de um tempo, inicia uma nova partida.
            {
                glutTimerFunc(1000, tempo, 0.1);
                glutTimerFunc(1000, tempo, 0.1);
                nova_rodada();
            }
        }

        if( (posicao_x+c2_1 >= -30-7          && posicao_x+c2_1 <= -30+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= -45-7          && posicao_x+c2_1 <= -45+7         && posicao_y+c2_2 >= -3-15         && posicao_y+c2_2 <= -3+15)     ||
            (posicao_x+c2_1 >= -70-7          && posicao_x+c2_1 <= -70+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= -75-7          && posicao_x+c2_1 <= -75+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= -90-7          && posicao_x+c2_1 <= -90+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= -105-7         && posicao_x+c2_1 <= -105+7        && posicao_y+c2_2 >= -12-15        && posicao_y+c2_2 <= -12+15)    ||
            (posicao_x+c2_1 >= -135-7         && posicao_x+c2_1 <= -135+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= -150-7         && posicao_x+c2_1 <= -150+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= -175-7         && posicao_x+c2_1 <= -175+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= -180-7         && posicao_x+c2_1 <= -180+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= -195-7         && posicao_x+c2_1 <= -195+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= -195-7         && posicao_x+c2_1 <= -195+7        && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
            (posicao_x+c2_1 >= +15-7          && posicao_x+c2_1 <= +15+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= +30-7          && posicao_x+c2_1 <= +30+7         && posicao_y+c2_2 >= -8.5-15       && posicao_y+c2_2 <= -8.5+15)   ||
            (posicao_x+c2_1 >= +45-7          && posicao_x+c2_1 <= +45+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= 45-7           && posicao_x+c2_1 <= 45+7          && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
            (posicao_x+c2_1 >= 60-7           && posicao_x+c2_1 <= 60+7          && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= -60-7          && posicao_x+c2_1 <= -60+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= +70-7          && posicao_x+c2_1 <= +70+7         && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= +75-7          && posicao_x+c2_1 <= +75+7         && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= 90-7           && posicao_x+c2_1 <= 90+7          && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= 90-7           && posicao_x+c2_1 <= 90+7          && posicao_y+c2_2 >= 8.5-0         && posicao_y+c2_2 <= 8.5+7)     ||
            (posicao_x+c2_1 >= +105-7         && posicao_x+c2_1 <= +105+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= +135-7         && posicao_x+c2_1 <= +135+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= +150-7         && posicao_x+c2_1 <= +150+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= +175-7         && posicao_x+c2_1 <= +175+7        && posicao_y+c2_2 >= -5-15         && posicao_y+c2_2 <= -5+15)     ||
            (posicao_x+c2_1 >= +180-7         && posicao_x+c2_1 <= +180+7        && posicao_y+c2_2 >= -7.5-15       && posicao_y+c2_2 <= -7.5+15)   ||
            (posicao_x+c2_1 >= +205-7         && posicao_x+c2_1 <= +205+7        && posicao_y+c2_2 >= -7.5-1500     && posicao_y+c2_2 <= -7.5+1500) ||
            (posicao_x+c2_1 >= -205-7         && posicao_x+c2_1 <= -205+7        && posicao_y+c2_2 >= -7.5-1500     && posicao_y+c2_2 <= -7.5+1500) ||
            (posicao_x+c2_1 >= +0-1000        && posicao_x+c2_1 <= +0+1000       && posicao_y+c2_2 >= -40-7         && posicao_y+c2_2 <= -40+24))
        {
            bola2 = false;  // Colidiu nos predios ou nas bordas laterais/inferior, entao sai da condicao de tiro.
            Tempo = 0;
            vez_jogada();   // Alterna jogada.
        }
    }
    else
    {
        bola2 = false;  // Sai da condicao de tiro.
        Tempo = 0;
        vez_jogada();   // Alterna jogada.
    }
}

void trajetoria_balistica_1()
{
    float Voy = Velocidade_1 * sin((angulo_1 + 90)/180.0 * PI),
          Vox = Velocidade_1 * cos((angulo_1 + 90)/180.0 * PI),

          tempo = 0.0, X = 0.0, Y = 2.0;

    glBegin(GL_LINE_STRIP);
    while(tempo <= 4 * (Voy * 2 / G))   // Desenha a linha da trajetoria, ate um dado tempo.
    {
        glVertex2f(X + Vox * tempo, Y + Voy * tempo - G * tempo * tempo / 2);
        tempo += 0.1;
    }
    glEnd();
}

void trajetoria_balistica_2()
{
    float Voy = Velocidade_2 * sin((angulo_2 + 90)/180.0 * PI),
          Vox = Velocidade_2 * cos((angulo_2 + 90)/180.0 * PI),

          tempo = 0.0, X = 0.0, Y = 2.0;

    glBegin(GL_LINE_STRIP);
    while(tempo <= 4 * (Voy * 2 / G))   // Desenha a linha da trajetoria, ate um dado tempo.
    {
        glVertex2f(X + Vox * tempo, Y + Voy * tempo - G * tempo * tempo / 2);
        tempo += 0.1;
    }
    glEnd();
}

//------------------ Funcoes para imprimir o texto na tela. ------------------
void renderbitmap(float x, float y, void *font, char* string)
{
    char *c;
    glRasterPos2f(x, y);
    for(c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void texto()
{
    char buf[100] = {0};

    if(jogada%2 == 0) // Se a jogada for par, e a vez do Player 1.
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        sprintf_s(buf,"Velocidade: %d m/s, Angulo: %d", Velocidade_1, angulo_1+90);
        renderbitmap(-39,38,GLUT_BITMAP_HELVETICA_18, buf);
    }
    // Controles do Player 1.
    sprintf_s(buf,"(PLAYER 1)");
    glColor3f(0.0f, 1.0f, 1.0f);
    renderbitmap(-39,36,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"+ VELOCIDADE:    W");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,35,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"- VELOCIDADE:    S");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,34,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"+ ANGULO:            D");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,33,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"- ANGULO:            A");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,32,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"ATIRAR:                  F");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,31,GLUT_BITMAP_HELVETICA_10, buf);

    if(jogada%2 != 0)   // Se a jogada for impar, e a vez do Player 2.
    {
        glColor3f(0.5,0.0,0.0);
        sprintf_s(buf,"Velocidade: %d m/s, Angulo: %d", Velocidade_2, angulo_2+90);
        renderbitmap(-39,38,GLUT_BITMAP_HELVETICA_18, buf);
    }

    // Controles do Player 2.
    sprintf_s(buf,"(PLAYER 2)");
    glColor3f(0.5,0.0,0.0);
    renderbitmap(-25,36,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"+ VELOCIDADE:    I");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-25,35,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"- VELOCIDADE:    K");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-25,34,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"+ ANGULO:            J");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-25,33,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"- ANGULO:            L");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-25,32,GLUT_BITMAP_HELVETICA_10, buf);
    sprintf_s(buf,"ATIRAR:                  H");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-25,31,GLUT_BITMAP_HELVETICA_10, buf);

    sprintf_s(buf,"NOVO JOGO:         B");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,29,GLUT_BITMAP_HELVETICA_10, buf);

    sprintf_s(buf,"SAIR:                      Q");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(-39,28,GLUT_BITMAP_HELVETICA_10, buf);

    // Informacoes da partida.
    sprintf_s(buf,"RODADA %d   (PLAYER 1) %d x %d (PLAYER 2)", rodada+1, ponto_1,ponto_2);
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(15,38,GLUT_BITMAP_HELVETICA_18, buf);

    sprintf_s(buf,"*O primeiro a marcar 3 pontos, ganha a rodada.");
    glColor3f(0.0f, 0.0f, 0.0f);
    renderbitmap(15,36,GLUT_BITMAP_HELVETICA_10, buf);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    // Gera números aleatorios de 0 a 3, para definir as posicoes dos carrinhos.
    n1 = rand()%4;
    n2 = rand()%4;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
    glutInitWindowPosition(300,80);
    glutInitWindowSize(1200,800);
    glutCreateWindow("Batalha Urbana - City 17");
    glutReshapeFunc(Atualiza_tamanho);
    glutDisplayFunc(Atualiza_desenho);
    glutKeyboardFunc(LeTeclado);

    // Numeros aleatorios de 0 a 3, para definir a cor do fundo.
    numero_aleatorio = rand()%3;

    if(numero_aleatorio == 0)   // Se o numero e 0, o fundo e verde.
    {
        r = 0.0;
        g = 0.5;
        b = 0.5;
    }
    if(numero_aleatorio == 1)   // Se o número e 1, o fundo e roxo.
    {
        r = 0.5;
        g = 0.5;
        b = 1.0;
    }
    if(numero_aleatorio == 2)   // Se o numero e 2, o fundo e amarelo.
    {
        r = 0.5;
        g = 0.5;
        b = 0.0;
    }
    glClearColor(r, g, b, 0);

    glutMainLoop();
    return 0;
}
