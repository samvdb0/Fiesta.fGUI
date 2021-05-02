#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <Psapi.h>
#include <set>
#include <string>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <vector>
#include <WinSock2.h>

#include "Data.h"
#include "Templates.h"
#include "Log.h"

#include <detours.h>
#define Detour(addr, target) DetourFunction((PBYTE) addr, (PBYTE) target)

#define PROJ_NAME "fGUI"
#define PROJ_VERSION "v1.0.0"
#define PROJ_COPYRIGHT "(c) saze 2021"
#endif //PCH_H