#include "MyFace.h"

MyFace::MyFace ()
{
	
}

MyFace::~MyFace ()
{
	
}

int MyFace::GetVertex0()
{
	return verts[0];
}
	
int MyFace::GetVertex1()
{
	return verts[1];
}
	
int MyFace::GetVertex2()
{
	return verts[2];
}

vector<int> MyFace::GetVertices()
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