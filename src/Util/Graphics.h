//
// Created by Lucas on 21-05-2024.
//

#pragma once

#include <vector>
#include "glad/glad.h"

GLuint CreateTextureFromRawPixels(std::vector<unsigned char>& pixels, int width, int height);