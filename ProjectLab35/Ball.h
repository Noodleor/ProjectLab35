// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Ball.generated.h"


UCLASS()
class PROJECTLAB35_API ABall : public AActor
{
	GENERATED_BODY()
	
public:
	//~ Begin ABall interface
	/**
	* Default constructor
	*/
	ABall();

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* m_pSphereVisualMesh;

	/**
	* Initialize the ball with specified parameters
	* @param AdjacencyArrays Adjacency matrix of the graph
	* @param startVertices Index of the starting vertices
    * @param vertexes Array of graph vertices
    * @param objectVelocity Velocity of the object
    * @param instanceTransform Transform of the instance
	*/
	void InitBall(TArray<TArray<float>> AdjacencyArrays, TArray<int32> startVertices, TArray<FVector> vertexes, int32 objectVelocity, FTransform instanceTransform);

	/**
	* Find the shortest path using Dijkstra's algorithm
	* @param AdjacencyArrays Adjacency matrix of the graph
	* @param startVertex Index of the starting vertex
	* @param endVertex Index of the ending vertex
	* @param Vertexes Array of graph vertexes
	* @return Array representing the shortest full path
	*/
	TArray<int32> FindPathDijkstra(TArray<TArray<float>> AdjacencyArrays, int32 startVertex, int32 endVertex, TArray<FVector> Vertexes);

	/**
	* Print the paths found by the Dijkstra's algorithm
	* @param Paths Array of paths to be printed
	*/
	void PrintPaths(TArray<int32> Paths);

	/**
	* Generate a new color based on the previous instance's color by shifting the hue
	* @param instanceNum The index of the current instance for which the color is to be generated
	* @return A new FLinearColor that represents the shifted color
	*/
	FLinearColor WheelColor(int32 instanceNum);

	/**
	* Draw the paths found by the Dijkstra's algorithm
	* @param Paths Array of paths to be drew
	* @param instancesNum The index of the current instance for which the path is to be drew
	* @param startVertexNum The index to start drawing
	*/
	void DrawPath(TArray<int32> Paths, int32 instancesNum, int32 startVertexNum);

	/**
	* Ñalculates the speed of a Ball
	* @param startVertex Index of the starting vertex
	* @param endVertex Index of the ending vertex
	* @param Vertices Array of vertices
	* @return Speed of Ball per tick
	*/
	FVector MoveSpeedPerTick(int32 startVertex, int32 endVertex, TArray<FVector> Vertices);
	//~ End ABall interface
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ULineBatchComponent* m_pLineBatchComponent;

	FTransform m_instanseTransform;

	TArray<TArray<FBatchedLine>> m_Lines;

	TArray<FLinearColor> m_RandomColor;

	int32 m_objectVelocity;

	TArray<TArray<int32>> m_Paths;

	TArray<FVector> m_Vertices;

	TArray<TArray<float>> m_AdjacencyArrays; 

	TArray<FVector> m_DeltaLocation;

	TArray<int32> m_Iter;

public:	
	/**
	* Called every frame
	* @param DeltaTime Time elapsed since the last frame
	*/
	virtual void Tick(float DeltaTime) override;

};
