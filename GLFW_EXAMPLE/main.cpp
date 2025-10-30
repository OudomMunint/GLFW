//Include GLEW
//#define GLEW_STATIC

#include <SOIL.h>

#include <GL/glew.h>

#include <iostream>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

//Define an error callback
static void error_callback(int error, const char* description)
{
        fputs(description, stderr);
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
}

static GLuint compileShader(GLenum type, const char* source)
{
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
                char buffer[512];
                glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
                std::cerr << "Shader compilation failed: " << buffer << std::endl;
                glDeleteShader(shader);
                return 0;
        }

        return shader;
}

static GLuint createProgram(const char* vertexSource, const char* fragmentSource)
{
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        if (vertexShader == 0 || fragmentShader == 0) {
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
                char buffer[512];
                glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
                std::cerr << "Program linking failed: " << buffer << std::endl;
                glDeleteProgram(program);
                program = 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
}

int main(void)
{
        //Set the error callback
        glfwSetErrorCallback(error_callback);

        //Initialize GLFW
        if (!glfwInit())
        {
                exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_SAMPLES, 16); // AA

        //Declare a window object
        GLFWwindow* window;

        //Create a window and create its OpenGL context
        window = glfwCreateWindow(1280, 720, "AMD Logo", NULL, NULL);

        //If the window couldn't be created
        if (!window)
        {
                fprintf(stderr, "Failed to open GLFW window.\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
        }

        //This function makes the context of the specified window current on the calling thread.
        glfwMakeContextCurrent(window);

        //Sets the key callback
        glfwSetKeyCallback(window, key_callback);

        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();

        //If GLEW hasn't initialized
        if (err != GLEW_OK)
        {
                fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
                return -1;
        }

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
}
)";

        const char* fragmentShaderSource = R"(#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D logoTexture;

void main()
{
        FragColor = texture(logoTexture, TexCoord);
}
)";

        GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
        if (shaderProgram == 0) {
                glfwDestroyWindow(window);
                glfwTerminate();
                return EXIT_FAILURE;
        }

        GLfloat vertices[] = {
                // positions          // texture coordinates
                -0.8f,  0.8f, 0.0f,    0.0f, 1.0f,
                -0.8f, -0.8f, 0.0f,    0.0f, 0.0f,
                 0.8f, -0.8f, 0.0f,    1.0f, 0.0f,
                 0.8f,  0.8f, 0.0f,    1.0f, 1.0f
        };

        GLuint indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width = 0;
        int height = 0;
        unsigned char* image = nullptr;
        const char* texturePaths[] = {
                "images/amd_vega_logo.png",
                "amd_vega_logo.png",
                "../images/amd_vega_logo.png"
        };
        for (const char* path : texturePaths) {
                image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
                if (image) {
                        break;
                }
        }

        if (!image) {
                std::cerr << "Failed to load AMD logo texture." << std::endl;
                glDeleteTextures(1, &texture);
                glDeleteBuffers(1, &ebo);
                glDeleteBuffers(1, &vbo);
                glDeleteVertexArrays(1, &vao);
                glDeleteProgram(shaderProgram);
                glfwDestroyWindow(window);
                glfwTerminate();
                return EXIT_FAILURE;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);

        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "logoTexture"), 0);

        //Set BG col
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Main Loop
        while (!glfwWindowShouldClose(window))
        {
                glClear(GL_COLOR_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                glUseProgram(shaderProgram);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

                //Swap buffers
                glfwSwapBuffers(window);

                //Get and organize events
                glfwPollEvents();

                // FPS counter
                static double previousSeconds = glfwGetTime();
                static int frameCount = 0;
                double currentSeconds = glfwGetTime();
                double elapsedSeconds = currentSeconds - previousSeconds;
                if (elapsedSeconds > 0.25) {
                        previousSeconds = currentSeconds;
                        double fps = static_cast<double>(frameCount) / elapsedSeconds;
                        char tmp[128];
                        sprintf(tmp, "AMD logo @ fps: %.2f", fps);
                        glfwSetWindowTitle(window, tmp);
                        frameCount = 0;
                }
                frameCount++;
        }

        glDeleteTextures(1, &texture);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shaderProgram);

        //Close OpenGL window and terminate GLFW
        glfwDestroyWindow(window);

        //Finalize and clean up GLFW
        glfwTerminate();

        exit(EXIT_SUCCESS);
}
