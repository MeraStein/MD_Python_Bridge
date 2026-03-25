#pragma once

#ifndef MD_RUNNER_H
#define MD_RUNNER_H

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h> 

__declspec(dllimport) void RunPythonScript(const char* scriptPath);

#endif
