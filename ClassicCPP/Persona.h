#pragma once
#include <cstring>

class Persona
{
public:
	Persona(const char* nombre)
	{
		if (nombre && nombre[0] != 0)
		{
			strcpy(m_nombre, nombre);
		}
	}
private:
	char m_nombre[1024];

};

