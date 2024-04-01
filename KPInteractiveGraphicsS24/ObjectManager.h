#pragma once
#include "GraphicsObject.h"
#include <iostream>
#include <unordered_map>

class ObjectManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<GraphicsObject>> objectMap;
public:
	inline std::shared_ptr<GraphicsObject> GetObject(const std::string& name) { return objectMap[name]; }
	inline void SetObject(std::string name, std::shared_ptr<GraphicsObject> object) { objectMap[name] = object; }
	void Update(double elapsedSeconds);
};

