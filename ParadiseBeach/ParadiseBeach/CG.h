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
//	Proteção do ficheiro de interface /////////////////////////////////////////
//
#ifndef CG_H
#define CG_H

#define _CRT_SECURE_NO_WARNINGS

//
//	Bibliotecas  ///////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <GL/glut.h>
#include <GL/glu.h>

using namespace std;
//
// Estruturas //////////////////////////////////////////////////////////////////
//
typedef struct Model {
	int vertices;
	int positions;
	int texels;
	int normals;
	int faces;
}Model;

typedef struct ModelMesh {
	float *positions;
	float *texels;
	float *normals;
	int *faces;
	float *vertex;
	float *tex;
	float *norms;
	int nVert;
}ModelMesh;
//
//	Funções ////////////////////////////////////////////////////////////////////
//
void removeChar(char * buf, char c) {
	int i;
	char *aux;
	aux = buf;
	for (i = 0; i < strlen(buf); i++) {
		if (*aux == c) *aux = ' ';
	}
}

void calculateVertices(int faces, ModelMesh model);

Model getOBJinfo(char* fp)
{
	// 2
	Model model = { 0 };

	// 3
	// Open OBJ file
	FILE* inOBJ = fopen(fp, "r");
	if (inOBJ == NULL) {
		printf("Impossible to open the file !\n");
		return model;
	}

	// 4
	// Read OBJ file
	char buff[255];
	while (fgets(buff, sizeof buff, inOBJ))
	{
		// 5
		char type[2];
		memcpy(type, buff, 2);
		char *t1 = "v ";
		char *t2 = "vt";
		char *t3 = "vn";
		char *t4 = "f ";

		// 6
		if (memcmp(type, t1, sizeof type) == 0)
			model.positions++;
		else if (memcmp(type, t2, sizeof type) == 0)
			model.texels++;
		else if (memcmp(type, t3, sizeof type) == 0)
			model.normals++;
		else if (memcmp(type, t4, sizeof type) == 0)
			model.faces++;
	}

	// 7
	model.vertices = model.faces * 3; // number vertices needed are 3 per face

									  // 8
									  // Close OBJ file
	fclose(inOBJ);

	// 9
	return model;
}

void extractOBJdata(char* fp, ModelMesh model)
{
	// Counters
	int p = 0;
	int t = 0;
	int n = 0;
	int f = 0;

	// Open OBJ file
	FILE* inOBJ = fopen(fp, "r");

	// Read OBJ file
	char buff[255];
	while (fgets(buff, sizeof buff, inOBJ))
	{
		char type[2];
		memcpy(type, buff, 2);
		char *t1 = "v ";
		char *t2 = "vt";
		char *t3 = "vn";
		char *t4 = "f ";

		// Positions
		if (memcmp(type, t1, sizeof type) == 0) {
			// 1
			// Extract tokens
			strtok(buff, " ");
			float aux;
			int i;
			for (i = 0; i < 3; i++) {
				aux = atof(strtok(NULL, " "));
				model.positions[p * 3 + i] = aux;
			}
			// 2
			p++;
		}
		// Texels
		else if (memcmp(type, t2, sizeof type) == 0) {
			// 1
			// Extract tokens
			strtok(buff, " ");
			float aux;
			int i;
			for (i = 0; i < 2; i++) {
				aux = atof(strtok(NULL, " "));
				model.texels[t * 2 + i] = aux;
			}
			// 2
			t++;
		}
		// Normals
		else if (memcmp(type, t3, sizeof type) == 0) {
			// 1
			// Extract tokens
			strtok(buff, " ");
			float aux;
			int i;
			for (i = 0; i < 3; i++) {
				aux = atof(strtok(NULL, " "));
				model.normals[n * 3 + i] = aux;
			}
			// 2
			n++;
		}
		// Faces
		else if (memcmp(type, t4, sizeof type) == 0) {
			// 1
			// Extract tokens
			int i;
			int pos;
			int vertexIndex[3], uvIndex[3], normalIndex[3];

			string aux = buff;
			aux = aux.substr(2); //ignore the type
			string aux2;
			if (aux.find("//") == string::npos) {//has texel
				pos = aux.find('/');
				vertexIndex[0] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find('/');
				uvIndex[0] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find(' ');
				normalIndex[0] = stoi(aux.substr(0, pos));

				aux = aux.substr(pos + 1);
				pos = aux.find('/');
				vertexIndex[1] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find('/');
				uvIndex[1] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find(' ');
				normalIndex[1] = stoi(aux.substr(0, pos));

				aux = aux.substr(pos + 1);
				pos = aux.find('/');
				vertexIndex[2] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find('/');
				uvIndex[2] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 1);
				pos = aux.find(' ');
				normalIndex[2] = stoi(aux.substr(0, pos));
			}
			else {
				pos = aux.find("//");
				vertexIndex[0] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 2);
				pos = aux.find(' ');
				normalIndex[0] = stoi(aux.substr(0, pos));
				uvIndex[0] = -1;

				aux = aux.substr(pos + 1);
				pos = aux.find("//");
				vertexIndex[1] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 2);
				pos = aux.find(' ');
				normalIndex[1] = stoi(aux.substr(0, pos));
				uvIndex[1] = -1;

				aux = aux.substr(pos + 1);
				pos = aux.find("//");
				vertexIndex[2] = stoi(aux.substr(0, pos));
				aux = aux.substr(pos + 2);
				pos = aux.find(' ');
				normalIndex[2] = stoi(aux.substr(0, pos));
				uvIndex[2] = -1;
			}

			for (i = 0; i < 3; i++) {
				model.faces[f * 9 + i * 3] = vertexIndex[i];
				model.faces[f * 9 + i * 3 + 1] = uvIndex[i];
				model.faces[f * 9 + i * 3 + 2] = normalIndex[i];
			}
			// 2
			f++;
		}
	}

	// Close OBJ file
	fclose(inOBJ);
	calculateVertices(f, model);
}

