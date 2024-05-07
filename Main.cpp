//
// Profesor: Ing. Jose Roque Roman Guadarrama
// Materia: Computacion Grafica e interaccion Humano-Computadora
// Semestre 2024-1
// Alumnos:
// Gonzalez Romero Daniel Vicente



// Cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <chrono>
#include <conio.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <irrklang/irrKlang.h>  //Audio

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// Iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


using namespace irrklang;

const float toRadians = 3.14159265f / 180.0f;


// Variables de animación palanca y resorte
float movPalanca = 0.0f;
float movResorte = 0.0f;
float escResorte = 0.0f;
float anguloResorte = 0.0f;

// Variables de animacion canica 
float movCanicax = 0.0f;
float movCanicay = 0.0f;
float movCanicaz = 0.0f;
int contCanica = 0;
float rotarCanica = 0.0f;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camaraFija;
Camera camaraCanica;

// Texturas
Texture pisoTexture;

// Modelos
Model pinball_M;
Model cristalPinball_M;
Model resorte_M;
Model palanca_M;
Model canica1_M;
Model canica2_M;
Model tuboTraslucido_M;

//Ricardo 
Model palanca_golpe_DER_M;
Model palanca_golpe_IZQ_M;
Model obstaculo_L_M;
Model obstaculo_C_M;
Model circulos_M;
Model bandera_roja_M;
Model bandera_azul_M;
Model eclipce_M;
Model hongo_M;
Model elipse_M;
Model puente_M;
Model edificio_M;

Model avatar_M;

Skybox skybox;

// Materiales
Material Material_brillante;
Material Material_opaco;



GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Luz direccional
DirectionalLight mainLight;

