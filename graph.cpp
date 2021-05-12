#ifdef WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <math.h>
#include "graph.h"
#include "drawcommon.h"

using namespace std;

CGraph::CGraph()
{

};



/**
***************************************************************************************************
* \fn  Реализация алгоритмов
*
*
***************************************************************************************************
*/
void CGraph::AlgImplementation()
{
    bool resultBool;
    cout << endl;
    resultBool = graphIsConnected(graph1, N_vert);
    cout << (resultBool ? "is Connected " : "is not Connected ") << endl;
    //if (true == resultBool) cout << "is Connected " << endl;
    resultBool = graphHasCycle(graph1, N_vert);
    //if (true == resultBool) cout << "has cycle " << endl;
    cout << (resultBool ? "has cycle " : "doesn\'t have cycle ") << endl;
}

// extended properties - init to zero
void CGraph::InitExtendedZero()
{
	int i;
	for (i = 0; i < N_vert; i++)
	{
		graph1[i].exists = false;
		graph1[i].msec_to_live = 0;
		graph1[i].physio_type = LIVE_PHTYPE_NO;
		graph1[i].azimuth = 0.0;
		graph1[i].radius = 0.0;
		graph1[i].delta_radius = 0.0;
		graph1[i].delta_azimuth = 0.0;
		graph1[i].delta_latitude = 0.0;
	}
}

// extended properties - init to values
void CGraph::InitExtended()
{
	int i;
	for (i = 0; i < N_vert; i++)
	{
		graph1[i].exists = rand() % 1;
		graph1[i].msec_to_live = 0;
		graph1[i].physio_type = LIVE_PHTYPE_NO;
		
		graph1[i].radius = (float)sqrt(graph1[i].x * graph1[i].x + graph1[i].y * graph1[i].y);

		graph1[i].delta_radius = (float)(rand() % 10) * 0.002 - 0.01;
		graph1[i].delta_azimuth = (float)(rand() % 10) * 0.2 - 1;
		graph1[i].delta_latitude = (float)(rand() % 10) * 0.2 - 1;
		cout << graph1[i].delta_radius << endl;
	}
}

void CGraph::StepExtended()
{
	int i;
	for (i = 0; i < N_vert; i++)
	{
		graph1[i].exists = rand() % 1;
		graph1[i].msec_to_live = 1000;
		graph1[i].physio_type = rand() % LIVE_PHTYPE_MAX + 1;
		//graph1[i].azimuth = rand() % 360;
		//graph1[i].radius = rand() % 40;
		graph1[i].radius += graph1[i].delta_radius;
		graph1[i].azimuth += graph1[i].delta_azimuth;
		graph1[i].latitude += graph1[i].delta_latitude;
		
	}
}





void CGraph::InitVertexesCoords()
{
    int i;
    for (i=0; i<N_vert; i++)
    {
        graph1[i].x = rand()*SCREEN_HALF_X_GL*2.0/RAND_MAX - SCREEN_HALF_X_GL;
        graph1[i].y = rand()*SCREEN_HALF_Y_GL*2.0/RAND_MAX - SCREEN_HALF_Y_GL;
        graph1[i].z = rand()*SCREEN_HALF_X_GL*2.0/RAND_MAX - SCREEN_HALF_X_GL;
    }
}

void CGraph::InitEdges()
{
    int i, j;

    if (N_edges <= 0) N_edges = 1; // protect from division by zero

    for (i=0; i<N_vert; i++)
    {
        // Проходим по треугольнику, чтоб не перезатирать уже выданные значения!
        for (j=i; j<N_vert; j++)
        {
            // being zero is 1 / N_edges possibility
            if (rand() % N_edges < 1.0) // is dangerous (division by zero)
            {
                graph1[i].edgeTo[j] = EDGE_YES;
                graph1[j].edgeTo[i] = EDGE_YES;
            }

            else
            {
                graph1[i].edgeTo[j] = EDGE_NO;
                graph1[j].edgeTo[i] = EDGE_NO;
            }

        }
    }
}




/**
***************************************************************************************************
* \fn  void init()
*
* случайная выдача:
 узлам - координат
 ребрам - наличия
***************************************************************************************************
*/
void CGraph::Init()
{
    int i;
    // Init IDs
    for (i=0; i<N_vert; i++)
    {
        graph1[i].ID = i;
    }
    InitVertexesCoords();
    InitEdges();
	if (live)
	{
		InitExtended();
		StepExtended();
	}
	else
	{
		InitExtendedZero();
	}
}
