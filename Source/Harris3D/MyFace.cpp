#include "MyFace.h"

MyFace::MyFace ()
{
	
}

MyFace::~MyFace ()
{
	
}

const vector<int>& MyFace::GetVertices() const
{
	return verts;
}

	
void MyFace::AddVertex(int vval)
{
	verts.push_back(vval);
}
	
void MyFace::AddVertices(int v1, int v2, int v3)
{
	verts.push_back(v1);
	verts.push_back(v2);
	verts.push_back(v3);
}

void MyFace::SetFaceIndex (int fi)
{
	fIndex = fi;
}