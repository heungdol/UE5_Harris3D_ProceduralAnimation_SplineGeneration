#pragma once

#include <vector>

using namespace std;

class MyFace
{
public:
	MyFace ();
	~MyFace ();

	int fIndex;
	vector<int> verts;
	
	int GetVertex0();
	int GetVertex1();
	int GetVertex2();
	vector<int> GetVertices();
	
	void AddVertex(int vval);
	void AddVertices(int v1, int v2, int v3);
};
