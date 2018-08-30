#pragma once

#ifdef LIBRARY_EXPORTS  
#define LIBRARY_API __declspec(dllexport)   
#else  
#define LIBRARY_API __declspec(dllimport)   
#endif

class A
{
public:
	static LIBRARY_API void Foo();
};
