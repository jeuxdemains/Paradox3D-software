#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Model.h"

#define MAX_OBJ_VTX_CNT 2000
#define MAX_OBJ_FACE_CNT 2000

void OBJ_LoadModel(char* fileName, Model_t* modelData);