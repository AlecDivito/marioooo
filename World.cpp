#include "World.h"
#include "Foreach.h"
#include "ParticleNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, Player::PlayableLevel level)
	: _target(outputTarget)
	, _sceneTexture()
	, _worldView(outputTarget.getDefaultView())
	, _textures()
	, _fonts(fonts)
	, _sceneGraph()
	, _sceneLayers()
	, _worldBounds(0.f, 0.f, 10000.0f, _worldView.getSize().y)
	, _spawnPosition(_worldView.getSize().x / 2.f, _worldView.getSize().y / 2.f)
	, _playerMario(nullptr)
	, _scrollSpeed(1.0f)
	, _blockSpawnPoints()
	, _enemySpawnPoints()
	, _currentLevel(level)
{
	_sceneTexture.create(_target.getSize().x, _target.getSize().y);

	loadTextures();
	buildScene();

	// prepare the view
	_worldView.setCenter(_spawnPosition);
}

void World::update(sf::Time dt)
{
	if (_playerMario->getPosition().x >= _worldView.getCenter().x)
	{
		_worldView.move(_scrollSpeed * dt.asSeconds() * _playerMario->getVelocity().x, 0.0f);
	}
	if (_playerMario->isDead() && !_playerMario->isMarkedForRemoval())
	{
		// reset mario
		resetMario();
	}
	if (_playerMario->getPosition().y > _worldView.getSize().y + 200 && !_playerMario->isMarkedForRemoval())
	{
		// reset mario
		if (_playerMario->getHitpoints() == 2)
		{
			_playerMario->hit();
			_playerMario->hit();
		}
		else
		{
			_playerMario->hit();
		}
	}
	if (!_playerMario->isGrounded())
	{
		if (_playerMario->isCrouched())
		{
			_playerMario->setVelocity(_playerMario->getVelocity().x * 0.96f, 0.0);
		}
		else
		{
			_playerMario->setVelocity(0.0f, _playerMario->getVelocity().y);
		}
	}
	else
	{
		_playerMario->setVelocity(0.0f, 0.0f);
	}

	if (!_playerMario->isMarkedForRemoval())
	{
		_sceneGraph.removeWrecks();
	}
	destroyEntitiesOutsideView();
	handleCollisions();

	while (!_commandQueue.isEmpty())
	{
		_sceneGraph.onCommand(_commandQueue.pop(), dt);
	}

	spawnEntities();

	_sceneGraph.update(dt, _commandQueue);
	adaptPlayerPosition();
}

void World::draw()
{
	//if (PostEffect::isSupported())
	{
	//	_sceneTexture.clear();
	//	_sceneTexture.setView(_worldView);
	//	_sceneTexture.draw(_sceneGraph);
	//	_sceneTexture.display();
	//	_bloomEffect.apply(_sceneTexture, _target);
	}
	// else
	{
		_target.setView(_worldView);
		_target.draw(_sceneGraph);
	}
}

CommandQueue & World::getCommandQueue()
{
	return _commandQueue;
}

bool World::hasAlivePlayer() const
{
	if (_playerMario == nullptr)
	{
		return false;
	}
 	else if (!_playerMario->isMarkedForRemoval())
	{
		return true;
	}
	return false;
}

bool World::hasPlayerReachedEnd() const
{
	return _worldBounds.width < _playerMario->getPosition().x;
}

int World::getPlayerScore() const
{
	return _playerMario->getScore();
}

void World::loadTextures()
{
	if (_currentLevel == Player::PlayableLevel::Level4)
		_textures.load(Textures::Background, "Media/Textures/Backgrounds3.0.png");
	else if (_currentLevel == Player::PlayableLevel::Level3)
		_textures.load(Textures::Background, "Media/Textures/Backgrounds4.0.png");
	else if (_currentLevel == Player::PlayableLevel::Level2)
		_textures.load(Textures::Background, "Media/Textures/Backgrounds5.0.png");
	else if (_currentLevel == Player::PlayableLevel::Level1)
		_textures.load(Textures::Background, "Media/Textures/Backgrounds6.0.png");
	else
		_textures.load(Textures::Background, "Media/Textures/Backgrounds3.0.png");
	_textures.load(Textures::EnemySheet, "Media/Textures/EnemySPriteSheet.png");
	_textures.load(Textures::PlayerSheet, "Media/Textures/PlayerSheet.png");
	_textures.load(Textures::BlockSheet, "Media/Textures/BlockspriteSheet.png");
	_textures.load(Textures::Explosion, "Media/Textures/Explosion.png");
	_textures.load(Textures::Particle, "Media/Textures/Particle.png");
	_textures.load(Textures::Entities, "Media/Textures/objects.png");
	_textures.load(Textures::FinishLine, "Media/Textures/FinishLine.png");
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = _playerMario->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	// position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	_playerMario->setPosition(position);
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Block | Category::Enemy;
	command.action = derivedAction<Entity>([this](Entity & e, sf::Time)
	{
		if (getBattlefieldBounds().left > e.getBoundingRect().left + e.getBoundingRect().width)
		{
			e.remove();
		}
	});
	_commandQueue.push(command);
}

