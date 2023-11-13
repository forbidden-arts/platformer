#include "Player.h"
#include "InputHandler.h"
#include "Constants.h"
#include "Coin.h"
#include <vector>
#include "Game.h"
#include "Door.h"

Player::Player(Game* pGame) : Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)), m_pGame(pGame)
{
    setPosition(sf::Vector2f(ScreenWidth * 0.5f, ScreenHeight * 0.5f)); //TODO: spawn position constant?
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setColor(sf::Color::Blue);
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;
    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;
    
    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));

    auto pRectangles = m_pGame->getRectangles();
    for (auto& pRectangle : pRectangles)
    {
        //Move player back if collided with level geometry
        if (pRectangle->collidesWith(this))
        {
            sf::Transformable::move(sf::Vector2f(XPush * deltaTime, 0.0f));
        }
    }
    
    setPosition(std::clamp(getPosition().x, -30.0f, (float)ScreenWidth), getPosition().y); //TODO: clean

}

void Player::updateIsDead()
{
    m_isDead = getPosition().y > ScreenHeight || getPosition().x < 0;
    if (m_isDead)
    {
        this->m_coins = 0;
    }
}

void Player::update(float deltaTime)
{
    std::vector<Coin*> Coins = m_pGame->getCoins();
    int i = 0;

    for (auto& temp : Coins)
    {
        if (temp->collidesWith(this))
        {
            temp->setCollected(true);
            m_coins++;
        }
    }

    if (m_pGame->getDoor()->collidesWith(this))
    {
        m_pGame->getDoor()->setTriggered(true);
    }
    auto pRectangles = m_pGame->getRectangles();
    for (auto& pRectangle : pRectangles)
    {
        //Move player back if collided with level geometry
        if (pRectangle->collidesWith(this))
        {
            sf::Transformable::move(sf::Vector2f(0.0f, Gravity * deltaTime * 2));
            // m_isGrounded = true;
        }
    }
}

