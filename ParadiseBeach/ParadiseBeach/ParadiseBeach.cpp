/******************************************************************************/
/**                                                    ,---------------.     **/
/**    COMPUTACAO GRAFICA                              |        .-.    |     **/
/**    MIEEC - FCTUC                                   |        !o|    |     **/
/**                                                    |>-,   ,-' | ,-<|     **/
/**    AUTORES: - Andr� Fonseca                        |  `---'   `-'  |     **/
/**             - Alexandre Martins                    `---------------'     **/
/******************************************************************************/
/* DESCRICAO:                                                                 */
/*                                                                            */
/*		                                                                      */
/******************************************************************************/
// 
//	Bibliotecas a incluir //////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "CG.h"
#include "tgaload.h"

//
//	Defini��es /////////////////////////////////////////////////////////////////
//
// Exemplo da defini��o de uma constante
#define PI    3.1415927
#define ESCAPE 27
#define SPACE 32

// Quantidade m�xima de texturas a serem usadas no programa
#define MAX_NO_TEXTURES 8

#define CUBE_TEXTURE 0

// Vetor com os n�meros das texturas
GLuint texture_id[MAX_NO_TEXTURES];

float ratio;
ModelMesh model1 = { 0 };
ModelMesh model2 = { 0 };
ModelMesh model3 = { 0 };
ModelMesh model4 = { 0 };
ModelMesh model5 = { 0 };
ModelMesh model6 = { 0 };
ModelMesh model7 = { 0 };
ModelMesh model8 = { 0 };
int largura = 1024; // largura da nossa janela de visualiza��o
int altura = 768; // altura da nossa janela de visualiza��o
double razao_aspeto = 1; // raz�o de aspecto da janela de visualiza��o
double limite_z_anterior = 1; // plano de recorte anterior
double limite_z_posterior = 20000; // plano de recorte posterior
double pos_x = 20; // posi��o da c�mara (x)
double pos_y = -140; // posi��o da c�mara (y)
double height = 5; // posi��o da c�mara (z)
double look_x = 0; // dire��o de visualiza��o da c�mara (x)
double look_y = 0; // dire��o de visualiza��o da c�mara (y)
double look_z = 4; // dire��o de visualiza��o da c�mara (z)
double campo_visao_y = 50; // campo de vis�o em y
int rato_dx, rato_dy;
int xloc, yloc;
float nuv1x, nuv2x, nuv3x, nuv4x;
float nuv1y, nuv2y, nuv3y, nuv4y;
bool sair = true;
bool pause = false;

bool activo[8];

float LuzEspBranco[] = { 1.0, 1.0, 1.0 };
float LuzAmbPreto[] = { 0.0, 0.0, 0.0 };
float LuzDifBranco[] = { 1.0, 1.0, 1.0 };
const float pos_luz[] = { 0, 1, 0, 0.0 };
float MShininess[] = { 50 };
//
//	Fun��es ////////////////////////////////////////////////////////////////////
//
// Fun��o para controlo das a��es nas teclas normais
// tecla - tecla premida
// x,y - posi��o do cursor a tecla do rato e premida
//
void keyboard(unsigned char tecla, int x, int y)
{
	if (pos_y >= 24 && pos_y <= 26 && pos_x <= -48 && sair) {
		//porta a sair
		//outOfPark = true;
		sair = false;
	}
	else if (pos_y >= 24 && pos_y <= 26 && pos_x >= -49
		&& pos_x <= -47 && !sair) {
		//porta a entrar
		//outOfPark = false;
		sair = true;
	}

	double k = 10; // o que a janela aumenta ou desloca
	double nx = look_x - pos_x; // vector de orienta��o da c�mara (x)
	double ny = look_y - pos_y; // vector de orienta��o da c�mara (y)
	double nz = look_z - height; // vector de orienta��o da c�mara (z)
	double n = sqrt(nx*nx + ny*ny + nz*nz); // norma do vector
	double x_mira = look_x;
	double y_mira = look_y;
	double xc = pos_x;
	double yc = pos_y;
	double teta = k / 180 * 3.14159265 / 10;
	nx /= n; // normaliza��o
	ny /= n;
	nz /= n;
	switch (tecla)
	{
	case ESCAPE: // tecla de escape e tecla 'q', para sa�da da aplica��o
		exit(0);
		break;
	case 'd': // andar para a direita
		pos_x -= ny*k / 6;
		pos_y += nx*k / 6;
		look_x -= ny*k / 6;
		look_y += nx*k / 6;
		break;
	case 'a': // andar para a esquerda
		pos_x += ny*k / 6;
		pos_y -= nx*k / 6;
		look_x += ny*k / 6;
		look_y -= nx*k / 6;
		break;
	case 'w': // andar em frente
		pos_x += nx*k / 6;
		look_x += nx*k / 6;
		pos_y += ny*k / 6;
		look_y += ny*k / 6;
		break;
	case 's': // andar para tr�s
		pos_x -= nx*k / 6;
		look_x -= nx*k / 6;
		pos_y -= ny*k / 6;
		look_y -= ny*k / 6;
		break;
	case SPACE:
		pause = !pause;
		if(pause)
			PlaySound(NULL, NULL, 0);
		else
			PlaySound(L"beach.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	}

	//end of space
	if (pos_x >= 301) {
		pos_x = 300;
	}
	else if (pos_x <= -301) {
		pos_x = -300;
	}
	if (pos_y >= -10) {
		pos_y = -11;
	}
	else if (pos_y <= -281) {
		pos_y = -280;
	}

}
//
// Fun��o para controlo das a��es do rato
// botao - botao premido
//estado - estado do botao
// x,y - posi��o do cursor do rato
//
void mouse(int botao, int estado, int x, int y) {
	if (botao == GLUT_LEFT_BUTTON)
	{
		if (estado == GLUT_UP)
		{
			// N�o faz nada
		}
		else if (estado == GLUT_DOWN)
		{
			rato_dx = x; rato_dy = y;
		}
	}
	else if (botao == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {}
}
//
// Fun��o para controlo das a��es do movimento do rato
// x,y - posi��o do cursor do rato quando uma tecla est� premida
//
void mouseMotion(int x, int y) {
	int xloc = x - rato_dx; // Para fazer pan
	int yloc = y - rato_dy; // Para fazer pan

	rato_dx = x;
	rato_dy = y;

	double k = 10; // o que a janela aumenta ou desloca
	double nx = look_x - pos_x; // vector de orienta��o da c�mara (x)
	double ny = look_y - pos_y; // vector de orienta��o da c�mara (y)
	double nz = look_z - height; // vector de orienta��o da c�mara (z)
	double n = sqrt(nx*nx + ny*ny + nz*nz); // norma do vector
	double x_mira = look_x;
	double y_mira = look_y;
	double xc = pos_x;
	double yc = pos_y;
	double teta = k / 180 * 3.14159265 / 30;
	nx /= n; // normaliza��o
	ny /= n;
	nz /= n;

	look_z -= yloc;//move na vertical

	if (xloc < 0) {
		look_x = cos(-teta)*(x_mira - xc) - sin(-teta)*(y_mira - yc) + xc;
		look_y = sin(-teta)*(x_mira - xc) + cos(-teta)*(y_mira - yc) + yc;
	}
	else if (xloc > 0) {
		look_x = cos(teta)*(x_mira - xc) - sin(teta)*(y_mira - yc) + xc;
		look_y = sin(teta)*(x_mira - xc) + cos(teta)*(y_mira - yc) + yc;
	}
}

void moveCloud1(int v) {
	if(!pause)
		nuv1x++;
	if (nuv1x > 110) {
		nuv1x = -110;
		nuv1y = rand() / 220 - 110;
	}

	glutTimerFunc(100, moveCloud1, 0);
}

void moveCloud2(int v) {
	if (!pause)
		nuv2x++;
	if (nuv2x > 110) {
		nuv2x = -110;
		nuv2y = rand() / 220 - 110;
	}

	glutTimerFunc(123, moveCloud2, 0);
}

void moveCloud3(int v) {
	if (!pause)
		nuv3x++;
	if (nuv3x > 110) {
		nuv3x = -110;
		nuv3y = rand() / 220 - 110;
	}

	glutTimerFunc(110, moveCloud3, 0);
}

void moveCloud4(int v) {
	if (!pause)
		nuv4x++;
	if (nuv4x > 110) {
		nuv4x = -110;
		nuv4y = rand() / 220 - 110;
	}

	glutTimerFunc(90, moveCloud4, 0);
}

void drawModel(ModelMesh modelo) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, modelo.vertex);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, modelo.tex);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, modelo.norms);
	glDrawArrays(GL_TRIANGLES, 0, modelo.nVert);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
