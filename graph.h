#ifndef GRAPH_HH
#define GRAPH_HH

#include "alg1.h"

/** ГРАФ: ОПРЕДЕЛЕНИЯ *************************************************/

/** чётко определенные параметры  *************************************************/

#define NMAX_VERT   64 // 1024   // graph MAX size  (array is not dynamic)
#define NMAX_EDGES  (NMAX_VERT*NMAX_VERT)   // formally, it is square of nodes quantity


/** субъективные параметры  *************************************************/


#define VERTEX_YES	1
#define VERTEX_NO	0

#define EDGE_YES	1
#define EDGE_NO		0

// типы существ
#define LIVE_PHTYPE_NO		 0
#define LIVE_PHTYPE_DRAGONFLY 1 // стрекоза
#define LIVE_PHTYPE_HONEYBEE  2  // пчела
#define LIVE_PHTYPE_MAX       2

#define LIVE_REGIME_NOT_SELECTED	(-1)
#define LIVE_REGIME_ON				1
#define LIVE_REGIME_OFF				0

//static int K_RAND = 20.0;  // possibility of edge existence


/** ГРАФ: предопределение типов *************************************************/
// граф как матрица смежности, размер NMAX*NMAX/2 (вершина и связанные с ней вершины)
typedef struct
{
    int ID;
    float x, y, z;
    int edgeTo[NMAX_VERT];

	// extended properties
	int physio_type;
	bool exists;
	int msec_to_live;
	float radius;
	float azimuth;
    float latitude;
	float delta_radius;
    float delta_azimuth;
	float delta_latitude;
} TNode;


// граф как набор рёбер с указанием номеров связанных с ним двух вершин
typedef struct
{
    int edge[2];
} TEdge;

typedef unsigned char byte;


/** ГРАФ. ****************************************************************/
class CGraph
{
public:
    CGraph();
    int N_vert = 32; // graph current size; was macro, but must be changeable
    int N_edges = 32; // edges _approximate_ quantity; was macro, but must be changeable
    //-------------------------------------------------------------------------------------------------
    void AlgImplementation();

    void Init();
    void InitEdges();
    void InitVertexesCoords();

	void InitExtended();
	void InitExtendedZero();
    void StepExtended();

    void Draw();
    void DrawNode(int id, TNode *gr);
    void DrawEdge(int ID1, int ID2, TNode *gr);

	bool live;
private:
    // собственно граф как набор вершин
    TNode graph1[NMAX_VERT];
    TEdge edge[NMAX_EDGES];
    // параметры "дизайна" (нечёткие, субъективные)
    const int big = (NMAX_VERT * 0.8);  // "большая" ли сеть
    //-------------------------------------------------------------------------------------------------
    bool graphIsConnected(TNode *graph1, int N);
    bool graphHasCycle(TNode *graph1, int N);
	
};

#endif // GRAPH_HH



