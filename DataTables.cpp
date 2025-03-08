#include "DataTables.h"
#include "Mario.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Block.h"
#include "Enemy.h"
#include "Particle.h"
#include "Coin.h"

#include <iostream>

using namespace std::placeholders;

const WindowData DataTables::window = {
	1280,
	720
};

CollisionDirection calculateCollisionDirection(const Mario & a, const Block & b)
{
	sf::FloatRect marioTopLeft(a.getBoundingRect().left + a.getBoundingRect().width, a.getBoundingRect().top, 1, 1);
	sf::FloatRect marioTopRight(a.getBoundingRect().left, a.getBoundingRect().top, 1, 1);
	sf::FloatRect marioBottomLeft(a.getBoundingRect().left + a.getBoundingRect().width, a.getBoundingRect().top + a.getBoundingRect().height, 1, 1);
	sf::FloatRect marioBottomRight(a.getBoundingRect().left, a.getBoundingRect().top + a.getBoundingRect().height, 1, 1);
	sf::FloatRect blockTopLeft(b.getBoundingRect().left + b.getBoundingRect().width, b.getBoundingRect().top, 1, 1);
	sf::FloatRect blockTopRight(b.getBoundingRect().left, b.getBoundingRect().top, 1, 1);
	sf::FloatRect blockBottomLeft(b.getBoundingRect().left + b.getBoundingRect().width, b.getBoundingRect().top + b.getBoundingRect().height, 1, 1);
	sf::FloatRect blockBottomRight(b.getBoundingRect().left, b.getBoundingRect().top + b.getBoundingRect().height, 1, 1);

	if (b.getBoundingRect().intersects(marioTopLeft) && b.getBoundingRect().intersects(marioTopRight) ||
		a.getBoundingRect().intersects(blockBottomLeft) && a.getBoundingRect().intersects(blockBottomRight))
	{
		return Bottom;
	}
	else if (b.getBoundingRect().intersects(marioBottomLeft) && b.getBoundingRect().intersects(marioBottomRight) ||
		a.getBoundingRect().intersects(blockTopLeft) && a.getBoundingRect().intersects(blockTopRight))
	{
		return Top;
	}
	else if (b.getBoundingRect().intersects(marioTopRight) && b.getBoundingRect().intersects(marioBottomRight) ||
		a.getBoundingRect().intersects(blockBottomLeft) && a.getBoundingRect().intersects(blockTopLeft))
	{
		return Left;
	}
	else if (b.getBoundingRect().intersects(marioTopLeft) && b.getBoundingRect().intersects(marioBottomLeft) ||
		a.getBoundingRect().intersects(blockBottomRight) && a.getBoundingRect().intersects(blockTopRight))
	{
		return Right;
	}
	else if (b.getBoundingRect().intersects(marioBottomLeft) && a.getBoundingRect().intersects(blockTopRight))
	{
		return BottomLeft;
	}
	else if (b.getBoundingRect().intersects(marioTopRight) && a.getBoundingRect().intersects(blockBottomLeft))
	{
		return TopRight;
	}
	else if (b.getBoundingRect().intersects(marioBottomRight) && a.getBoundingRect().intersects(blockTopLeft))
	{
		return BottomRight;
	}
	else if (b.getBoundingRect().intersects(marioTopLeft) && a.getBoundingRect().intersects(blockBottomRight))
	{
		return TopLeft;
	}
	else
	{
		return CollisionDirection();
	}
}

