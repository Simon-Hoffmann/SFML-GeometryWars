#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{
    //EntitieToAdd inserted in entities and entityMap
    for (auto entity : m_entitiesToAdd) {
        m_entities.push_back(entity);
        m_entityMap[entity->m_tag].push_back(entity);
    }

    removeDeadEntities(m_entities);
    for (auto& [tag,entityVec] : m_entityMap) {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    //TODO: remove all dead entities from the input vector
    //      this is called by the update function
    //iterator invalidation safety!!
}

ptr<Entity> EntityManager::addEntity(const EntityTag& tag)
{
    auto entity = ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    return entity;
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const EntityTag& tag)
{
    //TODO: this is incorrect, return the correct vector from the map
    return m_entities;
}

