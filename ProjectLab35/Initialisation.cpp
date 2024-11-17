// Fill out your copyright notice in the Description page of Project Settings.


#include "Initialisation.h"
#include "Engine/World.h"
#include "CreateGraph.h"
#include "Ball.h"

// Sets default values
AInitialisation::AInitialisation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(m_pSceneComponent);
	m_graphPointsCountX = 8;
	m_graphPointsCountY = 8;
	m_objectsCount = 4;
	m_offsetX = 10;
	m_offsetY = 10;
	m_offsetZ = 10;
	m_objectVelocity = 3;
}

// Called when the game starts or when spawned
void AInitialisation::BeginPlay()
{
	Super::BeginPlay();

	const auto world = GetWorld();

	const auto graph = world->SpawnActor<ACreateGraph>();

	graph->InitGraph(m_graphPointsCountX, m_graphPointsCountY, m_offsetX, m_offsetY, m_offsetZ);

	TArray<TArray<float>> adjacencyMatrix = graph->GetAdjacencyMatrix();
	TArray<FVector> vertices = graph->GetVertexes();

	auto Ball = world->SpawnActor<ABall>();

	TArray<int32> startVertices;
	for (int32 i = 0; i < m_objectsCount; i++)
	{
		int32 randStartVertex = FMath::RandRange(0, m_graphPointsCountX * m_graphPointsCountY - 1);

		m_InstanceTransform.SetLocation(vertices[randStartVertex] );

		float scaleFactor = 0.5;
		m_InstanceTransform.SetScale3D(FVector(1.f, 1.f, 1.f) * scaleFactor);

		startVertices.Add(randStartVertex);

		int32 instanceIndex = Ball->m_pSphereVisualMesh->AddInstance(m_InstanceTransform);
	}

	Ball->InitBall(adjacencyMatrix, startVertices, vertices, m_objectVelocity, m_InstanceTransform);
}

// Called every frame
void AInitialisation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

