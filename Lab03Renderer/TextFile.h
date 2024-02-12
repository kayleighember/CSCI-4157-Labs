#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "BaseObject.h"

class TextFile : public BaseObject
{
private:
	
public:
	TextFile();
	~TextFile() = default;
	std::string data;
	void Trim(std::string& str);
	void TextToStringStream(std::string filePath);
};

