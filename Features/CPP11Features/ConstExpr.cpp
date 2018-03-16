#include "stdafx.h"
#include "ConstExpr.h"

#if CPP_VER > 110
class ConstConstruct
{
	char * m_label;
public:

	constexpr ConstConstruct(const char* label):m_label(const_cast<char *>(label))
	{

	}
	const char* getLabel() const {
		return m_label;
	}
};

constexpr ConstConstruct objects[] = { "constString", "constString", "constString", "constString", "constString" };

constexpr ConstConstruct obj{ "Hello" };



//constexpr functions in C++ restriction, although this one should work but does not in VS 2013
constexpr unsigned fibonacci(unsigned i) {
	return (i <= 1u) ? i : (fibonacci(i - 1) + fibonacci(i - 2));
}

#endif


void constExpr()
{
#if CPP_VER > 110

	constexpr unsigned fib = fibonacci(20); //Stack limit with 30, warning C4593: 'fibonacci': 'constexpr' call evaluation step limit of 100000 exceeded; use /constexpr:steps<NUMBER> to increase the limit


	constexpr auto constString = "constString";

	constexpr const char* constStringArr[5] = { "constString", "constString", "constString", "constString", "constString" };
	auto& o1 = objects[1];
	cout << o1.getLabel() << el;

	constexpr int arraySize = sizeof(objects) / sizeof(ConstConstruct);
	int arraySizeRT = sizeof(objects) / sizeof(ConstConstruct);
	for (int i = 0; i < arraySize; i++)
	{
		cout << objects[i].getLabel() << el;
	}
#endif
}