void handleCollisionDirection(Mario & a, const Block & b, const CollisionDirection direction)
{
	if (direction == CollisionDirection::Bottom)
	{
		if (!a.isFalling())
		{
			// head but
			a.setFalling();
		}
	}
	else if (direction == CollisionDirection::Top)
	{
		auto yDiffrence = a.getBoundingRect().top + a.getBoundingRect().height - b.getBoundingRect().top;
		a.setGrounded(yDiffrence);
	}
	else if (direction == CollisionDirection::Right)
	{
		auto xDiffrence = a.getBoundingRect().left + a.getBoundingRect().width - b.getBoundingRect().left;
		a.setPushing(xDiffrence);
	}
	else if (direction == CollisionDirection::Left)
	{
		auto xDiffrence = a.getBoundingRect().left - b.getBoundingRect().left - b.getBoundingRect().width;
		a.setPushing(xDiffrence);
	}
	else if (direction == CollisionDirection::TopRight || direction == CollisionDirection::TopLeft)
	{
		if (a.isJumping())
		{
			auto yDiffrence = b.getBoundingRect().top + b.getBoundingRect().height - a.getBoundingRect().top;
			a.setFalling(yDiffrence);
		}
		else if (direction == CollisionDirection::TopRight)
		{
			auto xDiffrence = a.getBoundingRect().left - b.getBoundingRect().left - b.getBoundingRect().width;
			a.setPushing(xDiffrence);
		}
		else if (direction == CollisionDirection::TopLeft)
		{
			auto xDiffrence = a.getBoundingRect().left + a.getBoundingRect().width - b.getBoundingRect().left;
			a.setPushing(xDiffrence);
		}
	}
	else if (direction == CollisionDirection::BottomRight || direction == CollisionDirection::BottomLeft)
	{
		if (a.isFalling())
		{
			auto yDiffrence = a.getBoundingRect().top + a.getBoundingRect().height - b.getBoundingRect().top;
			a.setGrounded(yDiffrence);
		}
		else if (direction == CollisionDirection::BottomRight)
		{
			auto xDiffrence = a.getBoundingRect().left - b.getBoundingRect().left - b.getBoundingRect().width;
			a.setPushing(xDiffrence);
		}
		else if (direction == CollisionDirection::BottomLeft)
		{
			auto xDiffrence = a.getBoundingRect().left + a.getBoundingRect().width - b.getBoundingRect().left;
			a.setPushing(xDiffrence);
		}
	}
}

std::vector<MarioData> initializeMarioData()
{
	std::vector<MarioData> data(Mario::TypeCount);

	data[Mario::MarioPlayerSmall].hitpoints = 1;
	data[Mario::MarioPlayerSmall].speed = 400.f;
	data[Mario::MarioPlayerSmall].fireInterval = sf::seconds(1);
	data[Mario::MarioPlayerSmall].texture = Textures::PlayerSheet;
	data[Mario::MarioPlayerSmall].standTextRect = sf::IntRect(2, 360, 28, 40);
	data[Mario::MarioPlayerSmall].crouchTextRect = sf::IntRect(313, 373, 31, 28);
	data[Mario::MarioPlayerSmall].fallTextRect = sf::IntRect(244, 357, 32, 40);
	data[Mario::MarioPlayerSmall].jumpTextRect = sf::IntRect(209, 365, 34, 46);
	data[Mario::MarioPlayerSmall].deadTextRect = sf::IntRect(801, 354, 34, 49);
	data[Mario::MarioPlayerSmall].runningTextRect = sf::IntRect(34, 359, 34, 41);
	data[Mario::MarioPlayerSmall].frameNum = 2;
	data[Mario::MarioPlayerSmall].runningDuration = sf::seconds(.25f);

	data[Mario::MarioPlayerBig].hitpoints = 2;
	data[Mario::MarioPlayerBig].speed = 400.f;
	data[Mario::MarioPlayerBig].fireInterval = sf::seconds(1);
	data[Mario::MarioPlayerBig].texture = Textures::PlayerSheet;
	data[Mario::MarioPlayerBig].standTextRect = sf::IntRect(7, 463, 32, 56);
	data[Mario::MarioPlayerBig].crouchTextRect = sf::IntRect(410, 495, 32, 30);
	data[Mario::MarioPlayerBig].fallTextRect = sf::IntRect(336, 463, 32, 58);
	data[Mario::MarioPlayerBig].jumpTextRect = sf::IntRect(289, 460, 34, 63);
	data[Mario::MarioPlayerBig].deadTextRect = sf::IntRect();
	data[Mario::MarioPlayerBig].runningTextRect = sf::IntRect(41, 463, 32, 56);
	data[Mario::MarioPlayerBig].frameNum = 2;
	data[Mario::MarioPlayerBig].runningDuration = sf::seconds(.25f);

	return data;
}

