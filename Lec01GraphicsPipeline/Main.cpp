#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <glm/glm.hpp>


void OnWindowSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

struct Result {
    bool isSuccess;
    std::string message;

    Result() : isSuccess(true) {}
};

struct VertexData {
    glm::vec3 position, color;
};

static void Log(std::stringstream& log, const std::vector<char>& message)
{
    std::copy(message.begin(), message.end(), std::ostream_iterator<char>(log, ""));
}

static unsigned int CompileShaderSource(
    const std::string& shaderSource, int type, std::stringstream& logger, Result& result)
{
    unsigned shaderId = glCreateShader(type);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const char* source = (const char*)shaderSource.c_str();
    glShaderSource(shaderId, 1, &source, 0);

    glCompileShader(shaderId);

    int isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        int maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<char> infoLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(shaderId);

        Log(logger, infoLog);
        result.isSuccess = false;
        result.message = logger.str();
        return -1;
    }
    result.isSuccess = true;
    result.message = "Success!";
    return shaderId;
}

static Result CreateShaderProgram(
    std::string& vertexSource, std::string& fragmentSource, unsigned int& program)
{
    std::stringstream logger;
    Result result;

    unsigned int vertexShader =
        CompileShaderSource(vertexSource, GL_VERTEX_SHADER, logger, result);
    if (result.isSuccess == false) return result;

    unsigned int fragmentShader =
        CompileShaderSource(fragmentSource, GL_FRAGMENT_SHADER, logger, result);
    if (result.isSuccess == false) return result;

    // Time to link the shaders together into a program.

    program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    int isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Log(logger, infoLog);
        result.isSuccess = false;
        result.message = logger.str();
        return result;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    result.isSuccess = true;
    result.message = "Successfully created the shader!";
    return result;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "ETSU Computing Interactive Graphics", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1200, 800);
    glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);
    //glfwMaximizeWindow(window);

    std::string vertexSource =
        "#version 430\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec3 color;\n"
        "out vec4 fragColor;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(position, 1.0);\n"
        "   fragColor = vec4(color, 1.0);\n"
        "}\n";

    std::string fragmentSource =
        "#version 430\n"
        "in vec4 fragColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = fragColor;\n"
        "}\n";

    unsigned int shaderProgram;
    Result result = CreateShaderProgram(vertexSource, fragmentSource, shaderProgram);

    VertexData vertexData[6]{};
    vertexData[0] = { {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} };
    vertexData[1] = { {-0.5f,-0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} };
    vertexData[2] = { { 0.5f,-0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} };
    vertexData[3] = { {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };
    vertexData[4] = { { 0.5f,-0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };
    vertexData[5] = { { 0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };

    unsigned int vaoId, vboId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    std::string message = result.message;

    glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);

        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render the object
        if(result.isSuccess)
        {
            glBindVertexArray(vaoId);
            glUseProgram(shaderProgram);
            glBindBuffer(GL_ARRAY_BUFFER, vboId);
            // Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,
                3,              // Each position has 3 components
                GL_FLOAT,       // Each component is a 32-bit floating point value
                GL_FALSE,
                sizeof(VertexData), // The number of bytes to the next position
                (void*)0        // Byte offset of the first position in the array
            );
            // Colors
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                3,              // Each color has 3 components
                GL_FLOAT,       // Each component is a 32-bit floating point value
                GL_FALSE,
                sizeof(VertexData), // The number of bytes to the next color
                (void*)sizeof(glm::vec3) // Byte offset of the first color in the array
            );
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glUseProgram(0);
            glBindVertexArray(0);
        }


        ImGui::Begin("Computing Interactive Graphics");
        ImGui::Text(message.c_str());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / io.Framerate, io.Framerate);
        ImGui::ColorEdit3("Background color", (float*)&clearColor.r);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

