#ifndef luna_object_h
#define luna_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString*) AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*) AS_OBJ(value))->characters)

typedef enum {
	OBJ_STRING
} ObjType;

struct Obj
{
	ObjType type;
	struct Obj* next;
};

struct ObjString
{
	Obj obj;
	int length;
	char* characters;
	uint32_t hash;
};

ObjString* takeString(char* characters, int length);
ObjString* copyString(const char* characters, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type)
{
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif