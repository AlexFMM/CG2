/******************************************************************************/
/**                                                    ,---------------.     **/
/**    COMPUTACAO GRAFICA                              |        .-.    |     **/
/**    MIEEC - FCTUC                                   |        !o|    |     **/
/**                                                    |>-,   ,-' | ,-<|     **/
/**    AUTORES: - André Fonseca                        |  `---'   `-'  |     **/
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
//	Definições /////////////////////////////////////////////////////////////////
//
// Exemplo da definição de uma constante
#define PI    3.1415927
#define ESCAPE 27

// Quantidade máxima de texturas a serem usadas no programa
#define MAX_NO_TEXTURES 8

#define CUBE_TEXTURE 0

// Vetor com os números das texturas
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
int largura = 1024; // largura da nossa janela de visualização
int altura = 768; // altura da nossa janela de visualização
double razao_aspeto = 1; // razão de aspecto da janela de visualização
double limite_z_anterior = 1; // plano de recorte anterior
double limite_z_posterior = 20000; // plano de recorte posterior
double pos_x = 20; // posição da câmara (x)
double pos_y = -140; // posição da câmara (y)
double height = 5; // posição da câmara (z)
double look_x = 0; // direção de visualização da câmara (x)
double look_y = 0; // direção de visualização da câmara (y)
double look_z = 4; // direção de visualização da câmara (z)
double campo_visao_y = 50; // campo de visão em y
int rato_dx, rato_dy;
int xloc, yloc;
float incSwing11, incSwing12;
float incSwing21, incSwing22;
bool sobeSwing[2][2] = {
	{ false, false },
	{ false, false } };
float inc1, inc2;
bool sobeSeeSaw[2] = { false, false };
float rotSpin1, rotSpin2;
bool sair = true;
//bool outOfPark = false;

bool activo[8];

