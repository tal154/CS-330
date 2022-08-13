// Author: Anthony Lee
// code from SNHU tutorial will be initialed: AL
#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions

// GLM Math header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "ShapeGenerator.h"
#include "Vertex.h"

using namespace std; // Uses the standard namespace

// shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// code from tutorial AL
// Unnamed namespace
namespace
{
	const char* const WINDOW_TITLE = "Project Anthony Lee"; // Macro for window title

	// Variables for window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// Stores the GL data relative to a given mesh
	struct GLMesh
	{
		GLuint cubeVAO, cubeVBO;     // Handle for the vertex buffer object
		GLuint planeVAO, planeVBO;
		GLuint cylinderVAO, cylinderVBO;
		GLuint pyramidVAO, torusVBO;

		GLuint cubeNumIndices;   // Number of vertices of the mesh
		GLuint planeNumIndices;
		GLuint cylinderNumIndices;
		GLuint torusNumIndices;


		GLuint cubeIndexByteOffset;
		GLuint planeIndexByteOffset;
		GLuint cylinderIndexByteOffset;
		GLuint pyramidIndexByteOffset;


	};

	// Main GLFW window
	GLFWwindow* gWindow = nullptr;
	// Triangle mesh data
	GLMesh gMesh;
	// Shader program
	GLuint gProgramId;
	GLuint gLampProgramId;
	GLuint gLampProgramId2;

	GLuint gTextureId1;
	GLuint gTextureId2;
	GLuint gTextureId3;
	GLuint gTextureId4;
	bool gIsHatOn = true;

	glm::vec2 gUVScale(5.0f, 5.0f);
	GLint gTexWrapMode = GL_REPEAT;

	// MAKE SURE TO UPDATE SHAPE GENERATOR FOR NORMALS INSTEAD OF COLOR
	const uint NUM_VERTICES_PER_TRI = 3;
	const uint NUM_FLOATS_PER_VERTICE = 8;
	const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

	// camera
	Camera gCamera(glm::vec3(0.0f, 0.0f, 5.0f));
	GLuint theBufferID;
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;


	// timing
	float gDeltaTime = 0.0f; // time between current frame and last frame
	float gLastFrame = 0.0f;

	bool orthographic = false;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

	// Cube and light color
	glm::vec3 gObjectColor(1.f, 1.0f, 1.0f);
	glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 gLightColor2(.0f, .0f, 1.0f);


	// Light position and scale
	glm::vec3 gLightPosition(1.5f, 1.5f, .0f);
	glm::vec3 gLightPosition2(0.0f, -2.f, -15.0f);
	glm::vec3 gLightScale(.50f);
	glm::vec3 gLightScale2(.50f);
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
 // code from tutorial AL
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
bool UInitialize(int, char*[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh &mesh);
void UDestroyMesh(GLMesh &mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId);
void UDestroyShaderProgram(GLuint programId);
void keyCallback(GLFWwindow* window, int key, int scan, int action, int mods);
bool UCreateTexture(const char* filename, GLuint &textureId);
void UDestroyTexture(GLuint textureId);



/* Vertex Shader Source Code*/
const GLchar * vertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

	vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

	vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	vertexTextureCoordinate = textureCoordinate;
}
);

