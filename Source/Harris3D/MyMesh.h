// ==================================================================================================
// MyMesh.h
// Harris3D에 사용될 Mesh의 정보를 가져와 여러 계산을 수행하는 클래스입니다.
// ==================================================================================================

#pragma once

#include <vector>

#include "KismetProceduralMeshLibrary.h"
#include "MeshDescription.h"
#include "StaticMeshResources.h"
#include "MyVertex.h"
#include "MyFace.h"

UPROPERTY(BlueprintReadOnly, Category="Inspector")

// 최대 50000개의 Vertex로 구성된 Mesh만을 계산
#define VERTEX_NUMBER_MAX 50000
// Overlapped된 Vertex의 판별할 때 사용할 변수
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
	// 
	vector<MyFace> faces;
	vector<MyVertex> vertices;
	vector<int> overlappingVert;

	bool isEnableModel = false;
	double diagValue; //the diagonal of the bounding box, used for clustering

public:
	// Face 모음을 반환
	vector<MyFace>& GetFaces ();
	// Vertex 모음을 반환: Harris3D 연산 및 Vertex Type 연산에 활용
	vector<MyVertex>& GetVertices ();
	// Overlapped된 Vertex 반환
	const vector<int>& GetOverlappingVertices () const;

	// StaticMesh 정보를 가져와 MyMesh의 변수 값들을 세팅
	bool ReadFile(const UStaticMeshComponent* sm);

	// 인덱스를 통해 Vertex의 Location 및 Normal 반환 
	FVector GetVertexLocByIndex (int ii);
	FVector GetVertexNorByIndex (int ii);

	// Harris3D에 활용될 이웃 Vertex 계산
	set<int> CalculateNeighbourhood_Ring(int, int);

	// Vertex의 개수가 적당한 모델인지 판단
	const bool& GetIsEnableModel () const;
};
