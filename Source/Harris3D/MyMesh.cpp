#include "MyMesh.h"
#include <algorithm>

MyMesh::MyMesh()
{
	
}

MyMesh::MyMesh (const UStaticMeshComponent* sm)
{
	vertices.clear();
	faces.clear();
	overlappingVert.clear ();
	
	if (ReadFile(sm) == false)
		return;
	
	double xmin=0.0, xmax=0.0, ymin=0.0, ymax=0.0, zmin=0.0, zmax=0.0;

	for(int k = 0; k < vertices.size(); k++)
	{
		if(vertices[k].GetX() < xmin)
			xmin = vertices[k].GetX();
		else if(vertices[k].GetX() > xmax)
			xmax = vertices[k].GetX();

		if(vertices[k].GetY() < ymin)
			ymin = vertices[k].GetY();
		else if(vertices[k].GetY() > ymax)
			ymax = vertices[k].GetY();

		if(vertices[k].GetZ() < zmin)
			zmin = vertices[k].GetZ();
		else if(vertices[k].GetZ() > zmax)
			zmax = vertices[k].GetZ();
	}

	diagValue = sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin) + (zmax - zmin)*(zmax - zmin));
}

MyMesh::~MyMesh()
{
	
}

vector<MyFace>& MyMesh::GetFaces ()
{
	return faces;
}

vector<MyVertex>& MyMesh::GetVertices ()
{
	return vertices;
}

const vector<int>& MyMesh::GetOverlappingVertices () const
{
	return overlappingVert;
}

bool MyMesh::ReadFile(const UStaticMeshComponent* sm)
{
	int numVertices, numFaces;;
	
	// Static Mesh 정보 가져오기
	TArray <FVector> verts;
	TArray <int> tris;
	TArray <FVector> nors;
	TArray <FVector2D> uvs;
	TArray<FProcMeshTangent> tans;

	// TODO UV Map을 고려하지 않고 Vertex 및 Face를 찾을 수 있는 방법을 알게되면 적용시키는게 좋을 것 같다.
	// UV Map의 Vertex 중복문제 때문에 자원 낭비가 발생한다.
	UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(sm->GetStaticMesh(), 0, 0, verts, tris, nors, uvs, tans);
	
	numVertices = verts.Num();
	numFaces = tris.Num ();

	// 버텍스 개수가 너무 많으면
	if (numVertices > VERTEX_NUMBER_MAX)
	{
		isEnableModel = false;
		return false;
	}

	isEnableModel = true;
	
	for(int i = 0; i < numVertices; i++)
	{
		double xc, yc, zc;

		xc = verts [i].X;
		yc = verts [i].Y;
		zc = verts [i].Z;

		MyVertex v;
		v.SetVertexIndex (i);
		v.SetXYZ(xc,yc,zc);
		v.SetVertexNormal(nors [i]);

		overlappingVert.push_back(i);
		
		// 중복 버텍스 제거 위함
		// 처음부터 순회하면서 같은 위치에 있는 것을 중복으로 판단한다
		for (int j = 0; j < overlappingVert.size()-1; j++)
		{
			// 거리로 판단
			float dist = FVector::DistSquared(verts[i], verts[j]);
			if (dist < VERTEX_OVERLAP_DISTANCE_SQ)
			{
				overlappingVert [i] = j;
				v.SetVertexIndex (j);
				break;
			}
		}
		
		vertices.push_back(v);
	}

	// UStaticMesh->Face Vertex 3개 묶음
	for(int i = 0, fi = 0;  i < numFaces; i+=3, fi++)
	{
		int vt1, vt2, vt3;
		
		vt1 = overlappingVert [tris[i+0]];
		vt2 = overlappingVert [tris[i+1]];
		vt3 = overlappingVert [tris[i+2]];

		MyFace f;
		f.SetFaceIndex (fi);
		f.AddVertices (vt1,vt2,vt3);
		
		//create 1st ring neighbourhood, credit to source code
		vertices[vt1].AddNeighbour(vt2);
		vertices[vt1].AddNeighbour(vt3);
		
		vertices[vt2].AddNeighbour(vt1);
		vertices[vt2].AddNeighbour(vt3);
		
		vertices[vt3].AddNeighbour(vt1);
		vertices[vt3].AddNeighbour(vt2);

		faces.push_back(f);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("true"));
	return true;
}

FVector MyMesh::GetVertexLocByIndex (int ii)
{
	if (vertices.size() <= ii)
		return FVector (0, 0, 0);

	return FVector ( vertices [ii].GetX (),  vertices [ii].GetY (),  vertices [ii].GetZ ());
}

FVector MyMesh::GetVertexNorByIndex (int ii)
{
	if (vertices.size() <= ii)
		return FVector (0, 0, 1);

	return vertices [ii].GetVertexNormal();
}

//calculate the neighbourhood at ring N
set<int> MyMesh::CalculateNeighbourhood_Ring(int indexVertex, int ringSize)
{
	set<int> s_prev, s_current, newring, s_total, s_ring, temp;
	set<int> nbhood = vertices[indexVertex].GetNeighbours();

	s_prev.insert(indexVertex); //insert the index of the vertex
	s_current.insert(nbhood.begin(), nbhood.end()); //insert the neighbourhood at ring 1
	s_total.insert(nbhood.begin(), nbhood.end()); //set of all neighbours of the vertex

	//store neighbours at each ring
	set<int>* nhd = new set<int>[ringSize];
	nhd[0].insert(nbhood.begin(), nbhood.end()); // at ring 1
	
	set<int> set_nhd;
	for (int j = 1; j < ringSize; ++j)
	{
		set<int>::iterator itr;
		for (itr = nhd[j - 1].begin(); itr != nhd[j - 1].end(); ++itr)
		{
			set_nhd = vertices[*itr].GetNeighbours();
			s_ring.insert(set_nhd.begin(), set_nhd.end()); //add neighbours of each vertex at ring j-1
			set_nhd.clear();
		}

		//calculating the difference s_ring - s_current
		set_difference(s_ring.begin(), s_ring.end(), s_current.begin(), s_current.end(), inserter(temp, temp.end()));

		//calculating the difference (s_ring - s_current) - s_prev
		set_difference(temp.begin(), temp.end(), s_prev.begin(), s_prev.end(), inserter(newring, newring.end()));

		s_prev.insert(s_current.begin(), s_current.end());
		s_current.insert(s_ring.begin(), s_ring.end());
		s_ring.clear();

		// add it to the array of rings
		s_total.insert(newring.begin(), newring.end());
		nhd[j].insert(newring.begin(), newring.end());
	}

	delete[] nhd;

	return s_total;
}

bool MyMesh::GetIsEnableModel() const
{
	return isEnableModel;
}
