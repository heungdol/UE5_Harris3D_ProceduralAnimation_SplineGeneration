#pragma once

#include <vector>

#include "KismetProceduralMeshLibrary.h"
#include "MeshDescription.h"
#include "StaticMeshResources.h"
#include "MyVertex.h"
#include "MyFace.h"

UPROPERTY(BlueprintReadOnly, Category="Inspector")

#define VERTEX_NUMBER_MAX 50000
#define VERTEX_OVERLAP_DISTANCE_SQ 0.0001f

using namespace std;

class MyVertex;
class MyFace;
class MyMesh
{
public:
	MyMesh ();
	MyMesh (const UStaticMeshComponent* sm);
	~MyMesh ();

protected:
	vector<MyFace> faces;
	vector<MyVertex> vertices;
	vector<int> overlappingVert;

	bool isEnableModel = false;
	double diagValue; //the diagonal of the bounding box, used for clustering

public:
	vector<MyFace>& GetFaces ();
	vector<MyVertex>& GetVertices ();
	const vector<int>& GetOverlappingVertices () const;

	bool ReadFile(const UStaticMeshComponent* sm);
	FVector GetVertexLocByIndex (int ii);
	FVector GetVertexNorByIndex (int ii);

	set<int> CalculateNeighbourhood_Ring(int, int);
	bool GetIsEnableModel () const;
};
