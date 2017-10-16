/*
 * William Emerison Six
 *
 * Copyright 2016-2017 - William Emerison Six
 * All rights reserved
 * Distributed under Apache 2.0
 */

#include "common.h"
#include "main.h"

#include "shader.h"
#include "mainscene.h"
#include "gl-matrix-stack.h"
#include "load_asset.h"

#include "stb_image.h"






/*
 * The vertex and UV data
 */

GLfloat
wallVertices[] =
  {
    // triangle 1
    -40.0, 0.0, -40.0,
    40.0, 0.0, -40.0,
    40.0, 30.0, -40.0,
    // triangle 2
    40.0, 30.0, -40.0,
    -40.0, 30.0, -40.0,
    -40.0, 0.0, -40.0
  };

GLfloat
wallUVs[] = {
  // triangle 1
  0.0,0.0,
  1.0,0.0,
  1.0,1.0,
  // triangle 2
  1.0,1.0,
  0.0,1.0,
  0.0,0,0
};



enum VAO_IDS{
  WALLS,
  NumVAOS // calculated at compile-time, someone should write a book about that
};

static GLuint
VAOs[NumVAOS];

enum Buffer_IDS{
  Position,
  UV,
  NumBuffers // calculated at compile-time, someone should write a book about that
};

static GLuint
Buffers[NumBuffers];


static GLuint
wallsProgramID;

static GLuint
mvpMatrixLoc;

static GLuint
textureID;

static GLuint
wallTextureLoc;

void
main_scene_init_scene()
{
  /* the global Assimp scene object */
  struct aiScene aiScene;
  struct aiVector3D scene_min, scene_max, scene_center;


  if(!load_asset(MODELS_DIR "walls.dae",
                 &aiScene,
                 &scene_min,
                 &scene_max,
                 &scene_center)){
    fprintf(stderr, "Could not load asset/n");
    return;
  }


  glGenVertexArrays(NumVAOS, VAOs);
  GL_DEBUG_ASSERT();



  glBindVertexArray(VAOs[WALLS]);
  GL_DEBUG_ASSERT();

  // populate the buffers
  glGenBuffers(NumBuffers, Buffers);
  {
    // populate vertex buffer
    {
      // This will identify our vertex buffer
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      GL_DEBUG_ASSERT();
      // The following commands will talk about our 'vertexbuffer' buffer
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
      GL_DEBUG_ASSERT();
      // Give our vertices to OpenGL.
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(wallVertices),
                   wallVertices,
                   GL_STATIC_DRAW);
      GL_DEBUG_ASSERT();
    }

    // populate color buffer
    {
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV]);
      GL_DEBUG_ASSERT();
      glBufferData(GL_ARRAY_BUFFER,
                   sizeof(wallUVs),
                   wallUVs,
                   GL_STATIC_DRAW);
      GL_DEBUG_ASSERT();
    }

    enum Attribute_IDS{
      vPosition = 0,
      vUV = 1
    };


    // set the vertex data
    {
      glEnableVertexAttribArray(vPosition);
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[Position]);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(vPosition,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            BUFFER_OFFSET(0));
      GL_DEBUG_ASSERT();
    }

    // set the UV data
    {
      GL_DEBUG_ASSERT();

      glEnableVertexAttribArray(vUV);
      GL_DEBUG_ASSERT();
      glBindBuffer(GL_ARRAY_BUFFER, Buffers[UV]);
      GL_DEBUG_ASSERT();
      glVertexAttribPointer(vUV,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            BUFFER_OFFSET(0));
      GL_DEBUG_ASSERT();
    }


  }

  // load shaders
  {

    GLuint vertexShaderID = compile_shader(GL_VERTEX_SHADER,
                                           SHADER_DIR "walls.vert");

    GLuint fragmentShaderID =compile_shader(GL_FRAGMENT_SHADER,
                                            SHADER_DIR "walls.frag");

    wallsProgramID = link_shaders(vertexShaderID,fragmentShaderID);

    mvpMatrixLoc = glGetUniformLocation(wallsProgramID,
                                        "mvpMatrix");
    wallTextureLoc = glGetUniformLocation(wallsProgramID,
                                          "wallTexture");

    // clean up
    glDeleteShader(vertexShaderID);
    GL_DEBUG_ASSERT();
    glDeleteShader(fragmentShaderID);
    GL_DEBUG_ASSERT();
  }

  // initialize projection matrix
  {
    enum matrixType m = PROJECTION;
    mat4_identity(m);
    int32_t w, h;

    glfwGetWindowSize(window, &w, &h);
    mat4_perspective(45.0f,
                     (GLfloat)w / (GLfloat)h,
                     0.1f,
                     1000.0f);

  }
  //initialize the modelview matrix
  {
    mat4_identity(MODEL);
    mat4_identity(VIEW);
  }


  // load textures
  {
    int width,height,numberOf8BitComponentsPerPixel;
    unsigned char *thegrid = stbi_load(TEXTURE_DIR "thegrid.png",
                                       &width,
                                       &height,
                                       &numberOf8BitComponentsPerPixel,
                                       0);

    const GLuint mode =  numberOf8BitComponentsPerPixel == 4
      ? GL_RGBA
      : GL_RGB;


    // Create one OpenGL texture
    {
      glGenTextures(1, &textureID);
      GL_DEBUG_ASSERT();
      glBindTexture(GL_TEXTURE_2D, textureID);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_PACK_ALIGNMENT,1);
      GL_DEBUG_ASSERT();


      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GL_DEBUG_ASSERT();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      GL_DEBUG_ASSERT();
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
      GL_DEBUG_ASSERT();
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      GL_DEBUG_ASSERT();

      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   mode,
                   width,
                   height,
                   0,
                   mode,
                   GL_UNSIGNED_BYTE,
                   thegrid);
      GL_DEBUG_ASSERT();
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      GL_DEBUG_ASSERT();
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      GL_DEBUG_ASSERT();
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(thegrid);
  }
}