// Luces de tipo pointlight y spotlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearObjetos()
{
	unsigned int pisoIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat pisoVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(pisoVertices, pisoIndices, 32, 12);
	meshList.push_back(obj1);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();
	CrearObjetos();


	// Sonido
	ISoundEngine *sonido = createIrrKlangDevice();
	
	sonido->setSoundVolume(1.0f);
	sonido->play3D("Audio/RainCityAmbience.mp3", vec3df(0,0,0), true, false, true);

	ISound* cancion = sonido->play2D("Audio/Cave Stage-MVC2.mp3", true, false, true);
	cancion->setVolume(0.2f);


	// Camaras
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.9f, 0.6f);

	camaraFija = Camera(glm::vec3(182.61f, 155.09f, -11.65f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.6f);
	camaraFija.setCameraDirection(glm::vec3(-0.71, -0.70, -0.00));

	camaraCanica = Camera(glm::vec3((94.0f + movCanicax), (14.0f + movCanicay), (34.0f + movCanicaz)), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.6f);


	// Texturas

	pisoTexture = Texture("Textures/pisoMadera.jpeg");
	pisoTexture.LoadTextureA();


	// Modelos
	pinball_M = Model();
	pinball_M.LoadModel("Models/pinball.obj");
	cristalPinball_M = Model();
	cristalPinball_M.LoadModel("Models/cristal.obj");
	resorte_M = Model();
	resorte_M.LoadModel("Models/resorte.obj");
	palanca_M = Model();
	palanca_M.LoadModel("Models/palanca.obj");
	canica1_M = Model();
	canica1_M.LoadModel("Models/canicaMetalica.obj");
	canica2_M = Model();
	canica2_M.LoadModel("Models/canicaMetalica.obj");
	tuboTraslucido_M = Model();
	tuboTraslucido_M.LoadModel("Models/tuboPlasticoAmarillo.obj");

	//Ricardo
	palanca_golpe_IZQ_M = Model();
	palanca_golpe_IZQ_M.LoadModel("Models/Obstaculos/palanca_golpe_IZQ.obj");
	palanca_golpe_DER_M = Model();
	palanca_golpe_DER_M.LoadModel("Models/Obstaculos/palanca_golpe_DER.obj");
	obstaculo_L_M = Model();
	obstaculo_L_M.LoadModel("Models/Obstaculos/L.obj");
	obstaculo_C_M = Model();
	obstaculo_C_M.LoadModel("Models/Obstaculos/C.obj");
	circulos_M = Model();
	circulos_M.LoadModel("Models/Obstaculos/circulos.obj");
	bandera_roja_M = Model();
	bandera_roja_M.LoadModel("Models/Obstaculos/bandera_roja.obj");
	bandera_azul_M = Model();
	bandera_azul_M.LoadModel("Models/Obstaculos/bandera_azul.obj");
	eclipce_M = Model();
	eclipce_M.LoadModel("Models/Obstaculos/eclipce.obj");
	hongo_M = Model();
	hongo_M.LoadModel("Models/Obstaculos/hongo.obj");
	elipse_M = Model();
	elipse_M.LoadModel("Models/Obstaculos/elipse.obj");
	puente_M = Model();
	puente_M.LoadModel("Models/Obstaculos/puente.obj");
	edificio_M = Model();
	edificio_M.LoadModel("Models/Obstaculos/edificio.obj");
	avatar_M = Model();
	avatar_M.LoadModel("Models/personaje.obj");

	// Skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cyberoom_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cyberoom_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cyberoom_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cyberoom_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cyberoom_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cyberoom_ft.tga");
	

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	// Paletas
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		0.7f, 1.0f,
		70.0f, -10.0f, -10.5f, 
		0.8f, 0.001f, 0.001f);
	pointLightCount++;

	
	unsigned int spotLightCount = 0;
	
	// Tablero
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 0.6f,
		-10.0f, 250.0f, -10.0f,
		0.0f, -1.0f, 0.0f,
		0.0001f, 0.0001f, 0.0001f,
		30.0f);
	spotLightCount++;


	// Paletas
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		120.0f, 20.0f, -2.5f,
		-1.0f, 0.0f, 0.0f,
		0.001f, 0.0001f, 0.0001f,
		15.0f);
	spotLightCount++;*/

	/*spotLights2[0] = spotLights[1];
	spotLights2[1] = spotLights[0];*/


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::vec3 posicionCamara;
	glm::vec3 direccionCamara;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		if (mainWindow.getcamaraLibre())  // Camara libre
		{
			//Recibir eventos del usuario
			glfwPollEvents();
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

			// Clear the window
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			shaderList[0].UseShader();
			uniformModel = shaderList[0].GetModelLocation();
			uniformProjection = shaderList[0].GetProjectionLocation();
			uniformView = shaderList[0].GetViewLocation();
			uniformEyePosition = shaderList[0].GetEyePositionLocation();
			uniformColor = shaderList[0].getColorLocation();
			uniformTextureOffset = shaderList[0].getOffsetLocation();

			//información en el shader de intensidad especular y brillo
			uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
			uniformShininess = shaderList[0].GetShininessLocation();

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		}
		else 
		{
			if (mainWindow.getcambioCamara())  // Camara canica
			{
				glfwPollEvents();
				camaraCanica.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
				camaraCanica.setCameraPosition(glm::vec3((94.0f + movCanicax), (14.0f + movCanicay), (34.0f + movCanicaz)));


				// Clear the window
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				skybox.DrawSkybox(camaraCanica.calculateViewMatrix(), projection);
				shaderList[0].UseShader();
				uniformModel = shaderList[0].GetModelLocation();
				uniformProjection = shaderList[0].GetProjectionLocation();
				uniformView = shaderList[0].GetViewLocation();
				uniformEyePosition = shaderList[0].GetEyePositionLocation();
				uniformColor = shaderList[0].getColorLocation();
				uniformTextureOffset = shaderList[0].getOffsetLocation();

				//información en el shader de intensidad especular y brillo
				uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
				uniformShininess = shaderList[0].GetShininessLocation();

				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camaraCanica.calculateViewMatrix()));
				glUniform3f(uniformEyePosition, camaraCanica.getCameraPosition().x, camaraCanica.getCameraPosition().y, camaraCanica.getCameraPosition().z);
			}
			else
			{
				//Camara fija
				glfwPollEvents();
				//camaraFija.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


				// Clear the window
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				skybox.DrawSkybox(camaraFija.calculateViewMatrix(), projection);
				shaderList[0].UseShader();
				uniformModel = shaderList[0].GetModelLocation();
				uniformProjection = shaderList[0].GetProjectionLocation();
				uniformView = shaderList[0].GetViewLocation();
				uniformEyePosition = shaderList[0].GetEyePositionLocation();
				uniformColor = shaderList[0].getColorLocation();
				uniformTextureOffset = shaderList[0].getOffsetLocation();

				//información en el shader de intensidad especular y brillo
				uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
				uniformShininess = shaderList[0].GetShininessLocation();

				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camaraFija.calculateViewMatrix()));
				glUniform3f(uniformEyePosition, camaraFija.getCameraPosition().x, camaraFija.getCameraPosition().y, camaraFija.getCameraPosition().z);
			}
		}

		//posicionCamara = camera.getCameraPosition();
		//direccionCamara = camaraFija.getCameraDirection();

		// Fuentes de iluminación para shader
		shaderList[0].SetDirectionalLight(&mainLight);

		// Paletas
		if (mainWindow.getluzPaletas())
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount-1);
		}


		// Tablero
		if (mainWindow.getluzTablero())
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else 
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount-1);
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		// Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -143.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		// Maquina pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 30.0f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinball_M.RenderModel();

		// Animación palanca y resorte

		if (mainWindow.getAnimacionPalanca() == true && movPalanca <= 7.5f)
		{
			movPalanca += 0.003f;

			if (movResorte < 5.5)  //Limita que tanto retrocede el resorte
			{
				movResorte += 0.003f;
				escResorte -= 0.002f;
				anguloResorte += 0.004f;
			}

			if (movPalanca > 7.5f)
				mainWindow.setAnimacionPalanca(false);
		}
		else if (mainWindow.getAnimacionPalanca() == false && movPalanca > 0.0f)
		{
			movPalanca -= 0.07f;
			movResorte -= 0.07f;

			if (movResorte <= 0.0f)
			{
				movResorte = 0.0f;
				escResorte = 0.0f;
				anguloResorte = 0.0f;
			}

			if (movPalanca <= 0.0f)
			{
				movPalanca = 0.0f;
				movResorte = 0.0f;
				escResorte = 0.0f;
				anguloResorte = 0.0f;
			}
			escResorte += 0.04f;
			anguloResorte -= 0.06f;

		}

		// Resorte
		model = glm::mat4(1.0);
		if(movResorte < 5.5f)
			model = glm::translate(model, glm::vec3((108.0f + movResorte), 11.1f, -62.5f));
		else
			model = glm::translate(model, glm::vec3((108.0f + movResorte), (11.4f), -62.5f));
		model = glm::scale(model, glm::vec3((5.5f+escResorte), 5.0f, 5.0f));
		model = glm::rotate(model, (-15+anguloResorte)*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		resorte_M.RenderModel();


		// Palanca 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3((117.0f+movPalanca), 8.8f, -62.5f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 3.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		palanca_M.RenderModel();


		//Canica 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3((98.0f + movCanicax), (14.0f + movCanicay), (34.0f + movCanicaz)));
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
		model = glm::rotate(model, rotarCanica * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		canica1_M.RenderModel();

		//Canica 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(98.0f, 14.0f, 41.0f));
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		canica2_M.RenderModel();

		// Animacion canica
		if (mainWindow.getAnimacionCanica() == true)
		{
			rotarCanica += 30.0f* deltaTime;

			if (movPalanca <= 0.0f || (mainWindow.getAnimacionPalanca() == false && mainWindow.getSubir()))
			{
				if (movCanicax >= -180)   //La canica sube hasta el final del tubo
				{
					movCanicax -= 1.2f * deltaTime;
					movCanicay += 0.22 * deltaTime;
				}
				else if (movCanicaz <= -45)
				{
					movCanicaz += 3.0f * deltaTime;
				}
				else if (movCanicaz > -45)
				{
					mainWindow.setSubir(false);
				}
			}
			else if (movPalanca <= 0.0f || (mainWindow.getAnimacionPalanca() == false && mainWindow.getSubir()== false))
			{
				if (movCanicax <= -160)
				{
					movCanicay -= 0.02f * deltaTime;
					movCanicax += 5.0f * deltaTime;
					//movCanicaz += 0.01f * deltaTime;
				}
			}
			else if (movCanicaz > -96 || mainWindow.getAnimacionPalanca())  //Mueve la canica hasta el resorte
			{
				if (movCanicaz <= -96)   // La canica baja hasta que la animacion de la palanca termina
				{
					movCanicax += 0.01 * deltaTime;
					movCanicay -= 0.004 * deltaTime;
					rotarCanica = 0.0f;
				}
				else
				{
					movCanicaz -= 0.35 * deltaTime;
				}
			}
		}



		//OBSTACULOS RICARDO
		/////////////////////////////////////////////////////PALANCAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, 15.5f, 22.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (0 + mainWindow.getmoverpalanca01()) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		palanca_golpe_IZQ_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0, 15.5f, -28.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (0 + mainWindow.getmoverpalanca02()) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		palanca_golpe_DER_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.0f, 39.0f, 15.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (0 + mainWindow.getmoverpalanca03()) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		palanca_golpe_IZQ_M.RenderModel();

		///////////////////////////////////////////////// L

		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 18.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.7, 0.5, 1.2));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		obstaculo_L_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 17.5f, -45.0f));
		model = glm::scale(model, glm::vec3(1.7, 0.5, 1.2));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		obstaculo_L_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		///////////////////////////////////////////////////// C
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f, 20.5f, 30.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.8f, 2.0f));
		model = glm::rotate(model, 12 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		obstaculo_C_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f, 20.5f, -35.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.8f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 12 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		obstaculo_C_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		////////////////////////////////////////////////////////// CIRCULOS
		color = glm::vec3(0.325f, 0.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 29.5f, 25.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.0f));
		model = glm::rotate(model, 6 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		circulos_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 29.5f, -30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.0f));
		model = glm::rotate(model, 6 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		circulos_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		//////////////////////////////////////////////////// PUENTE

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.0f, 43.2f, -2.5f));
		model = glm::scale(model, glm::vec3(1.65f, 1.6f, 1.55f));
		model = glm::rotate(model, 13 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		puente_M.RenderModel();

		/////////////////////////////////////////////////////ECLIPSE
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 40.0f, 25.5f));
		model = glm::scale(model, glm::vec3(1.5f, 0.7f, 1.5f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		eclipce_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 40.0f, -30.5f));
		model = glm::scale(model, glm::vec3(1.5f, 0.7f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 190 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		eclipce_M.RenderModel();


		///////////////////////////////////////////////////////HONGO

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, 38.0f, -3.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-57.0f, 41.5f, 11.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-52.0f, 40.5f, -15.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		//////////////////////////////////////////////////////////ELIPSES
		color = glm::vec3(0.325f, 0.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 46.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		elipse_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 46.0f, 5.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		elipse_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		//////////////////////////////////////////////////EDIFICIOS

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 22.8f, 13.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		edificio_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 22.8f, -18.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		edificio_M.RenderModel();

		///////////////////////////////////////////////////BANDERAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 14.8f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bandera_roja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 14.8f, -15.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bandera_azul_M.RenderModel();


		// Avatar sin texturas 
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 14.8f, -15.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avatar_M.RenderModel();*/

		// Objetos traslucidos 
		
		// Tubo amarillo traslucido
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 30.0f, -63.0f));
		model = glm::scale(model, glm::vec3(3.0f, 4.0f, 4.7f));
		model = glm::rotate(model, -7 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		tuboTraslucido_M.RenderModel();
		glDisable(GL_BLEND);


		// Cristal traslucido maquina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 29.0f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristalPinball_M.RenderModel();
		glDisable(GL_BLEND);


		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	if (cancion)
		cancion->drop(); // release music stream.

	sonido->drop(); // delete engine

	//printf("Posicion de camara: %0.2f, %0.2f, %0.2f", posicionCamara.x, posicionCamara.y, posicionCamara.z);
	//printf("Direccion de camara: %0.2f, %0.2f, %0.2f", direccionCamara.x, direccionCamara.y, direccionCamara.z);

	return 0;/* 182.61, 155.09, -11.65*/
}
