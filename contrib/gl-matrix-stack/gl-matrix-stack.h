/* gl-matrix-stack.h

Copyright (c) 2017 William Emerison Six

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


ABOUT:
    - A single header library in C99 for matrix operations in a stack, used for OpenGL.
    - Single header library
    - Small codebase

USAGE:
    This library is self contained in one single header file and can be used either
    in header only mode or in implementation mode. The header only mode is used
    by default when included and allows including this header in other headers
    and does not contain the actual implementation.

    The implementation mode requires to define  the preprocessor macro
    GL_MATRIX_STACK_IMPLEMENTATION in *one* .c/.cpp file before #includeing this file, e.g.:

        #define GL_MATRIX_STACK_IMPLEMENTATION
        #include "gl-matrix-stack.h"

*/
#ifndef GL_MATRIX_STACK_H
#define GL_MATRIX_STACK_H

// GL_MATRIX_STACK_BEGIN_C_DECLS and GL_MATRIX_STACK_END_C_DECLS are used to ensure editors don't indent too much
#ifdef __cplusplus
#define GL_MATRIX_STACK_BEGIN_C_DECLS extern "C" {
#define GL_MATRIX_STACK_END_C_DECLS }
#else
#define GL_MATRIX_STACK_BEGIN_C_DECLS
#define GL_MATRIX_STACK_END_C_DECLS
#endif

GL_MATRIX_STACK_BEGIN_C_DECLS

/*
 * Public Types
 */

struct mat4_t{
  float m[16];
};


typedef float vec3_t;

enum matrixType{
  MODEL,
  VIEW,
  PROJECTION,
  MODELVIEW,
  MODELVIEWPROJECTION
} ;


/*
 * Function Prototypes
 */

/*
 * Sets the matrix to be the identity matrix
 */
void
mat4_identity(enum matrixType m);


/*
 * Adds a rotation on the X axis to the desired matrix.
 */
void
mat4_rotateX(enum matrixType m,
             float radians);

/*
 * Adds a rotation on the Y axis to the desired matrix.
 */
void
mat4_rotateY(enum matrixType m,
             float radians);

/*
 * Adds a rotation on the Z axis to the desired matrix.
 */
void
mat4_rotateZ(enum matrixType m,
             float radians);

/*
 * Adds a translation to the desired matrix.
 */
void
mat4_translate(enum matrixType m,
               vec3_t * vector);

/*
 * Adds a scaling to the desired matrix.
 */
void
mat4_scale(enum matrixType m,
           vec3_t * vector);


/*
 * Sets the projection matrix to perspective mode
 */
void
mat4_perspective(float radians,
                 float aspectRatio,
                 float nearZ,
                 float farZ);

/*
 * Sets the projection matrix to ortho mode
 */
void
mat4_ortho(float left,
           float right,
           float bottom,
           float top,
           float near,
           float far);

/*
 *
 */
void
mat4_multiply(enum matrixType m,struct mat4_t* rhs);


/*
 *
 */
void
mat4_push_matrix(enum matrixType m);

/*
 *
 */
void
mat4_pop_matrix(enum matrixType m);

const struct mat4_t * const
mat4_get_matrix(enum matrixType m);


#ifdef GL_MATRIX_STACK_IMPLEMENTATION

/*
 * Private Types
 */

// TODO - make these able to expand at run-time, aka not statically allocated
#ifndef MODEL_STACK_SIZE
#define MODEL_STACK_SIZE (100 * sizeof(struct mat4_t))
#endif
#ifndef VIEW_STACK_SIZE
#define VIEW_STACK_SIZE (30 * sizeof(struct mat4_t))
#endif
#ifndef PROJECTION_STACK_SIZE
#define PROJECTION_STACK_SIZE (1 * sizeof(struct mat4_t))
#endif

struct mat4_t model_stack_base[MODEL_STACK_SIZE];
struct mat4_t view_stack_base[VIEW_STACK_SIZE];
struct mat4_t projection_stack_base[PROJECTION_STACK_SIZE];

struct mat4_t * model_matrix = model_stack_base;
struct mat4_t * view_matrix = view_stack_base;
struct mat4_t * projection_matrix = projection_stack_base;

struct mat4_t modelview_matrix_base[sizeof(struct mat4_t)];
struct mat4_t modelviewprojection_matrix_base[sizeof(struct mat4_t)];

struct mat4_t *modelview_matrix = modelview_matrix_base;
struct mat4_t *modelviewprojection_matrix = modelviewprojection_matrix_base;



/*
 * Private procedures
 */