void
main_scene_leave_scene()
{

}


/*
 * camera data
 */

struct camera camera = {
  .x = 0.0,
  .y = 5.0,
  .z = 0.0,
  .rotationX = 0.0,
  .rotationY = 0.0
};


void
main_scene_draw_scene()
{

  // set viewport
  {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0,
               width, height);
  }

  // initialize projection matrix
  {
    enum matrixType m = PROJECTION;
    mat4_identity(m);

    int32_t w, h;

    glfwGetWindowSize(window, &w, &h);
    mat4_perspective(45.0f,
                     (GLfloat)w / (GLfloat)h,
                     0.1f,
                     1000.0f);

  }
  //initialize the modelview matrix
  {
    mat4_identity(MODEL);
    mat4_identity(VIEW);
  }




  // update camera from the controller
  {
    camera.x -= ( GLfloat ) sin( camera.rotationY) * left_axis.vertical;
    camera.z -= ( GLfloat ) cos( camera.rotationY) * left_axis.vertical;

    camera.x -= ( GLfloat ) cos(camera.rotationY) * left_axis.horizontal;
    camera.z += ( GLfloat ) sin(camera.rotationY) * left_axis.horizontal;

    camera.rotationX += right_axis.vertical;
    camera.rotationY += right_axis.horizontal;

  }


  // update camera from the keyboard
  {
    int state = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (state == GLFW_PRESS){
      camera.rotationY -= (GLfloat)0.03;
    }
    state = glfwGetKey(window, GLFW_KEY_LEFT);
    if (state == GLFW_PRESS){
      camera.rotationY += (GLfloat)0.03;
    }
    state = glfwGetKey(window, GLFW_KEY_UP);
    if (state == GLFW_PRESS){
      camera.x -= (GLfloat)sin(camera.rotationY);
      camera.z -= (GLfloat)cos(camera.rotationY);
    }
    state = glfwGetKey(window, GLFW_KEY_DOWN);
    if (state == GLFW_PRESS){
      camera.x += (GLfloat)sin(camera.rotationY);
      camera.z += (GLfloat)cos(camera.rotationY);
    }
  }


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_DEBUG_ASSERT();


  glUseProgram(wallsProgramID);
  GL_DEBUG_ASSERT();



  // translate from object coordinates to camera coordinates
  {
    GLfloat neg_camera[] = {-camera.x,-camera.y,-camera.z};
    mat4_rotateX(VIEW,
                 camera.rotationX);
    mat4_rotateY(VIEW,
                 -camera.rotationY);
    mat4_translate(VIEW,
                   neg_camera);

  }

  //draw the four walls
  {
    glBindVertexArray(VAOs[WALLS]);
    const GLfloat pi_over_two = 1.57079632679;
    GLfloat rotation = 0.0f;
    for(uint8_t i = 0;
        i<4;
        rotation+=pi_over_two,i++)
      {

        mat4_rotateY(MODEL,
                     rotation);


        const mat4_t * const matr = mat4_get_matrix(MODELVIEWPROJECTION);

        glUniformMatrix4fv(mvpMatrixLoc,
                           1,
                           GL_FALSE,
                           matr);
        GL_DEBUG_ASSERT();



        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        GL_DEBUG_ASSERT();
        glUniform1i(wallTextureLoc, 0);
        GL_DEBUG_ASSERT();
        glBindTexture(GL_TEXTURE_2D, textureID);
        GL_DEBUG_ASSERT();



        static GLuint numVertices = ARRAY_SIZE(wallVertices)/3;

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES,
                     0,
                     numVertices);
        GL_DEBUG_ASSERT();
      }
    // unbind the current VAO
    glBindVertexArray(0);

  }

}