std::vector<EnemyData> initializeEnemyData()
{
	std::vector<EnemyData> data(Enemy::TypeCount);

	data[Enemy::Type::Goomba].hitpoints = 1;
	data[Enemy::Type::Goomba].speed = 100.0f;
	data[Enemy::Type::Goomba].damage = 1;
	data[Enemy::Type::Goomba].texture = Textures::EnemySheet;
	data[Enemy::Type::Goomba].textureRect = sf::IntRect(5, 17, 19, 16);
	data[Enemy::Type::Goomba].startingFrame = sf::IntRect(5, 17, 19, 16);
	data[Enemy::Type::Goomba].frames = 2;
	data[Enemy::Type::Goomba].frameSpeed = sf::seconds(.5f);

	data[Enemy::Type::Turtle].hitpoints = 1;
	data[Enemy::Type::Turtle].speed = 100.0f;
	data[Enemy::Type::Turtle].damage = 1;
	data[Enemy::Type::Turtle].texture = Textures::EnemySheet;
	data[Enemy::Type::Turtle].textureRect = sf::IntRect(26, 564, 19, 27);
	data[Enemy::Type::Turtle].startingFrame = sf::IntRect(26, 564, 19, 27);
	data[Enemy::Type::Turtle].frames = 2;
	data[Enemy::Type::Turtle].frameSpeed = sf::seconds(.5f);

	data[Enemy::Type::Cactus].hitpoints = 5;
	data[Enemy::Type::Cactus].speed = 0.0f;
	data[Enemy::Type::Cactus].damage = 1;
	data[Enemy::Type::Cactus].texture = Textures::EnemySheet;
	data[Enemy::Type::Cactus].textureRect = sf::IntRect(75, 338, 24, 80);
	data[Enemy::Type::Cactus].startingFrame = sf::IntRect(75, 338, 24, 80);
	data[Enemy::Type::Cactus].frames = 4;
	data[Enemy::Type::Cactus].frameSpeed = sf::seconds(1.0f);

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::Missile].damage = 1;
	data[Projectile::Missile].speed = 600.0f;
	data[Projectile::Missile].texture = Textures::EnemySheet;
	data[Projectile::Missile].textureRect = sf::IntRect(459, 227, 16, 16);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].textureRect = sf::IntRect(112, 34, 16, 16);
	data[Pickup::HealthRefill].action = [](Mario& a) { a.addHealth(); };

	data[Pickup::OneUp].texture = Textures::Entities;
	data[Pickup::OneUp].textureRect = sf::IntRect(132, 34, 16, 16);
	data[Pickup::OneUp].action = [](Mario& a) { a.addLife(); };

	data[Pickup::ShellRefill].texture = Textures::EnemySheet;
	data[Pickup::ShellRefill].textureRect = sf::IntRect(480, 226, 16, 16);
	data[Pickup::ShellRefill].action = [](Mario & a) { a.addMissile(); };

	data[Pickup::Invcibility].texture = Textures::Entities;
	data[Pickup::Invcibility].textureRect = sf::IntRect(93, 14, 15, 16);
	data[Pickup::Invcibility].action = [](Mario& a) { a.setInvincibility(8000); };

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Propellant].lifetime = sf::seconds(0.6f);
	
	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(4.f);

	return data;
}

