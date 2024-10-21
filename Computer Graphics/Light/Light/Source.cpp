#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;


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

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
bool ambientLight = true, diffuseLight = true, specularLight = true;

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

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "grafika komputerowa", NULL, NULL);
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

    glfwSetKeyCallback(window, keyCallback);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // shadery
    Shader shaderProgram("default.vert", "source.frag");
    Shader shaderProgram2("default.vert", "recipent.frag");


    // vertex data
    GLfloat vertices[] = {
        // vertices coords         //normals               
        //front - niebieski na rysunku
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,       0.0f, 0.0f, -1.0f,

        //back - zielony
        -0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,       0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,

        //right - zolty
        -0.5f, -0.5f, -0.5f,      -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,      -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,      -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,      -1.0f, 0.0f, 0.0f,

        // left - bordowy
         0.5f,  0.5f,  0.5f,       1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,       1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,       1.0f, 0.0f, 0.0f,
         0.5f,  -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,

         // bottom - szary
        -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,      
         0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,       0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,       0.0f, -1.0f, 0.0f,

        // top - rozowy
        -0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f
    };


    GLuint indices[] = {
        0, 1, 2, 2, 3, 0, 
        4, 5, 6, 6, 7, 4, 
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    VAO VAO1;
    VAO1.Bind();
    
    VBO VBO1(vertices, sizeof(vertices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.LinkVBO(VBO1, 1);

    EBO EBO1(indices, sizeof(indices));
    
    VAO1.Unbind();

    auto prevT = chrono::high_resolution_clock::now();
    double prevFPS = glfwGetTime();
    int frameCount = 0;

    float orbitR = 3.0f;
    float orbitV = 40.0f;

    glEnable(GL_DEPTH_TEST);
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
        processInputKeyboard(window, deltaTime);
        
        // renderowanie
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(ambientLight ? 1.0f : 0.0f, diffuseLight ? 1.0f : 0.0f, specularLight ? 1.0f : 0.0f);
        glm::vec3 objectColor = glm::vec3(255.0f, 255.0f, 255.0f);

        shaderProgram.Activate();
        
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(orbitR * cos(glm::radians(orbit)), 0.0f, orbitR * sin(glm::radians(orbit))));
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

        shaderProgram.setMat4("model", model);
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setVec3("lightPosition", lightPosition);
        shaderProgram.setVec3("lightColor", lightColor);
        shaderProgram.setFloat("ambientLight", ambientLight ? 1.0f : 0.0f);
        shaderProgram.setFloat("diffuseLight", diffuseLight ? 1.0f : 0.0f);
        shaderProgram.setFloat("specularLight", specularLight ? 1.0f : 0.0f);
        shaderProgram.setVec3("objectColor", objectColor);
        shaderProgram.setVec3("viewPosition", cameraPosition);

        VAO1.Bind();
        //glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        objectColor = glm::vec3(0.0f, 0.25f, 0.0f);

        shaderProgram2.Activate();
        glm::mat4 modelRecipent = glm::mat4(1.0f);
        modelRecipent = glm::translate(modelRecipent, glm::vec3(0.0f, -1.5f, 0.0f));
        glm::mat4 view2 = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection2 = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

        shaderProgram2.setMat4("model", modelRecipent);
        shaderProgram2.setMat4("view", view2);
        shaderProgram2.setMat4("projection", projection2);
        lightPosition = glm::vec3(model[3]);
        shaderProgram2.setVec3("lightPosition", lightPosition);
        shaderProgram2.setVec3("lightColor", lightColor);
        shaderProgram2.setFloat("ambientLight", ambientLight ? 1.0f : 0.0f);
        shaderProgram2.setFloat("diffuseLight", diffuseLight ? 1.0f : 0.0f);
        shaderProgram2.setFloat("specularLight", specularLight ? 1.0f : 0.0f);
        shaderProgram2.setVec3("objectColor", objectColor);
        shaderProgram2.setVec3("viewPosition", cameraPosition);
        
        VAO1.Bind();
        //glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //
        glfwSwapBuffers(window);
    }

    
    VAO1.Delete();
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    */
    shaderProgram.Delete();
    shaderProgram2.Delete();

    glfwTerminate();
    return 0;
}

void processInputKeyboard(GLFWwindow* window, float deltaTime) {
    const float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_1 && action == GLFW_PRESS) ambientLight = !ambientLight;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) diffuseLight = !diffuseLight;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) specularLight = !specularLight;

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