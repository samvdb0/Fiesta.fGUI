#pragma once

#define CFG_PATH_DEFAULT "./resutil/configs/conf.default.ini"
#define CFG_PATH_ONE "./resutil/configs/conf.1.ini"
#define CFG_PATH_TWO "./resutil/configs/conf.2.ini"
#define CFG_PATH_THREE "./resutil/configs/conf.3.ini"

class Config
{
public:
	static void Load(const char* fileName);
	static void Save(const char* fileName);
	static void GenerateDefault(const char* fileName);
};

