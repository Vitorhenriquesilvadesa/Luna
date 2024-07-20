#include <stdio.h>
#include <string.h>

#include "lmemory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include "table.h"

#define ALLOCATE_OBJ(type, objectType) \
	(type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type)
{
	Obj* object = (Obj*)reallocate(NULL, 0, size);
	object->type = type;
	object->next = vm.objects;
	vm.objects = object;
	return object;
}

ObjFunction* newFunction()
{
	ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
	function->arity = 0;
	function->name = NULL;
	initChunk(&function->chunk);
	return function;
}

ObjNative* newNative(NativeFn function, uint8_t expectedArgCount)
{
	ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
	native->function = function;
	native->arity = expectedArgCount;
	return native;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash)
{
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	string->length = length;
	string->characters = chars;
	string->hash = hash;
	tableSet(&vm.strings, string, NULL_VAL);
	return string;
}

static uint32_t hashString(const char* key, int length)
{
	uint32_t hash = 2166136261u;

	for (int i = 0; i < length; i++)
	{
		hash ^= (uint8_t)key[i];
		hash *= 16777619;
	}

	return hash;
}

ObjString* takeString(char* characters, int length)
{
	uint32_t hash = hashString(characters, length);
	ObjString* intern = tableFindString(&vm.strings, characters, length, hash);

	if (intern != NULL)
	{
		FREE_ARRAY(char, characters, length + 1);
		return intern;
	}

	return allocateString(characters, length, hash);
}

ObjString* copyString(const char* characters, int length)
{
	uint32_t hash = hashString(characters, length);
	ObjString* intern = tableFindString(&vm.strings, characters, length, hash);

	if (intern != NULL) return intern;

	char* heapChars = ALLOCATE(char, length + 1);
	memcpy(heapChars, characters, length);
	heapChars[length] = '\0';


	return allocateString(heapChars, length, hash);
}

static void printFunction(ObjFunction* function)
{
	if (function->name == NULL)
	{
		printf("<script>");
		return;
	}

	printf("<fn %s>", function->name->characters);
}

void printObject(Value value)
{
	switch (OBJ_TYPE(value))
	{
	case OBJ_STRING:
		printf("%s", AS_CSTRING(value));
		break;

	case OBJ_FUNCTION:
		printFunction(AS_FUNCTION(value));
		break;

	case OBJ_NATIVE:
		printf("<native fn>");
		break;
	}
}
