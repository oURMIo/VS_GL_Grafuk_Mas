/* Элементы интерфейса */


void DrawTextMenu()
{
    float yyy=SCREEN_PX, dyyy=20.0, xxx=SCREEN_PX*0.9;
    for (float i=SCREEN_PX; i>=-SCREEN_PX; i -=dyyy)
        DrawLine2D(xxx + SCREEN_MX, i, SCREEN_PX, i);
    DrawLine2D(xxx-4, SCREEN_PX, xxx-4, -SCREEN_PX);

  //sprintf(str2, "NEW");
  //renderBitmapString(-SCREEN_PX*0.9, SCREEN_PX*0.9, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

    yyy+= 4;
    // RIGHT SIDE MENU
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "NEW");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Node");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Node");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Edge");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Edge");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Add Bunch");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Bunch");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

    sprintf(str2, "Add Cycle");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Delete Cycle");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Network-->BIG");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);
    sprintf(str2, "Network-->notBIG");
    renderBitmapString(xxx, yyy-=dyyy, 0, GLUT_BITMAP_TIMES_ROMAN_24, str2);

}
