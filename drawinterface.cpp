#ifdef WIN32
#include <Windows.h>
#endif

/* Элементы интерфейса */
#include "drawcommon.h"
#include "drawinterface.h"

#include <stdio.h>   // sprintf
#include <string.h>  // strcpy
#include <math.h>    // sqr

#include <iostream>
//#include <string>

using namespace std;

//static float enteringX = -SCREEN_PX*0.8, enteringY = SCREEN_PX/2;

void DrawTextMenu()
{
    char str2[64]; // temporary for strings
    float yyy=SCREEN_H_PX, dyyy=20.0, xxx=SCREEN_H_PX*0.9;

    for (float i=SCREEN_H_PX; i>=-SCREEN_H_PX; i -=dyyy)
        DrawLine2D(xxx + SCREEN_MX_GL, i, SCREEN_W_PX, i);
    DrawLine2D(xxx-4, SCREEN_H_PX, xxx-4, -SCREEN_H_PX);

  //sprintf(str2, "NEW");
  //RenderBitmapString(-SCREEN_PX*0.9, SCREEN_PX*0.9, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

    yyy+= 4;
    // RIGHT SIDE MENU
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "NEW");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Node");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Node");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Edge");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Edge");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Bunch");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Bunch");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

    sprintf(str2, "Add Cycle");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Cycle");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Network-->BIG");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Network-->notBIG");
    RenderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

}


/**
***************************************************************************************************
  COMMON

***************************************************************************************************
*/




 /**
***************************************************************************************************
* \fn  void drawSlider()  проверка попадания
* \param float4 coord - координаты прямоугольника (opengl)
* \param int x, int y - координаты клика (window)
* \param int varScreenKX, int varScreenKY - текущий размер окна
*
***************************************************************************************************
*/
bool CBaseVisualElement::isClickInsideRect(float4 coord, int x, int y, int varScreenW, int varScreenH)
{
    bool result = false;

    float tempX, tempY;
    tempX = Screen_to_GL_X(x, varScreenW);
    tempY = Screen_to_GL_Y(y, varScreenH);

    if (tempX > coord.x1)
        if (tempY > coord.y1)
            if (tempX < coord.x2)
                if (tempY < coord.y2)
                    result = true;
    return result;
}

















/**
***************************************************************************************************
  ELEMENTS

***************************************************************************************************
*/
/**
***************************************************************************************************
  EDITBOX

***************************************************************************************************
*/
/**
***************************************************************************************************
* \fn  constructor
*
*
***************************************************************************************************
*/
CEditBox::CEditBox(float4 coordGL)
{
    rectGL.left = coordGL.x1 < coordGL.x2 ? coordGL.x1 : coordGL.x2;
    rectGL.right = coordGL.x1 > coordGL.x2 ? coordGL.x1 : coordGL.x2;
    rectGL.top = coordGL.y1 < coordGL.y2 ? coordGL.y1 : coordGL.y2;
    rectGL.bottom = coordGL.y1 > coordGL.y2 ? coordGL.y1 : coordGL.y2;
}





/**
***************************************************************************************************
  SLIDER

***************************************************************************************************
*/

/**
***************************************************************************************************
* \fn  constructor
*
*
***************************************************************************************************
*/
CSlider::CSlider(float4 coordGL, float newMarkRatio, int varScreenW)
{
    // Compare left/right top/bottom, make habitual RECT
    rectGL.left = coordGL.x1 < coordGL.x2 ? coordGL.x1 : coordGL.x2;
    rectGL.right = coordGL.x1 > coordGL.x2 ? coordGL.x1 : coordGL.x2;
    rectGL.top = coordGL.y1 < coordGL.y2 ? coordGL.y1 : coordGL.y2;
    rectGL.bottom = coordGL.y1 > coordGL.y2 ? coordGL.y1 : coordGL.y2;



    // установка метки слайдера
    markRatio = newMarkRatio;
    mCoordGL = rectGL.left + (rectGL.right - rectGL.left) * markRatio;
}

/**
***************************************************************************************************
* \fn  void DrawHorizontal(float4 coordGL, float currentRatioFromX1)  рисование ползунка
*
*
***************************************************************************************************
*/
void CSlider::DrawHorizontal()
{
    glColor3fv(COLOR_SLIDER_DARK);
    DrawRect2D(rectGL.left, rectGL.top, rectGL.right, rectGL.bottom);
    glColor3fv(COLOR_SLIDER_LIGHT);
    DrawRect2D(rectGL.left+1, rectGL.top+1, rectGL.right-1, rectGL.bottom-1);

    // mark
    glColor3fv(COLOR_SLIDER_DARK);
    DrawRect2D(mCoordGL-1, rectGL.top, mCoordGL+1, rectGL.bottom); // mCoordGL is absolute GL
}

