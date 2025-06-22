#include "HelloWorld.h"
#include "Niflect/Component/RwTree/Serialization/JsonFormat.h"
#include "Resource.h"
#include "HelloWorld_private.h"

using namespace RwTree;

static void InitForTest(CHelloWorld& instance)
{
	instance.m_value = 1.23f;
	auto factory = GetResourceFactory();
	instance.m_res = factory->Find("Note_B.txt");
}
static bool operator==(const CHelloWorld& lhs, const CHelloWorld& rhs)
{
	return lhs.m_value == rhs.m_value
		&& lhs.m_res == rhs.m_res
		;
}

int main()
{
	auto factory = GetResourceFactory();
	factory->Register("Note_A.txt");
	factory->Register("Note_B.txt");
	factory->Register("Note_C.txt");

	{
		//Initialize types registry
		Niflect::CNiflectModuleRegistry reg;
		reg.InitLoadTimeModules();

		Niflect::CNiflectType* type = Niflect::StaticGetType<CHelloWorld>();
		printf("Registered type: %s\n", type->GetTypeName().c_str());

		//Save src to rw
		CHelloWorld src;
		InitForTest(src);
		CRwNode rw;
		SaveInstanceToRwNode(type, &src, &rw);
		//Load dst from rw
		CHelloWorld dst;
		LoadInstanceFromRwNode(type, &dst, &rw);
		assert(src == dst);

		//Serialize rw in JSON
		Niflect::CStringStream ss;
		CJsonFormat::Write(&rw, ss);
		printf("%s\n", ss.str().c_str());
	}

	DestroyResourceFactory();
	return 0;
}