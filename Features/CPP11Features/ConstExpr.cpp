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

#endif

void constExpr()
{
#if CPP_VER > 110
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