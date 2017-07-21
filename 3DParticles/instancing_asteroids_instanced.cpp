// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "Particle.h"

// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "SpotLight.h"
#include "Lamp.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "filesystem.h"

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1024;
#define NUM_PARTICLES 10000

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();


// Camera
Camera camera(glm::vec3(2.0f, 0.0f, 3.0f));


Attenuation attenuation = Attenuation (1.0f, 0.09f, 0.032f);
Intensity intensity = Intensity (glm::vec3 (0.1f, 0.1f, 0.1f),glm::vec3 (1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f) );

SpotLight spotLight = SpotLight (1, glm::vec3(1.0f,1.0f,1.0f),  glm::vec4(0.0f, 0.0f, 3.0f,1.0f),
                                 glm::vec4(0.0f, 0.0f, -1.0f,1.0f),attenuation,12.5f, 15.5f, intensity);

Lamp lamp = Lamp (glm::vec3(0.0f,0.0f,2.0f), 0.04f);



bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool start = false;
bool firstStep= false;


GLfloat rotationY=0.0f;
GLfloat rotationX=0.0f;

void updatePositions (std::vector <Particle>& particles, glm::mat4* modelMatrices, Model & rock, GLuint & buffer)
{
    for(GLuint i = 0; i < NUM_PARTICLES; i++)
    {
        //        glm::mat4 model;
        if(start)
            particles[i].updatePosition();

        modelMatrices[i] = glm::mat4();
        // 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]

        modelMatrices[i] = glm::rotate(modelMatrices[i], rotationY, glm::vec3(0.0, 1.0, 0.0));
        modelMatrices[i] = glm::rotate(modelMatrices[i], rotationX, glm::vec3(1.0, 0.0, 0.0));
        modelMatrices[i] = glm::translate(modelMatrices[i], particles[i].getPosition());

        // 2. Scale: Scale between 0.05 and 0.25f
        modelMatrices[i] = glm::scale(modelMatrices[i], glm::vec3(0.007f, 0.007f, 0.007f));

        // 4. Now add to list of matrices
        //        modelMatrices[i] = model;
    }

    // forward declare the buffer

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

    if (!firstStep)
    {
        firstStep = true;
    }
    else
        return;
    // Set transformation matrices as an instance vertex attribute (with divisor 1)

    for(GLuint i = 0; i < rock.meshes.size(); i++)
    {
        GLuint VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // Set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

}

int main()
{

    camera.Front = glm::vec3(-1.0f, 0.0f, -1.0f);
    GLfloat range [6] = {-0.5,0.5,-0.5,0.5, -0.5,0.5};
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Particle 3D", NULL, NULL); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader cubeShader("../Shaders/shader.vs", "../Shaders/shader.frag");
    Shader instanceShader("../Shaders/instanced_asteroids.vs", "../Shaders/instanced_asteroids.frag");
    Shader lampShader("../Shaders/planet.vs", "../Shaders/planet.frag");

    // Load models
    Model rock("../Model/rock/sphere.obj");
    Model lampModel("../Model/bulb/lamp.obj");




    GLfloat vertices[] ={
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -1,1,1,
        1,1,1,

        1,1,1,
        1,-1,1,

        1,-1,1,
        -1,-1,1,

        -1,-1,1,
        -1,1,1,

        -1,-1,1,
        -1,-1,-1,

        -1,-1,-1,
        -1,1,-1,

        -1,1,-1,
        -1,1,1,

        -1,1,-1,
        1,1,-1,

        1,1,-1,
        1,1,1,

        -1,-1,-1,
        1,-1,-1,

        1,-1,-1,
        1,-1,1,

        1,-1,-1,
        1,1,-1
    };




    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO); // We can also generate multiple VAOs or buffers at the same time
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO


    //light VAO


    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    // Set projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
    cubeShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Also of instance shader
    instanceShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[NUM_PARTICLES];

    // ================================
    // Generation points setup
    // ===============================

    std::vector <Particle> particles;
    for (int i = 0; i< NUM_PARTICLES; i++)
    {
        Particle p (range);
        particles.push_back(p);


    }




    GLuint buffer;
    glGenBuffers(1, &buffer);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        updatePositions(particles, modelMatrices, rock, buffer);
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear buffers
        glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Add transformation matrices
        cubeShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "view"), 1, GL_FALSE,  glm::value_ptr(camera.GetViewMatrix()));
        instanceShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(instanceShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));


        // Draw CUBE
        cubeShader.Use();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(0);
        glm::mat4 model = glm::mat4();
        model = glm::rotate(model, rotationY, glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, rotationX, glm::vec3(1.0, 0.0, 0.0));

        // Pass them to the shaders
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Draw container
        glBindVertexArray(VAO);

        glUniform4f(glGetUniformLocation(cubeShader.Program, "inColor"), 1.0f, 1.0f, 0.0f, 0.2f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniform4f(glGetUniformLocation(cubeShader.Program, "inColor"), 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 36, 36);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glDepthMask(1);



        // Draw particles
        instanceShader.Use();

        spotLight.SetUniformData(&instanceShader,"light");
        // Set material properties
        glUniform1f(glGetUniformLocation(instanceShader.Program, "material.shininess"), 32.0f);


        // NB: This could all be implemented as a method within the Model class, perhaps "DrawInstanced(const GLuint amount)"
        glActiveTexture(GL_TEXTURE0); // Activate proper texture unit before binding
        glUniform1i(glGetUniformLocation(instanceShader.Program, "texture_diffuse1"), 0); // Now set the sampler to the correct texture unit
        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
        for(GLuint i = 0; i < rock.meshes.size(); i++)
        {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, NUM_PARTICLES);
            glBindVertexArray(0);
        }
        // reset our texture binding
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);




        lamp.draw(lampShader, camera.GetViewMatrix(), projection, lampModel);
        // Swap the buffers
        glfwSwapBuffers(window);

    }

    delete[] modelMatrices;


    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if(keys[GLFW_KEY_UP])
        rotationX+=-deltaTime;
    if(keys[GLFW_KEY_DOWN])
        rotationX+=deltaTime;
    if(keys[GLFW_KEY_LEFT])
       rotationY+=deltaTime;
    if(keys[GLFW_KEY_RIGHT])
        rotationY+=-deltaTime;

    if(keys[GLFW_KEY_K])
    {
        spotLight.do_Movement("LEFT", deltaTime);
        lamp.do_Movement("LEFT", deltaTime);
    }


    if(keys[GLFW_KEY_L])
    {
        spotLight.do_Movement("RIGHT", deltaTime);
        lamp.do_Movement("RIGHT", deltaTime);
    }

}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        start= !start;

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
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

//    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
