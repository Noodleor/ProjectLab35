// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "Initialisation.h"
#include "CreateGraph.generated.h"

UCLASS()
class PROJECTLAB35_API ACreateGraph : public AActor
{
	GENERATED_BODY()
	
public:	
	//~ Begin ACreateGraph interface
	/**
	* Default constructor
	*/
	ACreateGraph();
	
	/**
	* Initialize the graph with specified parameters by functions calls
	* @param m_graphPointsCountX Number of graph points along the X-axis
	* @param graphPointsCountY Number of graph points along the Y-axis
	* @param offsetX Offset along the X-axis
	* @param offsetY Offset along the Y-axis
	* @param offsetZ Offset along the Z-axis
	*/
	void InitGraph(int32 m_graphPointsCountX, int32 graphPointsCountY, int32 offsetX, int32 offsetY, int32 offsetZ);

	/**
	* Checks intersection with nearby connections
	* @param adjacencyArrays Array of adjacency arrays
	* @param firstVertex First vertex index
	* @param secondVertex Second vertex index
	* @return true if there is an intersection, otherwise false
	*/
	bool CheckIntersect(TArray<TArray<float>> adjacencyArrays, int32 firstVertex, int32 secondVertex);

	/**
	* Generate the graph with specified row and column indices
	* @param row Row index
	* @param column Column index
	* @param offsetX Offset along the X-axis
	* @param offsetY Offset along the Y-axis
	* @param offsetZ Offset along the Z-axis
	*/
	void GenerateGraph(int32 row, int32 column, int32 offsetX, int32 offsetY, int32 offsetZ);

	/**
	* Draw the graph with specified row and column indices
	* @param row Row index
	* @param column Column index
	*/
	void DrawGraph(int32 row, int32 column);

	/**
	* Get the adjacency matrix of the graph
	* @return Adjacency matrix as a 2D array of floats
	*/
	TArray<TArray<float>> GetAdjacencyMatrix();

	/**
	* Get the vertexes of the graph
	* @return Vertexes as an array of FVector
	*/
	TArray<FVector> GetVertexes();

	/**
	* Print the graph matrix with specified row and column indices
	* @param row Row index
	* @param column Column index
	*/
	void PrintGraph(int32 row, int32 column);
	//~ End ACreateGraph interface
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FVector> m_Vertices;

	TArray<TArray<float>> m_AdjacencyArrays;

	ULineBatchComponent* m_pLineBatchComponent;

	TArray<FBatchedLine> m_lines;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
