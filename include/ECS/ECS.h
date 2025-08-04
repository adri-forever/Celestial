#ifndef ECS_h
#define ECS_h

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class EntityManager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 64;
constexpr std::size_t maxGroups = 64;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	Entity* entity;

	virtual ~Component() {}

	virtual void init() {}
	virtual void update() {}
	virtual void render() {}

};

class Entity {
private:
	EntityManager& eManager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	Entity(EntityManager& iManager) : eManager(iManager) {}
	Entity(EntityManager& iManager, std::string itag) : eManager(iManager), tag(itag) {}
	~Entity() {};

	std::string tag;

	void update() {	for (auto& c : components) { c->update(); } }
	void render() { for (auto& c : components) { c->render(); } }

	bool isActive() const { return active; }
	void destroy() { active = false; }

	//Group handling
	bool hasGroup(Group iGroup) const { return groupBitSet[iGroup]; }
	void addGroup(Group iGroup);
	void deleteGroup(Group iGroup) { groupBitSet[iGroup] = false; }

	//Component handling
	template <typename T>
	bool hasComponent() const {	return componentBitSet[getComponentTypeID<T>()]; }

	template <typename T, typename... TArgs>
	T& addComponent(TArgs... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T>
	T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class EntityManager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void update() { for (auto& e : entities) { e->update(); } }
	void render() {	for (auto& e : entities) { e->render(); } }

	void refresh() {
		for (std::size_t i=0u; i < maxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
				[i](Entity* iEntity) {
					return !iEntity->isActive() || !iEntity->hasGroup(i);
				}),
				std::end(v)
			);
		}

		// std::vector<T>.erase method removes elements in vector based on iterator for start until end
		// std::begin gives pointer to start of array
		// std::end .
		// std::remove_if moves to the back of our vector pointers based on lamba function return value (argument 3, true means push back)
		// putting the pointer to the end essentially lets us forget it to write over later if necessary
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& iEntity) {
				return !iEntity->isActive();
			}),
			std::end(entities)
		);
	}

	void addToGroup(Entity* iEntity, Group iGroup) {
		groupedEntities[iGroup].emplace_back(iEntity);
	}

	std::vector<Entity*>& getGroup(Group iGroup) {
		return groupedEntities[iGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

	void importEntities(std::string path);
};

#endif