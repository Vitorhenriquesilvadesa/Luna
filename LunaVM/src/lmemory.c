#include "compiler.h"
#include "lmemory.h"
#include "vm.h"
#include <stdlib.h>

#ifdef DEBUG_LOG_GC
#include <stdio.h>
#include "debug.h"
#endif

#define GC_HEAP_GROW_FACTOR 2

static void freeObject(Obj* object);

void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    vm.bytesAllocated += newSize - oldSize;

    if (newSize > oldSize)
    {
#ifdef DEBUG_LOG_GC
		printf("Allocating %d bytes of memory.\n", newSize);
#endif

#ifdef DEBUG_STRESS_GC
		collectGarbage();
#endif
        if (vm.bytesAllocated > vm.nextGC)
        {
            collectGarbage();
        }
    }

    if (newSize == 0)
    {
        if (pointer != NULL)
        {
            free(pointer);
            return NULL;
        }
    }

    void* result = realloc(pointer, newSize);

    if (result == NULL) exit(1);

    return result;
}

static void markRoots()
{
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++)
    {
        markValue(*slot);
    }

    for (int i = 0; i < vm.frameCount; i++)
    {
        markObject((Obj*)vm.frames[i].closure);
    }

    for (ObjUpvalue* upvalue = vm.openUpvalues; upvalue != NULL; upvalue = upvalue->next)
    {
        markObject((Obj*)upvalue);
    }

    markTable(&vm.globals);
    markCompilerRoots();
    markObject((Obj*)vm.initString);
}

static void markArray(ValueArray* array)
{
    for (int i = 0; i < array->count; i++)
    {
        markValue(array->values[i]);
    }
}

static void blanckenObject(Obj* object)
{
#ifdef DEBUG_LOG_GC
	printf("%p blacken ", (void*)object);
	printValue(OBJ_VAL(object));
	printf("\n");
#endif

    switch (object->type)
    {
    case OBJ_BOUND_METHOD:
        {
            ObjBoundMethod* bound = (ObjBoundMethod*)object;
            markValue(bound->receiver);
            markObject((Obj*)bound->method);
            break;
        }
    case OBJ_STRUCT:
        {
            ObjStruct* klass = (ObjStruct*)object;
            markObject((Obj*)klass->name);
            markTable(&klass->methods);
            break;
        }

    case OBJ_INSTANCE:
        {
            ObjInstance* instance = (ObjInstance*)object;
            markObject((Obj*)instance->klass);
            markTable(&instance->fields);
            break;
        }

    case OBJ_CLOSURE:
        {
            ObjClosure* closure = (ObjClosure*)object;
            markObject((Obj*)closure->function);

            for (int i = 0; i < closure->upvalueCount; i++)
            {
                markObject((Obj*)closure->upvalues[i]);
            }
            break;
        }

    case OBJ_FUNCTION:
        {
            ObjFunction* function = (ObjFunction*)object;
            markObject((Obj*)function->name);
            markArray(&function->chunk.constants);
            break;
        }

    case OBJ_UPVALUE:
        {
            markValue(((ObjUpvalue*)object)->closed);
            break;
        }

    case OBJ_NATIVE:
    case OBJ_STRING:
        break;
    }
}

static void traceReferences()
{
    while (vm.grayCount > 0)
    {
        Obj* object = vm.grayStack[--vm.grayCount];
        blanckenObject(object);
    }
}

static void sweep()
{
    Obj* previous = NULL;
    Obj* object = vm.objects;

    while (object != NULL)
    {
        if (object->isMarked)
        {
            object->isMarked = false;
            previous = object;
            object = object->next;
        }
        else
        {
            Obj* unreached = object;
            object = object->next;

            if (previous != NULL)
            {
                previous->next = object;
            }
            else
            {
                vm.objects = object;
            }

            freeObject(unreached);
        }
    }
}

void collectGarbage()
{
#ifdef DEBUG_LOG_GC
	printf("--gc begin \n");
	size_t before = vm.bytesAllocated;
#endif

    markRoots();
    traceReferences();
    tableRemoveWhite(&vm.strings);
    sweep();

    vm.nextGC = vm.bytesAllocated * GC_HEAP_GROW_FACTOR;

#ifdef DEBUG_LOG_GC
	printf("--gc end \n");
	printf("	now with %zu bytes. Collected %zu bytes (from %zu to %zu) next at %zu\n", 
		vm.bytesAllocated,
		before - vm.bytesAllocated, 
		before,
		vm.bytesAllocated,
		vm.nextGC
	);
#endif
}

void markValue(Value value)
{
    if (IS_OBJ(value)) markObject(AS_OBJ(value));
}

void markObject(Obj* object)
{
    if (object == NULL) return;
    if (object->isMarked) return;

#ifdef DEBUG_LOG_GC
	printf("%p mark ", (void*)object);
	printValue(OBJ_VAL(object));
	printf("\n");
#endif

    object->isMarked = true;

    if (vm.grayCapacity < vm.grayCount + 1)
    {
        vm.grayCapacity = GROW_CAPACITY(vm.grayCapacity);
        vm.grayStack = (Obj**)realloc(vm.grayStack, sizeof(Obj*) * vm.grayCapacity);

        if (vm.grayStack == NULL) exit(1);
    }

    vm.grayStack[vm.grayCount++] = object;
}

static void freeObject(Obj* object)
{
#ifdef DEBUG_LOG_GC
	printf("%p free type %d\n", (void*)object, object->type);
#endif

    switch (object->type)
    {
    case OBJ_BOUND_METHOD:
        {
            FREE(ObjBoundMethod, object);
            break;
        }

    case OBJ_STRUCT:
        {
            ObjStruct* klass = (ObjStruct*)object;
            freeTable(&klass->methods);
            FREE(ObjStruct, object);
            break;
        }

    case OBJ_STRING:
        {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->characters, string->length + 1);
            FREE(ObjString, object);
            break;
        }

    case OBJ_UPVALUE:
        {
            FREE(ObjUpvalue, object);
            break;
        }

    case OBJ_CLOSURE:
        {
            ObjClosure* closure = (ObjClosure*)object;
            FREE_ARRAY(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, object);
            break;
        }

    case OBJ_FUNCTION:
        {
            ObjFunction* function = (ObjFunction*)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }

    case OBJ_INSTANCE:
        {
            ObjInstance* instance = (ObjInstance*)object;
            freeTable(&instance->fields);
            FREE(ObjInstance, object);
            break;
        }

    case OBJ_NATIVE:
        {
            FREE(ObjNative, object);
            break;
        }
    }
}

void freeObjects()
{
    Obj* object = vm.objects;

    while (object != NULL)
    {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }

    free(vm.grayStack);
}
