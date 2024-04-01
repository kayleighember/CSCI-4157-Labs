#include "ObjectManager.h"

void ObjectManager::Update(double elapsedSeconds) {
	for (const std::pair<const std::string, std::shared_ptr<GraphicsObject>>& item : objectMap) {
		const auto& object = item.second;
		object->Update(elapsedSeconds);
	}
}

