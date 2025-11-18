#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


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
"uniform vec2 uOffset;\n"
"void main()\n"
"{\n"
"    vec2 pos = aPos.xy + uOffset;\n"
"    gl_Position = vec4(pos, aPos.z, 1.0);\n"
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
        // pos(x,y,z)   // color(r,g,b)
        -0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f, // top-left blue
         0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f, // top-right red
         0.5f, -0.5f, 0.0f,  1.0f,1.0f,0.0f, // bottom-right yellow
        -0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f  // bottom-left green
    };

    unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
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

    // get uniform location for offset
    int locOffset = glGetUniformLocation(shaderProgram, "uOffset");

    // visibility and mouse state
    bool visible = true;
    bool prevMousePressed = false;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // compute offset based on time to animate the shape
        float t = (float)glfwGetTime();
        float ox = std::sin(t * 1.0f) * 0.6f; // horizontal oscillation
        float oy = std::cos(t * 0.7f) * 0.3f; // vertical oscillation

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
            // transform into shape local space (shape centered at ox,oy with half-size 0.5)
            float localX = nx - ox;
            float localY = ny - oy;
            if (visible && localX >= -0.5f && localX <= 0.5f && localY >= -0.5f && localY <= 0.5f)
            {
                visible = false;
            }
        }
        prevMousePressed = mousePressed;

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw our colorful rectangle if visible
        if (visible)
        {
            glUseProgram(shaderProgram);
            // set offset uniform
            glUniform2f(locOffset, ox, oy);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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