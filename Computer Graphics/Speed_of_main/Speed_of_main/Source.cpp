#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

using namespace std;


const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 texCoord;\n"
"out vec3 vertexColor;\n"
"out vec2 vertexTexture;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(position, 1.0);\n"
"    vertexColor = color;\n"
"    vertexTexture = texCoord;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"in vec2 vertexTexture;\n"
"out vec4 fragmentColor;\n"
"uniform sampler2D uTexture;\n"
"void main()\n"
"{\n"
"    fragmentColor = texture(uTexture, vertexTexture);\n"
"}\0";

float pitch = 0.0f;
float yaw = -90.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 2.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void processInputKeyboard(GLFWwindow* window, float deltaTime);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

float previousX = static_cast<float>(1500.0f) / 2.0f;
float previousY = static_cast<float>(800.0f) / 2.0f;
bool first_window_enter = true;

float orbit = 0.0f;

int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    int monitorX, monitorY, monitorW, monitorH;
    glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorW, &monitorH);

    // Tworzenie okna
    const unsigned int window_width = 1500;
    const unsigned int window_height = 800;

    int windowPosX = monitorX + (monitorW - window_width) / 2;
    int windowPosY = monitorY + (monitorH - window_height) / 2;

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "computer graphics", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, windowPosX, windowPosY);
    glfwMakeContextCurrent(window);


    // inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // shadery
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint status;
    GLchar error_message[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, error_message);
        std::cout << "Error (Vertex shader): " << error_message << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_message);
        std::cout << "Error (Fragment shader): " << error_message << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, error_message);
        std::cout << "Error (Shader program): " << error_message << std::endl;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // vertex data
    GLfloat vertices[] = {
        // vertices coords         //colors                  // texture coords
        //front - blue
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         1.0f,  1.0f,

        //back - green
        -0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f,         0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f,         1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f,         1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f,         0.0f,  1.0f,

        //right - yellow
        -0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,

        // left - red
         0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f,  -0.5f,  0.5f,      0.0f, 0.0f, 0.0f,         0.0f,  0.0f,

         // bottom - grey
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,

        // top - pink
        -0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 0.0f,         0.0f,  0.0f,
    };


    GLuint indices[] = {
        //back
        0, 1, 2,
        2, 3, 0,
        //front
        4, 5, 6,
        6, 7, 4,
        //left
        0, 4, 7,
        7, 0, 3,
        //right
        5, 1, 2,
        2, 6, 5,
        //bottom
        4, 5, 1,
        1, 0, 4,
        //top
        7, 6, 2,
        2, 3, 7



    };

    int texW, texH, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* data = stbi_load("color.jpg", &texW, &texH, &nrChannels,0);

    if (data) cout << "Texture loaded properly." << endl;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    auto prevT = chrono::high_resolution_clock::now();
    double prevFPS = glfwGetTime();
    int frameCount = 0;

    float orbitR = 3.0f;
    float orbitV = 30.0f;

    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);
    // pętla zdarzeń
    while (!glfwWindowShouldClose(window))
    {
        
        auto currentT = chrono::high_resolution_clock::now();
        chrono::duration<float> deltaTimeDuration = currentT - prevT;
        float deltaTime = deltaTimeDuration.count();
        prevT = currentT;
        frameCount++;
        double currentTime = glfwGetTime();
        if (currentTime - prevFPS >= 1.0) {
            double frameTime = 1000.0 / double(frameCount);
            double FPS = double(frameCount) / (currentTime - prevFPS);
            string title = "FPS: " + to_string(FPS) + " T: " + to_string(frameTime) + " ms";
            glfwSetWindowTitle(window, title.c_str());
            frameCount = 0;
            prevFPS = currentTime;
        }
        
        orbit += orbitV * deltaTime;
        
        glfwPollEvents();
        processInputKeyboard(window,deltaTime);
        glEnable(GL_DEPTH_TEST);
        // renderowanie
        glClearColor(0.066f, 0.09f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(orbitR * cos(glm::radians(orbit)), 0.0f, orbitR*sin(glm::radians(orbit))));
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"),0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInputKeyboard(GLFWwindow* window, float deltaTime) {
    const float cameraSpeed = 1.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (first_window_enter) {
        previousX = xpos;
        previousY = ypos;
        first_window_enter = false;
    }
    float xdifference = xpos - previousX;
    float ydifference = previousY - ypos;

    previousX = xpos;
    previousY = ypos;

    const float sensitivity = 0.075f;
    xdifference *= sensitivity;
    ydifference *= sensitivity;
    pitch += ydifference;
    yaw += xdifference;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 cameraFront_new;
    cameraFront_new.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront_new.y = sin(glm::radians(pitch));
    cameraFront_new.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront_new);
}