float LuzEspBranco[] = { 1.0, 1.0, 1.0 };
float LuzAmbPreto[] = { 0.0, 0.0, 0.0 };
float LuzDifBranco[] = { 1.0, 1.0, 1.0 };
const float pos_luz[] = { 0, 1, 0, 0.0 };
float MShininess[] = { 100 };
//
//	Funções ////////////////////////////////////////////////////////////////////
//
// Função para controlo das ações nas teclas normais
// tecla - tecla premida
// x,y - posição do cursor a tecla do rato e premida
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
	double nx = look_x - pos_x; // vector de orientação da câmara (x)
	double ny = look_y - pos_y; // vector de orientação da câmara (y)
	double nz = look_z - height; // vector de orientação da câmara (z)
	double n = sqrt(nx*nx + ny*ny + nz*nz); // norma do vector
	double x_mira = look_x;
	double y_mira = look_y;
	double xc = pos_x;
	double yc = pos_y;
	double teta = k / 180 * 3.14159265 / 10;
	nx /= n; // normalização
	ny /= n;
	nz /= n;
	switch (tecla)
	{
	case ESCAPE: // tecla de escape e tecla 'q', para saída da aplicação
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
	case 's': // andar para trás
		pos_x -= nx*k / 6;
		look_x -= nx*k / 6;
		pos_y -= ny*k / 6;
		look_y -= ny*k / 6;
		break;
	/*case '1':
		activo[0] = !activo[0];
		break;
	case '2':
		activo[1] = !activo[1];
		break;
	case '3':
		activo[2] = !activo[2];
		break;
	case '4':
		activo[3] = !activo[3];
		break;
	case '5':
		activo[4] = !activo[4];
		break;
	case '6':
		activo[5] = !activo[5];
		break;
	case '7':
		activo[6] = !activo[6];
		break;
	case '8':
		activo[7] = !activo[7];
		break;
	case ' ':
		if (all_of(begin(activo), end(activo),
			[](bool i)
		{
			return !i;
		})) {
			fill(begin(activo), end(activo), true);
		}
		else {
			fill(begin(activo), end(activo), false);
		}
		break;
	default: // por defeito, não há nenhuma ação
		break;*/
	}

	//hitboxes for the fence
	/*if (!outOfPark) {
		if (pos_x <= -49) {
			pos_x = -48.5;
		}
		else if (pos_x >= 49) {
			pos_x = 48.5;
		}
		if (pos_y <= -49) {
			pos_y = -48.5;
		}
		else if (pos_y >= 49) {
			pos_y = 48.5;
		}
	}
	else {
		if (pos_x > -49 && pos_x <= -47 && pos_y >= -48 && pos_y <= 48
			&& (pos_y < 24 || pos_y > 26)) {
			pos_x = -49.5;
		}
		else if (pos_x <= 49 && pos_x >= 47 && pos_y >= -48 && pos_y <= 48) {
			pos_x = 49.5;
		}
		if (pos_y >= -50 && pos_y <= -47 && pos_x >= -47 && pos_x <= 47) {
			pos_y = -50.5;
		}
		else if (pos_y <= 50 && pos_y >= 47 && pos_x >= -47 && pos_x <= 47) {
			pos_y = 50.5;
		}
	}*/

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
// Função para controlo das ações do rato
// botao - botao premido
//estado - estado do botao
// x,y - posição do cursor do rato
//
void mouse(int botao, int estado, int x, int y) {
	if (botao == GLUT_LEFT_BUTTON)
	{
		if (estado == GLUT_UP)
		{
			// Não faz nada
		}
		else if (estado == GLUT_DOWN)
		{
			rato_dx = x; rato_dy = y;
		}
	}
	else if (botao == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {}
}
//
// Função para controlo das ações do movimento do rato
// x,y - posição do cursor do rato quando uma tecla está premida
//
void mouseMotion(int x, int y) {
	int xloc = x - rato_dx; // Para fazer pan
	int yloc = y - rato_dy; // Para fazer pan

	rato_dx = x;
	rato_dy = y;

	double k = 10; // o que a janela aumenta ou desloca
	double nx = look_x - pos_x; // vector de orientação da câmara (x)
	double ny = look_y - pos_y; // vector de orientação da câmara (y)
	double nz = look_z - height; // vector de orientação da câmara (z)
	double n = sqrt(nx*nx + ny*ny + nz*nz); // norma do vector
	double x_mira = look_x;
	double y_mira = look_y;
	double xc = pos_x;
	double yc = pos_y;
	double teta = k / 180 * 3.14159265 / 30;
	nx /= n; // normalização
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
//Funções que corrigem a iluminação
//
void iluminacao(void)
{
	glLightfv(GL_LIGHT0, GL_POSITION, pos_luz);

	glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspBranco);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbPreto);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifBranco);
}
//
// Adaptação da função que desenha a Cidade através de paralelipípedos
//
void drawPark()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	//em alteração -> testes
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzEspBranco);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);			glVertex3f(-500, 0.01, -500);
	glTexCoord2f(20.0, 0.0);		glVertex3f(500, 0.01, -500);
	glTexCoord2f(20.0, 20.0);		glVertex3f(500, 0.01, 50);
	glTexCoord2f(0.0, 20.0);		glVertex3f(-500, 0.01, 50);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture_id[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzAmbPreto); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzEspBranco);
	glColor3f(0.7,0.7,1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);			glVertex3f(-500, 0, 50);
	glTexCoord2f(200.0, 0.0);		glVertex3f(500, 0, 50);
	glTexCoord2f(200.0, 200.0);		glVertex3f(500, 0, 500);
	glTexCoord2f(0.0, 200.0);		glVertex3f(-500, 0, 500);
	glEnd();
	glPopMatrix();

	//Bandeira com bóia
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[2]);
	//glBindTexture(GL_TEXTURE_2D, texture_id[3]); -> problema: não consigo pôr duas texturas no mesmo objeto
	glTranslatef(0,0,-120);
	glScalef(2, 2, 2);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glColor3f(1,1,1);
	drawModel(model1);
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
	glColor3f(1,1,1);
	drawModel(model2);
	glEnd();
	glPopMatrix();

	//Chapeus de Sol
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	//glBindTexture(GL_TEXTURE_2D, texture_id[4]); -> problema: não consigo pôr duas texturas no mesmo objeto
	glColor3f(1,1,1);
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

	//Espreguiçadeiras
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glColor3f(2, 2, 2);
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

			glTranslatef((10 * i)-0.5, 0, (j * -12.5));
			glColor3f(1, 1, 1);
			glScalef(2, 2, 2);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			drawModel(model4);

			glPopMatrix();
		}
	}
	glEnd();

	//rede voley
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LuzAmbPreto);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LuzEspBranco);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LuzAmbPreto);
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
	glTranslatef(0, 0, -65);
	glScalef(2, 2, 2);
	glColor3f(1, 1, 1);
	drawModel(model5);
	glEnd();
	glPopMatrix();
}
//
// Função que desenha o sol
// diâmetro - diâmetro da esfera que representa o Sol
//
void drawSun(double x, double y, double z, double diametro)
{
	double numero_meridianos = 16; // numero de meridianos da esfera
	double numero_paralelos = 16; // numero de paralelos da esfera
	glPushMatrix(); // armazenamento da matriz de transformação actual
	glTranslatef(x, y, z); // colocação da esfera no local desejado
	glColor3f(1.0, 1.0, 0.0); // amarelo
	glutSolidSphere(diametro / 2,
		numero_meridianos, numero_paralelos); // desenho da esfera
	glPopMatrix(); // recuperação da matriz de transformação anterior
}
//
// Função de desenho na janela visível
//
void desenha()
{
	// posicionamento da câmara virtual com função gluLookAt
	glLoadIdentity();
	gluLookAt(pos_x, height, pos_y, look_x, look_z, look_y, 0, 1, 0);
	iluminacao();
	//draw dome
	glColor3f(0.53, 0.81, 0.98);
	glutSolidSphere(1000 / 2, 16, 16); // desenho da esfera
									   // desenha objectos
	drawSun(200, 400, 200, 50);
	drawPark();
	glutSwapBuffers();
}
//
// Função para redesenhar a cena
//
void redesenha_cena()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // definição da cor de limpeza da janela
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpeza dos buffers
														//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // definição do modelo
														// de iluminação
	desenha(); // desenho
}
//
// Função de redimensionamento da janela
//
void redimensiona_janela(int param_largura, int param_altura)
{
	if (param_altura == 0) // teste se a janela está minimizada
	{
		return;
	}
	glViewport(0, 0, param_largura, param_altura);
	razao_aspeto = (double)param_largura / (double)param_altura;
	largura = param_largura; // armazenamento da nova largura
	altura = param_altura; // armazenamento da nova altura
	glMatrixMode(GL_PROJECTION); // definição, como matriz activa,
								 // da matriz de projecção
	glLoadIdentity(); // reset à matriz activa
	gluPerspective(campo_visao_y, razao_aspeto, limite_z_anterior,
		limite_z_posterior);
	glMatrixMode(GL_MODELVIEW);
}
//
// Função para apresentar a Cena na janela de visualização
//
void mostra_cena(void)
{
	redesenha_cena();
}
//
// Função de inicialização das diversas configurações do GLUT
//
void InitGLUT()
{
	// definição do modo inicial de visualização
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// definição do tamanho inicial da janela de visualização
	glutInitWindowSize(largura, altura);
	// definição da posição inicial da janela
	glutInitWindowPosition(200, 100);
	// criação da janela de visualização
	glutCreateWindow("Paradise Beach");
	// chamada da função de redimensionamento
	glutReshapeFunc(redimensiona_janela);
	// redesenho a cena
	glutDisplayFunc(redesenha_cena);
	// ação quando uma tecla e premida
	glutKeyboardFunc(keyboard);
	// função para ir mostrando a cena
	glutIdleFunc(mostra_cena);
	// funções do mouse
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	incSwing11 = 0;
	incSwing12 = 0;
	incSwing21 = 0;
	incSwing22 = 0;
	inc1 = 0;
	inc2 = 0;
	rotSpin1 = 0;
	rotSpin2 = 0;
	fill(begin(activo), end(activo), true);
}
//
// Função de inicialização das diversas configurações do OpenGL
//
void InitGL()
{
	// definição da cor do fundo da janela de visualização (azul celeste)
	glClearColor(0.7f, 0.7f, 1.0f, 0.0f);
	// parâmetros de profundidade
	// definição do valor de limpeza do buffer de profundidade
	glClearDepth(1.0);
	// definição da função de comparação de profundidade
	glDepthFunc(GL_LEQUAL);
	// activação do teste de profundidade e iluminação
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_COLOR_MATERIAL);

	// modo de preenchimento de polígonos
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void initTexture() {
	// Define a forma de armazenamento dos pixels na textura
	// (1= alinhamento por byte)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Define quantas texturas serão usadas no programa 
	glGenTextures(1, texture_id);  // 1 = uma textura;
								   // texture_id = vetor que guardas os números das texturas

								   // Define o número da textura do cubo.
	texture_id[CUBE_TEXTURE] = 1001;

	// Define que tipo de textura será usada
	// GL_TEXTURE_2D ==> define que será usada uma textura 2D (bitmaps)
	// texture_id[CUBE_TEXTURE]  ==> define o número da textura 
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
	tgaLoad("tecido_almofada.tga", &temp_image, TGA_FREE);
}