void World::spawnEntities()
{
	auto leftMost = getBattlefieldBounds().left + getBattlefieldBounds().width;
	while (!_blockSpawnPoints.empty() && _blockSpawnPoints.back().rect.left < leftMost)
	{
		BlockSpawnPoint spawn = _blockSpawnPoints.back();

		std::unique_ptr<Block> block = std::make_unique<Block>(spawn.type, spawn.rect, _textures, _fonts);
		_sceneLayers[Forground]->attachChild(std::move(block));
		_blockSpawnPoints.pop_back();
	}

	while (!_enemySpawnPoints.empty() && _enemySpawnPoints.back().rect.left < leftMost)
	{
		EnemySpawnPoint spawn = _enemySpawnPoints.back();

		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(spawn.type, _textures);
		enemy->setScale(spawn.scaleX, spawn.scaleY);
		enemy->setPosition((float)spawn.rect.left, (float)spawn.rect.top);
		_sceneLayers[Forground]->attachChild(std::move(enemy));
		_enemySpawnPoints.pop_back();
	}
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::Mario, Category::Block))
		{
			auto& player = static_cast<Mario &>(*pair.first);
			auto& block = static_cast<Block &>(*pair.second);

			block.apply(player);
		}
		else if (matchesCategories(pair, Category::Mario, Category::Enemy))
		{
			auto& player = static_cast<Mario &>(*pair.first);
			auto& enemy = static_cast<Enemy &>(*pair.second);

			if (!player.isInvincible())
			{
				player.hit();
				player.setInvincibility(1000);
			}
		}
		else if (matchesCategories(pair, Category::Mario, Category::Pickup))
		{
			auto& player = static_cast<Mario &>(*pair.first);
			auto& pickup = static_cast<Pickup &>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
		}
		else if (matchesCategories(pair, Category::Projectile, Category::Enemy))
		{
			auto& projectile = static_cast<Projectile &>(*pair.first);
			auto& enemy = static_cast<Enemy &>(*pair.second);

			enemy.damage(projectile.getDamage());
			projectile.destroy();
		}
		else if (matchesCategories(pair, Category::Projectile, Category::Block))
		{
			auto & projectile = static_cast<Projectile &>(*pair.first);
			auto& block = static_cast<Block &>(*pair.second);
		
			projectile.destroy();
			block.damage(projectile.getDamage());
		}
		else if (matchesCategories(pair, Category::Enemy, Category::Block))
		{
			auto & enemy = static_cast<Enemy &>(*pair.first);
			auto& block = static_cast<Block &>(*pair.second);

			if (block.getType() == Block::Type::Ground)
			{
				auto yDiffrence = enemy.getBoundingRect().top + enemy.getBoundingRect().height - block.getBoundingRect().top;
				enemy.ground(yDiffrence);
			}
			else if (block.getType() == Block::Type::Wall)
			{
				enemy.switchDirection();
			}
		}
		else if (matchesCategories(pair, Category::Pickup, Category::Block))
		{
			auto & enemy = static_cast<Enemy &>(*pair.first);
			auto& block = static_cast<Block &>(*pair.second);

			if (block.getType() == Block::Type::Ground)
			{
				auto yDiffrence = enemy.getBoundingRect().top + enemy.getBoundingRect().height - block.getBoundingRect().top;
				enemy.ground(yDiffrence);
			}
			else if (block.getType() == Block::Type::Wall)
			{
				enemy.switchDirection();
			}
		}
	}
}

