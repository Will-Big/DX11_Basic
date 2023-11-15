#include "pch.h"
#include "MeshFilter.h"

#include "GameObject.h"

MeshFilter::MeshFilter(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

MeshFilter::~MeshFilter()
{
}