/*
 * Warning - mutates dest,  similarly to memcpy.
 */
void
private_mat4_multiply(struct mat4_t* dest,
                      const struct mat4_t* const lhs,
                      const struct mat4_t* const rhs){

#define DEST(row, column) dest->m[((column-1)*4)+row-1]
#define LHS(row, column)  lhs->m[((column-1)*4)+row-1]
#define RHS(row, column)  rhs->m[((column-1)*4)+row-1]

  DEST(1,1) = LHS(1,1)*RHS(1,1) + LHS(1,2)*RHS(2,1) + LHS(1,3)*RHS(3,1) + LHS(1,4)*RHS(4,1);
  DEST(1,2) = LHS(1,1)*RHS(1,2) + LHS(1,2)*RHS(2,2) + LHS(1,3)*RHS(3,2) + LHS(1,4)*RHS(4,2);
  DEST(1,3) = LHS(1,1)*RHS(1,3) + LHS(1,2)*RHS(2,3) + LHS(1,3)*RHS(3,3) + LHS(1,4)*RHS(4,3);
  DEST(1,4) = LHS(1,1)*RHS(1,4) + LHS(1,2)*RHS(2,4) + LHS(1,3)*RHS(3,4) + LHS(1,4)*RHS(4,4);

  DEST(2,1) = LHS(2,1)*RHS(1,1) + LHS(2,2)*RHS(2,1) + LHS(2,3)*RHS(3,1) + LHS(2,4)*RHS(4,1);
  DEST(2,2) = LHS(2,1)*RHS(1,2) + LHS(2,2)*RHS(2,2) + LHS(2,3)*RHS(3,2) + LHS(2,4)*RHS(4,2);
  DEST(2,3) = LHS(2,1)*RHS(1,3) + LHS(2,2)*RHS(2,3) + LHS(2,3)*RHS(3,3) + LHS(2,4)*RHS(4,3);
  DEST(2,4) = LHS(2,1)*RHS(1,4) + LHS(2,2)*RHS(2,4) + LHS(2,3)*RHS(3,4) + LHS(2,4)*RHS(4,4);

  DEST(3,1) = LHS(3,1)*RHS(1,1) + LHS(3,2)*RHS(2,1) + LHS(3,3)*RHS(3,1) + LHS(3,4)*RHS(4,1);
  DEST(3,2) = LHS(3,1)*RHS(1,2) + LHS(3,2)*RHS(2,2) + LHS(3,3)*RHS(3,2) + LHS(3,4)*RHS(4,2);
  DEST(3,3) = LHS(3,1)*RHS(1,3) + LHS(3,2)*RHS(2,3) + LHS(3,3)*RHS(3,3) + LHS(3,4)*RHS(4,3);
  DEST(3,4) = LHS(3,1)*RHS(1,4) + LHS(3,2)*RHS(2,4) + LHS(3,3)*RHS(3,4) + LHS(3,4)*RHS(4,4);

  DEST(4,1) = LHS(4,1)*RHS(1,1) + LHS(4,2)*RHS(2,1) + LHS(4,3)*RHS(3,1) + LHS(4,4)*RHS(4,1);
  DEST(4,2) = LHS(4,1)*RHS(1,2) + LHS(4,2)*RHS(2,2) + LHS(4,3)*RHS(3,2) + LHS(4,4)*RHS(4,2);
  DEST(4,3) = LHS(4,1)*RHS(1,3) + LHS(4,2)*RHS(2,3) + LHS(4,3)*RHS(3,3) + LHS(4,4)*RHS(4,3);
  DEST(4,4) = LHS(4,1)*RHS(1,4) + LHS(4,2)*RHS(2,4) + LHS(4,3)*RHS(3,4) + LHS(4,4)*RHS(4,4);

#undef DEST
#undef LHS
#undef RHS


}


static struct mat4_t ** private_mat4_get_matrix(enum matrixType m){
  switch(m){
  case MODEL:
    return &model_matrix;
  case VIEW:
    return &view_matrix;
  case PROJECTION:
    return &projection_matrix;
  case MODELVIEW: 
    private_mat4_multiply(modelview_matrix,
                          view_matrix,
                          model_matrix);
    return &modelview_matrix;
  default:
    private_mat4_multiply(modelview_matrix,
                          view_matrix,
                          model_matrix);
    private_mat4_multiply(modelviewprojection_matrix,
                          projection_matrix,
                          modelview_matrix);
    return &modelviewprojection_matrix;
  }
}

/*
 * Public procedures
 */

