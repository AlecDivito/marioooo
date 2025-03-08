#ifndef BOOK_DATATABLES_HPP
#define BOOK_DATATABLES_HPP

#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>


class Mario;
class Block;

enum CollisionDirection {
	Top,
	Bottom,
	Left,
	Right,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft
};

struct WindowData
{
	unsigned int width;
	unsigned int height;
};

namespace DataTables
{
	extern const WindowData window;
}

struct Direction
{
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct MarioData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::Time						fireInterval;
	// standing
	sf::IntRect						standTextRect;
	// crouching
	sf::IntRect						crouchTextRect;
	// falling
	sf::IntRect                     fallTextRect;
	// jumping
	sf::IntRect						jumpTextRect;
	// dead
	sf::IntRect						deadTextRect;
	// running animation
	sf::IntRect						runningTextRect;
	int								frameNum;
	sf::Time						runningDuration;

};

struct EnemyData
{
	int hitpoints;
	float speed;
	int damage;
	Textures::ID texture;
	sf::IntRect textureRect;
	sf::IntRect startingFrame;
	int frames;
	sf::Time frameSpeed;
};

struct BlockData
{
	int hitpoints;
	Textures::ID texture;
	sf::IntRect textureRect;
	bool isInvenciable;
	std::function<void(Mario&, Block &)> action;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

struct PickupData
{
	std::function<void(Mario&)> action;
	Textures::ID texture;
	sf::IntRect textureRect;
};

struct ParticleData
{
	sf::Color						color;
	sf::Time						lifetime;
};

struct CoinData
{
	Textures::ID texture;
	sf::IntRect textureRect;
	int frames;
	sf::Time frameSpeed;
	float lifeTime;
};

CollisionDirection calculateCollisionDirection(const Mario & a, const Block & b);
void handleCollisionDirection(Mario & a, const Block & b);
std::vector<MarioData> initializeMarioData();
std::vector<EnemyData> initializeEnemyData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<ParticleData> initializeParticleData();
std::vector<BlockData> initializeBlockData();
std::vector<CoinData> initializeCoinData();


#endif // BOOK_DATATABLES_HPP
