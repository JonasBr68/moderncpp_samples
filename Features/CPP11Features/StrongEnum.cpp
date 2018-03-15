#include "stdafx.h"
#include "StrongEnum.h"


// Example from https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum#answer-18335862

enum Color { red, green, blue };                    // plain enum 
enum Card { red_card, green_card, yellow_card };    // another plain enum 
#if CPP_VER > 98
enum class Animal { dog, deer, cat, bird, human };  // enum class
enum class Mammal { kangaroo, deer, human };        // another enum class
#endif
void strongEnums() {

	// examples of bad use of plain enums:
	Color color = Color::red;
	Card card = Card::green_card;

	int num = color;    // no problem

	if (color == Card::red_card) // no problem (bad)
		cout << "bad" << el;

	if (card == Color::green)   // no problem (bad)
		cout << "bad" << el;
#if CPP_VER > 98
	// examples of good use of enum classes (safe)
	Animal a = Animal::deer;
	Mammal m = Mammal::deer;
#endif

//#define NOTDEFINED
#ifdef NOTDEFINED
	int num2 = a;   // error
	if (m == a)         // error (good)
		cout << "bad" << el;

	if (a == Mammal::deer) // error (good)
		cout << "bad" << el;
#endif
}