void
mat4_identity(enum matrixType m){
  struct mat4_t * dest =  *private_mat4_get_matrix(m);

#define M(row, column) dest->m[((column-1)*4)+row-1]
  M(1,1) = 1; M(1,2) = 0; M(1,3) = 0; M(1,4) = 0;
  M(2,1) = 0; M(2,2) = 1; M(2,3) = 0; M(2,4) = 0;
  M(3,1) = 0; M(3,2) = 0; M(3,3) = 1; M(3,4) = 0;
  M(4,1) = 0; M(4,2) = 0; M(4,3) = 0; M(4,4) = 1;
#undef M
}

void
mat4_rotateX(enum matrixType m,
             float radians){
  struct mat4_t * dest =  *private_mat4_get_matrix(m);

  struct mat4_t copy_of_dest[sizeof(struct mat4_t)];

  memcpy(/*dest*/ copy_of_dest,
         /*src*/  dest,
         /*bytes*/ sizeof(struct mat4_t));

  const float c = cos(radians);
  const float s = sin(radians);

#define M(row, column) dest->m[((column-1)*4)+row-1]
#define COPY_OF_M(row, column) copy_of_dest->m[((column-1)*4)+row-1]

  /*
  M(1,1) M(1,2) M(1,3) M(1,4)     1   0    0    0
  M(2,1) M(2,2) M(2,3) M(2,4) *   0   cos  -sin 0
  M(3,1) M(3,2) M(3,3) M(3,4)     0   sin  cos  0
  M(4,1) M(4,2) M(4,3) M(4,4)     0   0    0    1

    =

  M(1,1)  M(1,2)*cos+M(1,3)*sin  M(1,2)*-sin+M(1,3)*cos  M(1,4)
  M(2,1)  M(2,2)*cos+M(2,3)*sin  M(2,2)*-sin+M(2,3)*cos  M(2,4)
  M(3,1)  M(3,2)*cos+M(3,3)*sin  M(3,2)*-sin+M(3,3)*cos  M(3,4)
  M(4,1)  M(4,2)*cos+M(4,3)*sin  M(4,2)*-sin+M(4,3)*cos  M(4,4)

  */

  M(1,2) = COPY_OF_M(1,2)*c+COPY_OF_M(1,3)*s;
  M(2,2) = COPY_OF_M(2,2)*c+COPY_OF_M(2,3)*s;
  M(3,2) = COPY_OF_M(3,2)*c+COPY_OF_M(3,3)*s;
  M(4,2) = COPY_OF_M(4,2)*c+COPY_OF_M(4,3)*s;

  M(1,3) = COPY_OF_M(1,2)*-s+COPY_OF_M(1,3)*c;
  M(2,3) = COPY_OF_M(2,2)*-s+COPY_OF_M(2,3)*c;
  M(3,3) = COPY_OF_M(3,2)*-s+COPY_OF_M(3,3)*c;
  M(4,3) = COPY_OF_M(4,2)*-s+COPY_OF_M(4,3)*c;


#undef COPY_OF_M
#undef M
}


void
mat4_rotateY(enum matrixType m,
             float radians){
  struct mat4_t * dest =  *private_mat4_get_matrix(m);

  struct mat4_t copy_of_dest[sizeof(struct mat4_t)];

  memcpy(/*dest*/ copy_of_dest,
         /*src*/  dest,
         /*bytes*/ sizeof(struct mat4_t));

  const float c = cos(radians);
  const float s = sin(radians);

#define M(row, column) dest->m[((column-1)*4)+row-1]
#define COPY_OF_M(row, column) copy_of_dest->m[((column-1)*4)+row-1]

  /*
    M(1,1) M(1,2) M(1,3) M(1,4)     cos  0    sin  0
    M(2,1) M(2,2) M(2,3) M(2,4) *   0    1    0    0
    M(3,1) M(3,2) M(3,3) M(3,4)     -sin 0    cos  0
    M(4,1) M(4,2) M(4,3) M(4,4)     0    0    0    1

    =

    M(1,1)*cos+M(1,3)*-sin    M(1,2)     M(1,1)*sin+M(1,3)*cos     M(1,4)
    M(2,1)*cos+M(2,3)*-sin    M(2,2)     M(2,1)*sin+M(2,3)*cos     M(2,4)
    M(3,1)*cos+M(3,3)*-sin    M(3,2)     M(3,1)*sin+M(3,3)*cos     M(3,4)
    M(4,1)*cos+M(4,3)*-sin    M(4,2)     M(4,1)*sin+M(4,3)*cos     M(4,4)
  */

  M(1,1) = COPY_OF_M(1,1)*c+COPY_OF_M(1,3)*-s;
  M(2,1) = COPY_OF_M(2,1)*c+COPY_OF_M(2,3)*-s;
  M(3,1) = COPY_OF_M(3,1)*c+COPY_OF_M(3,3)*-s;
  M(4,1) = COPY_OF_M(4,1)*c+COPY_OF_M(4,3)*-s;

  M(1,3) = COPY_OF_M(1,1)*s+COPY_OF_M(1,3)*c;
  M(2,3) = COPY_OF_M(2,1)*s+COPY_OF_M(2,3)*c;
  M(3,3) = COPY_OF_M(3,1)*s+COPY_OF_M(3,3)*c;
  M(4,3) = COPY_OF_M(4,1)*s+COPY_OF_M(4,3)*c;
#undef M
#undef COPY_OF_M
}

