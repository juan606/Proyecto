/*
Proyecto final
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

//bibliotecas para el manejo de archivos
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
//#include <string> 

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

using namespace std;
const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
bool avanza;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
//
int banderaCamara = 3;
GLfloat cam1x = -25.0f, cam1y = 0.0f, cam1z = 25.0f; //cuarto dia muertos
GLfloat cam2x = 90.0f, cam2y = 0.0f, cam2z = 90.0f; //cuarto navidad
GLfloat cam3x = 0.0f, cam3y = 0.0f, cam3z = 0.0f; //camara libre
GLfloat cam1yaw = 0.0f, cam1pitch = 0.0f;
GLfloat cam2yaw = 0.0f, cam2pitch = 0.0f;
GLfloat cam3yaw = 0.0f, cam3pitch = 0.0f;
int banderaCanasta = 1;


Texture brickTexture;
Texture plainTexture;
Texture pisoTexture;
Texture paredLadrilloTexture;
Texture paredLadrillo2Texture;
Texture terrainTexture;
Texture TexTree;
Texture carTexture;
Texture panMuertoTexture;
Texture Tagave;
Texture vidrioTexture;
Texture papelPicado1;
Texture papelPicado2;
Texture papelPicado3;
Texture papelPicado4;
Texture digimon1;
Texture digimon2;
Texture digimon3;
Texture nacimiento1;
Texture nacimiento2;
Texture nacimiento3;
Texture nacimiento4;
Texture nacimiento5;
Texture nacimiento6;
Texture paredPerro;
Texture pecebre; 
Texture Mar;
Texture Mensaje;
Texture PizzaTex;
Texture RocaTex;
Texture Techo;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Blackhawk_M;
Model Windo;
Model Tree;
Model Roca;
Model Pizza;
Model Pan;
Model Dulce;
Model Calabaza;
Model Mesa;
Model Gisado;
Model PanMuerto;
Model NocheBuena;
Model Huesos;
Model Pinata;
Model Corona;
Model MesaC;
Model Cama;
Model Cama2;
Model Sillon;
Model Comedor;
Model Canasta;
Model SillaM;
Model Tv;
Model Lampara;
Model Barco;
Model Marco;
Model Puerta;
Model Regalo;
Model Dulces;
Model TRICERTP;
Skybox skybox;
Skybox nocheskybox;

//int
int banderaSkybox = 0;
int dia = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat girollanta = 0.0f;
GLfloat desplazamientoX = 0.0f;
GLfloat desplazamientoY = 0.0f;
GLfloat desplazamientoZ = 0.0f;
GLfloat giroY = 0.0f;
GLfloat giroZ = 0.0f;
int bandera = 0;
//control luz spotlight
int banderaL = 0;
float luzX = 0;
float animacionJC=0,Ax=0,Az=0;
// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void creaArchivo(string name) {
	ofstream fs(name.c_str());
	fs.close();
}

string leerArchivo(string name, long numlinea) {
	ifstream fs(name.c_str(), ios::in);
	char linea[128];
	long contador = 0L;
	if (fs.fail())
		printf("el fichero no existe\n");
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));

			if (numlinea == contador) {
				//cout << linea << endl;
				string str(linea);
				return str;
				break;
			}
			contador++;
		}
	fs.close();

}

void readFile(string name) {
	ifstream fs(name.c_str(), ios::in);
	char linea[128];
	long contador = 0L;
	if (fs.fail())
		printf("el fichero no existe\n");
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));
			cout << linea << endl;
		}
	fs.close();
}

void editarArchivo(string name, string contenido) {
	ofstream fs(name.c_str(), ios::app);
	fs << contenido << endl;
	fs.close();
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int terrainIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat terrainVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	5.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 5.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		5.0f, 5.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int wallIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat wallVertices[] = {
		-10.0f, -10.0f, 0.0f,	0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		10.0f, -10.0f, 0.0f,	5.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-10.0f, 10.0f, 0.0f,	0.0f, 2.0f,	0.0f, 0.0f, -1.0f,
		10.0f, 10.0f, 0.0f,		5.0f, 2.0f,	0.0f, 0.0f, -1.0f,
	};

	unsigned int cuadroIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat cuadroVertices[] = {
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	0.0f, 0.0f, -1.0f,
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f,	0.0f, 0.0f, -1.0f,
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	//0
	Mesh* obj0 = new Mesh();
	obj0->CreateMesh(terrainVertices, terrainIndices, 32, 6);
	meshList.push_back(obj0);
	//1
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

	//2
	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(wallVertices, wallIndices, 32, 6);
	meshList.push_back(obj2);

	//3 cuadro simple
	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(cuadroVertices, cuadroIndices, 32, 6);
	meshList.push_back(obj3);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0,
posYavion = 2.0,
posZavion = 0;

float movAvion_x = 0.0f,
movAvion_y = 0.0f,
movAvion_z = 0.0f,
giroAvion = 0;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para IncrementoZ
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	string name = "animation.txt";
	std::string s = "";
	std::ostringstream strsX, strsY, strsZ, strsRY, strs;

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	strsX << movAvion_x;
	s = strsX.str();
	editarArchivo(name, s);

	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	strsY << movAvion_y;
	s = strsY.str();
	editarArchivo(name, s);

	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	strsZ << movAvion_z;
	s = strsZ.str();
	editarArchivo(name, s);

	KeyFrame[FrameIndex].giroAvion = giroAvion;
	strsRY << giroAvion;
	s = strsRY.str();
	editarArchivo(name, s);

	FrameIndex++;
	strs << FrameIndex;
	s = strs.str();
	creaArchivo("frames.txt");
	editarArchivo("frames.txt", s);
}

void cargaFrames(void) {
	//KeyFrame[MAX_FRAMES];
	FrameIndex = 0;
	playIndex = 0;

	string name = "animation.txt";
	string salida = leerArchivo("frames.txt", 0L);
	std::string::size_type sz;   // alias of size_t
	long numFrames = std::stol(salida, &sz);

	long i;
	for (i = 0; i <= numFrames - 1; i++) {

		salida = leerArchivo(name, 4 * i);
		double l_posX = std::stod(salida, &sz);
		KeyFrame[FrameIndex].movAvion_x = l_posX;

		salida = leerArchivo(name, 4 * i + 1);
		double l_posY = std::stod(salida, &sz);
		KeyFrame[FrameIndex].movAvion_y = l_posY;

		salida = leerArchivo(name, 4 * i + 2);
		double l_posZ = std::stod(salida, &sz);
		KeyFrame[FrameIndex].movAvion_z = l_posZ;

		salida = leerArchivo(name, 4 * i + 3);
		double l_rotY = std::stod(salida, &sz);
		KeyFrame[FrameIndex].giroAvion = l_rotY;

		FrameIndex++;
	}
}

void resetElements(void)
{
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued� aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

void inputKeyframes(bool* keys)
{

	//Para vaciar el fichero
	if (keys[GLFW_KEY_V]) {

		creaArchivo("animation.txt");
		creaArchivo("frames.txt");
		glfwWaitEventsTimeout(1.7);

	}

	//Para leer el fichero
	if (keys[GLFW_KEY_R]) {

		readFile("animation.txt");
		//cargaFrames();
		glfwWaitEventsTimeout(1.7);

	}

	//Para cargar las posiciones guardadas en el arreglo de estructuras
	if (keys[GLFW_KEY_L]) {

		cargaFrames();
		glfwWaitEventsTimeout(1.7);

	}
	

	//To Save a KeyFrame
	if (keys[GLFW_KEY_G]){

		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			glfwWaitEventsTimeout(1.7); //delay para evitar lecturas erroneas del teclado
		}
	}
	
	//To play KeyFrame animation 
	if (keys[GLFW_KEY_P])
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		glfwWaitEventsTimeout(1.7);
	}

}

/* FIN KEYFRAMES*/

