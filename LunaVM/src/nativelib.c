#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#ifndef luna_native_lib
#define luna_native_lib

#include "nativelib.h"
#include "object.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Value clockNative(int argCount, Value* args)
{
	return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

Value inputNative(int argCount, Value* args)
{
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        // Remove o caractere de nova linha se estiver presente
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        // Calcula o comprimento da string sem o caractere de nova linha
        len = strlen(buffer);

        // Aloca memória para a string final
        char* heapBuffer = (char*)malloc(len + 1);
        if (heapBuffer == NULL) {
            return NULL_VAL;
        }

        // Copia a string do buffer para o heap
        strcpy(heapBuffer, buffer);

        // Cria o objeto string e retorna
        ObjString* string = takeString(heapBuffer, (int)len);
        if (string == NULL) {
            free(heapBuffer);
            return NULL_VAL;
        }

        return OBJ_VAL(string);
    }

    return NULL_VAL;
}



Value openNative(int argCount, Value* args)
{
    if (!IS_STRING(args[0])) {
        return NULL_VAL;
    }

    const char* path = AS_CSTRING(args[0]);

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return NULL_VAL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize + 1);

    if (buffer == NULL) {
        fclose(file);
        return NULL_VAL;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != (size_t)fileSize) {
        free(buffer);
        fclose(file);
        return NULL_VAL;
    }

    buffer[fileSize] = '\0';

    fclose(file);

    ObjString* string = takeString(buffer, fileSize);
    free(buffer);

    return OBJ_VAL(string);
}

Value stringLengthNative(int argCount, Value* args) {
    if (argCount != 1 || !IS_STRING(args[0])) {
        return NULL_VAL;
    }

    ObjString* string = AS_STRING(args[0]);
    return NUMBER_VAL(string->length);
}

Value toNumberNative(int argCount, Value* args)
{
    if (argCount != 1 || !IS_STRING(args[0])) {
        return NULL_VAL;
    }

    ObjString* string = AS_STRING(args[0]);
    const char* str = string->characters;


    char* end;
    double number = strtod(str, &end);


    if (end == str) {

        return NULL_VAL;
    }

    return NUMBER_VAL(number);
}

Value cosNative(int argCount, Value* args)
{
    if(!IS_NUMBER(args[0])) return NULL_VAL;
    
    return NUMBER_VAL(cos(AS_NUMBER(args[0])));
}
Value sinNative(int argCount, Value* args)
{
    if(!IS_NUMBER(args[0])) return NULL_VAL;

    return NUMBER_VAL(sin(AS_NUMBER(args[0])));
}
Value tanNative(int argCount, Value* args)
{
    if(!IS_NUMBER(args[0])) return NULL_VAL;

    return NUMBER_VAL(tan(AS_NUMBER(args[0])));
}
Value powNative(int argCount, Value* args)
{
    if(!IS_NUMBER(args[0]) || !IS_NUMBER(args[1])) return NULL_VAL;

    return NUMBER_VAL(pow(AS_NUMBER(args[0]), AS_NUMBER(args[1])));
}
Value sqrtNative(int argCount, Value* args)
{
    if(!IS_NUMBER(args[0])) return NULL_VAL;

    return NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
}

Value charAtNative(int argCount, Value* args)
{
    if (!IS_STRING(args[0]) || !IS_NUMBER(args[1])) {
        return NULL_VAL;
    }

    ObjString* str = AS_STRING(args[0]);
    int index = (int)AS_NUMBER(args[1]);

    if (index < 0 || index >= str->length) {
        return NULL_VAL;
    }

    char* charBuffer = (char*)malloc(2);
    if (charBuffer == NULL) {
        return NULL_VAL;
    }

    charBuffer[0] = str->characters[index];
    charBuffer[1] = '\0';

    ObjString* resultString = takeString(charBuffer, 1);
    if (resultString == NULL) {
        free(charBuffer);
        return NULL_VAL;
    }

    return OBJ_VAL(resultString);
}

Value substrNative(int argCount, Value* args)
{
    if (!IS_STRING(args[0]) || !IS_NUMBER(args[1]) || !IS_NUMBER(args[2])) {
        return NULL_VAL;
    }

    ObjString* str = AS_STRING(args[0]);
    int start = (int)AS_NUMBER(args[1]);
    int end = (int)AS_NUMBER(args[2]);

    if (start < 0 || start >= (int)str->length || end < start || end > (int)str->length) {
        return NULL_VAL;
    }

    int length = end - start;
    char* substrBuffer = (char*)malloc(length + 1);
    if (substrBuffer == NULL) {
        return NULL_VAL;
    }

    memcpy(substrBuffer, str->characters + start, length);
    substrBuffer[length] = '\0';

    ObjString* resultString = takeString(substrBuffer, length);
    free(substrBuffer);

    if (resultString == NULL) {
        return NULL_VAL;
    }

    return OBJ_VAL(resultString);
}

Value writeNative(int argCount, Value* args) {
    if (argCount != 2 || !IS_STRING(args[0]) || !IS_STRING(args[1])) {
        return NULL_VAL;
    }

    const char* path = AS_CSTRING(args[0]);
    const char* content = AS_CSTRING(args[1]);

    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        return BOOL_VAL(false);
    }

    size_t contentLength = strlen(content);
    size_t bytesWritten = fwrite(content, 1, contentLength, file);

    fclose(file);

    if (bytesWritten != contentLength) {
        return BOOL_VAL(false);
    }

    return BOOL_VAL(true);
}




#endif
