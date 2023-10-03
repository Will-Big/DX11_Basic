#include "pch.h"
#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour() : Super(ComponentType::Script)
{
}

MonoBehaviour::~MonoBehaviour()
{
}

void MonoBehaviour::Awake()
{
	Component::Awake();
}

void MonoBehaviour::Update()
{
	Component::Update();
}