void
mat4_rotateZ(enum matrixType m,
             float radians){
  struct mat4_t * dest =  *private_mat4_get_matrix(m);

  struct mat4_t copy_of_dest[sizeof(struct mat4_t)];

  memcpy(/*dest*/ copy_of_dest,
         /*src*/  dest,
         /*bytes*/ sizeof(struct mat4_t));

  const float c = cos(radians);
  const float s = sin(radians);

#define M(row, column) dest->m[((column-1)*4)+row-1]
#define COPY_OF_M(row, column) copy_of_dest->m[((column-1)*4)+row-1]

  /*
  M(1,1) M(1,2) M(1,3) M(1,4)     cos -sin 0    0
  M(2,1) M(2,2) M(2,3) M(2,4) *   sin cos  0    0
  M(3,1) M(3,2) M(3,3) M(3,4)     0   0    1    0
  M(4,1) M(4,2) M(4,3) M(4,4)     0   0    0    1

    =

  M(1,1)*cos+M(1,2)*sin    M(1,1)*-sin+M(1,2)*cos M(1,3) M(1,4)
  M(2,1)*cos+M(2,2)*sin    M(2,1)*-sin+M(2,2)*cos M(2,3) M(2,4)
  M(3,1)*cos+M(3,2)*sin    M(3,1)*-sin+M(3,2)*cos M(3,3) M(3,4)
  M(4,1)*cos+M(4,2)*sin    M(4,1)*-sin+M(4,2)*cos M(4,3) M(4,4)


  */

  M(1,1) = COPY_OF_M(1,1)*c+COPY_OF_M(1,2)*s;
  M(2,1) = COPY_OF_M(2,1)*c+COPY_OF_M(2,2)*s;
  M(3,1) = COPY_OF_M(3,1)*c+COPY_OF_M(3,2)*s;
  M(4,1) = COPY_OF_M(4,1)*c+COPY_OF_M(4,2)*s;

  M(1,2) = COPY_OF_M(1,1)*-s+COPY_OF_M(1,2)*c;
  M(2,2) = COPY_OF_M(2,1)*-s+COPY_OF_M(2,2)*c;
  M(3,2) = COPY_OF_M(3,1)*-s+COPY_OF_M(3,2)*c;
  M(4,2) = COPY_OF_M(4,1)*-s+COPY_OF_M(4,2)*c;
#undef M
#undef COPY_OF_M
}