/* Fragment Shader Source Code*/
const GLchar * fragmentShaderSource = GLSL(440,
	in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // for outgoing color

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightColor2;
uniform vec3 lightPos;
uniform vec3 lightPos2;
uniform vec3 viewPosition;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform bool multipleTextures;
uniform vec2 uvScale;

void main()
{
	/*Phong lighting model calculations to generate ambient, diffuse, and specular components* /

	//Calculate Ambient lighting*/
	float ambientStrength = 0.3f; // Set ambient or global lighting strength
	vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

	//Calculate Diffuse lighting*/
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse = impact * lightColor; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity = 1.0f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;

	fragmentColor = texture(uTexture, vertexTextureCoordinate * uvScale);
	if (multipleTextures)
	{
		vec4 extraTexture = texture(uTexture4, vertexTextureCoordinate * uvScale);
		if (extraTexture.a != 0.0)
			fragmentColor = extraTexture;
	}

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

	// Calculate phong result
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU

	vec3 norm2 = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection2 = normalize(lightPos2 - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact2 = max(dot(norm, lightDirection2), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse2 = impact2 * lightColor2; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity2 = 0.8f; // Set specular light strength
	float highlightSize2 = 16.0f; // Set specular highlight size
	vec3 viewDir2 = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir2 = reflect(-lightDirection2, norm2);// Calculate reflection vector
	//Calculate specular component
	float specularComponent2 = pow(max(dot(viewDir2, reflectDir2), 0.0), highlightSize2);
	vec3 specular2 = specularIntensity2 * specularComponent2 * lightColor2;

	vec3 phong2 = (ambient + diffuse2 + specular2) * textureColor.xyz;

	fragmentColor += vec4(phong2, 1.0);
}
);

/* Lamp Shader Source Code*/
const GLchar * lampVertexShaderSource = GLSL(440,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

		//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);


/* Fragment Shader Source Code*/
const GLchar * lampFragmentShaderSource = GLSL(440,

	out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

void main()
{
	fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

/* Lamp Shader2 Source Code*/
const GLchar * lampVertexShaderSource2 = GLSL(440,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

		//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);


/* Fragment Shader Source Code*/
const GLchar * lampFragmentShaderSource2 = GLSL(440,

	out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

void main()
{
	fragmentColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char *image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}


// code from tutorial AL
// main function. Entry point to the OpenGL program
int main(int argc, char* argv[])
{
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// Create the mesh
	UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object

	// Create the shader program
	UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId);




	if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
		return EXIT_FAILURE;

	if (!UCreateShaderProgram(lampVertexShaderSource2, lampFragmentShaderSource2, gLampProgramId2))
		return EXIT_FAILURE;

	const char * texFilename = "7109446991_24746694d5_b.jpg";
	if (!UCreateTexture(texFilename, gTextureId1))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	texFilename = "photo-1601370690183-1c7796ecec61.jpg";
	if (!UCreateTexture(texFilename, gTextureId2))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	texFilename = "Bluesquare.png";
	if (!UCreateTexture(texFilename, gTextureId3))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}
	texFilename = "2200497.jpg";
	if (!UCreateTexture(texFilename, gTextureId4))
	{
		cout << "Failed to load texture " << texFilename << endl;
		return EXIT_FAILURE;
	}

	// Tell OpenGL for each sampler which texture unit it belongs to (only has to be done once).
	glUseProgram(gProgramId);
	// We set the texture as texture unit 0.
	glUniform1i(glGetUniformLocation(gProgramId, "uTexture1"), 0);
	// We set the texture as texture unit 1
	glUniform1i(glGetUniformLocation(gProgramId, "uTexture1"), 1);
	// We set the texture as texture unit 2
	glUniform1i(glGetUniformLocation(gProgramId, "uTexture1"), 2);
	//// We set the texture as texture unit 3
	glUniform1i(glGetUniformLocation(gProgramId, "uTexture1"), 3);
	//// We set the texture as texture unit 3



	// Sets the background color of the window to black (it will be implicitely used by glClear)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(gWindow))
	{
		// per-frame timing
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

		// input
		// -----
		UProcessInput(gWindow);

		// Render this frame
		URender();

		glfwPollEvents();
	}

	// Release mesh data
	UDestroyMesh(gMesh);

	// Release texture
	UDestroyTexture(gTextureId1);
	UDestroyTexture(gTextureId2);
	UDestroyTexture(gTextureId3);

	// Release shader program
	UDestroyShaderProgram(gProgramId);

	exit(EXIT_SUCCESS); // Terminates the program successfully
}


// code from tutorial AL
// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// GLFW: window creation
	// ---------------------
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);

	glfwSetCursorPosCallback(*window, UMousePositionCallback);
	glfwSetScrollCallback(*window, UMouseScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Displays GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

	return true;
}
// code from tutorial AL
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		gCamera.ProcessKeyboard(DOWN, gDeltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCamera.ProcessKeyboard(UP, gDeltaTime);

	glfwSetKeyCallback(window, keyCallback);
}

void keyCallback(GLFWwindow* window, int key, int scan, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (orthographic)
		{
			projection = glm::ortho(-5.f, 5.f, -5.f, 5.f, -50.f, 50.f);
			orthographic = false;
		}
		else
		{
			projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
			orthographic = true;
		}
	}
}

// code from tutorial AL
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; // Reversed since y-coordinates go from bottom to top

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// camera.h has been changed to adjust camera speed and not zoom
	gCamera.ProcessMouseScroll(yoffset);
}



// code from tutorial AL
// Functioned called to render a frame
void URender()
{
	float angle;

	//enable z-depth
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	// 1. Scales the object by 2
	glm::mat4 scaleHitbox = glm::scale(glm::vec3(10.0f, 1.0f, 5.0f));
	glm::mat4 scaleSmallButton = glm::scale(glm::vec3(.150f, .25f, .150f));
	glm::mat4 scalePlane = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 scalePyramid = glm::scale(glm::vec3(3.0f, 10.0f, 3.0f));
	glm::mat4 scaleLargeButton = glm::scale(glm::vec3(.250f, .250f, .250f));
	glm::mat4 scaleBatteryPack = glm::scale(glm::vec3(2.0f, .20f, 4.0f));


	// rotating
	glm::mat4 rotation = glm::rotate(180.f, glm::vec3(0.f, 1.0f, 0.f));

	glm::mat4 rotation2 = glm::rotate(30.f, glm::vec3(0.f, .0f, .1f));


	//translations
	//hitbox
	glm::mat4 translationHitbox = glm::translate(glm::vec3(0.0, -3.0f, -10.0f));
	//buttons
	glm::mat4 translationButton1 = glm::translate(glm::vec3(1.0, -2.8f, -10.0f));
	glm::mat4 translationButton2 = glm::translate(glm::vec3(2.0, -2.8f, -10.0f));
	glm::mat4 translationButton3 = glm::translate(glm::vec3(3.0, -2.8f, -10.0f));
	glm::mat4 translationButton4 = glm::translate(glm::vec3(4.0, -2.8f, -10.0f));
	glm::mat4 translationButton5 = glm::translate(glm::vec3(1.0, -2.8f, -11.0f));
	glm::mat4 translationButton6 = glm::translate(glm::vec3(2.0, -2.8f, -11.0f));
	glm::mat4 translationButton7 = glm::translate(glm::vec3(3.0, -2.8f, -11.0f));
	glm::mat4 translationButton8 = glm::translate(glm::vec3(4.0, -2.8f, -11.0f));
	glm::mat4 translationButton9 = glm::translate(glm::vec3(-1.0, -2.8f, -10.0f));
	glm::mat4 translationButton10 = glm::translate(glm::vec3(-1.5, -2.8f, -10.2f));
	glm::mat4 translationButton11 = glm::translate(glm::vec3(-2.0, -2.8f, -10.0f));
	glm::mat4 translationButton12 = glm::translate(glm::vec3(0.0, -2.8f, -9.0f));
	//pyramid
	glm::mat4 translationPyramid = glm::translate(glm::vec3(8.0, -3.5f, -12.0f));
	//plane
	glm::mat4 translationBattery = glm::translate(glm::vec3(-4.0, -2.345f, -9.5f));
	// Model matrix: Transformations are applied right-to-left.
	// hitbox
	glm::mat4 hitbox = translationHitbox * 1.0f * scaleHitbox;
	// button
	glm::mat4 button1 = translationButton1 * 1.0f * scaleSmallButton;
	glm::mat4 button2 = translationButton2 * 1.0f * scaleSmallButton;
	glm::mat4 button3 = translationButton3 * 1.0f * scaleSmallButton;
	glm::mat4 button4 = translationButton4 * 1.0f * scaleSmallButton;
	glm::mat4 button5 = translationButton5 * 1.0f * scaleSmallButton;
	glm::mat4 button6 = translationButton6 * 1.0f * scaleSmallButton;
	glm::mat4 button7 = translationButton7 * 1.0f * scaleSmallButton;
	glm::mat4 button8 = translationButton8 * 1.0f * scaleSmallButton;
	glm::mat4 button9 = translationButton9 * 1.0f * scaleSmallButton;
	glm::mat4 button10 = translationButton10 * 1.0f * scaleSmallButton;
	glm::mat4 button11 = translationButton11 * 1.0f * scaleSmallButton;
	glm::mat4 button12 = translationButton12 * 1.0f * scaleLargeButton;
	// plane
	glm::mat4 plane = translationHitbox * 1.0f * scalePlane;
	// pyramid
	glm::mat4 pyramid = translationPyramid * rotation * scalePyramid;
	// battery
	glm::mat4 battery = translationBattery * 1.0f * scaleBatteryPack;


	glm::mat4 transformation(1.0f);

	// camera/view transformation
	glm::mat4 view = gCamera.GetViewMatrix();

	// Set the shader to be used
	glUseProgram(gProgramId);

	// Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(gProgramId, "lightColor");
	GLint lightColorLoc2 = glGetUniformLocation(gProgramId, "lightColor2");
	GLint lightPositionLoc = glGetUniformLocation(gProgramId, "lightPos");
	GLint lightPositionLoc2 = glGetUniformLocation(gProgramId, "lightPos2");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");

	// Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
	glUniform3f(lightColorLoc2, gLightColor2.r, gLightColor2.g, gLightColor2.b);
	glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
	glUniform3f(lightPositionLoc2, gLightPosition2.x, gLightPosition2.y, gLightPosition2.z);
	const glm::vec3 cameraPosition = gCamera.Position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	GLint UVScaleLoc = glGetUniformLocation(gProgramId, "uvScale");
	glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));
	// Sends transform information to the Vertex shader
	GLuint transformLocation = glGetUniformLocation(gProgramId, "shaderTransform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformation));
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, gIsHatOn);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId1);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cubeVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(hitbox));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the hitbox
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cubeNumIndices);

	glm::vec2 gUVScale(1.0f, 1.0f);
	GLint UVScaleLoc1 = glGetUniformLocation(gProgramId, "uvScale");
	glUniform2fv(UVScaleLoc1, 1, glm::value_ptr(gUVScale));
	// Sends transform information to the Vertex shader

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId4);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(battery));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the hitbox
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cubeNumIndices);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId3);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button1));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button2));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button3));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button4));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button5));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button6));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button7));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button8));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button9));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button10));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button11));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// Activate the VBOs contained within the mesh's VAO for cube
	glBindVertexArray(gMesh.cylinderVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(button12));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the buttons
	glDrawArrays(GL_TRIANGLES, 0, gMesh.cylinderNumIndices);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId1);
	// torus
	glBindVertexArray(gMesh.pyramidVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pyramid));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// draws pyramid
	glDrawArrays(GL_TRIANGLES, 0, gMesh.torusNumIndices);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureId2);
	// activea VBOs contained in mesh's VAO for plane
	glBindVertexArray(gMesh.planeVAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(plane));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// draws plane
	glDrawArrays(GL_TRIANGLES, 0, gMesh.planeNumIndices);

	// LAMP: draw lamp 1 a.k.a. ceiling lamp
	//----------------
	glUseProgram(gLampProgramId);

	glBindVertexArray(gMesh.cubeVAO);

	//Transform the smaller cube used as a visual que for the light source
	glm::mat4 model = glm::translate(gLightPosition) * glm::scale(gLightScale);

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(gLampProgramId, "model");
	viewLoc = glGetUniformLocation(gLampProgramId, "view");
	projLoc = glGetUniformLocation(gLampProgramId, "projection");

	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, gMesh.cubeNumIndices);

	// lamp 2 a.k.a. monitor light
	glUseProgram(gLampProgramId2);

	//Transform the smaller cube used as a visual que for the light source
	model = glm::translate(gLightPosition2) * glm::scale(gLightScale2);

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(gLampProgramId, "model");
	viewLoc = glGetUniformLocation(gLampProgramId, "view");
	projLoc = glGetUniformLocation(gLampProgramId, "projection");

	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, gMesh.cubeNumIndices);

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implements the UCreateMesh function
void UCreateMesh(GLMesh &mesh)
{

	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData plane = ShapeGenerator::makePlane();
	ShapeData cylinder = ShapeGenerator::makeCylinder();
	ShapeData pyramid = ShapeGenerator::makePyramid();

	//glGenVertexArrays(1, &mesh.cubeVAO); // we can also generate multiple VAOs or buffers at the same time
	//glBindVertexArray(mesh.cubeVAO);

	glGenBuffers(1, &theBufferID);
	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		cube.vertexBufferSize() + cube.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize()
		+
		cylinder.vertexBufferSize() + cylinder.indexBufferSize() +
		pyramid.vertexBufferSize() + pyramid.indexBufferSize()
		, 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	mesh.cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	mesh.planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cylinder.vertexBufferSize(), cylinder.vertices);
	currentOffset += cylinder.vertexBufferSize();
	mesh.cylinderIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cylinder.indexBufferSize(), cylinder.indices);
	currentOffset += cylinder.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, pyramid.vertexBufferSize(), pyramid.vertices);
	currentOffset += pyramid.vertexBufferSize();
	mesh.pyramidIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, pyramid.indexBufferSize(), pyramid.indices);
	currentOffset += pyramid.indexBufferSize();

	mesh.cubeNumIndices = cube.numIndices;

	mesh.planeNumIndices = plane.numIndices;

	mesh.cylinderNumIndices = cylinder.numIndices;

	mesh.torusNumIndices = pyramid.numIndices;


	glGenVertexArrays(1, &mesh.cubeVAO);

	glGenVertexArrays(1, &mesh.planeVAO);

	glGenVertexArrays(1, &mesh.cylinderVAO);

	glGenVertexArrays(1, &mesh.pyramidVAO);


	// Create VBO
	// cube
	glBindVertexArray(mesh.cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	// plane
	glBindVertexArray(mesh.planeVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	// cylinder
	glBindVertexArray(mesh.cylinderVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint cylinderByteOffset = planeByteOffset + plane.vertexBufferSize() + plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)cylinderByteOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cylinderByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cylinderByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	// torus
	glBindVertexArray(mesh.pyramidVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint pyramidByteOffset = cylinderByteOffset + cylinder.vertexBufferSize() + plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)pyramidByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(pyramidByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(pyramidByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);
}

// code from tutorial AL
void UDestroyMesh(GLMesh &mesh)
{
	glDeleteVertexArrays(1, &mesh.cubeVAO);
	glDeleteBuffers(2, &mesh.cubeVBO);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint &textureId)
{
	int width, height, channels;
	unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVertically(image, width, height, channels);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			cout << "Not implemented to handle image with " << channels << " channels" << endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}


void UDestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}


// code from tutorial AL
// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId)
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	programId = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile the vertex shader
	// check for shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glCompileShader(fragmentShaderId); // compile the fragment shader
	// check for shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);   // links the shader program
	// check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programId);    // Uses the shader program

	return true;
}



// code from tutorial AL
void UDestroyShaderProgram(GLuint programId)
{
	glDeleteProgram(programId);
}