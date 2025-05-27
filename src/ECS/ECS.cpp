#include "ECS/ECS.h"

// #include <fstream>

//Cannot be defined in header because of forward declaration
void Entity::addGroup(Group iGroup) {
	groupBitSet[iGroup] = true;
	eManager.addToGroup(this, iGroup);
}

void EntityManager::importEntities(std::string path) {
	/*std::fstream file;
	file.open(path, std::ios::in);

	file.close()*/
	//xml? json? yaml?
	//plain CSV?
	//after all, the data we need for each object is x0, v0, m, r(?), name
}