/**
***************************************************************************************************
* \fn  void MarkMoveHorizontal(int clickPx, float varScreenW)   перемещение метки ползунка
* \return ratio of mark position, 0..1
*
***************************************************************************************************
*/
void CSlider::MarkMoveHorizontal(int clickPx, float varScreenW)
{
    mCoordGL = Screen_to_GL_X(clickPx, varScreenW);
    markRatio = (mCoordGL - rectGL.left) / (rectGL.right - rectGL.left);
}

/**
***************************************************************************************************
* \fn  void ImplementNVert(float x, float varScreenW, CGraph graph1)
*
***************************************************************************************************
*/
void CSlider::ImplementNVert(float x, float varScreenW, CGraph graph1)
{
    MarkMoveHorizontal(x, varScreenW);
    graph1.N_vert = markRatio * NMAX_VERT;
    graph1.Init();
}

/**
***************************************************************************************************
* \fn  void CSlider::ImplementNEdges(float x, float varScreenW, CGraph graph1)
*
***************************************************************************************************
*/
void CSlider::ImplementNEdges(float x, float varScreenW, CGraph graph1)
{
    MarkMoveHorizontal(x, varScreenW);
    graph1.N_edges = markRatio * NMAX_VERT;
    graph1.InitEdges();
}






/**
***************************************************************************************************
  RADIOBUTTON

***************************************************************************************************
*/
/**
***************************************************************************************************
* \fn  constructor
*
***************************************************************************************************
*/
//CRadioButtonGroup::CRadioButtonGroup(const float *param, int n_elem_p, int n_selected_p, tstr6 *str6_in)
CRadioButtonGroup::CRadioButtonGroup(const float *param, int n_elem_p, int n_selected_p, tstr6 *str6_in)
{
    //rectGL.left = coordGL.x1 < coordGL.x2 ? coordGL.x1 : coordGL.x2;s
    //rectGL.right = coordGL.x1 > coordGL.x2 ? coordGL.x1 : coordGL.x2;
    //rectGL.top = coordGL.y1 < coordGL.y2 ? coordGL.y1 : coordGL.y2;
    //rectGL.bottom = coordGL.y1 > coordGL.y2 ? coordGL.y1 : coordGL.y2;
    top = param[0];
    dy = param[1];
    xc = param[2];

	r = { RADIO_R_OUTER, RADIO_R_INNER, RADIO_R_SELECTED };

    n_elem = n_elem_p;
	for (int i = 0; i < n_elem_p; i++)
	{
		strcpy(str6[i], str6_in[i]);
		//cout << str6_in[i];  // good
		cout << str6[i];
		//str6_in++;
	}
    n_selected = n_selected_p;
}

/**
***************************************************************************************************
* \fn
*
***************************************************************************************************
*/
void CRadioButtonGroup::DrawButton(int num, bool isSelected)
{
    glColor3fv(COLOR_RADIO_OUTER);
    DrawCircle2D(xc, top - num * dy, r.outer, isSelected);
    glColor3fv(COLOR_RADIO_INNER);
    DrawCircle2D(xc, top - num * dy, r.inner, isSelected);
    glColor3fv(COLOR_RADIO_SELECTED);
    DrawCircle2D(xc, top - num * dy, r.selected, isSelected);

}

/**
***************************************************************************************************
* \fn
*
***************************************************************************************************
*/
void CRadioButtonGroup::DrawGroup()
{
    for (int i = 0; i < n_elem; i++ )
    {
        DrawButton(i, (i == n_selected));
		RenderBitmapString(xc + r.outer * 2, top - i * dy - r.inner, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)str6[i]);
    }
}

/**
***************************************************************************************************
* \fn
*
***************************************************************************************************
*/
int CRadioButtonGroup::setSelectedElemIfClicked(int x, int y, int varScreenW, int varScreenH)
{
    int result = (-1);
    float clickGL_X, clickGL_Y, tempDX, tempDY;

    clickGL_X = Screen_to_GL_X(x, varScreenW);
    clickGL_Y = Screen_to_GL_Y(y, varScreenH);
    tempDX = xc - clickGL_X;

    for (int i = 0; i < n_elem; i++)
    {
       tempDY = (top - dy * (float)(i)) - clickGL_Y;
	   if (tempDX*tempDX + tempDY*tempDY  <  r.outer*r.outer)
       {
            result = i;
            n_selected = i;
            break;
       }
    }
    return result;
}
