#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;


const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"    vertexColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 fragmentColor;\n"
"uniform vec3 kolorDrugiegoProstokata;\n"
"uniform bool czyNadpisacKolor;\n"
"void main()\n"
"{\n"
"    if(czyNadpisacKolor == true) {\n"
"       fragmentColor = vec4(kolorDrugiegoProstokata, 1.0);\n"
"    } else {\n"
"       fragmentColor = vec4(vertexColor, 1.0);\n"
"    }\n"
"}\0";


int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna
    const unsigned int window_width = 1000;
    const unsigned int window_height = 800;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "grafika komputerowa", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


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

    //Do prostokątów
    // vertex data
    GLfloat vertices[] = {
        // coordinates          // colors
        -0.8f,  0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1 vertex 1, triangle 2 vertex 1
        -0.8f, -0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1   vertex 2
         0.8f, -0.6f, 0.0f,     0.0f, 1.0f, 0.0f, // triangle 1 vertex 3, triangle 2 vertex 2
         0.8f,  0.6f, 0.0f,     0.0f, 1.0f, 0.0f  // triangle 2   vertex 3
    };
    GLfloat vert2[] = {
        -0.95f,-0.2f, 0.0f,
        -0.95f,-0.95f, 0.0f,
        -0.2f, -0.95f, 0.0f,
        -0.2f, -0.2f, 0.0f
    };

    GLfloat linie[] = {
       -0.8f, 0.6f , 0.0f,       0.0f, 0.0f, 0.0f,
        0.8f, 0.6f , 0.0f,       1.0f, 0.0f, 0.0f,
        0.8f, -0.6f, 0.0f,       0.0f, 0.0f, 0.0f,
       -0.8f, -0.6f, 0.0f,       0.0f, 0.0f, 1.0f
    };


    GLuint indices[] = {
        0, 1, 2,    // triangle 1
        0, 2, 3     // triangle 2
    };
    GLuint indicesLinie[] = {
        0, 1, // ---- górny
        1, 2, // | prawy
        2, 3, // ---- dolny
        3, 0, // | lewy
    };


    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    GLuint VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert2), vert2, GL_STATIC_DRAW);

    GLuint EBO2;
    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    GLuint VAO3;
    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    GLuint VBO3;
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(linie), linie, GL_STATIC_DRAW);

    GLuint EBO3;
    glGenBuffers(1, &EBO3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLinie), indicesLinie, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);


    //Dwa prostokąty
    // pętla zdarzeń
    while (!glfwWindowShouldClose(window))
    {
        // renderowanie
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        GLint nadpiszKolor = glGetUniformLocation(shaderProgram, "czyNadpisacKolor");
        glUniform1i(nadpiszKolor, GL_FALSE);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform1i(nadpiszKolor, GL_TRUE);
        float rectColor[] = { 0.5f, 0.0f, 0.0f };
        GLint lokalizacjaKoloru = glGetUniformLocation(shaderProgram, "kolorDrugiegoProstokata");
        glUniform3fv(lokalizacjaKoloru, 1, rectColor);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Jeden prostokat liniami + gradient kolorow
    /*
    while (!glfwWindowShouldClose(window))
    {
        // renderowanie
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        GLint nadpiszKolor = glGetUniformLocation(shaderProgram, "czyNadpisacKolor");
        glUniform1i(nadpiszKolor, GL_FALSE);
        glBindVertexArray(VAO3);
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &EBO3);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
    }


