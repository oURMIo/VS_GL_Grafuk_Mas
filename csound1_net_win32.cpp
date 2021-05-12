/*********************************************************************************
* \file  main.c
* \brief Sender OSC data
*
* \author  Zerg
*
* Sends float number to specified PORT on localhost by liblo (lo_send).
* Tested with SuperCollider IDE as receiver (description see in project (in comment or separate file)
*
*
* Libraries:
*   -lglut -lGL -llo
*
*
* Оси OpenGL:  X вправо, Y вверх, Z у даляется за экран
* Copyright @
**********************************************************************************/
// Project parts
#ifdef WIN32
#include <Windows.h>
#endif
#include "graph.h"
#include "drawcommon.h"
#include "drawinterface.h"
#include "alg1.h"

// Graphics
#include <gl/GL.h>
#ifdef WIN32
#include "gl/glut.h"
#else
#include <GL/freeglut_std.h>  // или glut.h
#endif


// Common
#include <string.h>  // strcpy
#include <stdlib.h>
#include <iostream>
#include <ctime>

// Net send
#ifdef IS_LO
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <lo/lo_macros.h>
#endif

using namespace std;

/** ВСЕ ОПРЕДЕЛЕНИЯ *********************************************************************/
//#define IS_LO  // do we have library
//#define SEND  // (client regime)


//  Шлём в сеть. Читает на данный момент SuperCollider (2016 лето)
// Порт. Один и тот же зададим как число и как строку, чтоб не преобразовывать программно
#ifdef SEND
#define PORT        7771    // обычно 7770, но у меня почему-то занят был
#define PORT_STR    "7771"    //57121
#endif // SEND



#define DO_STR  {\
"DO_ENTER_NODE", \
"DO_ENTER_EDGE", \
"DO_SEND_X"}

#define DO_ENTER_NODE   0
#define DO_ENTER_EDGE   1
#define DO_SEND_X       2 // send mouse x-coordinate as float
#define DO_NEVER        3



/** ПЕРЕМЕННЫЕ *********************************************************************/
#ifdef IS_LO
lo_address addr_client = NULL;
#endif

int result;  // код возврата для функций csound
int sendDelay_msec = 2000; // timer for net send

int done = 0;   // not used
float value = (float)0xff;  // ff is visible in Wireshark



// Для ввода нового значения:
static bool entering = true;  // режим есть ввод/нет ввода
static char enterString[32] = "";  // строка для нового значения
int currentSym = 0;   // текущее количество введенных символов

float angle_y = 0.0;  // угол поворота сцены


bool logOn = false;
int do_mode = DO_SEND_X;

int fun = 0;

static float varScreenW = SCREEN_KX_INITIAL;
static float varScreenH = SCREEN_KY_INITIAL;

/** ГРАФ: ПЕРЕМЕННЫЕ *************************************************/
int N = 32; // graph current size; was macro, but I want to change it
//int n = 32;  // graph size

// собственно граф как набор вершин
TNode graph1[NMAX_VERT] = {0};

TEdge edge[NMAX_EDGES] = {0};





/** Локальные прототипы  *********************************************************/
/* void error(int num, const char *m, const char *path);

int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, void *data, void *user_data);

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data);

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data);*/

void timer(int param);
void netSendOSC();
void initGraphs();

/** Вспомогательное  *********************************************************/
#ifdef NEVER
void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, void *data, void *user_data)
{
    int i;

    if (logOn)
    {
        printf("path: <%s>\n", path);
        for (i = 0; i < argc; i++) {
            printf("arg %d '%c' ", i, types[i]);
            lo_arg_pp((lo_type)types[i], argv[i]);
            printf("\n");
    }
    printf("\n");
    fflush(stdout);
    }

    return 1;
}

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, void *data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("%s <- f:%f, i:%d\n\n", path, argv[0]->f, argv[1]->i);
    fflush(stdout);

    return 0;
}

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, void *data, void *user_data)
{
    done = 1;   // not used
    printf("quiting\n\n");
    fflush(stdout);

    return 0;
}
#endif




