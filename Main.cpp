///*
//Semestre 2024-1
//Alumno:
//
//*/
////para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <chrono>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

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

const float toRadians = 3.14159265f / 180.0f;


// Variables de animación
float movPalanca = 0.0f;
float movResorte = 0.0f;
float escResorte = 0.0f;
float anguloResorte = 0.0f;




Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;




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

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.9f, 0.6f);


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


	// Skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 0.0f, 1.0f,
		5.0f, 3.0f,
		-39.0f, 40.0f, -3.5f, 
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		5.0f, 3.0f,
		-59.0f, 42.5f, 11.0f, 
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,
		5.0f, 3.0f,
		-54.0f, 41.5f, -15.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		120.0f, 20.0f, -2.5f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		-10.0f, 30.0f, -60.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-10.0f, 30.0f, 55.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


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

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


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
		model = glm::translate(model, glm::vec3(98.0f, 14.5f, 34.0f));
		//model = glm::translate(model, glm::vec3(18.0f, 28.5f, -63.0f));  //Aux para ver canica en tubo amarillo
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		canica1_M.RenderModel();

		//Canica 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-99.0f, 60.0f, -2.5f));
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		canica2_M.RenderModel();


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

		////////////////////////////////////////////////////////// CIRCULOS

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

		//////////////////////////////////////////////////////////ELIPSES

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

	return 0;
}