void DisplayHouse(glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	
	
	
	//plano piso
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -1.95f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	//techo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 13.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();
	//pared 1 ofrenda
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 5.0f, 25.0f));
	model = glm::scale(model, glm::vec3(2.5f, 0.8f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredPerro.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 2 opuesta ala ofrenda
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(15.0f, 5.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-15.0f, 5.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 12.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 3 sillo
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(25.0f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 2.5f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 4 Tv
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-25.0f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 2.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	glm::mat4 modelCuarto2(1.0);

	modelCuarto2= glm::mat4(1.0);
	modelCuarto2 = glm::translate(modelCuarto2, glm::vec3(90.0f, 0.0f, 90.0f));

	//techo
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, 13.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	//plano piso
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, -1.95f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();



	//pared 1 ofrenda
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, 5.0f, 25.0f));
	model = glm::scale(model, glm::vec3(2.5f, 0.8f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 2 opuesta ala ofrenda
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(15.0f, 5.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-15.0f, 5.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, 12.0f, -25.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrillo2Texture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 3 sillo
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(25.0f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 2.5f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredPerro.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	//pared 4 Tv
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-25.0f, 5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 2.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredLadrilloTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	paredPerro.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();


}

void DisplayCasa(glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	//Elementos de la casa en general
	glm::mat4 modelCuarto2(1.0);
	modelCuarto2 = glm::mat4(1.0);
	modelCuarto2 = glm::translate(modelCuarto2, glm::vec3(90.0f, 0.0f, 90.0f));

	//sila
	//Mesa
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -3.7f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	MesaC.RenderModel();
	//Mesa 2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, -3.7f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	MesaC.RenderModel();
}

void DisplayEscenarioMuertos(glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	//Aqui se dibujan los elementos y modelos del cuarto de dia de muertos
	//gerarquia con modelAux
	glm::mat4 modelAux;


	//Puerta
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(4.5f, -3.0f, -24.5f));
	model = glm::scale(model, glm::vec3(12.0f, 7.0f,14.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Puerta.RenderModel();

	//Puerta marco
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(5.0f, -3.0f, -23.0f));
	model = glm::scale(model, glm::vec3(12.0f, 7.0f, 28.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Marco.RenderModel();

	//Lampara
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(18.0f, -2.0f, -7.0f));
	//model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Lampara.RenderModel();

	//Lampara
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-18.0f, -2.0f, -14.0f));
	//model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Lampara.RenderModel();
	//Lampara
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-18.0f, -2.0f, 14.0f));
	//model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Lampara.RenderModel();
	//Mesa
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelAux = model;
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Mesa.RenderModel();

	//Gisado
	model = modelAux;
	model = glm::translate(model, glm::vec3(-1.5f, 2.6f, 1.0f));
	modelAux = model;
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Gisado.RenderModel();
	//Pizza
	model = modelAux;
	model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Pizza.RenderModel();

	//Pan
	model = modelAux;
	model = glm::translate(model, glm::vec3(2.5f, 0.2f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Pan.RenderModel();

	//Pan de muerto
	model = modelAux;
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	PanMuerto.RenderModel();


	//Dulce
	model = modelAux;
	model = glm::translate(model, glm::vec3(2.5f, 0.2f, 1.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Dulce.RenderModel();
	//Huesos
	model = modelAux;
	model = glm::translate(model, glm::vec3(1.6f, -4.6f, 4.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Huesos.RenderModel();




	//Calabaza 1
	model = modelAux;
	model = glm::translate(model, glm::vec3(7.6f, -4.6f, 5.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Calabaza.RenderModel();

	//Calabaza 2
	model = modelAux;
	model = glm::translate(model, glm::vec3(-2.6f, -4.6f, 5.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Calabaza.RenderModel();
	//silla de meser
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(25.0f, -2.0f, -39.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	SillaM.RenderModel();
	//sillon normal
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(40.0f, -2.0f, 25.0f));
	model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Sillon.RenderModel();



	//Tv
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-20.0f, -2.0f, -10.0f));
	//model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -40.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Tv.RenderModel();


	//papel picado
	//blending: transparencia o traslucidez de una imagen
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model = modelAux;
	model = glm::translate(model, glm::vec3(0.0f, 7.0f, 0.0f));
	//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();
	//
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();
	//

	//
	model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();
	//
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	papelPicado4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	glDisable(GL_BLEND);
}

void DisplayEscenarioNavidad(glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	//Aqui se dibujan los elementos y modelos del cuarto de Navidad
	glm::mat4 modelCuarto2(1.0);
	modelCuarto2 = glm::mat4(1.0);
	modelCuarto2 = glm::translate(modelCuarto2, glm::vec3(90.0f, 0.0f, 90.0f));

	//Puerta
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(4.5f, -3.0f, -24.5f));
	model = glm::scale(model, glm::vec3(12.0f, 7.0f, 14.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Puerta.RenderModel();

	//Puerta marco
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(5.0f, -3.0f, -23.0f));
	model = glm::scale(model, glm::vec3(12.0f, 7.0f, 28.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Marco.RenderModel();

	//Regalo
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-10.0f, -3.0f, 15.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Regalo.RenderModel();
	//Regalo2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-13.0f, -3.0f, 12.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Regalo.RenderModel();
	//Regalo3
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-7.0f, -3.0f, 20.0f));
	model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Regalo.RenderModel();
	//sillon normal
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(40.0f, -2.0f, 25.0f));
	model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Sillon.RenderModel();

	//Tv
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-20.0f, -2.0f, -10.0f));
	//model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -40.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Tv.RenderModel();


	//Arbol de navidad
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-26.0f, -3.0f, 26.0f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Tree.RenderModel();

	//silla de meser
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(25.0f, -2.0f, -35.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	SillaM.RenderModel();

	//corona
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(15.0f, -2.0f, 18.0f));
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Corona.RenderModel();

	//Noche buena1
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(22.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(20.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(18.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(16.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(14.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();
	

	//Noche buena1
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-22.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-20.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-18.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-16.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-14.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();

	//Noche buena2
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	NocheBuena.RenderModel();
	//pi�ata
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-34.0f, 5.0f, 4.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Pinata.RenderModel();

	//Lampara
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(18.0f, -2.0f, -7.0f));
	//model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 14.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Lampara.RenderModel();

	//Lampara
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(-18.0f, -2.0f, -14.0f));
	//model = glm::translate(model, glm::vec3(-12.0f, -2.0f, -22.0f));
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//pisoTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Lampara.RenderModel();


	//Barco
	Ax = Ax + 0.1;
	if (Ax < 2.0 && animacionJC ==0) {
		
		Az = sqrt(4-Ax*Ax);

	}
	else {
		
		if ( Ax < 2.0 ) {
			Az = -sqrt(4 - Ax * Ax);
			animacionJC = 1;
		}
		else {
			animacionJC = 0;
		}
	}
	if (Ax>2) {
		Ax = 0;
	}
	printf("%f", Ax);
	printf("%f", Az);

	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(10.0f+Ax, -1.0f, 20.0f+Az));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Barco.RenderModel();

	//plano piso nocimiento
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 20.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	terrainTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	//plano piso nocimiento
	model = modelCuarto2;
	model = glm::translate(model, glm::vec3(10.0f, -1.5f, 20.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Mar.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	//nacimiento
	glm::mat4 modelTemp(1.0);
	//blending: transparencia o traslucidez de una imagen
	model = modelCuarto2;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 20.0f));
	modelTemp = model;


	// pecebre
	model = modelTemp;
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, 2.5f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Mensaje.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	// pecebre
	model = modelTemp;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pecebre.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//animales
	model = modelTemp;
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento5.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	model = modelTemp;
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento2.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//angeles y pastores, etc
	model = modelTemp;
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento6.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	model = modelTemp;
	model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento3.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	model = modelTemp;
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	//
	model = modelTemp;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	nacimiento1.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	glDisable(GL_BLEND);

	
	
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	//CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(-37.0f, 3.0f, 12.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Textures
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/madera2.tga");
	pisoTexture.LoadTextureA();
	paredLadrilloTexture = Texture("Textures/ladrillos1.tga");
	paredLadrilloTexture.LoadTextureA();
	paredLadrillo2Texture = Texture("Textures/pared2.tga");
	paredLadrillo2Texture.LoadTextureA();
	RocaTex = Texture("Textures/pared2.png");
	RocaTex.LoadTextureA();
	PizzaTex = Texture("Textures/pared2.png");
	PizzaTex.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	terrainTexture = Texture("Textures/pasto.tga");
	terrainTexture.LoadTextureA();
	panMuertoTexture = Texture("Textures/ch1.png");
	panMuertoTexture.LoadTextureA();
	vidrioTexture = Texture("Textures/VIDRIO.tga");
	vidrioTexture.LoadTextureA();
	papelPicado1 = Texture("Textures/papel_picado1.tga");
	papelPicado1.LoadTextureA();
	papelPicado2 = Texture("Textures/papel_picado2.tga");
	papelPicado2.LoadTextureA();
	papelPicado3 = Texture("Textures/papel_picado3.tga");
	papelPicado3.LoadTextureA();
	papelPicado4 = Texture("Textures/ingenieria_papel.tga");
	papelPicado4.LoadTextureA();
	digimon1 = Texture("Textures/wizardmon.tga");
	digimon1.LoadTextureA();
	digimon2 = Texture("Textures/blackwargraymon.tga");
	digimon2.LoadTextureA();
	digimon3 = Texture("Textures/leomon.tga");
	digimon3.LoadTextureA();
	nacimiento1 = Texture("Textures/nacimiento1.tga");
	nacimiento1.LoadTextureA();
	nacimiento2 = Texture("Textures/animales1.tga");
	nacimiento2.LoadTextureA();
	nacimiento3 = Texture("Textures/reyesMagos.tga");
	nacimiento3.LoadTextureA();
	nacimiento4 = Texture("Textures/pastor1.tga");
	nacimiento4.LoadTextureA();
	nacimiento5 = Texture("Textures/animales2.tga");
	nacimiento5.LoadTextureA();
	nacimiento6 = Texture("Textures/angel1.tga");
	nacimiento6.LoadTextureA();
	paredPerro = Texture("Textures/perro2.tga");
	paredPerro.LoadTextureA(); 
	pecebre = Texture("Textures/casita.png");
	pecebre.LoadTextureA();
	Mar = Texture("Textures/mar.tga");
	Mar.LoadTextureA();
	/*Techo = Texture("Textures/tec.png");
	Techo.LoadTextureA();*/

	Mensaje = Texture("Textures/mensaje.png");
	Mensaje.LoadTextureA();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Models
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Windo = Model();
	Windo.LoadModel("Models/window.obj");

	Tree = Model();
	Tree.LoadModel("Models/10447_Pine_Tree_v1_L3b.obj");

	Roca = Model();
	Roca.LoadModel("Models/Pizza.obj");

	Pizza = Model();
	Pizza.LoadModel("Models/Corona.obj");

	Pan = Model();
	Pan.LoadModel("Models/Pan.obj");
	Calabaza = Model();
	Calabaza.LoadModel("Models/Pumpkin.obj");
	Mesa = Model();
	Mesa.LoadModel("Models/table.obj");
	
	Gisado = Model();
	Gisado.LoadModel("Models/13561_Shrimp_Sausage_Jambalaya_v1_L1.obj");
	
	Dulce = Model();
	Dulce.LoadModel("Models/HalloweenCandy.obj");

	PanMuerto = Model();
	PanMuerto.LoadModel("Models/Comida2.obj");

	NocheBuena = Model();
	NocheBuena.LoadModel("Models/Poinsetta.obj");
	
	Huesos = Model();
	Huesos.LoadModel("Models/skull.obj");
	
	Pinata = Model();
	Pinata.LoadModel("Models/pinata.obj");

	Corona = Model();
	Corona.LoadModel("Models/Componente_58.obj");


	MesaC = Model();
	MesaC.LoadModel("Models/mesa.obj");

	/*Cama = Model();
	Cama.LoadModel("Models/cama.obj");

	Cama2 = Model();
	Cama2.LoadModel("Models/cama2.obj");*/

	Comedor = Model();
	Comedor.LoadModel("Models/comedor.obj");

	/*Sillon = Model();
	Sillon.LoadModel("Models/sofaD.obj");*/

	Canasta = Model();
	Canasta.LoadModel("Models/canasta.obj");

	Sillon = Model();
	Sillon.LoadModel("Models/sillon.obj");

	Canasta = Model();
	Canasta.LoadModel("Models/canasta.obj");

	SillaM = Model();
	SillaM.LoadModel("Models/sillaM.obj");

	Tv = Model();
	Tv.LoadModel("Models/tv.obj");


	Lampara = Model();
	Lampara.LoadModel("Models/lampara.obj");

	Barco = Model();
	Barco.LoadModel("Models/barco.obj");


	Marco = Model();
	Marco.LoadModel("Models/marco.obj");

	Puerta = Model();
	Puerta.LoadModel("Models/puerta.obj");

	Regalo = Model();
	Regalo.LoadModel("Models/regalo.obj");

	//Dulces = Model();
	//Dulces.LoadModel("Models/dulces.obj");

	//pruebas
	TRICERTP = Model();
	TRICERTP.LoadModel("Models/Skate_Ramp_Fun_Box_08_.obj");
	//


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	

	
	
		

	unsigned int spotLightCount = 0;
	//luz fija
	//spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
	//	0.0f, 2.0f,
	//	10.0f, 0.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	10.0f);
	//spotLightCount++;
	////linterna
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//cargando keyframes de archivo de texto
	//cargaFrames();


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/hills_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_up.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_lf.tga");
	skybox = Skybox(skyboxFaces);

	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base1.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base5.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base3.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base4.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base2.jpg");
	skyboxFacesNoche.push_back("Textures/Skybox/moon/moon_base6.jpg");

	nocheskybox = Skybox(skyboxFacesNoche);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	int auxLuz = 0;
	int auxLux1 = 0;
	int auxLux2 = 0;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		//skybox 
		if (dia <= 600) {
			dia++;
			banderaSkybox = 0;

		}
		if (dia > 600 && dia <= 1200) {
			dia++;
			banderaSkybox = 1;
		}
		if (dia > 1200) {
			dia = 0;
		}

		/***********************/
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		banderaCamara = mainWindow.getBanderaCamara();

		if (banderaCamara == 0) {
			camera.setCameraPosition(glm::vec3(cam3x,cam3y,cam3z),cam3yaw,cam3pitch);
			banderaCanasta = 1;
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
		}
		if (banderaCamara == 1) {
			camera.setCameraPosition(glm::vec3(cam1x, cam1y, cam1z), cam1yaw, cam1pitch);
			banderaCanasta = 0;
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
		}
		if (banderaCamara == 2) {
			camera.setCameraPosition(glm::vec3(cam2x, cam2y, cam2z), cam2yaw, cam2pitch);
			banderaCanasta = 0;
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
		}
		if (banderaCamara == 3) {
			camera.setCameraPosition(glm::vec3(cam3x, cam3y, cam3z), cam3yaw, cam3pitch);
			banderaCanasta = 1;
			camera.keyControlFree(mainWindow.getsKeys(), deltaTime);
		}
		bool aux_Luz=true;
		if (mainWindow.getsKeys()[GLFW_KEY_V]) {
			auxLuz = 0;
			auxLux1 = 0;
			auxLux2 = 0;

		}
		if (mainWindow.getsKeys()[GLFW_KEY_Z]) {
			auxLuz = 10;
			auxLux1 = 0;
			auxLux2 = 0;
		}

		if (mainWindow.getsKeys()[GLFW_KEY_X]) {
			auxLuz = 0;
			auxLux1 = 10;
			auxLux2 = 0;
			
		}
		if (mainWindow.getsKeys()[GLFW_KEY_C]) {
			auxLuz = 0;
			auxLux1 = 0;
			auxLux2 = 10;
		}
		if (auxLuz>0) {
			pointLightCount = 0;
			pointLights[0] = PointLight(1.0f, 2.0f, 0.0f,
				0.0f, 1.0f,
				5.5f, 0.0f, 15.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;

			pointLights[1] = PointLight(1.0f, 2.0f, 0.0f,
				0.0f, 1.0f,
				-5.5f, 0.0f, 15.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;
		}
		if (auxLux1>0)
		{
			pointLightCount = 0;
			pointLights[0] = PointLight(2.0f, 2.0f, 2.0f,
				0.0f, 1.0f,
				108.0f, 3.0f, 83.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;

			pointLights[1] = PointLight(2.0f, 2.0f, 2.0f,
				0.0f, 1.0f,
				72.0f, 3.0f, 76.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;
			aux_Luz = false;
		}
		if (auxLux2>0)
		{
			pointLightCount = 0;
			pointLights[0] = PointLight(2.0f, 2.0f, 2.0f,
				0.0f, 1.0f,
				18.0f, 3.0f, -7.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;

			pointLights[1] = PointLight(2.0f, 2.0f, 2.0f,
				0.0f, 1.0f,
				-18.0f, 3.0f, -14.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;


			pointLights[2] = PointLight(2.0f, 2.0f, 2.0f,
				0.0f, 1.0f,
				-18.0f, 3.0f, 14.0f,
				0.1f, 0.1f, 0.1f);
			pointLightCount++;
		}

		

		/*if (!mainWindow.getLuz2()) {
			pointLightCount = 0;
			luzX = luzX + 1;
			pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
				0.0f, 1.0f,
				92.0f+luzX, 3.0f, 115.0f,
				0.05f, 0.05f, 0.05f);
			pointLightCount++;

			if (luzX < 4) {
				luzX = 0;
			}
		}
*/
		
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (banderaSkybox == 0) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		if (banderaSkybox == 1) {
			nocheskybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();



		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[1].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		banderaL = mainWindow.getBandera();
		if (banderaL == 0) {
			spotLightCount = 1;
		}
		else {
			spotLightCount = 2;
		}

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelJerarquico(1.0);

		//plano terreno
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		terrainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		if (banderaCanasta == 1) {
			//Canasta con camara
			model = glm::mat4(1.0);
			model = glm::translate(model, camera.getCameraPosition());
			model = glm::rotate(model, -camera.getCameraYaw() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, camera.getCameraPitch() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(2.0f, -1.5f, 0.5f));
			modelJerarquico = model;
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Canasta.RenderModel();
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.2f, 1.0f, -0.4f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//Canasta con camara
			model =	modelJerarquico ;
			model = glm::translate(model, glm::vec3(-0.6f, 1.0f, -0.4f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.4f, 1.0f, -0.4f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.8f, 1.0f, -0.4f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.2f, 1.0f, -0.8f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.6f, 1.0f, -0.8f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.4f, 1.0f, -0.8f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();

			//Canasta con camara
			model = modelJerarquico;
			model = glm::translate(model, glm::vec3(-0.8f, 1.0f, -0.8f));
			//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Dulce.RenderModel();
			//a�adir mas objetos de forma jerarquica a la canasta
		}

		//dibujando la estructura de la casa
		DisplayHouse(model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//renderizado TODO LO QUE NO SEA CUARTO
		DisplayCasa(model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//renderizado del cuarto navide�o
		DisplayEscenarioNavidad(model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//renderizado del cuarto de dia de muertos
		DisplayEscenarioMuertos(model, uniformModel, uniformSpecularIntensity, uniformShininess);

		
		

		

		glDisable(GL_BLEND);
		//blending: transparencia o traslucidez de una imagen
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();

		//actualiza valores de la camara
		if (banderaCamara == 0) {
			cam3x = camera.getCameraPosition().x;
			cam3y = camera.getCameraPosition().y;
			cam3z = camera.getCameraPosition().z;
			cam3yaw = camera.getCameraYaw();
			cam3pitch = camera.getCameraPitch();
		}
		if (banderaCamara == 1) {
			cam1x = camera.getCameraPosition().x;
			cam1y = camera.getCameraPosition().y;
			cam1z = camera.getCameraPosition().z;
			cam1yaw = camera.getCameraYaw();
			cam1pitch = camera.getCameraPitch();
		}
		if (banderaCamara == 2) {
			cam2x = camera.getCameraPosition().x;
			cam2y = camera.getCameraPosition().y;
			cam2z = camera.getCameraPosition().z;
			cam2yaw = camera.getCameraYaw();
			cam2pitch = camera.getCameraPitch();
		}
		if (banderaCamara == 3) {
			cam3x = camera.getCameraPosition().x;
			cam3y = camera.getCameraPosition().y;
			cam3z = camera.getCameraPosition().z;
			cam3yaw = camera.getCameraYaw();
			cam3pitch = camera.getCameraPitch();
		}

	}

	return 0;
}