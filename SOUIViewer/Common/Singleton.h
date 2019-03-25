#pragma once
#include <assert.h>

/**
* @class      CSingleton
* @brief      µ¥ÀýÄ£°å
*
* Describe
*/
template <typename T>
class CSingleton
{
protected:
	static    T* ms_Singleton;

public:
	CSingleton()
	{
		assert(!ms_Singleton);
		ms_Singleton = static_cast<T*>(this);
	}
	virtual ~CSingleton()
	{
		assert(ms_Singleton);
		ms_Singleton = 0;
	}
	static T& getSingleton()
	{
		assert(ms_Singleton);
		return (*ms_Singleton);
	}
	static T* getSingletonPtr()
	{
		return (ms_Singleton);
	}

private:
	CSingleton& operator=(const CSingleton&)
	{
		return *this;
	}
	CSingleton(const CSingleton&) {}
};
