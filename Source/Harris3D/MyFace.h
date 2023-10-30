// ==================================================================================================
// MyFace.h
// Harris3D에 사용될 MyMesh의 기본 단위가 되는 클래스입니다.
// MyVertex 3개로 구성됩니다.
// ==================================================================================================

#pragma once

#include <vector>

using namespace std;

class MyFace
{
public:
	MyFace ();
	~MyFace ();

protected:
	// Face의 인덱스
	int fIndex;

	// Face를 구성하고 있는 Vertex의 인덱스 모음 (3개)
	vector<int> verts;
	
public:
	
	// Vertex 추가
	void AddVertex(int vval);
	void AddVertices(int v1, int v2, int v3);

	// Face를 구성하고 있는 Vertex 모음 반환
	const vector<int>& GetVertices() const;

	// Face Index 설정 및 반환
	void SetFaceIndex (int fi); 
	const int& GetFaceIndex () const;
};
