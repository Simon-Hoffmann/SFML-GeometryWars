#pragma once
#include <string>

#include "Common.h"
#include "Components.h"

enum class EntityTag {
	player,
	enemy,
	smallEnemy,
	bullet
};

class Entity
{
private:
	friend class EntityManager;
	bool m_active = true;
	size_t m_id = 0;
	EntityTag m_tag = EntityTag::enemy;

	//constructor and destructor
	Entity(const size_t id, const EntityTag& tag);

public:

	//component pointers
	ptr<CTransform> cTransform;
	ptr<CShape> cShape;
	ptr<CCollision> cCollision;
	ptr<CInput> cInput;
	ptr<CScore> cScore;
	ptr<CLifespan> cLifespan;
	
	//private member access functions
	bool isActive() const;
	const EntityTag& tag() const;
	const size_t id() const;
	void destroy();
};