void World::buildScene()
{
	// Initialize the diffrent layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == Forground) ? Category::SceneLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		_sceneLayers[i] = layer.get();
		_sceneGraph.attachChild(std::move(layer));
	}

	// prepare the tiled background
	sf::Texture & worldTexture = _textures.get(Textures::Background);
	worldTexture.setRepeated(true);

	float viewWidth = _worldView.getSize().y;
	sf::IntRect textureRect(_worldBounds);
	textureRect.width += static_cast<int>(viewWidth);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(worldTexture, textureRect));
	backgroundSprite->setPosition(_worldBounds.left - viewWidth, _worldBounds.top);
	backgroundSprite->setScale(_worldView.getSize().x / worldTexture.getSize().x, _worldView.getSize().y / worldTexture.getSize().y);
	_sceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add the finish line to the scene
	// adding it in right now to judge stuff
	sf::Texture & finishTexture = _textures.get(Textures::FinishLine);
	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
	finishSprite->setPosition(_worldBounds.width, 0);
	_sceneLayers[Forground]->attachChild(std::move(finishSprite));

	// Add enemys
	{
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(960, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(1100, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Cactus, sf::IntRect(700, 500, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Cactus, sf::IntRect(1299, 500, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Cactus, sf::IntRect(1899, 500, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Cactus, sf::IntRect(3245, 500, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Cactus, sf::IntRect(3899, 500,  0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(1599, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(2099, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(2250, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(2250, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(2750, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(3400, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(3900, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Turtle, sf::IntRect(4300,  575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(1300, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(1650, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(2250, 575, 0, 0), 4.0, 4.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(2900, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(3300, 575, 0, 0), 2.0, 2.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(3700, 575, 0, 0), 7.0, 7.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(4100, 575, 0, 0), 3.0, 3.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(4250, 575, 0, 0), 3.0, 3.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(4500, 575, 0, 0), 3.0, 3.0));
		_enemySpawnPoints.push_back(EnemySpawnPoint(Enemy::Type::Goomba, sf::IntRect(4750, 575, 0, 0), 3.0, 3.0));
	}

	// Add partilce node to the scene
	// std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
	// mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));

	// Add in Ground block
	{
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Ground, sf::IntRect(0, 600, 32 * 150, 32 * 3)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Ground, sf::IntRect(5400, 600, 32 * 32, 32 * 3)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Ground, sf::IntRect(6400, 600, 32 * 32, 32 * 3)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Ground, sf::IntRect(7000, 600, 32 * 128, 32 * 3)));
	}

	// Add walls
	{
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(500, 0, 32 * 3, 32 * 17)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(1000, 600 - 64, 32, 32 * 3)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(800, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(1000, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(1750, 578, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(1500, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(2000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(2300, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(2700, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(3000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(3200, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(3500, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(4000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(4200, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(4500, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(4750, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(5000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(6000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(8000, 310, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(8500, 568, 32, 32 * 8)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7500, 568, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7532, 536, 32, 32 * 2)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7564, 504, 32, 32 * 3)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7596, 488, 32, 32 * 4)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(7628, 456, 32, 32 * 5)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Wall, sf::IntRect(8000, 568, 32, 32)));
	}

	// blocks
	{
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::CoinBlock, sf::IntRect(168, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::CoinBlock, sf::IntRect(200, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(765, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Breakable, sf::IntRect(264, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Breakable, sf::IntRect(296, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(1354, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(1456, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(1579, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(1678, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(358, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(390, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(422, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(454, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(486, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(168, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(200, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(232, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(264, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(296, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(326, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(358, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(390, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(422, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(454, 200, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(486, 200, 32, 32)));

		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(2500, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(3200, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(4200, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(4750, 400, 32, 32)));
		_blockSpawnPoints.push_back(BlockSpawnPoint(Block::Type::Mystery, sf::IntRect(5200, 400, 32, 32)));

	}

	std::sort(_blockSpawnPoints.begin(), _blockSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.rect.left > rhs.rect.left;
	});

	std::sort(_enemySpawnPoints.begin(), _enemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.rect.left > rhs.rect.left;
	});

	// Add players mario
	std::unique_ptr<Mario> player(new Mario(Mario::MarioPlayerBig, _textures, _fonts));
	_playerMario = player.get();
	_playerMario->setPosition(50.0f, 60.0f);
	_sceneLayers[FrontGround]->attachChild(std::move(player));
}

void World::resetMario()
{
	_playerMario->setPosition(50.0f, 60.0f);
	_playerMario->addHealth();
	_playerMario->addHealth();
	_playerMario->removeLife();
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

