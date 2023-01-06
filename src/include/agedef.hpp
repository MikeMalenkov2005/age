#ifndef AGEDEF_HPP
#define AGEDEF_HPP

#ifdef AGE_EXPORTS
#define AGE_API __declspec(dllexport)
#else
#define AGE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <fstream>
#include <string>

typedef unsigned int uint;

#endif
