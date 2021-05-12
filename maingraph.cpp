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
* Система координат OpenGL: центр "в центре чертежа", справа дополнительный участок для элементов управления
*
*
*
*
* Copyright @
**********************************************************************************/
#ifndef WIN32
#define SEND  // (client regime)
#define LO // is included LO library
#endif


#ifdef WIN32
#include <Windows.h>
#endif

// Project parts
#include "graph.h"
#include "drawcommon.h"
#include "drawinterface.h"
#include "alg1.h"

// Graphics
#ifdef WIN32
#include <gl/GL.h>
#include "gl/glut.h"
#else
#include <GL/freeglut_std.h>  // или glut.h
#endif

// Common
#include <string.h>  // strcpy
#include <stdlib.h>
#include <iostream>
#include <fstream>  // now for writing log only
#include <ctime>

// Net send
#ifdef LO
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <lo/lo_macros.h>
#endif

using namespace std;

/** ВСЕ ОПРЕДЕЛЕНИЯ *********************************************************************/
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
int result;  // код возврата для функций csound
int sendDelay_msec = 2000; // timer for net send

#ifdef LO
lo_address addr_client = NULL;
#endif
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

static float currentScreenWidth = SCREEN_W_PX; //SCREEN_KX_INITIAL;
static float currentScreenHeight = SCREEN_H_PX; //SCREEN_KY_INITIAL;

ofstream f_out;


/** ИНТЕРФЕЙС: ПЕРЕМЕННЫЕ *************************************************/
CSlider   Slider1(SLIDER_1_RECT_GL, 0.25, currentScreenWidth);
CSlider   Slider2(SLIDER_2_RECT_GL, 0.5, currentScreenWidth);
//CSlider   Slider3(SLIDER_3_RECT_GL, 0.4, currentScreenWidth);
//CSlider   Slider4(SLIDER_4_RECT_GL, 0.3, currentScreenWidth);
//CSlider   Slider5(SLIDER_5_RECT_GL, 0.65, currentScreenWidth);

//CEditBox  EditBox1();


tstr6 RADIOGROUP1_STRINGS[4] = { "alg1  ", "alg2  ", "alg3  ", "alg4  " };
tstr6 RADIOGROUP2_STRINGS[4] = { "normal", "life  ", "", "" };

CRadioButtonGroup RadioGroup2(&RADIOGROUP_2_COORD3[0], 2, 0, (tstr6*)&RADIOGROUP2_STRINGS);
CRadioButtonGroup RadioGroup1(&RADIOGROUP_1_COORD3[0], 4, 2, (tstr6*)&RADIOGROUP1_STRINGS[0]);

/** ГРАФ: ПЕРЕМЕННЫЕ *************************************************/


CGraph graphC;



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
           // std::cout << "#value = " << value << endl;
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

    // TODO make halt with ESC

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
    int res;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