//
//Fun��es que corrigem a ilumina��o
//
void iluminacao(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, pos_luz);

	glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspBranco);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbPreto);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifBranco);
}
//
// Adapta��o da fun��o que desenha a Cidade atrav�s de paralelip�pedos
//
void drawScene()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	//em altera��o -> testes
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzEspBranco);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);			glVertex3f(-500, 1, -500);
	glTexCoord2f(400.0, 0.0);		glVertex3f(500, 1, -500);
	glTexCoord2f(400.0, 400.0);		glVertex3f(500, 1, 50);
	glTexCoord2f(0.0, 400.0);		glVertex3f(-500, 1, 50);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture_id[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzEspBranco);
	glColor3f(0.7, 0.7, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);			glVertex3f(-500, 0, 50);
	glTexCoord2f(200.0, 0.0);		glVertex3f(500, 0, 50);
	glTexCoord2f(200.0, 200.0);		glVertex3f(500, 0, 500);
	glTexCoord2f(0.0, 200.0);		glVertex3f(-500, 0, 500);
	glEnd();
	glPopMatrix();

	//Bandeira com b�ia
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[2]);
	glTranslatef(0, 0, -120);
	glScalef(2, 2, 2);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	drawModel(model1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
	glTranslatef(0, 0, -120);
	glScalef(2, 2, 2);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	drawModel(model7);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glTranslatef(0, 0, -120);
	glScalef(2, 2, 2);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f(1, 0.27, 0);
	drawModel(model8);
	glEnd();
	glPopMatrix();

	//Cadeira Nadador Salvador
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
	glTranslatef(5, 0, -120);
	glScalef(2, 2, 2);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	drawModel(model2);
	glEnd();
	glPopMatrix();

	//Chapeus de Sol
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	//glBindTexture(GL_TEXTURE_2D, texture_id[4]); -> problema: n�o consigo p�r duas texturas no mesmo objeto
	glColor3f(1, 1, 1);
	glScalef(2, 2, 2);
	glPushMatrix();

	//lado direito
	for (int i = -7; i <= -1; i++) {
		for (int j = 1; j <= 4; j++) {
			glBindTexture(GL_TEXTURE_2D, texture_id[5]);
			glPushMatrix();

			glTranslatef((20 * i), 0, (j * -25));
			glColor3f(1, 1, 1);
			glScalef(2, 2, 2);
			drawModel(model3);

			glPopMatrix();
		}
	}

	//lado esquerdo
	for (int i = 1; i <= 7; i++) {
		for (int j = 1; j <= 4; j++) {
			glBindTexture(GL_TEXTURE_2D, texture_id[5]);
			glPushMatrix();

			glTranslatef((20 * i), 0, (j * -25));
			glColor3f(1, 1, 1);
			glScalef(2, 2, 2);
			drawModel(model3);

			glPopMatrix();
		}
	}
	glEnd();

	//Espregui�adeiras
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glColor3f(1, 1, 1);
	glScalef(2, 2, 2);
	glPushMatrix();

	//lado direito
	for (int i = -7; i <= -1; i++) {
		for (int j = 1; j <= 4; j++) {
			glBindTexture(GL_TEXTURE_2D, texture_id[6]);
			glPushMatrix();

			glTranslatef((10 * i) - 0.5, 0, (j * -12.5));
			glColor3f(1, 1, 1);
			glScalef(2, 2, 2);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			drawModel(model4);

			glPopMatrix();
		}
	}

	//lado esquerdo
	for (int i = 1; i <= 7; i++) {
		for (int j = 1; j <= 4; j++) {
			glBindTexture(GL_TEXTURE_2D, texture_id[6]);
			glPushMatrix();

			glTranslatef((10 * i) - 0.5, 0, (j * -12.5));
			glColor3f(1, 1, 1);
			glScalef(2, 2, 2);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			drawModel(model4);

			glPopMatrix();
		}
	}
	glEnd();

	//baliza1
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
	glTranslatef(-10, 0, -65);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glScalef(2, 2, 2);
	glColor3f(1, 1, 1);
	drawModel(model5);
	glEnd();
	glPopMatrix();

	//baliza2
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
	glTranslatef(10, 0, -65);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glScalef(2, 2, 2);
	glColor3f(1, 1, 1);
	drawModel(model5);
	glEnd();
	glPopMatrix();

	//nuvens
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[8]);

	//nuvem 1
	glTranslatef(10 + nuv1x, 50, 60 + nuv1y);
	glScalef(4, 4, 4);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 2
	glTranslatef(-50 + nuv2x, 50, 50 + nuv2y);
	glScalef(5, 5, 5);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 3
	glTranslatef(60 + nuv3x, 50, 15 + nuv3y);
	glScalef(3, 3, 3);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 4
	glTranslatef(60 + nuv4x, 50, 35 + nuv4y);
	glScalef(3, 3, 3);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();

	//nuvem 5
	glTranslatef(-40 + nuv1x, 50, 30 + nuv1y);
	glScalef(4, 4, 4);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 6
	glTranslatef(25 + nuv2x, 45, 25 + nuv2y);
	glScalef(5, 5, 5);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 7
	glTranslatef(-40 + nuv3x, 55, 20 + nuv3y);
	glScalef(3, 3, 3);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();
	glPopMatrix();

	//nuvem 8
	glTranslatef(30 + nuv4x, 60, 15 + nuv4y);
	glScalef(3, 3, 3);
	glColor3f(1, 1, 1);
	drawModel(model6);
	glEnd();

	glPopMatrix();


}
//
// Fun��o que desenha o sol
// di�metro - di�metro da esfera que representa o Sol
//
void drawSun(double x, double y, double z, double diametro)
{
	double numero_meridianos = 16; // numero de meridianos da esfera
	double numero_paralelos = 16; // numero de paralelos da esfera
	glPushMatrix(); // armazenamento da matriz de transforma��o actual
	glTranslatef(x, y, z); // coloca��o da esfera no local desejado
	glColor3f(1.0, 1.0, 0.0); // amarelo
	glutSolidSphere(diametro / 2, numero_meridianos, numero_paralelos); // desenho da esfera
	glPopMatrix(); // recupera��o da matriz de transforma��o anterior
}
//
// Fun��o de desenho na janela vis�vel
//
void desenha()
{
	// posicionamento da c�mara virtual com fun��o gluLookAt
	glLoadIdentity();
	gluLookAt(pos_x, height, pos_y, look_x, look_z, look_y, 0, 1, 0);
	iluminacao();
	//draw dome
	glColor3f(0.53, 0.81, 0.98);
	glutSolidSphere(1000 / 2, 16, 16); // desenho da esfera
									   // desenha objectos
	drawSun(200, 400, 200, 50);
	drawScene();
	glutSwapBuffers();
}
//
// Fun��o para redesenhar a cena
//
void redesenha_cena()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // defini��o da cor de limpeza da janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpeza dos buffers
														//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // defini��o do modelo
														// de ilumina��o
	desenha(); // desenho
}
//
// Fun��o de redimensionamento da janela
//
void redimensiona_janela(int param_largura, int param_altura)
{
	if (param_altura == 0) // teste se a janela est� minimizada
	{
		return;
	}
	glViewport(0, 0, param_largura, param_altura);
	razao_aspeto = (double)param_largura / (double)param_altura;
	largura = param_largura; // armazenamento da nova largura
	altura = param_altura; // armazenamento da nova altura
	glMatrixMode(GL_PROJECTION); // defini��o, como matriz activa,
								 // da matriz de projec��o
	glLoadIdentity(); // reset � matriz activa
	gluPerspective(campo_visao_y, razao_aspeto, limite_z_anterior, limite_z_posterior);
	glMatrixMode(GL_MODELVIEW);
}
//
// Fun��o para apresentar a Cena na janela de visualiza��o
//
void mostra_cena(void)
{
	redesenha_cena();
}
//
// Fun��o de inicializa��o das diversas configura��es do GLUT
//
void InitGLUT()
{
	// defini��o do modo inicial de visualiza��o
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// defini��o do tamanho inicial da janela de visualiza��o
	glutInitWindowSize(largura, altura);
	// defini��o da posi��o inicial da janela
	glutInitWindowPosition(200, 100);
	// cria��o da janela de visualiza��o
	glutCreateWindow("Paradise Beach");
	// chamada da fun��o de redimensionamento
	glutReshapeFunc(redimensiona_janela);
	// redesenho a cena
	glutDisplayFunc(redesenha_cena);
	// a��o quando uma tecla e premida
	glutKeyboardFunc(keyboard);
	// fun��o para ir mostrando a cena
	glutIdleFunc(mostra_cena);
	// fun��es do mouse
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	nuv1x = nuv2x = nuv3x = nuv4x = 0;
	nuv1y = rand() / 220 - 110; 
	nuv2y = rand() / 220 - 110; 
	nuv3y = rand() / 220 - 110; 
	nuv4y = rand() / 220 - 110;

}
//
// Fun��o de inicializa��o das diversas configura��es do OpenGL
//
void InitGL()
{
	// defini��o da cor do fundo da janela de visualiza��o (azul celeste)
	glClearColor(0.7f, 0.7f, 1.0f, 0.0f);
	// par�metros de profundidade
	// defini��o do valor de limpeza do buffer de profundidade
	glClearDepth(1.0);
	// defini��o da fun��o de compara��o de profundidade
	glDepthFunc(GL_LEQUAL);
	// activa��o do teste de profundidade e ilumina��o
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_COLOR_MATERIAL);

	// modo de preenchimento de pol�gonos
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void initTexture() {
	// Define a forma de armazenamento dos pixels na textura
	// (1= alinhamento por byte)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Define quantas texturas ser�o usadas no programa 
	glGenTextures(1, texture_id);  // 1 = uma textura;
								   // texture_id = vetor que guardas os n�meros das texturas

								   // Define o n�mero da textura do cubo.
	texture_id[CUBE_TEXTURE] = 1001;

	// Define que tipo de textura ser� usada
	// GL_TEXTURE_2D ==> define que ser� usada uma textura 2D (bitmaps)
	// texture_id[CUBE_TEXTURE]  ==> define o n�mero da textura 
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE]);

	// carrega a uma imagem TGA 
	image_t temp_image;
	tgaLoad("areia.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 1] = 1002;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 1]);
	tgaLoad("oceano.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 2] = 1003;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 2]);
	tgaLoad("tecido_bandeira.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 3] = 1004;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 3]);
	tgaLoad("metal_postes.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 4] = 1005;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 4]);
	tgaLoad("madeira_cadeira.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 5] = 1006;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 5]);
	tgaLoad("topo_chapeu_sol.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 6] = 1007;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 6]);
	tgaLoad("madeira_espreguicadeira.tga", &temp_image, TGA_FREE);

	texture_id[CUBE_TEXTURE + 8] = 1009;
	glBindTexture(GL_TEXTURE_2D, texture_id[CUBE_TEXTURE + 8]);
	tgaLoad("nuvens.tga", &temp_image, TGA_FREE);
}


////////////////////////////////////////////////////////////////////////////////
//	Programa Principal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	// Inicializa o GLUT
	glutInit(&argc, argv);

#pragma
	//FreeConsole();
	InitGLUT();
	InitGL();
	initTexture();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	cout << "A carregar modelos\n" << endl;

	//Load the 3D models from the .obj files
	char *bandeiraP1 = "bandeira_boia_p1.obj";
	char *bandeiraP2 = "bandeira_boia_p2.obj";
	char *bandeiraP3 = "bandeira_boia_p3.obj";
	char *cadeiraLifeguard = "cadeira_lifeguard.obj";
	char *chapeuPraia = "chapeu_praia.obj";
	char *espreguicadeira = "espreguicadeira.obj";
	char *baliza = "baliza.obj";
	char *nuvem = "nuvem.obj";

	cout << "Modelo 1\t";
	// Model Info
	Model model = getOBJinfo(bandeiraP1);

	// Model Data
	model1.nVert = model.vertices;
	model1.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model1.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model1.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model1.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model1.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model1.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model1.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(bandeiraP1, model1);

	model = getOBJinfo(bandeiraP2);

	// Model Data
	model7.nVert = model.vertices;
	model7.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model7.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model7.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model7.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model7.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model7.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model7.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(bandeiraP2, model7);

	model = getOBJinfo(bandeiraP3);

	// Model Data
	model8.nVert = model.vertices;
	model8.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model8.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model8.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model8.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model8.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model8.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model8.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(bandeiraP3, model8);

	cout << "Done!" << endl;
	cout << "Modelo 2\t";

	//Model Info
	model = getOBJinfo(cadeiraLifeguard);

	// Model Data
	model2.nVert = model.vertices;
	model2.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model2.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model2.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model2.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model2.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model2.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model2.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(cadeiraLifeguard, model2);

	cout << "Done!" << endl;
	cout << "Modelo 3\t";

	model = getOBJinfo(chapeuPraia);

	// Model Data
	model3.nVert = model.vertices;
	model3.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model3.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model3.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model3.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model3.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model3.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model3.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(chapeuPraia, model3);

	cout << "Done!" << endl;
	cout << "Modelo 4\t";

	model = getOBJinfo(espreguicadeira);

	// Model Data
	model4.nVert = model.vertices;
	model4.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model4.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model4.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model4.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model4.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model4.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model4.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(espreguicadeira, model4);

	cout << "Done!" << endl;
	cout << "Modelo 5\t";

	model = getOBJinfo(baliza);

	// Model Data
	model5.nVert = model.vertices;
	model5.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model5.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model5.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model5.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model5.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model5.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model5.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(baliza, model5);

	cout << "Done!" << endl;
	cout << "Modelo 6\t";

	model = getOBJinfo(nuvem);

	// Model Data
	model6.nVert = model.vertices;
	model6.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model6.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model6.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model6.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model6.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model6.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model6.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(nuvem, model6);

	cout << "Done!" << endl;

	glutTimerFunc(100, moveCloud1, 0);
	glutTimerFunc(100, moveCloud2, 0);
	glutTimerFunc(100, moveCloud3, 0);
	glutTimerFunc(100, moveCloud4, 0);

	PlaySound(L"beach.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutMainLoop();

	return EXIT_SUCCESS;
}