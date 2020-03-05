/*
 * Copyright 2018 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include <GL/glut.h>

int touch_started = 0;
int touch_ended = 0;

void mouseCB(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            touch_started = 1;
            emscripten_run_script("document.location.href = 'https://www.youtube.com';");
            // EM_ASM({console.log('I received: ' + $0);}, touch_started);
        }
        // else if(state == GLUT_UP)
        // {
        //     touch_ended = -1;
        //     EM_ASM({console.log('I received: ' + $0);}, touch_ended);
        // }
    }
}
GLuint compile_shader(GLenum shaderType, const char *src)
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (!isCompiled)
  {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    char *buf = (char*)malloc(maxLength+1);
    glGetShaderInfoLog(shader, maxLength, &maxLength, buf);
    printf("%s\n", buf);
    free(buf);
    return 0;
  }

   return shader;
}

GLuint create_program(GLuint vertexShader, GLuint fragmentShader)
{
   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);
   glBindAttribLocation(program, 0, "apos");
   glBindAttribLocation(program, 1, "acolor");
   glLinkProgram(program);
   return program;
}

static void go()
{
  // int argc = 1;
  // char *argv[1] = {(char*)"Something"};
  // glutInit(&argc, argv);

  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
  emscripten_webgl_make_context_current(ctx);
  // emscripten_set_canvas_element_size("#canvas", 70, 40);

  EM_ASM(var main = document.getElementById("canvas");
        main.style.border = "0 none";
        main.style.position = "absolute";
        main.style.left = "100px";
        main.style.top = "110px";
        main.style.height = "29px";
        main.style.width = "98px";);

  static const char vertex_shader[] =
    "attribute vec4 apos;"
    "attribute vec4 acolor;"
    "varying vec4 color;"
    "void main() {"
      "color = vec4(1.0,0.0,0.0,0.5);"
      "gl_Position = apos;"
    "}";
  GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);

  static const char fragment_shader[] =
    "precision lowp float;"
    "varying vec4 color;"
    "void main() {"
      "gl_FragColor = color;"
    "}";
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

  GLuint program = create_program(vs, fs);
  glUseProgram(program);

  // static const float pos_and_color[] = {
  // //     x,     y, r, g, bglDrawElements
  //    -0.6f, -0.6f, 1, 0, 0,
  //     0.6f, -0.6f, 0, 1, 0,
  //     0.6f,  0.6f, 0, 0, 1,
  //    -0.6f, -0.6f, 1, 0, 0,
  //    -0.6f,  0.6f, 0, 1, 0,
  //     0.6f,  0.6f, 0, 0, 1,
  // };

  static const float pos_and_color[] = {
  //     x,     y, r, g, bglDrawElements
     -1.0f, -1.0f, 1, 0, 0,
      1.0f, -1.0f, 0, 1, 0,
      1.0f,  1.0f, 0, 0, 1,
     -1.0f, -1.6f, 1, 0, 0,
     -1.0f,  1.0f, 0, 1, 0,
      1.0f,  1.0f, 0, 0, 1,
  };

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pos_and_color), pos_and_color, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 20, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 20, (void*)8);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glutMouseFunc(&mouseCB);
}

int main(int argc, char** argv) 
{
    //setup WebGL etc
    glutInit(&argc, argv);
    emscripten_set_main_loop(go, 0, 0);
}