// Рисование всей системы
void DrawGraph()
{
    int i, j;
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            if (1 == graph1[i].egdeTo[j])
                DrawEdge(i, j, graph1);
        }
    }
    for (i=0; i<N; i++)
    {
        DrawNode(i, graph1);
    }
    /*for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            if (1 == gr[i].egdeTo[j])
                DrawEdge(i, j);
        }
        DrawNode(i);
    }*/
}


/**
***************************************************************************************************
* \fn  void keyboard_s(int key, int, int)
*
* \param - keycode, x==???, y==???
* \return  - no
*
* Keyboard 32bit code handler  // Обработчик нажатия клавиши с двухбайтовым кодом
***************************************************************************************************
*/
void keyboard_s(int key, int, int)
{
    if (GLUT_KEY_UP == key)
    {
        sendDelay_msec += 100;
    }
    if (GLUT_KEY_DOWN == key)
    {
        sendDelay_msec -= (sendDelay_msec > 0) ? 100 : 0;
    }
    if (GLUT_KEY_LEFT == key)
    {
        angle_y += 1;
    }
    if (GLUT_KEY_RIGHT == key)
    {
        angle_y -= 1;
    }

}

/**
***************************************************************************************************
* \fn  void keyboard(unsigned char c, int, int)
*
* \param - keycode, ???, ???
* \return  - no
*
* Keyboard handler // Обработчик нажатия клавиши
***************************************************************************************************
*/
void keyboard(unsigned char c, int, int)
{
    if (entering)
    {
        enterString[currentSym] = c; // текущему символу присваиваем значение нажатой клавиши
        enterString[currentSym+1] = '\0'; // завершаем строку нулевым символом
        currentSym++;

        if (13 == c)  // при нажатии "Enter"
        {
            value = atof(enterString); // преобразуем строку в вещественное число
            std::cout << "#value = " << value << endl;
            currentSym = 0;  // обнуляем счетчик символов
            entering = false;  // инвертируем режим ввода/вывода - выключаем ввод
        }
    }
    if (!entering)
    {
        strcpy(enterString, ""); // обнуляем строку
        entering = true;
    }

    if (32 == c)
    {
        do_mode ++;
        if (DO_NEVER == do_mode)
        {
            do_mode = 0;
        }
    }

    if (('L' == c) || ('l' == c)) logOn = !logOn;  // logOn != logOn;
    if (('F' == c) || ('f' == c)) fun = 1 - fun;
}

/**
***************************************************************************************************
*
* \fn  void mouseButton(int button, int state, int x, int y)
*
* \param - keycode, сursor x, cursor y
* \return  - no
*
* Mouse click handler // Обработчик нажатия кнопки мыши
***************************************************************************************************
*/
void mouseButton(int button, int state, int x, int y)
{
	float clickRatio;
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
//        std::cout << "#value = " << value << endl;
        value = (float)x;

        // Reinit graph structure
        if ((x < SCREEN_WX*0.1) && (y < SCREEN_WY*0.1))
            initGraphs();

        // is inside slider
		if (isClickInsideRect(COORD_SLIDER_1, x, y, varScreenW, varScreenH, &clickRatio))
            initGraphs();

    }
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
        if (x < 400)
        {
            glScalef(1.01, 1.01, 1.01);
        }
        if (x > 400)
        {
            glScalef(0.99, 0.99, 0.99);
        }
	}

}

/**
***************************************************************************************************
*
* \fn  void mouseMove(int x, int y)
*
* \param - keycode, сursor x, cursor y
* \return  - no
*
* Mouse move handler // Обработчик перемещения курсора мыши
***************************************************************************************************
*/
void mouseMove(int x, int y) {   ;   }

void mouseWheel(int x, int y, int z)  {  ;  }

