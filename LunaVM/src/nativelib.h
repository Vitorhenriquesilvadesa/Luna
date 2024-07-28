#pragma once

#include "value.h"

Value clockNative(int argCount, Value* args);
Value inputNative(int argCount, Value* args);
Value openNative(int argCount, Value* args);
Value stringLengthNative(int argCount, Value* args);
Value toNumberNative(int argCount, Value* args);
Value cosNative(int argCount, Value* args);
Value sinNative(int argCount, Value* args);
Value tanNative(int argCount, Value* args);
Value powNative(int argCount, Value* args);
Value sqrtNative(int argCount, Value* args);
Value charAtNative(int argCount, Value* args);
Value substrNative(int argCount, Value* args);
Value writeNative(int argCount, Value* args);