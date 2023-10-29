    //Include GLEW  
    //#define GLEW_STATIC
	
	//Library for loading textures (Simple OpenGL Image Library)
	/*#include <SOIL.h>*/

    #include <GL/glew.h>  

	#include<iostream> //cout
	#include <fstream> //fstream

    //Include GLFW  
    #include <GLFW/glfw3.h>  
      
    //Include the standard C++ headers  
    #include <stdio.h>  
    #include <stdlib.h>  


    //Define an error callback  
    static void error_callback(int error, const char* description)  
    {  
        fputs(description, stderr);  
        _fgetchar();  
    }  
      
    //Define the key input callback  
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)  
    {  
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
        glfwSetWindowShouldClose(window, GL_TRUE);  
    } 

    bool getShaderCompileStatus(GLuint shader){
        //Get status
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_TRUE){
            return true;
        } else {
            //Get log
            char buffer[512];
            glGetShaderInfoLog(shader, 512, NULL, buffer);
            std::cout << buffer << std::endl;
            return false;
        }
    }
      
    int main( void )  
    {  
        //Set the error callback  
        glfwSetErrorCallback(error_callback);  
      
        //Initialize GLFW  
        if (!glfwInit())  
        {  
            exit(EXIT_FAILURE);  
        }  
      
        //Set the GLFW window creation hints - these are optional  
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); //Request a specific OpenGL version  
        glfwWindowHint(GLFW_SAMPLES, 16); //Request 4x antialiasing  
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
      
        //Declare a window object  
        GLFWwindow* window;  
      
        //Create a window and create its OpenGL context  
        window = glfwCreateWindow(1280, 720, "Test Window", NULL, NULL);  
      
        //If the window couldn't be created  
        if (!window)  
        {  
            fprintf( stderr, "Failed to open GLFW window.\n" );  
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

        //TODO: create Vertex array object

        // Example: generate vertex buffers
        GLuint buffer;
        glGenBuffers(1, &buffer);

        //TODO: load vertices and bind vertex buffer
        
        //TODO: create and bind element buffer
        
       // Load the TAA vertex shader from file
        std::ifstream vertIn("taa.vert");
        std::string vertContents((std::istreambuf_iterator<char>(vertIn)),
            std::istreambuf_iterator<char>());
        const char* vertSource = vertContents.c_str();

        // Compile the TAA vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertSource, NULL);
        glCompileShader(vertexShader);

        // Check for compilation errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Error compiling TAA vertex shader: " << infoLog << std::endl;
        }

        // Load the TAA fragment shader from file
        std::ifstream fragIn("taa.frag");
        std::string fragContents((std::istreambuf_iterator<char>(fragIn)),
            std::istreambuf_iterator<char>());
        const char* fragSource = fragContents.c_str();

        // Compile the TAA fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragSource, NULL);
        glCompileShader(fragmentShader);

        // Check for compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "Error compiling TAA fragment shader: " << infoLog << std::endl;
        }

        std::ifstream in("shader.vert");
        std::string contents((std::istreambuf_iterator<char>(in)), 
                              std::istreambuf_iterator<char>());
        const char* vertSource2 = contents.c_str();

        //Example: compile a shader source file for vertex shading
        GLuint vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertSource, NULL);
        glCompileShader(vertexShader);
        
        //Example: check that the shader compiled and print any errors
        getShaderCompileStatus(vertexShader);
        getShaderCompileStatus(fragmentShader);

        //TODO: load and compile fragment shader shader.frag

        //TODO: link shaders into a program and bind outColor variable

        //TODO: link vertex data (position, colour and texture coords) to shader

        //TODO: Create texture buffer:

        //TODO: Load image into texture buffer
        
        //TODO: Set texture parameters with glTexParameteri(...)

        //Set a background color  
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Main Loop  
        do  
        {  
            //Clear color buffer  
            glClear(GL_COLOR_BUFFER_BIT);

            //MSAA
            glEnable(GL_MULTISAMPLE);

            // draw the amd vega logo
            GLfloat vertices[] = {
                // Triangle 1
                -0.5f, 0.5f, 0.0f,
                0.0f, -0.5f, 0.0f,
                -1.0f, -0.5f, 0.0f,

                // Triangle 2
                0.0f, -0.5f, 0.0f,
                0.5f, 0.5f, 0.0f,
                1.0f, -0.5f, 0.0f
            };

            // Bind the vertex data to the vertex buffer object
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Compile and link the vertex and fragment shaders into a shader program
            const char* vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";
            const char* fragmentShaderSource = "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
                "}\n\0";

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);

            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);

            GLuint shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            // Link the vertex data to the shader program
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            // Define how the triangle should be drawn
            glUseProgram(shaderProgram);
            glDrawArrays(GL_TRIANGLES, 0, 6);
      
            //Swap buffers  
            glfwSwapBuffers(window);  
            //Get and organize events, like keyboard and mouse input, window resizing, etc...  
            glfwPollEvents();  

            // FPS counter
            static double previousSeconds = glfwGetTime();
            static int frameCount;
            double currentSeconds = glfwGetTime();
            double elapsedSeconds = currentSeconds - previousSeconds;
            if (elapsedSeconds > 0.25) {
                previousSeconds = currentSeconds;
                double fps = (double)frameCount / elapsedSeconds;
                char tmp[128];
                sprintf(tmp, "opengl @ fps: %.2f", fps);
                glfwSetWindowTitle(window, tmp);
                frameCount = 0;
            }
            frameCount++;
      
        } //Check if the ESC key had been pressed or if the window had been closed  
        while (!glfwWindowShouldClose(window));  
      
        //Close OpenGL window and terminate GLFW  
        glfwDestroyWindow(window);  
        //Finalize and clean up GLFW  
        glfwTerminate();  
      
        exit(EXIT_SUCCESS);  
    }  