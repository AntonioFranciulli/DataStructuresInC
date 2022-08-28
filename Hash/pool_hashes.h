// Fuente utilizada para obtener las funciones de hashing: 
// https://www.partow.net/programming/hashfunctions/#AvailableHashFunctions

unsigned int RSHash(const char* str, unsigned int length);
unsigned int PJWHash(const char* str, unsigned int length);
unsigned int SDBMHash(const char* str, unsigned int length);
unsigned int DJBHash(const char* str, unsigned int length);
unsigned int DEKHash(const char* str, unsigned int length);
unsigned int APHash(const char* str, unsigned int length);