std::vector<BlockData> initializeBlockData()
{
	std::vector<BlockData> data(Block::TypeCount);

	data[Block::Indestructible].action = [](Mario & a, const Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);
		handleCollisionDirection(a, b, direction);
	};
	data[Block::Indestructible].hitpoints = 100000;
	data[Block::Indestructible].texture = Textures::BlockSheet;
	data[Block::Indestructible].textureRect = sf::IntRect(0, 0, 32, 32);
	data[Block::Indestructible].isInvenciable = true;

	data[Block::Ground].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);
		handleCollisionDirection(a, b, direction);
	};
	data[Block::Ground].hitpoints = 100000;
	data[Block::Ground].texture = Textures::BlockSheet;
	data[Block::Ground].textureRect = sf::IntRect(96, 0, 32, 32);
	data[Block::Ground].isInvenciable = true;


	data[Block::SteelBrick].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);
		handleCollisionDirection(a, b, direction);
	};
	data[Block::SteelBrick].hitpoints = 10000;
	data[Block::SteelBrick].texture = Textures::BlockSheet;
	data[Block::SteelBrick].textureRect = sf::IntRect(128, 32, 32, 32);
	data[Block::SteelBrick].isInvenciable = true;

	data[Block::Wall].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);
		handleCollisionDirection(a, b, direction);
	};
	data[Block::Wall].hitpoints = 2;
	data[Block::Wall].texture = Textures::BlockSheet;
	data[Block::Wall].textureRect = sf::IntRect(0, 0, 32, 32);
	data[Block::Wall].isInvenciable = true;

	data[Block::Breakable].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);
		if (direction == CollisionDirection::Bottom)
		{
			if (!a.isFalling())
			{
				b.setHeadBut();
				b.applyDamage();
			}
		}
		else if (direction == CollisionDirection::TopRight || direction == CollisionDirection::TopLeft)
		{
			if (a.isJumping())
			{
				b.setHeadBut();
				b.applyDamage();
			}
		}
		handleCollisionDirection(a, b, direction);
	};
	data[Block::Breakable].hitpoints = 1;
	data[Block::Breakable].texture = Textures::BlockSheet;
	data[Block::Breakable].textureRect = sf::IntRect(288, 0, 32, 32);
	data[Block::Breakable].isInvenciable = false;

	data[Block::CoinBlock].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);

		if (direction == CollisionDirection::Bottom)
		{
			if (!a.isFalling())
			{
				a.addScore();
				b.setHeadBut();
				b.applyDamage();
			}
		}
		else if (direction == CollisionDirection::TopRight || direction == CollisionDirection::TopLeft)
		{
			std::cout << "triggered" << std::endl;
			if (!a.isFalling())
			{
				std::cout << "bottom" << std::endl;
				a.addScore();
				b.setHeadBut();
				b.applyDamage();
			}
		}
		handleCollisionDirection(a, b, direction);
	};
	data[Block::CoinBlock].hitpoints = 5;
	data[Block::CoinBlock].texture = Textures::BlockSheet;
	data[Block::CoinBlock].textureRect = sf::IntRect(288, 32, 32, 32);
	data[Block::CoinBlock].isInvenciable = false;

	data[Block::Mystery].action = [](Mario & a, Block & b)
	{
		auto direction = calculateCollisionDirection(a, b);

		if (direction == CollisionDirection::Bottom)
		{
			if (!a.isFalling())
			{
				b.setHeadBut();
				b.applyDamage();
			}
		}
		else if (direction == CollisionDirection::TopRight || direction == CollisionDirection::TopLeft)
		{
			if (a.isJumping())
			{
				b.setHeadBut();
				b.applyDamage();
			}
		}
		handleCollisionDirection(a, b, direction);
	};
	data[Block::Mystery].hitpoints = 1;
	data[Block::Mystery].texture = Textures::BlockSheet;
	data[Block::Mystery].textureRect = sf::IntRect(320, 32, 32, 32);
	data[Block::Mystery].isInvenciable = false;

	return data;
}

std::vector<CoinData> initializeCoinData()
{
	std::vector<CoinData> data(Coin::TypeCount);

	data[Coin::YellowCoin].texture = Textures::Entities;
	data[Coin::YellowCoin].textureRect = sf::IntRect(14, 14, 12, 16);
	data[Coin::YellowCoin].frames = 4;
	data[Coin::YellowCoin].frameSpeed = sf::seconds(.5f);
	data[Coin::YellowCoin].lifeTime = 1500;

	data[Coin::BlueCoin].texture = Textures::Entities;
	data[Coin::BlueCoin].textureRect = sf::IntRect(14, 34, 12, 16);
	data[Coin::BlueCoin].frames = 4;
	data[Coin::BlueCoin].frameSpeed = sf::seconds(.5f);
	data[Coin::BlueCoin].lifeTime = 1500;

	return data;
}




