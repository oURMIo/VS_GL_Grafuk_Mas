/*
Алгоритмы определения свойств графа -
неориентированного


Относительный размер (% ?)
Свойство связности (наличие отсутствие, возникновение, исчезновение)
Скопление + его критерий (наличие отсутствие, возникновение, исчезновение)
Цикличность (наличие отсутствие, возникновение, исчезновение)
Плотность ( ? )

*/

#include "graph.h"

// TNode gr[NMAX]
/**
***************************************************************************************************
*
* \fn  bool graphConnectivity(TNode *gr, int N)
*
* \param
*  TNode *gr  - указатель на начало матрицы
*  int N      - общее количество элементов
* \return  - no
*
* Определение связности графа в данный момент (рекурсией)
***************************************************************************************************
*/
// todo: динамическое определение связности!!! без полного пересчёта
byte vertexes[NMAX_VERT] = {0};  // все вершины непройденные
static int countRecourseMarkVertexes = 0;
// вспомогательная функция: пометка вершины
void markVertex(TNode *graph1, int vertexToMark)
{
	vertexes[vertexToMark] = 1;
	countRecourseMarkVertexes++; // переменная для подсчёта, не несёт другой полезной функции
	for (int i=0; i<NMAX_VERT; i++)
	{
		// если есть связь
		if (VERTEX_YES == graph1[vertexToMark].edgeTo[i])
		{
			// то если там вершина непомеченная
			if (0 == vertexes[i])
			{
				// то работаем с нею
				markVertex(graph1, i);
			}
		}
	}
}


bool CGraph::graphIsConnected(TNode *graph1, int N_vert)
{
	// todo: не только в этой функции - проверять сбои выполнения, для рекурсии это важно
	// (может, глубину рекурсии считать)
	bool result = true;

    int i;
	// init auxiliary variables
	for (i = 0; i < N_vert; i++) vertexes[i] = 0;
    countRecourseMarkVertexes = 0;

	vertexes[0] = 1;
	markVertex(graph1, 0);

	// просмотрим, все ли вершины помечены
	for (int i=0; i<N_vert; i++)
	{
		//  если хоть одна не помечена - прекращаем перебор вершин, граф не связен
		if (0 == vertexes[i])
		{
			result = false;
			break;
		}
	}
	return result;
}



/**
***************************************************************************************************
*
* \fn  bool graphHasCycle(TNode *gr, int N)
*
* \param
*  TNode *gr  - указатель на начало матрицы
*  int N      - общее количество элементов
* \return  - no
*
* Определение связности графа в данный момент (рекурсией)
***************************************************************************************************
*/
byte vertexes1[NMAX_VERT] = {0};  // все вершины непройденные
const int notMarked = 0, in_progress = 1, finished = 2;
bool isCycle = false;

void markVertex1(TNode *graph1, int curVertex, int N_vert, int ancestor)
{
    int j;
    vertexes1[curVertex] = in_progress;
    for (j = 0; j < N_vert; j++)
    {
        if (curVertex != j)
        if (graph1[curVertex].edgeTo[j] == VERTEX_YES)
        {
            //if (vertexes1[curVertex] != finished)
            if (vertexes1[j] == notMarked)
            {
                markVertex1(graph1, j, N_vert, curVertex);
            }
            if ((vertexes1[j] == in_progress) && (j != ancestor))
            {
                isCycle = true;
                break;  // return;
            }

        }

    }

    vertexes1[curVertex] = finished;

    return;
}


// функция определения наличия цикла (выше см. вспомогательную функцию)
bool CGraph::graphHasCycle(TNode *graph1, int N_vert)
{
	bool result = false;

    int i;

	// init auxiliary variables
	isCycle = false;   // important not to miss ))
	for (i = 0; i < N_vert; i++) vertexes1[i] = notMarked;

    // циклы может иметь и несвязный граф!! поэтому просматриваем каждую
    for (i = 0; i < N_vert; i++)
    {
        markVertex1(graph1, i, N_vert, i);
        if (isCycle)
        {
            result = true;
            break;
        }

    }

	return result;
}




//byte nNeighbours[NMAX_VERT] = {0}; // сколько соседей
// посчитаем соседей для каждой вершины
/*void calculateNeighbours(TNode *graph1)
{
	for (int i=0; i<NMAX_VERT; i++)
	{
		nNeighbours[i] = 0;
		for (int j=0; j<NMAX_VERT; j++)
		{
			if (i!=j)
                if (VERTEX_YES == graph1[i].egdeTo[j])
					nNeighbours[i]++;
		}
	}
    //for (i = 0; i < NMAX_VERT; i++) nNeighbours[i] = 0;
}

bool markVertex1(TNode *graph1, int vertexToMark)
{
	static bool result = false;
	// если помечена
	if (1 == vertexes1[vertexToMark])
	{
		result = true;
	}
	vertexes1[vertexToMark] = 1;
	// если не крайняя - входим в соседние
	if (nNeighbours[vertexToMark] > 1)
	{
		for (int i=0; i<NMAX_VERT; i++)
		{
            if (VERTEX_YES == graph1[i].egdeTo[i])
			{
				markVertex1(graph1, i);
			}
		}
	}
	return result;
} */