/**
***************************************************************************************************
*
* \fn void timer(int param)
*
* \param - not needed
*
* timer func
***************************************************************************************************
*/
void algImplementation()
{
    bool resultBool;
    resultBool = graphIsConnected(graph1, N);
    if (true == resultBool) cout << "is Connected " << endl;
    resultBool = graphHasCycle(graph1);
    if (true == resultBool) cout << "has cycle " << endl;
}

/**
***************************************************************************************************
*
* \fn void timer(int param)
*
* \param - not needed
*
* timer func
***************************************************************************************************
*/
void timer(int param)
{
    algImplementation();  // call all functions with algorithms

    netSendOSC();  // net send by OSC

    glutTimerFunc(sendDelay_msec, timer, 0);  // set the next timerFunc implementation
}

// колесо мыши - не используется
void spaceball(int, int, int)
{
    int i = 0;
    i += 1;
}

/** ФУНКЦИИ OPENGL   **************************************************************/
// Настройки освещения
void initLight()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   //glEnable(GL_NORMALIZE);
    float light0_ambient[4] = {0.1, 0.1, 0.1, 0.1};
//    float light1_ambient[4] = {0.1, 0.1, 0.1, 0.1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0_ambient);
//    glTranslatef(0.0, 0.0, -N/2.0);
    float light0_direction[] = {0.0, 0.0, 90.0, 0.0};
    float light1_direction[] = {0.0, 0.0, -90.0, 0.0};
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_direction);
    //glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
}

// Изменение размеров/положения окна
void reshape(int w, int h) //
{
    glViewport(0,0, w,h);
    varScreenW = w;
    varScreenH = h;
    //glClearColor(0.4, 0.5, 1.0, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_PX, SCREEN_PX+SCREEN_MX,    -SCREEN_PY, SCREEN_PY,    -SCREEN_PX, SCREEN_PX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


/**
***************************************************************************************************
* \fn  void keyboard(unsigned char c, int, int)
*
* \param - keycode, ???, ???
* \return  - no
*
* Frame display //  Обработчик сообщения об отрисовке кадра
***************************************************************************************************
*/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    #ifdef RECV
    sprintf(str2, "LeftMouse CLICK to playSOUND!!!!!!");
    renderBitmapString(-N*0.95, -N/2.0, 0,  GLUT_BITMAP_TIMES_ROMAN_24, str2);
    #endif

    glPushMatrix(); // запоминание матрицы

    //-------------------------------------------------------------------------------
    //   И Н Т Е Р Ф Е Й С
    //-------------------------------------------------------------------------------


	//DrawHorizontalSlider(COORD_SLIDER_1, 1);
	DrawHorizontalSlider(SLIDER_1);



    //-------------------------------------------------------------------------------
    //   Г Р А Ф - сама модель
    //-------------------------------------------------------------------------------

    glRotatef(angle_y, 0.0, 1.0, 0.0);  // rotate scene
    DrawGraph();


    glPopMatrix();
    // окончание работы с кадром
    glFlush();
    glFinish();
}


/**
***************************************************************************************************
* \fn  void initGraphs()
*
* случайная выдача
 узлам - координат
 ребрам - наличия
***************************************************************************************************
*/
void initGraphs()
{
    int i, j;
    for (i=0; i<N; i++)
    {
        graph1[i].ID = i;
        graph1[i].x = rand()*SCREEN_PX*2.0/RAND_MAX - SCREEN_PX;
        graph1[i].y = rand()*SCREEN_PY*2.0/RAND_MAX - SCREEN_PY;
        graph1[i].z = rand()*SCREEN_PX*2.0/RAND_MAX - SCREEN_PX;
        // Проходим по треугольнику, чтоб не перезатирать уже выданные значения!
        for (j=i; j<N; j++)
        {

            if (1>rand()*K_RAND/RAND_MAX)
            {
                graph1[i].egdeTo[j] = 1;
                graph1[j].egdeTo[i] = 1;
            }

            else
            {
                graph1[i].egdeTo[j] = 0;
                graph1[j].egdeTo[i] = 0;
            }

        }
    }
}


