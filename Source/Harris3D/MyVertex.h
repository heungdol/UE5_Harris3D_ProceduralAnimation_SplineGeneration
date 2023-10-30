// ==================================================================================================
// MyVertex.h
// Harris3D에 사용될 MyMesh의 가장 기본 단위가 되는 클래스입니다.
// MyVertex의 3개로 MyFace를 구성할 수 있습니다.
// ==================================================================================================

#pragma once

#include <set>

#include "KismetProceduralMeshLibrary.h"
#include "MeshDescription.h"
#include "MyMesh.h"
#include "Math/Vector.h"

using namespace std;

// MyVertex의 타입을 구분하기 위한 Enum
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
	// Vertex 위치
	double x;
	double y;
	double z;

	// Vertex 노멀 방향
	FVector vNormal;
	
	// Mesh에서 구성될 때의 인덱스 값
	int vIndex;

	// Harris3D 연산을 위한 주변 Vertex들의 정보
	set<int> neighbours;

	// Vertex 타입
	EVertexType vType = EVertexType::NONE;

public:
	// Vertex Location 반환
	const double GetX() const;
	const double GetY() const;
	const double GetZ() const;
	
	// Vertex Location 설정
	void SetX(double xval);
	void SetY(double yval);
	void SetZ(double zval);
	void SetXYZ(double xval, double yval, double zval);
	void SetXYZ(FVector loc);

	// Vertex Normal 설정 및 반환
	void SetVertexNormal (FVector vn);
	const FVector GetVertexNormal () const;
	
	// Vertex Index 설정 및 반환
	void SetVertexIndex (int vi);
	const int& GetVertexIndex () const; 

	// Vertex 이웃 설정 및 반환
	void AddNeighbour(int v);
	const set<int>& GetNeighbours() const;

	// Vertex 타입 계산 및 반환
	void CalculateVertexType (MyMesh* myMesh, int ringSize, float dotFlat);
	const EVertexType GetVertexType () const;
};
