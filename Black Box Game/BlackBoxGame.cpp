#include "BlackBoxGame.h"

using namespace BlackBoxEngine;

#include <cassert>

#include "Actors/Card.h"

void BlackBoxGame::InitGame()
{
	BB_LOG(LogType::kMessage, "Game starting");
    
    MakeCard();
    //m_pEngineManager->m_pActorManager->LoadLevel( "../Assets/Levels/ExampleLevel.xml" );

    m_pEngineManager->SetGameUpdate( [this]() { GameUpdate(); } );
}

BlackBoxGame::BlackBoxGame()
    : m_pEngineManager(BlackBoxManager::NewSingleton())
{
    m_pEngineManager->CreateWindow(
        "Dragon Quest Clone", kDefaultXPos, kDefaultYPos, kDefaultWidth, kDefaultHeight, BB_Window::kWindowFlag_Resizable
    );
    BB_LOG(LogType::kMessage, "Window Created");
}

BlackBoxGame::~BlackBoxGame()
{
    if (m_pEngineManager)
        m_pEngineManager->DeleteSingleton();
}

void BlackBoxGame::Launch()
{
    assert(m_pEngineManager);
    m_pEngineManager->InitEngine();
    InitGame();
    m_pEngineManager->RunEngine();
}

void BlackBoxGame::GameUpdate()
{

}

BlackBoxGame* BlackBoxGame::NewSingleton()
{
    DeleteSingleton();
    s_pBlackBoxGame = new BlackBoxGame;
    return s_pBlackBoxGame;
}

BlackBoxGame* BlackBoxGame::Get()
{
    assert(s_pBlackBoxGame);
    return s_pBlackBoxGame;
}

void BlackBoxGame::DeleteSingleton()
{
    if (s_pBlackBoxGame)
    {
        delete s_pBlackBoxGame;
        s_pBlackBoxGame = nullptr;
    }
}
