/* General includes */
#include <iostream>
#include <vector>

#include "../Include/CubeGeometry.h"
#include "../Include/PlaneGeomtery.h"
#include "../Include/UVSphereGeometry.h"
#include "../Include/ConeGeometry.h"
#include "../Include/TriangleMesh.h"
#include "../Include/LineArray.h"
#include "../Include/GraphicsObject.h"
#include "../Include/OBJMesh.h"
#include "../Include/tank/TankObject.h"
#include "../Include/Skybox.h"

/* Screen parameters */
const int width = 800;
const int height = 450;

/* Functions to handle input */
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_movement(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

/* Stuff to read the mouse input to move the camera */
GLfloat lastX = width / 2.0;
GLfloat lastY = height / 2.0;

bool firstMouseInput = true;

//Mouse button flags
bool middleMouse = false;

//Key pressed flags
bool keys[1024];

//For calculating delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//For scene selection
static int e = 0;
bool stillRunning = true;

int main(void)
{
    /* Attempt to initialise GLFW3, the window manager */
	if(!glfwInit())
	{
		std::cout << "Failed to start GLFW3" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Set up the main window */
	GLFWwindow* window = glfwCreateWindow(width, height, "Coursework 1", NULL, NULL);
	if(!window)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* Set the required callback functions */
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_movement);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Set up GLEW before using any OpenGL functions */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW." << std::endl;
		return -1;
	}

	/* Tell OpenGL the size of the rendering window */
	glViewport(0, 0, width, height);

	/* Turn on depth testing to make stuff in front actually look like it's in front. */
	glEnable(GL_DEPTH_TEST);

    /* Load the shader programs */
	Shader celShader("Shaders/CelShader.vert", "Shaders/CelShader.frag");
	Shader skyboxShader("Shaders/SkyboxShader.vert", "Shaders/SkyboxShader.frag");

	Skybox sky("Images/mp_tf/mp_tf/thefog");

    /* Create the first tank */
    TankObject duskTank("Images/DuelTankBody", "Images/DuelTankTurret", "Images/DuelTankGun", glm::vec3(0.0f), glm::quat());

    const struct Material rock1Mat = {"Images/Rock/Rock1_DIFFUSE.png", "Images/Rock/Rock1_SPECULAR.png", 8.0f};
    OBJMesh rock1Mesh("Models/Rock1.obj", rock1Mat);
    GraphicsObject RockObject(&rock1Mesh, glm::vec3(5.0f, 0.0f, 5.0f), glm::quat());

    const struct Material floatingIsland1Mat = {"Images/Rock/FloatingIsland1DIFFUSE.png", "Images/Rock/Rock1_SPECULAR.png", 8.0f};
    OBJMesh floatingIsland1Mesh("Models/FloatingIsland1.obj", floatingIsland1Mat);
    GraphicsObject floatingIslandObject(&floatingIsland1Mesh, glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(), 2.0f);

    /* Create some lights */
    std::vector<LightSource*> lights;
    DirectionalLight sun(glm::vec3(-0.5f, -1.0f, -0.5f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));
    PointLight defaultPoint(LIGHT_POS, 1.0, 0.09, 0.032, 0, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
    SpotLight spotLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
    lights.push_back(&sun);
    lights.push_back(&defaultPoint);
    lights.push_back(&spotLight);

	/* Main loop */
	while(!glfwWindowShouldClose(window) && stillRunning)
	{
	    //Calculate the time since the last frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		/* Rendering commands */
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //Black
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Generate the view matrix */
		glm::mat4 view;
		view = camera.GetViewMatrix();
		/* Generate the projection matrix */
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

		celShader.Use();
		duskTank.RotateTurret(deltaTime);
		duskTank.Draw(celShader, view, projection, lights);
		RockObject.Draw(celShader, view, projection, lights);
		floatingIslandObject.Draw(celShader, view, projection, lights);

		//Draw the skybox last
		skyboxShader.Use();
		sky.Draw(skyboxShader, view, projection);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
	}

	/* Terminate properly */
	glfwTerminate();
	return 0;
}

/*
* Record the states of keys when one changes
*/
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Check to see if a new key has been pressed or released
	if (action == GLFW_PRESS)
	{
		keys[key] = true;

		//Additional checks for scene selection
		if(keys[GLFW_KEY_A])
            e = 0;
        else if(keys[GLFW_KEY_B])
            e = 1;
        else if(keys[GLFW_KEY_C])
            e = 2;
        else if(keys[GLFW_KEY_D])
            e = 3;
        else if(keys[GLFW_KEY_E])
            e = 4;
        else if(keys[GLFW_KEY_F])
            e = 5;
        else if(keys[GLFW_KEY_Q] || keys[GLFW_KEY_ESCAPE])
            stillRunning = false; //Set the flag to close next frame
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_input(yoffset);
}

/*
* Record the changes in position of the mouse, use it to update the camera
*/
void mouse_movement(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouseInput)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouseInput = false;
	}

	//Difference between current moose position and previous
	GLfloat deltaX = xPos - lastX;
	GLfloat deltaY = lastY - yPos;

	//Update previous
	lastX = xPos;
	lastY = yPos;


	if (middleMouse)
	{
		if (keys[GLFW_KEY_LEFT_SHIFT])
			camera.pan_camera(deltaX, deltaY);
		else
			camera.move_camera(deltaX, deltaY);
	}
}

/*
* Record the state of the middle mouse when it changes
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		middleMouse = true;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		middleMouse = false;
}
