#pragma once

#ifndef PCH_H
#define PCH_H
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <optional>
#include <thread>
#include <Windows.h>

using std::string, std::cout, std::endl, std::optional, std::nullopt;

#include <logger.h>
#include "SourceSDK/Defenitions.h"
#include "Utils/Utils.h"
#include "Utils/matrix.h"
#include "Utils/vector.h"

#endif