//        std::cout << "#value = " << value << endl;
        value = (float)x;

        // Reinit graph structure
        if ((x < SCREEN_W_PX*0.1) && (y < SCREEN_H_PX*0.1))
            graphC.AlgImplementation();  // call all functions with algorithms

        // graphC.Init();

        // is inside sliders

        // TODO: наследовать класс от graph и slider, т. к. slider не имеет доступа к graphC
        // SLIDER1 - quantity of vertexes
        // TODO: ПЕРЕДЕЛАТЬ И перевод координат, и внести в класс размеры в пикселях
        if (Slider1.isClickInsideRect(SLIDER_1_RECT_GL, x, y, currentScreenWidth, currentScreenHeight))
        {
            Slider1.MarkMoveHorizontal(x, currentScreenWidth);
            graphC.N_vert = Slider1.markRatio * NMAX_VERT;
            graphC.Init();
        }

        // SLIDER2 - quantity of edges
        if (Slider2.isClickInsideRect(SLIDER_2_RECT_GL, x, y, currentScreenWidth, currentScreenHeight))
        {
            Slider2.MarkMoveHorizontal(x, currentScreenWidth);
            graphC.N_edges = (int)((1.0 - Slider2.markRatio) * (NMAX_VERT-1));
            //f_out << graphC.N_edges << " ";
            graphC.InitEdges();
        }

		// Побочный эффект по else: если значение НЕ менялось, то присваивание всё равно выполняется
		// Выбор алгоритма движения
        res = RadioGroup1.setSelectedElemIfClicked(x, y, currentScreenWidth, currentScreenHeight);
		
		// Выбор режима
		res = LIVE_REGIME_NOT_SELECTED;
		res = RadioGroup2.setSelectedElemIfClicked(x, y, currentScreenWidth, currentScreenHeight);
		if (res == LIVE_REGIME_OFF)
		{
			graphC.live = false;
			graphC.InitExtendedZero();
		}
		else if (res == LIVE_REGIME_ON)
		{
			graphC.live = true;
			graphC.InitExtended();
			graphC.StepExtended();  // сразу, чтоб не было паузы
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
void mouseMove(int x, int y)
{
    // use this handler
    mouseButton(GLUT_LEFT_BUTTON, GLUT_DOWN, x, y);

}

void mouseWheel(int x, int y, int z)
{
    ;
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

    netSendOSC();  // net send by OSC
	if (graphC.live)
		graphC.StepExtended();
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
    currentScreenWidth = w;
    currentScreenHeight = h;
    //glClearColor(0.4, 0.5, 1.0, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_HALF_X_GL, SCREEN_HALF_X_GL+SCREEN_MX_GL,    -SCREEN_HALF_Y_GL, SCREEN_HALF_Y_GL,    -SCREEN_HALF_X_GL, SCREEN_HALF_X_GL);
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

    glPushMatrix(); // запоминание матрицы координат OpenGL

    //-------------------------------------------------------------------------------
    //   И Н Т Е Р Ф Е Й С
    //-------------------------------------------------------------------------------




    Slider1.DrawHorizontal();
    Slider2.DrawHorizontal();
//    Slider3.DrawHorizontal();
//    Slider4.DrawHorizontal();
//    Slider5.DrawHorizontal();


    RadioGroup1.DrawGroup();
	RadioGroup2.DrawGroup();

    //DrawTextMenu();

    //-------------------------------------------------------------------------------
    //   Г Р А Ф - сама модель
    //-------------------------------------------------------------------------------

    glRotatef(angle_y, 0.0, 1.0, 0.0);  // rotate scene
    graphC.Draw();



    //this is a test DrawCircle2D(30.0, 30.0, 20.0, true);
    //this is a test DrawCircle2D(30.0, 50.0, 30.0, false);

    glPopMatrix();
    // окончание работы с кадром
    glFlush();
    glFinish();
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
    //cout << "OSC send delay=="<< sendDelay_msec << ", ";
    cout << "sent " << value << ", ";

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

void Keyboard(unsigned char Key, int X, int Y)
{
	if (Key == 27) exit(30);
}

/** enterpoint *********************************************************************/
int main(int argc, char **argv)
{
	// init GLUT, create window
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_W_PX, SCREEN_H_PX);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);


    time_t t = std::time(NULL);
    srand(t);

    // Вывод текущего времени в заголовок окна
    //string s = std::asctime(std::localtime(&t));
    //glutCreateWindow(s.c_str());
    glutCreateWindow("Graph imitation model");

    initLight();

    // handlers

	glutKeyboardFunc(keyboard);

    glutDisplayFunc(display);
	glutMouseFunc(mouseButton);
	//glutSpaceballRotateFunc(mouseWheel);
	glutMotionFunc(mouseMove);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_s);
    glutTimerFunc(1000, timer, 0);     //void glutTimerFunc(unsigned int msecs, void (*func)(int value), value);

    graphC.Init();
    f_out.open("log1.txt", ios_base::out);


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
    f_out << "===";
    f_out.close();
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

