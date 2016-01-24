#pragma once

#include <assert.h>
#include <Windows.h>
#include <stdio.h>

static char buff[256];  // Needs a buffer - big enough
	// Magic macro
#define out(A,...) sprintf_s(buff,A,##__VA_ARGS__); OutputDebugString(buff);

#define UNUSED(x) x

#define GAME_WIDTH 800
#define GAME_HEIGHT 600