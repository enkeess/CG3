#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <sstream>
#define GLEW_STATIC
#include <GLEW/glew.h>

#include <GLFW/glfw3.h>

#include <string>

const GLuint WIDTH = 1000, HEIGHT = 800;

void callback(GLFWwindow* window, int k, int code, int action, int mode)
{
    if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "flags", nullptr, nullptr);
    glfwMakeContextCurrent(window);
   
    glfwSetKeyCallback(window, callback);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
    const GLchar* vs = " #version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec2 texCoord;\n"

        "out vec2 TexCoord;\n"
        "uniform float time;\n"
        "uniform mat4 m;\n"
        "uniform mat4 project;\n"
        "uniform mat4 spec;\n"

        "void main()\n"
        "{\n"
        "   vec3 pos = position;\n"
        "  if (((pos.x > 0)) && (pos.y > -1.5))\n"
        "    pos.x = pos.x + sin(time) * sin(length(position.x));\n"
        

        "pos.z -= ((gl_InstanceID / 11) * 3 + 5 + (gl_InstanceID % 3)*0.5f);\n"
        "pos.x -= ((gl_InstanceID % 11) * 3 - 2 + (gl_InstanceID % 2)*0.3f);\n"
        "gl_Position = project * spec * m * vec4(pos, 1.0f);\n"
        "TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);\n"
        "}";
    GLuint vss = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vss, 1, &vs, NULL);
    glCompileShader(vss);
    const GLchar* fr = "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D text1;\n"
        "void main()\n"
        "{\n"
        "   {color = texture(text1, TexCoord);}\n"
        "}";
    GLuint frg = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg, 1, &fr, NULL);
    glCompileShader(frg);
    GLuint Program = glCreateProgram();
    glAttachShader(Program, vss);
    glAttachShader(Program, frg);
    glLinkProgram(Program);
    glDeleteShader(vss);
    glDeleteShader(frg);


    int width, height;
    const GLfloat offset = 0.0666f;
    const GLuint len = 15;
    GLfloat* vertices = new GLfloat[4500];
    for (GLuint i = 0; i < len; i++)
        for (GLuint j = 0; j < len; j++)
        {
            vertices[(i * len + j) * 20 + 0] = offset * i;
            vertices[(i * len + j) * 20 + 1] = offset * j;
            vertices[(i * len + j) * 20 + 2] = 0;
            vertices[(i * len + j) * 20 + 3] = offset * i;
            vertices[(i * len + j) * 20 + 4] = offset * j;
            vertices[(i * len + j) * 20 + 5] = offset * (1 + i);
            vertices[(i * len + j) * 20 + 6] = offset * j;
            vertices[(i * len + j) * 20 + 7] = 0;
            vertices[(i * len + j) * 20 + 8] = offset * (1 + i);
            vertices[(i * len + j) * 20 + 9] = offset * j;
            vertices[(i * len + j) * 20 + 10] = offset * i;
            vertices[(i * len + j) * 20 + 11] = offset * (1 + j);
            vertices[(i * len + j) * 20 + 12] = 0;
            vertices[(i * len + j) * 20 + 13] = offset * i;
            vertices[(i * len + j) * 20 + 14] = offset * (1 + j);
            vertices[(i * len + j) * 20 + 15] = offset * (1 + i);
            vertices[(i * len + j) * 20 + 16] = offset * (1 + j);
            vertices[(i * len + j) * 20 + 17] = 0;
            vertices[(i * len + j) * 20 + 18] = offset * (1 + i);
            vertices[(i * len + j) * 20 + 19] = offset * (1 + j);
        }
    GLuint* indices = new GLuint[1350];
    for (GLuint i = 0; i < len; i++)
        for (GLuint j = 0; j < len; j++)
        {
            indices[(i * len + j) * 6] = (i * len + j) * 4;
            indices[(i * len + j) * 6 + 1] = (i * len + j) * 4 + 1;
            indices[(i * len + j) * 6 + 2] = (i * len + j) * 4 + 2;
            indices[(i * len + j) * 6 + 3] = (i * len + j) * 4 + 2;
            indices[(i * len + j) * 6 + 4] = (i * len + j) * 4 + 3;
            indices[(i * len + j) * 6 + 5] = (i * len + j) * 4 + 1;
     
        }
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4500, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 1350, indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
 

    GLuint VBO2, VAO2;
    GLfloat vertices2[] = { 0.f,1.f,0, -15.f,-15.f,   -0.08f,1.f,0,  -15.f,-15.f,     -0.08f,-1.f,0,  -15.f,-15.f,     -0.08f,-1.f,0,  -15.f,-15.f,    0.f,-1.f,0,  -15.f,-15.f,    0.f,1.f,0,  -15.f,-15.f };
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 30, vertices2, GL_STATIC_DRAW);
  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
   
    GLuint VBO3, VAO3;
    GLfloat vertices3[] = {-40.f, -2.f, -0.f, 0.11f, 0.12f,    -40.f, -2.f, -45.f, 0.11f, 0.12f,      20.f, -2.f, -0.f, 0.11f, 0.12f,      20.f, -2.f, -0.f, 0.11f, 0.12f,      20.f, -2.f, -45.f, 0.11f, 0.12f,     -40.f, -2.f, -45.f, 0.11f, 0.12f}
    ;glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 30, vertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
 
    GLuint t1;
    
    glGenTextures(1, &t1);
    glBindTexture(GL_TEXTURE_2D, t1); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int smth;
    unsigned char* foto = stbi_load("./pic/asdf.jpg", &width, &height, &smth, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, foto);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t1);
        GLint tmp = glGetUniformLocation(Program, "text1");
        glUniform1i(tmp, 0);
     

        glUseProgram(Program);
        glBindVertexArray(VAO2);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 121);
        glBindVertexArray(0);
       
        glBindVertexArray(VAO3);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
        glBindVertexArray(0);

        glm::mat4 m = glm::rotate(glm::mat4(1.0f), glm::radians((GLfloat)55), glm::vec3(1.0f, 0.f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(Program, "m"), 1, GL_FALSE, glm::value_ptr(m));

        glm::mat4 project = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(Program, "project"), 1, GL_FALSE, glm::value_ptr(project));
        glUniform1f(glGetUniformLocation(Program, "time"), (GLfloat)glfwGetTime());
        glm::mat4 spectator = glm::translate(glm::mat4(1.0f), glm::vec3(12.5f, -10.5f, -19.5f));
        glUniformMatrix4fv(glGetUniformLocation(Program, "spec"), 1, GL_FALSE, glm::value_ptr(spectator));
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES,1350, GL_UNSIGNED_INT, 0, 121);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}