void
mat4_translate(enum matrixType m,
               vec3_t * vector){
  struct mat4_t * dest =  *private_mat4_get_matrix(m);
#define M(row, column) dest->m[((column-1)*4)+row-1]
  /*
  M(1,1) M(1,2) M(1,3) M(1,4)     1 0 0 x
  M(2,1) M(2,2) M(2,3) M(2,4) *   0 1 0 y
  M(3,1) M(3,2) M(3,3) M(3,4)     0 0 1 z
  M(4,1) M(4,2) M(4,3) M(4,4)     0 0 0 1

    =

  M(1,1) M(1,2) M(1,3) (M(1,1)*x + M(1,2)*y + M(1,3)*z + M(1,4)*w)
  M(2,1) M(2,2) M(2,3) (M(2,1)*x + M(2,2)*y + M(2,3)*z + M(2,4)*w)
  M(3,1) M(3,2) M(3,3) (M(3,1)*x + M(3,2)*y + M(3,3)*z + M(3,4)*w)
  M(4,1) M(4,2) M(4,3) (M(4,1)*x + M(4,2)*y + M(4,3)*z + M(4,4)*w)
  */

  const float x = vector[0],
    y = vector[1],
    z = vector[2];

  M(1,4) = M(1,1)*x + M(1,2)*y + M(1,3)*z + M(1,4);  // w is 1 in modelview
  M(2,4) = M(2,1)*x + M(2,2)*y + M(2,3)*z + M(2,4);  // w is 1 in modelview
  M(3,4) = M(3,1)*x + M(3,2)*y + M(3,3)*z + M(3,4);  // w is 1 in modelview
  M(4,4) = M(4,1)*x + M(4,2)*y + M(4,3)*z + M(4,4);  // w is 1 in modelview
#undef M
}
void
mat4_scale(enum matrixType m,
           vec3_t * vector){
/*
  M(1,1) M(1,2) M(1,3) M(1,4)     x 0 0 0
  M(2,1) M(2,2) M(2,3) M(2,4) *   0 y 0 0
  M(3,1) M(3,2) M(3,3) M(3,4)     0 0 z 0
  M(4,1) M(4,2) M(4,3) M(4,4)     0 0 0 1

  =

  M(1,1)*x  M(1,2)*y  M(1,3)*z  M(1,4)
  M(2,1)*x  M(2,2)*y  M(2,3)*z  M(2,4)
  M(3,1)*x  M(3,2)*y  M(3,3)*z  M(3,4)
  M(4,1)*x  M(4,2)*y  M(4,3)*z  M(4,4)
*/
  struct mat4_t * dest =  *private_mat4_get_matrix(m);
#define M(row, column) dest->m[((column-1)*4)+row-1]

  const float x = vector[0],
    y = vector[1],
    z = vector[2];


  M(1,1) = M(1,1)*x;
  M(2,1) = M(2,1)*x;
  M(3,1) = M(3,1)*x;
  M(4,1) = M(4,1)*x;

  M(1,2) = M(1,2)*y;
  M(2,2) = M(2,2)*y;
  M(3,2) = M(3,2)*y;
  M(4,2) = M(4,2)*y;

  M(1,3) = M(1,3)*z;
  M(2,3) = M(2,3)*z;
  M(3,3) = M(3,3)*z;
  M(4,3) = M(4,3)*z;
#undef M

}



/*
 * Make the projection matrix have the perspective projection
 *
 * http://www.songho.ca/opengl/gl_projectionmatrix.html
 */

void
mat4_perspective(float fovy,
                 float aspectRatio,
                 float nearZ,
                 float farZ){
  const float top = nearZ * tan(fovy * 3.14159265358979323846 / 360.0);
  const float right = top * aspectRatio;
#define M(row, column) projection_matrix->m[((column-1)*4)+row-1]

  M(1,1) = nearZ/right; M(1,2) = 0;          M(1,3) = 0;                          M(1,4) = 0;
  M(2,1) = 0;           M(2,2) = nearZ/top;  M(2,3) = 0;                          M(2,4) = 0;
  M(3,1) = 0;           M(3,2) = 0;          M(3,3) = -(farZ+nearZ)/(farZ-nearZ); M(3,4) = -2*(farZ*nearZ)/(farZ-nearZ);
  M(4,1) = 0;           M(4,2) = 0;          M(4,3) = -1;                         M(4,4) = 0;

#undef M

}




void
mat4_multiply(enum matrixType m,struct mat4_t* rhs){
  struct mat4_t * destMatrix =  *private_mat4_get_matrix(m);

  struct mat4_t copy_of_dest[sizeof(struct mat4_t)];

  memcpy(/*dest*/ copy_of_dest,
         /*src*/   destMatrix,
         /*bytes*/ sizeof(struct mat4_t));

  private_mat4_multiply(destMatrix, copy_of_dest, rhs);


}


void
mat4_push_matrix(enum matrixType m){
  struct mat4_t ** top_of_stack =  private_mat4_get_matrix(m);
  memcpy(/*dest*/  *top_of_stack + sizeof(struct mat4_t),
         /*src*/   *top_of_stack,
         /*bytes*/ sizeof(struct mat4_t));
  *top_of_stack += sizeof(struct mat4_t);
}


void
mat4_pop_matrix(enum matrixType m){
  struct mat4_t ** top_of_stack =  private_mat4_get_matrix(m);
  *top_of_stack -= sizeof(struct mat4_t);
}

const struct mat4_t * const
mat4_get_matrix(enum matrixType m){
  return *private_mat4_get_matrix(m);
}


#endif //GL_MATRIX_STACK_IMPLEMENTATION

GL_MATRIX_STACK_END_C_DECLS

#undef GL_MATRIX_STACK_BEGIN_C_DECLS
#undef GL_MATRIX_STACK_END_C_DECLS

#endif
