#ifndef DRAWCOMMON_H
#define DRAWCOMMON_H

/** Graphics: DEFINITIONS *************************************************/
// Graphics
#ifdef WIN32
#include <gl/GL.h>
#include "gl/glut.h"
#else
#include <GL/freeglut_std.h>  // или glut.h
#endif

#include "graph.h"




// Мыслим в абстрактной системе координат OpenGL
#define SCREEN_HALF_X_GL 200.0         // размер _полуоси_ OpenGL для графа:      (-SCREEN_PX) .. SCREEN_PX
#define SCREEN_HALF_Y_GL 200.0         // размер _полуоси_ OpenGL для графа:      (-SCREEN_PX) .. SCREEN_PX

#define SCREEN_MX_GL 100.0         // ширина меню (добавляется к ширине, предназначенной для графа)

#define SCREEN_KX_INITIAL 2.0   // соотношение единичных отрезков систем координат окна и opengl
#define SCREEN_KY_INITIAL 2.0   // соотношение единичных отрезков систем координат систем окна и opengl



#define SPHERE_SLICES   20

#define SCREEN_W_PX (SCREEN_HALF_X_GL * 2 + SCREEN_MX_GL) * SCREEN_KX_INITIAL // размер окна в ОС    0 .. SCREEN_WX
#define SCREEN_H_PX (SCREEN_HALF_Y_GL * 2) * SCREEN_KY_INITIAL // размер окна в ОС                0 .. SCREEN_WX
#define R 7.0     // радиус сферы



/** Graphics: PROTOTYPES *************************************************/
/****  NOT DEPENDING ON GRAPH  ************/
void DrawLine2D(float x1, float y1, float x2, float y2);
void DrawRect2D(float x1, float y1, float x2, float y2);
void DrawSphere(float x, float y, float z, float r);
void DrawCircle2D(float xr, float yr, float r, bool brushed);

void RenderBitmapString(float x, float y, float z, void *font, char *string);


float Screen_to_GL_X(float x, float varScreenW);
float Screen_to_GL_Y(float y, float varScreenH);

/****  DEPENDING ON GRAPH  ************/
void DrawEdge(int ID1, int ID2, TNode *gr);
void DrawNode(int id, TNode *gr);





#endif // DRAWCOMMON_H
