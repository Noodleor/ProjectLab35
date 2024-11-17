// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Math/RandomStream.h"
#include "Math/Color.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"


// Sets default values
ABall::ABall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pLineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(TEXT("Line"));

	m_pSphereVisualMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(m_pSphereVisualMesh);
	m_pSphereVisualMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		m_pSphereVisualMesh->SetStaticMesh(SphereVisualAsset.Object);
		
	}

}

void ABall::InitBall(TArray<TArray<float>> AdjacencyArrays, TArray<int32> startVertices, TArray<FVector> vertexes, int32 objectVelocity, FTransform instanceTransform)
{
	m_AdjacencyArrays = AdjacencyArrays;
	m_Vertices = vertexes;
	m_objectVelocity = objectVelocity;
	m_instanseTransform = instanceTransform;

	m_Lines.SetNum(startVertices.Num());
	m_Iter.Init(0, startVertices.Num());
	m_DeltaLocation.SetNum(startVertices.Num(), false);

	for (int32 instanceNum = 0; instanceNum < startVertices.Num(); instanceNum++)
	{
		int32 randEndVertex = FMath::RandRange(0, vertexes.Num() - 1);
		while (startVertices[instanceNum] == randEndVertex)
		{
			randEndVertex = FMath::RandRange(0, vertexes.Num() - 1);
		}

		m_Paths.Add(FindPathDijkstra(AdjacencyArrays, startVertices[instanceNum], randEndVertex, vertexes));

		if (instanceNum > 0)
		{
			m_RandomColor.Add(WheelColor(instanceNum) );
		}
		else
		{
			m_RandomColor.Add(FLinearColor(FMath::RandRange(0.f, 360.f), 1.f, FMath::FRand()).HSVToLinearRGB());
		}
		

		DrawPath(m_Paths[instanceNum], instanceNum, 0);

		m_DeltaLocation[instanceNum] = MoveSpeedPerTick(m_Paths[instanceNum][0], m_Paths[instanceNum][1], m_Vertices);
		
	}
}

TArray<int32> ABall::FindPathDijkstra(TArray<TArray<float>> AdjacencyArrays, int32 startVertex, int32 endVertex, TArray<FVector> Vertexes)
{
	int32 CurrentVertex = startVertex;
	int32 MinValuePath = INT_MAX;
	TArray<float> m_Marks;
	TArray<int32> MinPaths;
	TArray<int32> VisitedVertices;
	TArray<int32> Paths;
	TArray<int32> endVertNearbys;
	//FString ArrayContents;
	//ArrayContents += FString::Printf(TEXT(" \n"));

	m_Marks.Init(FLT_MAX, Vertexes.Num());
	m_Marks[CurrentVertex] = 0.0;

	while (VisitedVertices.Num() != (Vertexes.Num() - 1) )
	{
		float MinMark = FLT_MAX;
		int32 MinMarkIndex = CurrentVertex;
		for (int32 NearbyVertex = 0; NearbyVertex < Vertexes.Num() ; NearbyVertex++)
		{
 			if (AdjacencyArrays[CurrentVertex][NearbyVertex] && !(VisitedVertices.Contains(NearbyVertex)) && !(endVertNearbys.Contains(CurrentVertex) && NearbyVertex == endVertex) )
			{
				//ArrayContents += FString::Printf(TEXT(" (%d,%d)"), CurrentVertex, NearbyVertex);
				if (m_Marks[CurrentVertex] + AdjacencyArrays[CurrentVertex][NearbyVertex] < m_Marks[NearbyVertex])
				{
					m_Marks[NearbyVertex] = m_Marks[CurrentVertex] + AdjacencyArrays[CurrentVertex][NearbyVertex];
				}
				if (MinMark > m_Marks[NearbyVertex])
				{
					MinMark = m_Marks[NearbyVertex];
					MinMarkIndex = NearbyVertex;
				}
				if (NearbyVertex == endVertex )
				{
					MinMarkIndex = CurrentVertex;
					endVertNearbys.Add(CurrentVertex);
					if (MinValuePath > m_Marks[NearbyVertex])
					{
						MinValuePath = m_Marks[NearbyVertex];

						MinPaths.Reset();
						MinPaths.Append(Paths);
					}
					
					break;
				}

				//ArrayContents += FString::Printf(TEXT(" %f %f %d ;"), m_Marks[NearbyVertex], MinMark, MinMarkIndex);
				
			}
		}
		//ArrayContents += FString::Printf(TEXT("\n"));

		if (!VisitedVertices.Contains(CurrentVertex))
		{
			VisitedVertices.Add(CurrentVertex);

		}
		
		if (MinMarkIndex == CurrentVertex)
		{
			if (VisitedVertices.Find(CurrentVertex) == 0)
			{
				break;
			}
			else 
			{
				MinMarkIndex = VisitedVertices[VisitedVertices.Find(CurrentVertex) - 1];
			}

			Paths.Remove(CurrentVertex);
		}
		if (!Paths.Contains(MinMarkIndex))
		{
			Paths.Add(MinMarkIndex);
		}
		
		CurrentVertex = MinMarkIndex;
		
		
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ArrayContents);

	if (!MinPaths.Contains(startVertex))
	{
		MinPaths.Insert(startVertex, 0);
	}
	MinPaths.Add(endVertex);
	return(MinPaths);
}

void ABall::PrintPaths(TArray<int32> Paths)
{
	FString ArrayContents;
	ArrayContents += FString::Printf(TEXT(" \n"));
	for (int32 i = 0; i < (Paths.Num() ); i++)
	{
		ArrayContents += FString::Printf(TEXT(" %d "), Paths[i]);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ArrayContents);
}

