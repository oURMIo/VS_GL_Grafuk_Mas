#ifdef WIN32
#include <Windows.h>
#endif

/* Common graphics elements */
#include "drawcommon.h"
#include "graph.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

/****  ELEMENTS _NOT_ DEPENDING ON GRAPH STRUCTURE ******************************************/

// Вывод строки
void RenderBitmapString(float x, float y, float z, void *font, char *string)
{
  char *c;
  glColor3f(1.0, 0.0, 0.0);
  glPushMatrix();
  glRasterPos3f(x, y, z);
  for (c=string; *c != '\0'; c++) glutBitmapCharacter(font, *c);   // glutBitmapCharacter(f,  'D');
  glPopMatrix();
 }



//  RenderBitmapInt(0, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, 1234567);
void RenderBitmapInt(float x, float y, float z, void *font, int number)
{
	char *c;
	char str[24];
	glPushMatrix();
	glRasterPos3f(x, y, z);
	sprintf(str, "%d", number);
	for (c = str; *c != '\0'; c++) glutBitmapCharacter(font, *c);   // glutBitmapCharacter(f,  'D');
	glPopMatrix();
}

// Рисование линии
void DrawLine2D(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_STRIP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void DrawRect2D(float x1, float y1, float x2, float y2)
{
    //glRectf(x1, y1, x2, y2);

    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}

// Рисование сферы
void DrawSphere(float x, float y, float z, float r)
{
    glTranslatef(x, y, z);
    glutSolidSphere(r, SPHERE_SLICES, SPHERE_SLICES);
    glTranslatef(-x, -y, -z);
}

// TODO переписать по Брезенхейму
void DrawCircle2D(float xr, float yr, float r, bool brushed)
{
    int parts = (int) r + 5;   // TODO привязать к радиусу в ОКОННЫХ ПИКСЕЛЯХ, а не opengl
    float x = 0, y = 0, angle = 0, d_angle = 6.29 / (parts-1);
    glBegin(brushed ? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < parts; i++)
    {
        x = xr + r * cos(angle);
        y = yr + r * sin(angle);
        glVertex2f(x, y);
        angle += d_angle;
    }
    glEnd();
}


/**** BETWEEN SYSTEMS ****************************************************************************/
/**
***************************************************************************************************
* \fn  coordinates window to gl
*
*
***************************************************************************************************
*/
float Screen_to_GL_X(float x, float varScreenW)
{
    float tempX;
    tempX = (float)x * ((2.0*SCREEN_HALF_X_GL + SCREEN_MX_GL) / varScreenW) - SCREEN_HALF_X_GL;
    return tempX;
}

/**
***************************************************************************************************
* \fn  coordinates window to gl
*
*
***************************************************************************************************
*/
float Screen_to_GL_Y(float y, float varScreenH)
{
    float tempY;
    tempY = -(float)y * (2.0*SCREEN_HALF_Y_GL)/varScreenH + SCREEN_HALF_Y_GL;
    return tempY;
}


/****  ELEMENTS DEPENDING ON GRAPH STRUCTURE *****************************************************/
// Рисование узла
void CGraph::DrawNode(int id, TNode *gr)
{
    extern int fun;
    char str2[64]; // temporary for strings
    glColor3f(1.0, 1.0, 1.0);
    DrawSphere(gr[id].x, gr[id].y, gr[id].z, R);
    gr[id].x += gr[id].delta_azimuth;

    if (0 == fun)
    {
        glDisable(GL_LIGHTING);
        glColor3f(0.3, 0.0, 0.0);
        sprintf(str2, "%d", gr[id].ID);
        float dx_shift = (float)strlen(str2) * 2.0 + R*0.2;  //
        RenderBitmapString(gr[id].x - dx_shift, gr[id].y - R*0.5, gr[id].z, GLUT_BITMAP_TIMES_ROMAN_24, str2);
        glEnable(GL_LIGHTING);
    }
    if (1 == fun)
    {
        //extern int N;
        float offsetY = 0.5 * id / N_vert;//0.5 * rand() / RAND_MAX;
        glColor3f(0.0, 0.3, 0.7);
        DrawSphere(gr[id].x - 0.5*R, gr[id].y + offsetY*R, gr[id].z - 0.6 * R, 0.3 * R);
        DrawSphere(gr[id].x + 0.5*R, gr[id].y + offsetY*R, gr[id].z - 0.6 * R, 0.3 * R);

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        for (float angle = -1.0; angle < 1.0; angle+=0.1)
        {
            glVertex3f(gr[id].x - 1.01*R*sin(angle), gr[id].y - offsetY - 0.4 * R * sin(-fabs(angle)) - 0.4 * R, gr[id].z - 1.01*R*cos(angle));
        }
        glEnd();
    }
}

// Рисование ребра (по номерам узла)
void CGraph::DrawEdge(int ID1, int ID2, TNode *gr)
{
    glBegin(GL_LINE_STRIP);
    glVertex3f(gr[ID1].x, gr[ID1].y, gr[ID1].z);
    glVertex3f(gr[ID2].x, gr[ID2].y, gr[ID2].z);
    glEnd();
}




// Рисование всей системы
void CGraph::Draw()
{
    int i, j;
    for (i=0; i<N_vert; i++)
    {
        for (j=0; j<N_vert; j++)
        {
            if (1 == graph1[i].edgeTo[j])
                DrawEdge(i, j, graph1);
        }
    }
    for (i=0; i<N_vert; i++)
    {
        DrawNode(i, graph1);
    }

}

/** ФУНКЦИИ OPENGL ?  ****************************************************************************/




