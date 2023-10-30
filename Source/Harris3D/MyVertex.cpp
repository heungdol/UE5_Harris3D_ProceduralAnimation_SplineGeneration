#include "MyVertex.h"

MyVertex::MyVertex ()
{
	
}

MyVertex::~MyVertex ()
{
	
}

const double MyVertex::GetX() const
{
	return x;
}
	
const double MyVertex::GetY() const
{
	return y;
}
	
const double MyVertex::GetZ() const
{
	return z;
}

void MyVertex::SetX(double xval)
{
	x = xval;
}
	
void MyVertex::SetY(double yval)
{
	y = yval;
}
	
void MyVertex::SetZ(double zval)
{
	z = zval;
}

void MyVertex::SetXYZ(double xval, double yval, double zval)
{
	x = xval;
	y = yval;
	z = zval;
}

void MyVertex::SetXYZ(FVector loc)
{
	x = loc.X;
	y = loc.Y;
	z = loc.Z;
}

const FVector MyVertex::GetVertexNormal () const
{
	return vNormal;
}

void MyVertex::SetVertexNormal (FVector vn)
{
	vNormal = vn;
}
	
void MyVertex::AddNeighbour(int v)
{
	neighbours.insert(v);
}
	
const set<int>& MyVertex::GetNeighbours() const
{
	return neighbours;
}

void MyVertex::CalculateVertexType (MyMesh* myMesh, int ringSize, float dotFlat)
{
	// 주변 이웃 Vertex를 이용해 Vertex 타입을 계산
	set <int> neighbours_ring = myMesh->CalculateNeighbourhood_Ring (vIndex, ringSize);
	
	// 진행할 수 없다면
	if (neighbours_ring.size() == 0)
		return;

	// FVector neighboursPos_sum = FVector::Zero();
	FVector neighboursPos_avg = FVector::Zero();

	// 이웃을 순환하며 모든 Location의 합 계산
	set<int>::iterator iter = neighbours_ring.begin();
	for (; iter != neighbours_ring.end(); ++iter)
		neighboursPos_avg += myMesh->GetVertexLocByIndex(*iter);

	// Location의 합을 통해 평균 Location 계산
	neighboursPos_avg = neighboursPos_avg / (1.0 * neighbours_ring.size());

	// Vertex의 Location 및 Normal를 이용하여 방향 계산
	FVector direction_self = GetVertexNormal ();
	FVector direction_avg = FVector (GetX(), GetY (), GetZ ()) - neighboursPos_avg;
	direction_avg.Normalize();

	// Vertex의 방향과 평균을 이용한 방향의 내적을 이용하여 Vertex 타입 판단
	float dotP = FVector::DotProduct(direction_self, direction_avg);
	if (abs(dotP) < dotFlat)
	{
		vType = EVertexType::VERTEX_FLAT;
	}
	else
	{
		if (dotP > 0)
		{
			vType = EVertexType::VERTEX_BUMP;
		}
		else
		{
			vType = EVertexType::VERTEX_SINK;
		}
	}
}

const EVertexType MyVertex::GetVertexType() const
{
	return vType;
}

const int& MyVertex::GetVertexIndex () const
{
	return vIndex;
}

void MyVertex::SetVertexIndex (int vi)
{
	vIndex = vi;
}