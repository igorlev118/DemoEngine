#pragma once

#include <cctype>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#define WIN32_LEAD_AND_MEAN
#include <windows.h>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Common/Debug.hpp"
#include "Common/NonCopyable.hpp"
#include "Common/ScopeGuard.hpp"
#include "Common/Utility.hpp"
#include "Logger/Logger.hpp"