/**
***************************************************************************************************
* \fn  void netSendOSC()
*
* sending OSC
***************************************************************************************************
*/
void netSendOSC()
{

#ifdef SEND
    // OSCsend 1, "localhost", PORT, "./", "ff", 0.7, 440.0
    //lo_send(addr_client, "localhost", "ff", 0.1f, 23.0f);
    lo_send(addr_client, "localhost", "ff", (float)value, (float)value/1000.0);
    //lo_send(addr_client, "localhost", "iiii", 0xfe, 0xfe, 0xfe, 0xfe );
    cout << sendDelay_msec << ", ";

#endif
}

/**
***************************************************************************************************
*
* \fn void idleFunc()
*
* always
***************************************************************************************************
*/
void idleFunc()
{
    display();
}



/** enterpoint *********************************************************************/
int main(int argc, char **argv)
{
	// init GLUT, create window
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WX, SCREEN_WY);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);


    time_t t = std::time(NULL);
    srand(t);

    // Вывод текущего времени в заголовок окна
    //string s = std::asctime(std::localtime(&t));
    //glutCreateWindow(s.c_str());
    glutCreateWindow("Graph imitation model");

    initLight();
	initSliders();

    // handlers
    glutDisplayFunc(display);
	glutMouseFunc(mouseButton);
	//glutSpaceballRotateFunc(mouseWheel);
	glutMotionFunc(mouseMove);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_s);
    glutTimerFunc(1000, timer, 0);     //void glutTimerFunc(unsigned int msecs, void (*func)(int value), value);


    initGraphs();


  /*  int submenu1, submenu2, selectMessage;
submenu1 = glutCreateMenu(selectMessage);
glutAddMenuEntry("abc", 1);
glutAddMenuEntry("ABC", 2);
submenu2 = glutCreateMenu(3);
glutAddMenuEntry("Green", 1);
glutAddMenuEntry("Red", 2);
glutAddMenuEntry("White", 3);
glutCreateMenu(selectFont);
glutAddMenuEntry("9 by 15", 0);
glutAddMenuEntry("Times Roman 10", 1);
glutAddMenuEntry("Times Roman 24", 2);
glutAddSubMenu("Messages", submenu1);
glutAddSubMenu("Color", submenu2);
glutAttachMenu(GLUT_RIGHT_BUTTON);*/

    #ifdef SEND
    addr_client = lo_address_new("localhost", PORT_STR);
    #endif

    //################################################################################
    glutMainLoop(); // Main loop
    //################################################################################
    return 0;
}

/*
Launch SuperCollider IDE,
load the project (.scd),
boot server from main menu:  "Language - Boot Server", "Language - Evaluate file"

*/

/*

// There is short example file that can receive numbers and renders sound

n = NetAddr("127.0.0.1", 7770); // IP and port
p = Server.new("thor4", n);
p.makeWindow; // make the gui window
p.boot; // boot it

thisProcess.openUDPPort(7771); // attempt to open
thisProcess.openPorts; // list all open ports

//----------------------------------------------------
(
SynthDef(\sine, {|freq = 440, ch = 540, dur = 0.1|
    var osc;
    osc = SinOsc.ar(freq, 0, 0.6);
    Out.ar(0, osc);
	osc = PinkNoise.ar(freq);
    Out.ar(1, osc);
}).add;

//----------------------------------------------------

f = { |msg, time, addr|
	if(msg[0] == 'localhost')
	{
		"msg[0]: % msg[1]: % msg[2]: % msg[3]: % \n".postf(msg[0], msg[1], msg[2], msg[3]);

		p.sendMsg("/s_new", "sine", x = s.nextNodeID, 1, 1, "ch", 0, "freq", msg[1]);
		//p.sendMsg("/n_free", x);
		//x = Synth(\sine, [\freq, 660]);
		//x = Synth(\sine, [\freq, 440]);
	}
};
thisProcess.addOSCRecvFunc(f);
);

 */

