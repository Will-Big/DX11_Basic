#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd)
{
	_hWnd = hWnd;
	_width = GWinSizeX;
	_height = GWinSizeY;
}

void Game::Update()
{
}

void Game::Render()
{
}
