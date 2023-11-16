#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLboolean getAnimacionPalanca() { return animacionPalanca; }
	GLvoid setAnimacionPalanca(bool activacion) { animacionPalanca = activacion; }
	GLboolean getAnimacionCanica() { return animacionCanica; }
	GLvoid setAnimacionCanica(bool activacion) { animacionCanica = activacion; }
	GLint getmoverpalanca01() { return moverpalanca01; }
	GLint getmoverpalanca02() { return moverpalanca02; }
	GLint getmoverpalanca03() { return moverpalanca03; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLint moverpalanca01 = 0;
	GLint moverpalanca02 = 0;
	GLint moverpalanca03 = 0;
	
	GLboolean animacionPalanca;
	GLboolean animacionCanica;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void ManejaMouseBoton(GLFWwindow* window, int button, int action, int mods);

};

