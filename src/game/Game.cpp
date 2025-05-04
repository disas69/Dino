﻿#include "game/Game.h"

namespace GameConfig
{
int OBSTACLE_POOL_SIZE = 10;
int GROUND_POOL_SIZE = 2;
}

void Game::Initialize()
{
    // Init variables
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();

    // Load json config and apply settings
    m_gameSettings = GameSettings();

    // Init components
    m_camera.SetOffset({m_screenWidth / 2.0f - m_cameraXOffset, m_screenHeight / 2.0f});
    m_camera.SetRotation(0.f);
    m_camera.SetZoom(0.8f);

    // Load resources
    m_dinoTexture = raylib::Texture("resources/sprites/Dino_Idle.png");
    m_obstacleTexture = raylib::Texture("resources/sprites/Cactus_Small_Single.png");
    m_groundTexture = raylib::Texture("resources/sprites/Ground.png");

    // Create game objects
    m_player = std::make_unique<SpriteObject>(&m_dinoTexture);
    m_player->SetPosition({m_playerStartOffset, m_screenHeight - m_dinoTexture.GetHeight() - m_groundOffset});

    m_obstacles = std::vector<std::unique_ptr<SpriteObject>>(GameConfig::OBSTACLE_POOL_SIZE);
    for (int i = 0; i < GameConfig::OBSTACLE_POOL_SIZE; ++i)
    {
        m_obstacles[i] = std::make_unique<SpriteObject>(&m_obstacleTexture);
        m_obstacles[i]->SetPosition({600.0f + i * 200, m_screenHeight - m_obstacleTexture.GetHeight() - m_groundOffset});
    }

    m_grounds = std::vector<std::unique_ptr<SpriteObject>>(GameConfig::GROUND_POOL_SIZE);
    for (int i = 0; i < GameConfig::GROUND_POOL_SIZE; ++i)
    {
        m_grounds[i] = std::make_unique<SpriteObject>(&m_groundTexture);
        m_grounds[i]->SetPosition({i * m_grounds[i]->GetRectangle().width, m_screenHeight - m_groundTexture.GetHeight() - m_groundOffset});
    }

    // Load best score from json

    // Init game state

    // Init UI and open start screen
}

void Game::UpdateGame(float deltaTime)
{
    float speed = m_gameSettings.PlayerSettings.MovementSpeed;

    // Handle input
    if (IsKeyDown(KEY_RIGHT))
    {
        m_player->SetPosition({m_player->GetPosition().x + speed * deltaTime, m_player->GetPosition().y});
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        m_player->SetPosition({m_player->GetPosition().x - speed * deltaTime, m_player->GetPosition().y});
    }

    // Update player, update animations
    Vector2 playerPos = m_player->GetPosition();

    // Update ground position
    auto& currentGround = m_grounds[m_groundIndex];
    if (currentGround->GetPosition().x < playerPos.x - m_playerStartOffset)
    {
        m_groundIndex = (m_groundIndex + 1) % GameConfig::GROUND_POOL_SIZE;
        auto& nextGround = m_grounds[m_groundIndex];
        nextGround->SetPosition({currentGround->GetPosition().x + currentGround->GetRectangle().width, m_screenHeight - m_groundTexture.GetHeight() - m_groundOffset});
    }

    // Update obstacles position

    // Check collisions

    // Update game state

    // Update camera
    m_camera.SetTarget(playerPos);
}

void Game::DrawGame(raylib::Window& window)
{
    window.BeginDrawing();
    window.ClearBackground(RAYWHITE);

    // Draw game objects
    m_camera.BeginMode();
    for (const auto& ground : m_grounds)
    {
        ground->Draw();
    }

    for (const auto& obstacle : m_obstacles)
    {
        obstacle->Draw();
    }

    m_player->Draw();
    m_camera.EndMode();

    // Draw UI

    window.EndDrawing();
}

void Game::Dispose()
{
    // Save best score to json

    // Destroy objects

    // Unload resources
    m_dinoTexture.Unload();
    m_obstacleTexture.Unload();
    m_groundTexture.Unload();
}