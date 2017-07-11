// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

#include "Texture.h"
#include "Model.h"

#include "ObjectWithPosition.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Properties
GLuint screenWidth = 1000, screenHeight = 1000;
std::vector <ObjectWithPosition> cubes;
std::vector <ObjectWithPosition> spheres;

// Function prototypes
bool detectCollisions(const float & delta);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();


// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "fpsCameraClass - LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader ourShader("../shaders/shader.vs", "../shaders/shader.frag");
    // Setup and compile our shaders
    Shader modelShader("../shaders/model_loading.vs", "../shaders/model_loading.frag");

    Model ourModel("../Models/earth/earth.obj",1);


    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {


        -100.0f, -0.0f, -100.0f,  0.0f, 100.0f,
        100.0f, -0.0f, -100.0f,  100.0f, 100.0f,
        100.0f, -0.0f,  100.0f,  100.0f, 0.0f,
        100.0f, -0.0f,  100.0f,  100.0f, 0.0f,
        -100.0f, -0.0f,  100.0f,  0.0f, 0.0f,
        -100.0f, -0.0f, -100.0f,  0.0f, 100.0f,

    };

    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat cube[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    cubes.push_back(ObjectWithPosition(glm::vec3(2.0f, 2.0f, 2.0f), 1.0f));
    cubes.push_back(ObjectWithPosition(glm::vec3(5.0f, 0.5f, 3.0f), 1.0f));
    cubes.push_back(ObjectWithPosition(glm::vec3(8.0f, 0.5f, 2.0f), 1.0f));
    spheres.push_back(ObjectWithPosition(glm::vec3(-10.0f, 0.0f, 15.0f), 1.0f));
    spheres.push_back(ObjectWithPosition(glm::vec3(-8.0f, 0.0f, -2.0f), 1.0f));

    GLuint VBOFloor, VAOFloor;
    glGenVertexArrays(1, &VAOFloor);
    glGenBuffers(1, &VBOFloor);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAOFloor);

    glBindBuffer(GL_ARRAY_BUFFER, VBOFloor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    GLuint VBOCube, VAOCube;
    glGenVertexArrays(1, &VAOCube);
    glGenBuffers(1, &VBOCube);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAOCube);

    glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    MyTexture textureFloor("../Images/floor.jpg");

    MyTexture textureCube("../Images/container2.png");

    textureFloor.setParameters(GL_REPEAT, GL_REPEAT,GL_LINEAR, GL_LINEAR);

    textureCube.setParameters(GL_REPEAT, GL_REPEAT,GL_LINEAR, GL_LINEAR);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth/(float)screenHeight, 0.1f, 50.0f);


        // Draw our first triangle
        ourShader.Use();

        textureFloor.bindTexture("ourTexture1", &ourShader);


        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAOFloor);

        // Calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));


        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        textureFloor.unbindTexture();

        textureCube.bindTexture("ourTexture1", &ourShader);
        for (int i = 0; i < cubes.size(); ++i) {


            glBindVertexArray(VAOCube);

            // Calculate the model matrix for each object and pass it to shader before drawing
            model = glm::mat4();
            model = glm::translate(model, cubes[i].getPosition());


            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        glBindVertexArray(0);
        textureCube.unbindTexture();

        modelShader.Use();   // <-- Don't forget this one!
        // Transformation matrices
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        for (int i = 0; i < spheres.size(); ++i) {

            // Draw the loaded model
            model = glm::mat4();
            model = glm::translate(model, spheres[i].getPosition()); // Translate it down a bit so it's at the center of the scene
            //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
            glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(modelShader.Program, "texture_diffuse1"), 1);
            ourModel.Draw(modelShader);
        }





        // Swap the buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAOFloor);
    glDeleteBuffers(1, &VBOFloor);

    glDeleteVertexArrays(1, &VAOCube);
    glDeleteBuffers(1, &VBOCube);
    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W] && !detectCollisions(deltaTime))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S] && !detectCollisions(-deltaTime))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A] && !detectCollisions(deltaTime))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D] && !detectCollisions(-deltaTime))
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;


    lastX = xpos;
    lastY = ypos;



    camera.ProcessMouseMovement(xoffset, 0.0f);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}



bool detectCollisions(const float & delta)
{
    for (int i = 0; i < cubes.size(); ++i) {
        if (cubes[i].detectCollision(camera, delta))
            return true;
    }

    for (int i = 0; i < spheres.size(); ++i) {
        if (spheres[i].detectCollision(camera, delta))
            return true;
    }
    return false;
}