void
main_scene_draw_nuklear(struct nk_context *ctx){
  if (nk_begin(ctx,
               "Demo",
               nk_rect(50, 50, 200, 200),
	       NK_WINDOW_BORDER
               | NK_WINDOW_MOVABLE
               | NK_WINDOW_SCALABLE
	       | NK_WINDOW_CLOSABLE
               | NK_WINDOW_MINIMIZABLE
               | NK_WINDOW_TITLE))
    {
      nk_menubar_begin(ctx);
      nk_layout_row_begin(ctx,
                          NK_STATIC,
                          25,
                          2);
      nk_layout_row_push(ctx,
                         45);
      if (nk_menu_begin_label(ctx,
                              "FILE",
                              NK_TEXT_LEFT,
                              nk_vec2(120, 200))) {
	nk_layout_row_dynamic(ctx,
                              30,
                              1);
	nk_menu_item_label(ctx,
                           "OPEN",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "CLOSE",
                           NK_TEXT_LEFT);
	nk_menu_end(ctx);
      }
      nk_layout_row_push(ctx,
                         45);
      if (nk_menu_begin_label(ctx,
                              "EDIT",
                              NK_TEXT_LEFT,
                              nk_vec2(120, 200))) {
	nk_layout_row_dynamic(ctx,
                              30,
                              1);
	nk_menu_item_label(ctx,
                           "COPY",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "CUT",
                           NK_TEXT_LEFT);
	nk_menu_item_label(ctx,
                           "PASTE",
                           NK_TEXT_LEFT);
	nk_menu_end(ctx);
      }
      nk_layout_row_end(ctx);
      nk_menubar_end(ctx);

      enum {EASY, HARD};
      static uint8_t op = EASY;
      static uint32_t property = 20;
      nk_layout_row_static(ctx,
                           30,
                           80,
                           1);
      if (nk_button_label(ctx,
                          "button")){
        printf("Button Pressed! \n");
      }
      nk_layout_row_dynamic(ctx,
                            30,
                            2);
      if (nk_option_label(ctx,
                          "easy",
                          op == EASY))
        op = EASY;
      if (nk_option_label(ctx,
                          "hard",
                          op == HARD))
        op = HARD;
      nk_layout_row_dynamic(ctx,
                            25,
                            1);
      nk_property_int(ctx,
                      "Compression:",
                      0,
                      &property,
                      100,
                      10,
                      1);
    }
  nk_end(ctx);

  /* -------------- EXAMPLES ---------------- */
  /*calculator(ctx);*/
  /*overview(ctx);*/
  /*node_editor(ctx);*/
  /* ----------------------------------------- */


}
