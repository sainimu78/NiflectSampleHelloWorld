#include "NiflectHelloWorld.h"
#include <stdio.h>
#include "NiflectHelloWorld_private.h"

int main()
{
	Niflect::CNiflectTable table;
	Niflect::GeneratedRegisterTypes(&table);
	Niflect::GeneratedInitTypes();
	table.InitTypesLayout();

	printf("Type name: %s\n", table.GetTypeByIndex(0)->GetTypeName().c_str());

	return 0;
}