FLinearColor ABall::WheelColor(int32 instanceNum)
{
	FLinearColor HSVColor = (m_RandomColor[instanceNum - 1] ).LinearRGBToHSV();

	float step = 360.f / (float)m_Lines.Num();

	HSVColor.R += step;
	if (HSVColor.R > 360.f)
	{
		HSVColor.R -= 360.f;
	}

	step = 1.f / (float)m_Lines.Num();

	HSVColor.B += step;
	if (HSVColor.B > 1.f)
	{
		HSVColor.B -= 1.f;
	}


	FLinearColor color = HSVColor.HSVToLinearRGB();
	return (color);
}

void ABall::DrawPath( TArray<int32> Paths, int32 instancesNum, int32 startVertexNum)
{
	
	for (int32 count = startVertexNum; count < Paths.Num() - 1; count++)
	{
		FVector startPoint = m_Vertices[m_Paths[instancesNum][count]] + 1.f;
		FVector endPoint = m_Vertices[m_Paths[instancesNum][count + 1]] + 1.f;

		startPoint.Z += (instancesNum / 10.f);
		endPoint.Z += (instancesNum / 10.f);

		FBatchedLine line = FBatchedLine(startPoint, endPoint, m_RandomColor[instancesNum], -1, 5, 0);

		FVector vector = endPoint - startPoint;

		FVector rotatedVector1 = FVector(vector.X * FMath::Cos(FMath::DegreesToRadians(135)) - vector.Y * FMath::Sin(FMath::DegreesToRadians(135)),
										 vector.X * FMath::Sin(FMath::DegreesToRadians(135)) + vector.Y * FMath::Cos(FMath::DegreesToRadians(135)), 0) / 10;
		FVector rotatedVector2 = FVector(vector.X * FMath::Cos(FMath::DegreesToRadians(-135)) - vector.Y * FMath::Sin(FMath::DegreesToRadians(-135)),
										 vector.X * FMath::Sin(FMath::DegreesToRadians(-135)) + vector.Y * FMath::Cos(FMath::DegreesToRadians(-135)), 0) / 10;

		FVector arrowHead1 = endPoint + rotatedVector1;
		FVector arrowHead2 = endPoint + rotatedVector2;

		FBatchedLine arrowline1 = FBatchedLine(endPoint, arrowHead1, m_RandomColor[instancesNum], -1, 5, 0);
		FBatchedLine arrowline2 = FBatchedLine(endPoint, arrowHead2, m_RandomColor[instancesNum], -1, 5, 0);

		m_Lines[instancesNum].Add(line);
		m_Lines[instancesNum].Add(arrowline1);
		m_Lines[instancesNum].Add(arrowline2);
	}

	m_pLineBatchComponent->DrawLines(m_Lines[instancesNum]);
}

FVector ABall::MoveSpeedPerTick(int32 startVertex, int32 endVertex, TArray<FVector> Vertices)
{
	FVector startVertexVector = Vertices[startVertex];
	FVector endVertexVector = Vertices[endVertex];
	FVector MoveSpeed = (endVertexVector - startVertexVector);
	return MoveSpeed.GetSafeNormal() * m_objectVelocity;
}


// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = FVector(0, 0, 0);

	for (int32 instanceNum = 0; instanceNum < m_Paths.Num(); instanceNum++)
	{

		if (m_pSphereVisualMesh->GetInstanceTransform(instanceNum, m_instanseTransform, false))
		{
			CurrentLocation = m_instanseTransform.GetLocation();
		}

		if (FVector::Dist(CurrentLocation, m_Vertices[m_Paths[instanceNum][m_Iter[instanceNum] + 1]]) > 5 )
		{
			FVector NewLocation = CurrentLocation + m_DeltaLocation[instanceNum];
			
			m_instanseTransform.SetLocation(NewLocation);

			m_pSphereVisualMesh->UpdateInstanceTransform(instanceNum, m_instanseTransform, false);
		}
		else if (m_Iter[instanceNum] < m_Paths[instanceNum].Num() - 2)
		{
			m_Iter[instanceNum]++;

			m_instanseTransform.SetLocation(m_Vertices[m_Paths[instanceNum][m_Iter[instanceNum]]]);

			m_pSphereVisualMesh->UpdateInstanceTransform(instanceNum, m_instanseTransform, false);

			m_DeltaLocation[instanceNum] = MoveSpeedPerTick(m_Paths[instanceNum][m_Iter[instanceNum]], m_Paths[instanceNum][m_Iter[instanceNum] + 1], m_Vertices);

			m_Lines[instanceNum].Empty();

			m_pLineBatchComponent->Flush();

			for (int32 instanceN = 0; instanceN < m_Paths.Num(); instanceN++)
			{
				DrawPath(m_Paths[instanceN], instanceN, m_Iter[instanceN]);
			}
		}
		else
		{
			m_Paths[instanceNum] = FindPathDijkstra(m_AdjacencyArrays, m_Paths[instanceNum][m_Iter[instanceNum] + 1], FMath::RandRange(0, m_Vertices.Num() - 1), m_Vertices);

			m_Iter[instanceNum] = 0;

			m_Lines[instanceNum].Empty();

			m_pLineBatchComponent->Flush();

			for (int32 instanceN = 0; instanceN < m_Paths.Num(); instanceN++)
			{
				DrawPath(m_Paths[instanceN], instanceN, m_Iter[instanceN]);
			}

			m_DeltaLocation[instanceNum] = MoveSpeedPerTick(m_Paths[instanceNum][m_Iter[instanceNum]], m_Paths[instanceNum][m_Iter[instanceNum] + 1], m_Vertices);
		}

		m_pSphereVisualMesh->MarkRenderStateDirty();
		
	}
	
}

