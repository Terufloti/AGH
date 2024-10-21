#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


#include <iostream>

using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 texCoord;\n"
"out vec3 vertexColor;\n"
"out vec2 vertexTexture;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"    vertexColor = color;\n"
"    vertexTexture = texCoord;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"in vec2 vertexTexture;\n"
"out vec4 fragmentColor;\n"
"uniform sampler2D uTexture;\n"
"uniform float scrollUpMix;\n"
"void main()\n"
"{\n"
"    fragmentColor = mix(texture(uTexture, vertexTexture), vec4(0.0, 0.0, 1.0, 1.0), scrollUpMix);\n"
"}\0";

// vertex data
GLfloat vertTrojkat[] = {
    // coordinates          // colors               //textures
     0.6f,  0.6f, 0.0f,       0.0f, 1.0f, 0.0f,       0.0f, 1.0f,// triangle 1 vertex 1
     0.9f,  0.6f, 0.0f,       0.0f, 1.0f, 0.0f,       0.0f, 0.0f,// triangle 1 vertex 2
     0.75f, 0.9f, 0.0f,       0.0f, 1.0f, 0.0f,       1.0f, 0.0f,// triangle 1 vertex 3
};
GLfloat vertKwadrat[] = { 
    -0.95f, -0.2f,  0.0f,     0.0f, 1.0f, 0.0f,       0.0f, 0.0f,
    -0.95f, -0.95f, 0.0f,     0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
    -0.2f,  -0.95f, 0.0f,     0.0f, 1.0f, 0.0f,       1.0f, 1.0f,
    -0.2f,  -0.2f,  0.0f,     1.0f, 1.0f, 1.0f,       1.0f, 0.0f
};

GLuint indTrojkat[] = {
    0, 1, 2
};

GLuint indKwadrat[] = {
    0, 1, 2,
    0, 2, 3
};

GLuint VAOTrojkat, VBOTrojkat, EBOTrojkat;
GLuint VAOKwadrat, VBOKwadrat, EBOKwadrat;

bool rysujTrojkat = false;
bool rysujProstokat = false;
float scrollUpMix = 0.0f;

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

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scroll_callback);

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

    //TEKSTURY
    int width1, height1, nrChannels1;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* data = stbi_load("wood.jpeg", &width1, &height1, &nrChannels1, 0);
    
    if (data) cout << "Tekstura 1 sie zaladowala" << endl;

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* data2 = stbi_load("color.jpg", &width1, &height1, &nrChannels1, 0);

    if (data2) cout << "Tekstura 2 sie zaladowala" << endl;

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    
    glGenVertexArrays(1, &VAOTrojkat);
    glBindVertexArray(VAOTrojkat);

    glGenBuffers(1, &VBOTrojkat);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTrojkat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertTrojkat), vertTrojkat, GL_STATIC_DRAW);

    glGenBuffers(1, &EBOTrojkat);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTrojkat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indTrojkat), indTrojkat, GL_STATIC_DRAW);

    scroll_callback(window, 0, 0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glGenVertexArrays(1, &VAOKwadrat);
    glBindVertexArray(VAOKwadrat);

    glGenBuffers(1, &VBOKwadrat);
    glBindBuffer(GL_ARRAY_BUFFER, VBOKwadrat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertKwadrat), vertKwadrat, GL_STATIC_DRAW);

    glGenBuffers(1, &EBOKwadrat);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKwadrat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indKwadrat), indKwadrat, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    
    glBindVertexArray(0);
    


    glViewport(0, 0, (GLuint)window_width, (GLuint)window_height);

    // pętla zdarzeń
    while (!glfwWindowShouldClose(window))
    {
        // renderowanie
        glClearColor(0.066f, 0.09f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "scrollUpMix"), scrollUpMix);
        if (rysujTrojkat) {
            glBindTexture(GL_TEXTURE_2D, texture2);
            glBindVertexArray(VAOTrojkat);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            
        }
        
        if (rysujProstokat) {
            glBindTexture(GL_TEXTURE_2D, texture1);
            glBindVertexArray(VAOKwadrat);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        
        glBindVertexArray(0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAOTrojkat);
    glDeleteBuffers(1, &VBOTrojkat);
    glDeleteBuffers(1, &EBOTrojkat);

    glDeleteVertexArrays(1, &VAOKwadrat);
    glDeleteBuffers(1, &VBOKwadrat);
    glDeleteBuffers(1, &EBOKwadrat);

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        cout << "Wcisnieto 1" << endl;
        rysujTrojkat = true;
        rysujProstokat = false;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        cout << "Wcisnieto 2" << endl;
        rysujTrojkat = false;
        rysujProstokat = true;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        cout << "Wcisnieto 3" << endl;
        rysujTrojkat = true;
        rysujProstokat = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollUpMix += (yoffset > 0) ? 0.1f : -0.1f;
    cout << scrollUpMix << endl;
    if (scrollUpMix > 1.0f) {
        scrollUpMix = 1.0f;
    }
    else if (scrollUpMix < 0.0f) {
        scrollUpMix = 0.0f;
    }
}