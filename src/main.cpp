#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";

int main()
{
    // Initialisation de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Création de la fenêtre
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Activation du contexte OpenGL
    glfwMakeContextCurrent(window);

    // Initialisation de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configuration du viewport et du callback pour le redimensionnement
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);



    // build and compile our shader program
    // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // pos(x,y,z)      // color(r,g,b)
        // Face avant
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        // Face arrière
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    };
    unsigned int indices[] = {
        // Face avant
        0, 1, 2,
        2, 3, 0,
        // Face arrière
        4, 5, 6,
        6, 7, 4,
        // Face gauche
        4, 0, 3,
        3, 7, 4,
        // Face droite
        1, 5, 6,
        6, 2, 1,
        // Face bas
        4, 5, 1,
        1, 0, 4,
        // Face haut
        3, 2, 6,
        6, 7, 3,
    };

    // Vertex Buffer Object and Vertex Array Object and Element Buffer Object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // get uniform location for model, view, projection
    unsigned int locModel = glGetUniformLocation(shaderProgram, "model");
    unsigned int locView = glGetUniformLocation(shaderProgram, "view");
    unsigned int locProjection = glGetUniformLocation(shaderProgram, "projection");

    // cube position and speed
    glm::vec3 cubePos = glm::vec3(0.0f);
    glm::vec3 cubeVel = glm::vec3(1.0f, 0.5f, 0.0f);

    // visibility and mouse state
    bool visible = true;
    bool prevMousePressed = false;

    // delta time variables
    double lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // compute delta time
        double currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // update cube position
        cubePos += cubeVel * deltaTime;

        // bounce within bounds
        if (cubePos.x <= -1.0f || cubePos.x >= 1.0f)
            cubeVel.x = -cubeVel.x;
        if (cubePos.y <= -1.0f || cubePos.y >= 1.0f)
            cubeVel.y = -cubeVel.y;
        if (cubePos.z <= -1.0f || cubePos.z >= 1.0f)
            cubeVel.z = -cubeVel.z;

        // compute model, view, projection matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // mouse click handling (rising edge)
        int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        bool mousePressed = (leftState == GLFW_PRESS);
        if (mousePressed && !prevMousePressed)
        {
            double cx, cy;
            glfwGetCursorPos(window, &cx, &cy);
            int w, h;
            glfwGetWindowSize(window, &w, &h);
            float nx = (float)(cx / w) * 2.0f - 1.0f;
            float ny = -((float)(cy / h) * 2.0f - 1.0f);
            // transform into shape local space (shape centered at model position)
            glm::mat4 mvp = projection * view * model;
            glm::vec4 clipSpace = glm::vec4(nx, ny, -1.0f, 1.0f);
            glm::vec4 eyeSpace = glm::inverse(projection) * clipSpace;
            eyeSpace = glm::vec4(eyeSpace.x, eyeSpace.y, -1.0f, 0.0f);
            glm::vec4 worldSpace = glm::inverse(view) * eyeSpace;
            glm::vec4 localSpace = glm::inverse(model) * worldSpace;
            // Simple bounding box check (approximate)
            if (visible && localSpace.x >= -0.5f && localSpace.x <= 0.5f &&
                localSpace.y >= -0.5f && localSpace.y <= 0.5f &&
                localSpace.z >= -0.5f && localSpace.z <= 0.5f)
            {
                visible = false;
            }
        }
        prevMousePressed = mousePressed;

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our colorful cube if visible
        if (visible)
        {
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Libération des ressources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Nettoyage et fermeture
    glfwTerminate();
    return 0;

}