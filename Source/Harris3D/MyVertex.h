﻿#pragma once

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

protected:
	double x;
	double y;
	double z;

	FVector vNormal;
	int vIndex;
	set<int> neighbours;

	EVertexType vType = EVertexType::NONE;

public:
	const double GetX() const;
	const double GetY() const;
	const double GetZ() const;
	
	void SetX(double xval);
	void SetY(double yval);
	void SetZ(double zval);
	void SetXYZ(double xval, double yval, double zval);

	const FVector GetVertexNormal () const;
	void SetVertexNormal (FVector vn);
	
	void AddNeighbour(int v);
	const set<int>& GetNeighbours() const;

	void CalculateVertexType (MyMesh* myMesh, int ringSize, float dotFlat);
	const EVertexType GetVertexType () const;

	void SetVertexIndex (int vi);
};
