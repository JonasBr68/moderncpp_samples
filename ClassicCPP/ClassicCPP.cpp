// ClassicCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Persona.h"

int main()
{
	Persona* p1 = new Persona("Jonas Brandel");
	
	delete p1;
	p1 = NULL;
    return 0;
}

