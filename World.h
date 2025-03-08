#pragma once


#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Mario.h"
#include "CommandQueue.h"
#include "Command.h"
// #include "BloomEffect.h"
#include "Block.h"
#include "Enemy.h"
#include "Player.h"

#include <SFML/Graphics/RenderTexture.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

namespace sf
{
	class RenderTarget;
}

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderTarget & outputTarget, FontHolder & fonts, Player::PlayableLevel level);
	void update(sf::Time dt);
	void draw();

	CommandQueue & getCommandQueue();

	bool hasAlivePlayer() const;
	bool hasPlayerReachedEnd() const;
	int getPlayerScore() const;

private:
	void loadTextures();
	void adaptPlayerPosition();
	void handleCollisions();
	void destroyEntitiesOutsideView();
	void spawnEntities();


	void buildScene();
	void resetMario();
	sf::FloatRect getViewBounds() const;
	sf::FloatRect getBattlefieldBounds() const;

private:
	enum Layer
	{
		Background,
		Forground,
		FrontGround,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint(sf::IntRect rect, float x = 1, float y = 1)
			: rect(rect)
			, scaleX(x)
			, scaleY(y)
		{
		}

		sf::IntRect rect;
		float scaleX;
		float scaleY;
	};

	struct EnemySpawnPoint : public SpawnPoint
	{
		EnemySpawnPoint(Enemy::Type type, sf::IntRect rect, float x = 1, float y = 1)
			: SpawnPoint(rect, x, y)
			, type(type)
			
		{
		}
		
		Enemy::Type type;
	};

	struct BlockSpawnPoint : public SpawnPoint
	{
		BlockSpawnPoint(Block::Type type, sf::IntRect rect, float x = 1, float y = 1)
			: SpawnPoint(rect, x, y)
			, type(type)

		{
		}

		Block::Type type;
	};

private:
	sf::RenderTarget & _target;
	sf::RenderTexture _sceneTexture;
	sf::View _worldView;
	TextureHolder _textures;
	FontHolder & _fonts;

	SceneNode _sceneGraph;
	std::array<SceneNode *, LayerCount> _sceneLayers;
	CommandQueue _commandQueue;
	float _scrollSpeed;

	sf::FloatRect _worldBounds;
	sf::Vector2f _spawnPosition;
	Mario * _playerMario;

	std::vector<BlockSpawnPoint> _blockSpawnPoints;
	std::vector<EnemySpawnPoint> _enemySpawnPoints;
	Player::PlayableLevel _currentLevel;
};