void calculateVertices(int faces, ModelMesh model) {
	int i;
	for (i = 0; i < faces; i++) {
		int index = model.faces[i * 9];		//1st
											//vertex
		model.vertex[i * 9] = model.positions[(index - 1) * 3];			//x1
		model.vertex[i * 9 + 1] = model.positions[(index - 1) * 3 + 1];	//y1
		model.vertex[i * 9 + 2] = model.positions[(index - 1) * 3 + 2];	//z1
																		//texture
		index = model.faces[i * 9 + 1];
		if (index == -1) {
			model.tex[i * 6] = -1;
			model.tex[i * 6 + 1] = -1;
		}
		else {
			model.tex[i * 6] = model.texels[(index - 1) * 2];			//t1
			model.tex[i * 6 + 1] = model.texels[(index - 1) * 2 + 1];	//t2
		}
		index = model.faces[i * 9 + 2];
		model.norms[i * 9] = model.normals[(index - 1) * 3];
		model.norms[i * 9 + 1] = model.normals[(index - 1) * 3 + 1];
		model.norms[i * 9 + 2] = model.normals[(index - 1) * 3 + 2];

		index = model.faces[i * 9 + 3];		//2nd
		model.vertex[i * 9 + 3] = model.positions[(index - 1) * 3];		//x2
		model.vertex[i * 9 + 4] = model.positions[(index - 1) * 3 + 1];	//y2
		model.vertex[i * 9 + 5] = model.positions[(index - 1) * 3 + 2];	//z2
																		//texture
		index = model.faces[i * 9 + 4];
		if (index == -1) {
			model.tex[i * 6 + 2] = -1;
			model.tex[i * 6 + 3] = -1;
		}
		else {
			model.tex[i * 6 + 2] = model.texels[(index - 1) * 2];		//t3
			model.tex[i * 6 + 3] = model.texels[(index - 1) * 2 + 1];	//t4
		}
		index = model.faces[i * 9 + 5];
		model.norms[i * 9 + 3] = model.normals[(index - 1) * 3];
		model.norms[i * 9 + 4] = model.normals[(index - 1) * 3 + 1];
		model.norms[i * 9 + 5] = model.normals[(index - 1) * 3 + 2];

		index = model.faces[i * 9 + 6];		//3rd
		model.vertex[i * 9 + 6] = model.positions[(index - 1) * 3];		//x3
		model.vertex[i * 9 + 7] = model.positions[(index - 1) * 3 + 1];	//y3
		model.vertex[i * 9 + 8] = model.positions[(index - 1) * 3 + 2];	//z4
																		//texture
		index = model.faces[i * 9 + 7];
		if (index == -1) {
			model.tex[i * 6 + 4] = -1;
			model.tex[i * 6 + 5] = -1;
		}
		else {
			model.tex[i * 6 + 4] = model.texels[(index - 1) * 2];		//t5
			model.tex[i * 6 + 5] = model.texels[(index - 1) * 2 + 1];	//t6
		}
		index = model.faces[i * 9 + 8];
		model.norms[i * 9 + 6] = model.normals[(index - 1) * 3];
		model.norms[i * 9 + 7] = model.normals[(index - 1) * 3 + 1];
		model.norms[i * 9 + 8] = model.normals[(index - 1) * 3 + 2];
	}
}
//
//	Fim do ficheiro de interface ///////////////////////////////////////////////
//
#endif