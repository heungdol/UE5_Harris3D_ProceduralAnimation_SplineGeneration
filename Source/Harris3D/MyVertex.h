#pragma once

#include <set>

#include "KismetProceduralMeshLibrary.h"
#include "MeshDescription.h"
#include "MyMesh.h"

using namespace std;

UENUM(BlueprintType)
enum class EVertexType : uint8
{
	NONE
	, VERTEX_BUMP
	, VERTEX_SINK
	, VERTEX_FLAT
};

class MyMesh;
class MyFace;
class MyVertex
{
public:
	MyVertex ();
	~MyVertex ();

	double x;
	double y;
	double z;

	FVector vNormal;
	int vIndex;
	set<int> neighbours;

	EVertexType vType = EVertexType::NONE;

	double GetX();
	double GetY();
	double GetZ();
	
	void SetX(double xval);
	void SetY(double yval);
	void SetZ(double zval);
	void SetXYZ(double xval, double yval, double zval);

	FVector GetVertexNormal ();
	void SetVertexNormal (FVector vn);
	
	void AddNeighbour(int v);
	set<int> GetNeighbours();
	//TArray <int> GetNeighbours_TArray ();

	void CalculateVertexType (MyMesh* myMesh, int ringSize, float dotFlat);
	EVertexType GetVertexType ();
};