////////////////////////////////////////////////////////////////////////////////
//
//Timer functions
//
////////////////////////////////////////////////////////////////////////////////
/*void rodaSpinner1(int value) {
	if (activo[0])
		rotSpin1++;
	if (rotSpin1 == 360)
		rotSpin1 = 0;
	glutTimerFunc(25, rodaSpinner1, 0);
}

void rodaSpinner2(int value) {
	if (activo[1])
		rotSpin2++;
	if (rotSpin2 == 360)
		rotSpin2 = 0;
	glutTimerFunc(20, rodaSpinner2, 0);
}

void incSeeSaw1(int value) {
	if (!activo[2]) {
		if (inc1 != 0) {
			if (inc1 <= 15 && inc1 >= 0)
				inc1--;
			else if (inc1 >= -15 && inc1 <= 0) {
				inc1++;
			}
		}
	}
	else {
		if (inc1 < 15 && !sobeSeeSaw[0])
			inc1++;
		else if (inc1 == 15 && !sobeSeeSaw[0]) {
			sobeSeeSaw[0] = !sobeSeeSaw[0];
		}
		else if (inc1 > -15 && sobeSeeSaw[0]) {
			inc1--;
		}
		else if (inc1 == -15 && sobeSeeSaw[0]) {
			sobeSeeSaw[0] = !sobeSeeSaw[0];
		}
	}
	glutTimerFunc(40, incSeeSaw1, 0);
}

void incSeeSaw2(int value) {
	if (!activo[3]) {
		if (inc2 != 0) {
			if (inc2 <= 15 && inc2 >= 0)
				inc2--;
			else if (inc2 >= -15 && inc2 <= 0) {
				inc2++;
			}
		}
	}
	else {
		if (inc2 < 15 && !sobeSeeSaw[1])
			inc2++;
		else if (inc2 == 15 && !sobeSeeSaw[1]) {
			sobeSeeSaw[1] = !sobeSeeSaw[1];
		}
		else if (inc2 > -15 && sobeSeeSaw[1]) {
			inc2--;
		}
		else if (inc2 == -15 && sobeSeeSaw[1]) {
			sobeSeeSaw[1] = !sobeSeeSaw[1];
		}
	}
	glutTimerFunc(45, incSeeSaw2, 0);
}

void rotSwing11(int value) {
	if (!activo[4]) {
		if (incSwing11 != 0) {
			if (incSwing11 <= 45 && incSwing11 >= 0)
				incSwing11--;
			else if (incSwing11 >= -45 && incSwing11 <= 0) {
				incSwing11++;
			}
		}
	}
	else {
		if (incSwing11 < 45 && !sobeSwing[0][0])
			incSwing11++;
		else if (incSwing11 == 45 && !sobeSwing[0][0]) {
			sobeSwing[0][0] = !sobeSwing[0][0];
		}
		else if (incSwing11 > -45 && sobeSwing[0][0]) {
			incSwing11--;
		}
		else if (incSwing11 == -45 && sobeSwing[0][0]) {
			sobeSwing[0][0] = !sobeSwing[0][0];
		}
	}
	glutTimerFunc(40, rotSwing11, 0);
}

void rotSwing12(int value) {
	if (!activo[5]) {
		if (incSwing12 != 0) {
			if (incSwing12 <= 40 && incSwing12 >= 0)
				incSwing12--;
			else if (incSwing12 >= -40 && incSwing12 <= 0) {
				incSwing12++;
			}
		}
	}
	else {
		if (incSwing12 < 40 && !sobeSwing[0][1])
			incSwing12++;
		else if (incSwing12 == 40 && !sobeSwing[0][1]) {
			sobeSwing[0][1] = !sobeSwing[0][1];
		}
		else if (incSwing12 > -40 && sobeSwing[0][1]) {
			incSwing12--;
		}
		else if (incSwing12 == -40 && sobeSwing[0][1]) {
			sobeSwing[0][1] = !sobeSwing[0][1];
		}
	}
	glutTimerFunc(35, rotSwing12, 0);
}

void rotSwing21(int value) {
	if (!activo[6]) {
		if (incSwing21 != 0) {
			if (incSwing21 <= 30 && incSwing21 >= 0)
				incSwing21--;
			else if (incSwing21 >= -30 && incSwing21 <= 0) {
				incSwing21++;
			}
		}
	}
	else {
		if (incSwing21 < 30 && !sobeSwing[1][0])
			incSwing21++;
		else if (incSwing21 == 30 && !sobeSwing[1][0]) {
			sobeSwing[1][0] = !sobeSwing[1][0];
		}
		else if (incSwing21 > -30 && sobeSwing[1][0]) {
			incSwing21--;
		}
		else if (incSwing21 == -30 && sobeSwing[1][0]) {
			sobeSwing[1][0] = !sobeSwing[1][0];
		}
	}
	glutTimerFunc(40, rotSwing21, 0);
}

void rotSwing22(int value) {
	if (!activo[7]) {
		if (incSwing22 != 0) {
			if (incSwing22 <= 60 && incSwing22 >= 0)
				incSwing22--;
			else if (incSwing22 >= -60 && incSwing22 <= 0) {
				incSwing22++;
			}
		}
	}
	else {
		if (incSwing22 < 60 && !sobeSwing[1][1])
			incSwing22++;
		else if (incSwing22 == 60 && !sobeSwing[1][1]) {
			sobeSwing[1][1] = !sobeSwing[1][1];
		}
		else if (incSwing22 > -60 && sobeSwing[1][1]) {
			incSwing22--;
		}
		else if (incSwing22 == -60 && sobeSwing[1][1]) {
			sobeSwing[1][1] = !sobeSwing[1][1];
		}
	}
	glutTimerFunc(30, rotSwing22, 0);
}*/
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
	char *bandeiraBoia = "bandeira_boia.obj";
	char *cadeiraLifeguard = "cadeira_lifeguard.obj";
	char *chapeuPraia = "chapeu_praia.obj";
	char *espreguicadeira = "espreguicadeira.obj";
	char *redeVoley = "rede_voley.obj";

	cout << "Modelo 1\t";
	// Model Info
	Model model = getOBJinfo(bandeiraBoia);

	// Model Data
	model1.nVert = model.vertices;
	model1.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model1.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model1.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model1.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model1.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model1.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model1.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(bandeiraBoia, model1);

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

	model = getOBJinfo(redeVoley);

	// Model Data
	model5.nVert = model.vertices;
	model5.positions = (float *)malloc(sizeof(float) * model.positions * 3);
	model5.texels = (float *)malloc(sizeof(float) * model.texels * 2);
	model5.normals = (float *)malloc(sizeof(float) * model.normals * 3);
	model5.faces = (int *)malloc(sizeof(int) * model.faces * 9);
	model5.vertex = (float*)malloc(sizeof(float) * model.vertices * 3);
	model5.tex = (float*)malloc(sizeof(float) * model.vertices * 2);
	model5.norms = (float*)malloc(sizeof(float) * model.vertices * 3);

	extractOBJdata(redeVoley, model5);

	cout << "Done!" << endl;

	/*glutTimerFunc(0, rodaSpinner1, 0);
	glutTimerFunc(15, rodaSpinner2, 0);
	glutTimerFunc(0, incSeeSaw1, 0);
	glutTimerFunc(10, incSeeSaw2, 0);
	glutTimerFunc(0, rotSwing11, 0);
	glutTimerFunc(10, rotSwing12, 0);
	glutTimerFunc(8, rotSwing21, 0);
	glutTimerFunc(16, rotSwing22, 0);
	// Esperando por eventos*/

	PlaySound(L"beach.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glutMainLoop();

	

	return EXIT_SUCCESS;
}