#pragma once

#include <vector>

using namespace std;

class MyFace
{
public:
	MyFace ();
	~MyFace ();

protected:
	int fIndex;
	vector<int> verts;
	
public:
	const vector<int>& GetVertices() const;
	
	void AddVertex(int vval);
	void AddVertices(int v1, int v2, int v3);

	void SetFaceIndex (int fi); 
};
