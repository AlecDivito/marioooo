#ifndef BOOK_CATEGORY_HPP
#define BOOK_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None = 0,
		SceneLayer = 1 << 0,
		Player = 1 << 1,
		Mario = 1 << 2,
		Enemy = 1 << 3,
		Pickup = 1 << 4,
		PlayerProjectile = 1 << 5,
		EnemyProjectile = 1 << 6,
		ParticleSystem = 1 << 7,
		Block = 1 << 8,
		Coin = 1 << 9,

		Entity = Mario | Enemy,
		Projectile = PlayerProjectile | EnemyProjectile,
	};
}

#endif // BOOK_CATEGORY_HPP
