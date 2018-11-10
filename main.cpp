// Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "transform.hpp"
#include "Shader.h"

#include "SOIL.h"

void init();
void display();
void reshape(int, int);
void idle();

GLuint    program;

GLint     loc_a_vertex;
GLint     loc_a_texcoord;

GLint     loc_u_pvm_matrix;

GLint     loc_u_texid;
GLint     loc_u_texid_frame;

GLuint    texid;
GLuint    texid_frame;

float model_scale = 1.0f;
float model_angle = 0.0f;

kmuvcl::math::mat4x4f mat_PVM;

GLfloat vertices[] = {
  // front
  -1, 1, 1,   1, 1, 1,   1,-1, 1,  -1,-1, 1,
  // back
  1, 1, -1,  -1, 1, -1,  -1,-1, -1,   1,-1, -1,
  // top
  -1, 1, -1,   1, 1, -1,   1, 1, 1,  -1, 1, 1,
  // bottom
  -1,-1, 1,   1,-1, 1,   1,-1, -1,  -1,-1, -1,
  // right
  1, 1, 1,   1, 1, -1,   1,-1, -1,   1,-1, 1,
  // left
  -1, 1, -1,  -1, 1, 1,  -1,-1, 1,  -1,-1, -1,
};

GLfloat texcoords[] = {
  // front
  0,1,  1,1,  1,0,  0,0,
  // back
  0,1,  1,1,  1,0,  0,0,
  // top
  0,1,  1,1,  1,0,  0,0,
  // bottom
  0,1,  1,1,  1,0,  0,0,
  // right
  0,1,  1,1,  1,0,  0,0,
  // left
  0,1,  1,1,  1,0,  0,0,
};

GLushort indices[] = {
  //front
  0, 3, 2,   2, 1, 0,
  //back
  4, 7, 6,   6, 5, 4,
  // top
  8,11,10,  10, 9, 8,
  // bottom
  12,15,14,  14,13,12,
  //right
  16,19,18,  18,17,16,
  //left
  20,23,22,  22,21,20,
};


std::chrono::time_point<std::chrono::system_clock> prev, curr;

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "failed to initialize glew" << std::endl;
    return -1;
  }

  init();

  glutMainLoop();

  return 0;
}

void init()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glFrontFace(GL_CCW);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for wireframe rendering

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/texture_vert.glsl", "./shader/texture_frag.glsl");

  // TODO: get shader variable locations
  loc_u_pvm_matrix = glGetUniformLocation(program, "u_pvm_matrix");

  loc_u_texid      = glGetUniformLocation(program, "u_texid");
  loc_u_texid_frame= glGetUniformLocation(program, "u_texid_frame");

  loc_a_vertex     = glGetAttribLocation(program, "a_vertex");
  loc_a_texcoord   = glGetAttribLocation(program, "a_texcoord");

  int width, height, channels;
  unsigned char* image = SOIL_load_image("tex.jpg",
    &width, &height, &channels, SOIL_LOAD_RGB);

  // TODO: generate texture
  glGenTextures(1, &texid);

  glBindTexture(GL_TEXTURE_2D, texid);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

  SOIL_free_image_data(image);

  unsigned char* frame = SOIL_load_image("frame.jpg",
    &width, &height, &channels, SOIL_LOAD_RGB);

  glGenTextures(1, &texid_frame);

  glBindTexture(GL_TEXTURE_2D, texid_frame);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

  SOIL_free_image_data(frame);

  prev = curr = std::chrono::system_clock::now();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting
  kmuvcl::math::mat4x4f   mat_Proj, mat_View, mat_Model;

  kmuvcl::math::mat4x4f   mat_R_X, mat_R_Y, mat_R_Z;

  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(60.0f, 1.0f, 0.001f, 10000.0f);

  // camera extrinsic param
  mat_View = kmuvcl::math::lookAt(
    0.0f, 0.0f, 5.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

  mat_Model = kmuvcl::math::scale (model_scale, model_scale, model_scale);
  mat_R_Z = kmuvcl::math::rotate(model_angle*0.7f, 0.0f, 0.0f, 1.0f);
  mat_R_Y = kmuvcl::math::rotate(model_angle,      0.0f, 1.0f, 0.0f);
  mat_R_X = kmuvcl::math::rotate(model_angle*0.5f, 1.0f, 0.0f, 0.0f);

  mat_Model = mat_R_X*mat_R_Y*mat_R_Z*mat_Model;

  mat_PVM = mat_Proj*mat_View*mat_Model;

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);

  // TODO: using texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texid);
  glUniform1i(loc_u_texid, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texid_frame);
  glUniform1i(loc_u_texid_frame, 1);

  // TODO: send gpu texture coordinate
  glVertexAttribPointer(loc_a_vertex,   3, GL_FLOAT, GL_FALSE, 0, vertices);    //OpenGL에 데이터가 버퍼 객체의 어디에 있는지 알려준다
  glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, texcoords);

  glEnableVertexAttribArray(loc_a_vertex);
  glEnableVertexAttribArray(loc_a_texcoord);

  glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

  glDisableVertexAttribArray(loc_a_vertex);     // 속성을 자동으로채움
  glDisableVertexAttribArray(loc_a_texcoord);

  glUseProgram(0);
  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
}

void idle()
{
  curr = std::chrono::system_clock::now();

  std::chrono::duration<float> elaped_seconds = (curr - prev);

  model_angle += 10 * elaped_seconds.count();

  prev = curr;

  glutPostRedisplay();
}
