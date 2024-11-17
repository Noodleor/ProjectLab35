// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateGraph.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Ball.h"

// Sets default values
ACreateGraph::ACreateGraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pLineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatcher"));
}

void ACreateGraph::InitGraph(int32 graphPointsCountX, int32 graphPointsCountY, int32 offsetX, int32 offsetY, int32 offsetZ)
{
	GenerateGraph(graphPointsCountX, graphPointsCountY, offsetX, offsetY, offsetZ);

	DrawGraph(graphPointsCountX, graphPointsCountY);

}

bool ACreateGraph::CheckIntersect(TArray<TArray<float>> adjacencyArrays, int32 firstVertex, int32 secondVertex)
{
	if (firstVertex != 0 && secondVertex < adjacencyArrays.Num() - 1)
	{
		if (adjacencyArrays[firstVertex - 1][secondVertex + 1])
		{
			return true;
		}

		if (secondVertex != 0 && firstVertex < adjacencyArrays.Num() - 1)
		{
			if (adjacencyArrays[firstVertex + 1][secondVertex - 1])
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


void ACreateGraph::GenerateGraph(int32 row, int32 column, int32 offsetX, int32 offsetY, int32 offsetZ)
{
	FVector MinBounds(-offsetX, -offsetY, -offsetZ);
	FVector MaxBounds(offsetX, offsetY, offsetZ);
	m_Vertices.Reserve(row * column);

	// generate coordinate values for each vertex
	for (int32 i = 0; i < row; i++)
	{
		for (int32 j = 0; j < column; j++)
		{
			m_Vertices.Add(FVector(FMath::RoundToFloat((i*100.0f + FMath::RandRange(MinBounds.X, MaxBounds.X)) * 1000.0f) / 1000.0f,
				FMath::RoundToFloat((j*100.0f + FMath::RandRange(MinBounds.Y, MaxBounds.Y)) * 1000.0f) / 1000.0f,
				FMath::RoundToFloat((FMath::RandRange(MinBounds.Z, MaxBounds.Z) * 1000.0f) / 1000.0f) ) );
		}
	}
	
	// generate connections between some vertices
	m_AdjacencyArrays.SetNum(row * column);

	for (int i = 0; i < row * column; i++)
	{
		m_AdjacencyArrays[i].SetNum(row * column);
		for (int j = 0; j < column * row; j++)
		{
			m_AdjacencyArrays[i][j] = 0.0f;
		}
	}

	TArray<int32> ConnectedVertices;
	ConnectedVertices.Add(0);
	while (ConnectedVertices.Num() != row * column)
	{
		int32 VertexToConnect = ConnectedVertices[FMath::RandRange(0, ConnectedVertices.Num() - 1)];
		int32 RandomVertex = FMath::RandRange(0, (row * column) - 1);
		if ( !(ConnectedVertices.Contains(RandomVertex)) && 
			(FVector::Dist(m_Vertices[VertexToConnect], m_Vertices[RandomVertex]) <= (FVector::Dist(FVector(0, 0, 0), FVector(100 + offsetX, 100 + offsetY, offsetZ))) ) &&
			(VertexToConnect < RandomVertex) && !(CheckIntersect(m_AdjacencyArrays, VertexToConnect, RandomVertex)) )
		{
			m_AdjacencyArrays[VertexToConnect][RandomVertex] = FVector::Dist(m_Vertices[VertexToConnect], m_Vertices[RandomVertex]);
			m_AdjacencyArrays[RandomVertex][VertexToConnect] = FVector::Dist(m_Vertices[VertexToConnect], m_Vertices[RandomVertex]);
			ConnectedVertices.Add(RandomVertex);
		}
	}
	
}

void ACreateGraph::DrawGraph(int32 row, int32 column)
{
	for (int32 i = 0; i < (row * column ); i++)
	{
		for (int32 j = 0; j < (row * column); j++)		
		{
			if (m_AdjacencyArrays[i][j] && j > i)
			{
				FBatchedLine line = FBatchedLine(m_Vertices[i], m_Vertices[j], FLinearColor(1, 1, 1, 0.5), -1, 3, 0);
				m_lines.Add(line);
			}			
		}
	}
	m_pLineBatchComponent->DrawLines(m_lines);
}

TArray<TArray<float>> ACreateGraph::GetAdjacencyMatrix()
{
	return m_AdjacencyArrays;
}

TArray<FVector> ACreateGraph::GetVertexes()
{
	return m_Vertices;
}

void ACreateGraph::PrintGraph(int32 row, int32 column)
{
	FString ArrayContents;
	ArrayContents += FString::Printf(TEXT(" \n"));
	for (int32 i = 0; i < (row * column); i++)
	{
		ArrayContents += FString::Printf(TEXT("%2d :"), i);
		for (int32 j = 0; j < (row * column); j++)
		{
			ArrayContents += FString::Printf(TEXT(" [%d]%.1f"), j, m_AdjacencyArrays[i][j]);
		}
		ArrayContents += FString::Printf(TEXT("\n"));
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ArrayContents);
}

// Called when the game starts or when spawned
void ACreateGraph::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreateGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

