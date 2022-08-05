#include "MyVertex.h"

MyVertex::MyVertex ()
{
	
}

MyVertex::~MyVertex ()
{
	
}

double MyVertex::GetX()
{
	return x;
}
	
double MyVertex::GetY()
{
	return y;
}
	
double MyVertex::GetZ()
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



FVector MyVertex::GetVertexNormal ()
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
	
set<int> MyVertex::GetNeighbours()
{
	return neighbours;
}
	
/*TArray <int> MyVertex::GetNeighbours_TArray ()
{
	TArray <int> ret;

	set <int>::iterator it;

	for (it = neighbours.begin(); it != neighbours.end(); it++)
	{
		ret.Push(*it);
	}
		
	return ret;
}*/

void MyVertex::CalculateVertexType (MyMesh* myMesh, int ringSize, float dotFlat)
{
	set <int> neighbours_ring = myMesh->CalculateNeighbourhood_Ring (vIndex, ringSize);
	
	if (neighbours_ring.size() == 0)
		return;

	FVector neighboursPos_sum = FVector::Zero();
	FVector neighboursPos_avg = FVector::Zero();

	set<int>::iterator iter = neighbours_ring.begin();
	for (; iter != neighbours_ring.end(); ++iter)
		neighboursPos_sum += myMesh->GetVertexLocByIndex(*iter);

	neighboursPos_avg = neighboursPos_sum / (1.0 * neighbours_ring.size());

	//cout << neighbours_ring.size() << endl;

	FVector direction_self = myMesh->GetVertexNorByIndex(vIndex);
	FVector direction_avg = myMesh->GetVertexLocByIndex(vIndex) - neighboursPos_avg;
	direction_avg = direction_avg / FVector::Distance(FVector::Zero(), direction_avg);

	//cout << direction_self.X << ", " << direction_self.Y << ", "<< direction_self.Z << endl;
	//cout << direction_avg.X << ", " << direction_avg.Y << ", "<< direction_avg.Z << endl;
	//cout << FVector::DotProduct(direction_self, direction_avg) << endl;

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

EVertexType MyVertex::GetVertexType()
{
	return vType;
}
