#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"    gl_Position = model * vec4(position.x, position.y, position.z, 1.0);\n"
"    vertexColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"uniform vec3 color;\n"
"out vec4 fragmentColor;\n"
"void main()\n"
"{\n"
"    fragmentColor = vec4(color, 1.0);\n"
"}\0";


int main()
{
    // inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Tworzenie okna
    const unsigned int window_width = 1500;
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


    // vertex data
    GLfloat vert1[] = {
        // coordinates          // colors
        -0.15f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,
         0.15f,  0.0f,  0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,   0.3f,  0.0f,     0.0f, 1.0f, 0.0f
    };
    GLuint indi1[] = {
        0, 1, 2,
    };
    
    glm::mat4 model1 = glm::mat4(1.0f);
    //model1 = glm::translate(model1, glm::vec3(0.0f, 0.05f, 0.0f)); //Przesunięcie
    glm::mat4 model2 = glm::mat4(1.0f);
    //model2 = glm::rotate(model2, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //Piruet
    glm::mat4 model3 = glm::mat4(1.0f);
    //model3 = glm::scale(model3, glm::vec3(0.0f, 0.0, 0.0f)); //Skalowanie
    glm::mat4 model4 = glm::mat4(1.0f);
    //model4 = glm::translate(model4, glm::vec3(0.0f, 0.0f, 0.0f)); //Przesunięcie
    //model4 = glm::rotate(model4, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Piruet
    //model4 = glm::scale(model4, glm::vec3(0.0f, 0.0f, 0.0f)); //Skalowanie
    
    GLuint VAO1;
    GLuint VBO1;
    GLuint EBO1;

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert1), vert1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indi1), indi1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    model1 = glm::translate(model1, glm::vec3(-0.5f, 0.5f, 0.0f));
    glm::vec3 color1(1.0f, 0.0f, 0.0f);
    model2 = glm::translate(model2, glm::vec3(0.5f, 0.5f, 0.0f));
    glm::vec3 color2(0.0f, 1.0f, 0.0f);
    model3 = glm::translate(model3, glm::vec3(0.5f, -0.5f, 0.0f));
    glm::vec3 color3(0.0f, 0.0f, 1.0f);
    model4 = glm::translate(model4, glm::vec3(-0.5f, -0.5f, 0.0f));
    glm::vec3 color4(1.0f, 1.0f, 0.0f);

    float offset = 0.1f;
    float predkosc = 0.01f;
    float spin = 0.0f;
    float scaleOffset = 0.75f;

    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);

    // pętla zdarzeń
    while (!glfwWindowShouldClose(window))
    {
        offset += predkosc * 0.1f;
        scaleOffset += predkosc * 0.1f;
        if (scaleOffset > 10.0f || scaleOffset < 0.0f) scaleOffset =-scaleOffset;

        spin += glm::radians(0.5f);
        if (offset > 0.2f || offset < -0.2f) {
            predkosc = -predkosc;
        }
        // renderowanie
        glClearColor(0.066f, 0.09f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "color");

        glm::mat4 animModel1 = glm::translate(model1, glm::vec3(offset, offset, 0.0f));
        glm::mat4 animModel2 = glm::rotate(model2, spin, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 animModel3 = glm::scale(model3, glm::vec3(scaleOffset, scaleOffset, 0.0f));
        glm::mat4 animModel4;
        animModel4 = glm::translate(model4, glm::vec3(offset, offset, 0.0f));
        animModel4 = glm::scale(animModel4, glm::vec3(scaleOffset, scaleOffset, 0.0f));
        animModel4 = glm::rotate(animModel4, spin, glm::vec3(0.0f, 0.0f, 1.0f));
        
        

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(animModel1));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color1));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(animModel2));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color2));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(animModel3));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color3));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(animModel4));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color4));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}