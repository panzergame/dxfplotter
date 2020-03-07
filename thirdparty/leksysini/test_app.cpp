#include <iostream>
#include "iniparser.hpp"

// User-defined structure that can be loaded to\from INI
struct test_val
{
	test_val():a(0),b(0.0),c(0){}
	test_val(int pa, double pb, int pc):a(pa),b(pb),c(pc){}
	int a;
	double b;
	int c;
	bool operator== (const test_val& rt) const
	{
        return (a == rt.a && b == rt.b && c == rt.c);
	}
	bool operator!= (const test_val& rt) const {return !(*this == rt);}
};

// You need to declare these 2 operators to use your classes with Leksys' INIParser
std::ostream& operator<< (std::ostream& stream, const test_val& value)
{
	stream << '{' << value.a << ',' << value.b << ',' << value.c << '}';
	return stream;
}
std::istream& operator>> (std::istream& stream, test_val& value)
{
	char sb;
	stream >> sb;
	if (sb != '{') return stream;
	stream >> value.a >> sb >> value.b >> sb >> value.c >> sb;
	if (sb != '}') {value.a = 0; value.b = 0.0; value.c = 0;}
	return stream;
}

void show_help()
{
	std::cout << "Leksys' INIParser test program\n"
		"Usage: test_app [-i input_file] [-o output_file]\n"
		"Pass [input_file] for optional loading file test (TEST11)\n"
		"Pass [output_file] for optional saving file test (TEST12)\n"
		"---------------------------------------------------------"
		<< std::endl;
}
int main(int argc, char** argv)
{
    INI::File ft, ft2;
	std::string input_file;
	std::string output_file;
	show_help();
	// Bad way to parse input parameters, but why bother
	for (int i = 1; i < argc; i+=2)
	{
		std::string param = argv[i];
		if (param.size() != 2 || param[0] != '-' || i == argc - 1)
		{
			std::cerr << "ERROR! Wrong parameter " << param << std::endl;
			return -1;
		}
		if (param[1] == 'i')
			input_file = argv[i+1];
		else if (param[1] == 'o')
			output_file = argv[i+1];
		else
		{
			std::cerr << "ERROR! Unknown parameter " << param << std::endl;
			return -1;
		}
	}
	
	// [TEST1] Get & Set INT value through section pointer
	ft.GetSection("MainProg")->SetValue("value1",456);
	if (ft.GetSection("MainProg")->GetValue("value1").AsInt() != 456 ||
		ft.GetSection("MainProg")->Name() != "MainProg")
	{
		std::cerr << "Failed to pass [TEST1]" << std::endl;
		return 1;
	}
	std::cout << "[TEST1] passed" << std::endl;
	
	// [TEST2] Get & Set boolean value through File object
	ft.SetValue("MainProg:value2_0",false);
	ft.SetValue("MainProg:value2_1",true);
	ft.SetValue("MainProg:value2_2",0);
	ft.SetValue("MainProg:value2_3",1);
	ft.SetValue("MainProg:value2_4","FALSE");
	ft.SetValue("MainProg:value2_5","TRUE");
	ft.SetValue("MainProg:value2_6","false");
	ft.SetValue("MainProg:value2_7","true");
	if (ft.GetValue("MainProg:value2_0").AsBool() || !ft.GetValue("MainProg:value2_1").AsBool() ||
		ft.GetValue("MainProg:value2_2").AsBool() || !ft.GetValue("MainProg:value2_3").AsBool() ||
		ft.GetValue("MainProg:value2_4").AsBool() || !ft.GetValue("MainProg:value2_5").AsBool() ||
		ft.GetValue("MainProg:value2_6").AsBool() || !ft.GetValue("MainProg:value2_7").AsBool())
	{
		std::cerr << "Failed to pass [TEST2]" << std::endl;
		return 2;
	}
	std::cout << "[TEST2] passed" << std::endl;

	// [TEST3] Set array value
	ft.SetValue("MainProg:value3",INI::Array() << true << 100 << 200.55 << "String");
	INI::Array value3 = ft.GetSection("MainProg")->GetValue("value3").AsArray();
	if (!value3[0].AsBool() || value3[1].AsInt() != 100 || 
		 value3[2].AsDouble() != 200.55 || value3[3].AsString() != "String")
	{
		std::cerr << "Failed to pass [TEST3]" << std::endl;
		return 3;
	}
	std::cout << "[TEST3] passed" << std::endl;

	// [TEST4] Direct access to array values
	ft.SetArrayValue("MainProg:value3",1,105);
	ft.GetSection("MainProg")->SetArrayValue("value3",6,700.67);
	if (ft.GetValue("MainProg:value3").AsArray()[1].AsInt() != 105 ||
		ft.GetSection("MainProg")->GetValue("value3").AsArray().GetValue(6).AsDouble() != 700.67)
	{
		std::cerr << "Failed to pass [TEST4]" << std::endl;
		return 4;
	}
	std::cout << "[TEST4] passed" << std::endl;

	// [TEST5] Comments
	ft.GetSection("MainProg")->SetComment("value3","Test array");
	ft.SetValue("MainProg2:value1", 1, "Comment 2");
	if (ft.GetSection("MainProg")->GetComment("value3") != "Test array" ||
		ft.GetSection("MainProg2")->GetComment("value1") != "Comment 2")
	{
		std::cerr << "Failed to pass [TEST5]" << std::endl;
		return 5;
	}
	std::cout << "[TEST5] passed" << std::endl;

	// [TEST6] Section iteration using iterator
	// Add key "value_test" with value "1" to all of the sections, existing so far (MainProg & MainProg2)
	for (INI::File::sections_iter it = ft.SectionsBegin(); it != ft.SectionsEnd(); it++)
		it->second->SetValue("value_test",1,"Testing value");
	// Read those keys
	for (INI::File::sections_iter it = ft.SectionsBegin(); it != ft.SectionsEnd(); it++)
	{
		if (it->second->GetValue("value_test",0).AsInt() != 1)
		{
			std::cerr << "Failed to pass [TEST6]" << std::endl;
			return 6;
		}
	}
	std::cout << "[TEST6] passed" << std::endl;

	// [TEST7] Parent & Child Sections
	ft.GetSection("MainProg2")->GetSubSection("Sub1")->GetSubSection("SubSub1")->SetValue("value1",120);
	if (ft.GetValue("MainProg2.Sub1.SubSub1:value1").AsInt() != 120 || 
		ft.GetSection("MainProg2.Sub1")->GetSubSection("SubSub1")->GetValue("value1").AsInt() != 120)
	{
		std::cerr << "Failed to pass [TEST7]" << std::endl;
		return 7;
	}
	std::cout << "[TEST7] passed" << std::endl;

	// [TEST8] User-defined class storage
	test_val tp(1,120.555,3);
	ft.GetSection("MainProg2.Sub1")->SetValue("test_val",tp,"User-defined class");
	if (tp != ft.GetValue("MainProg2.Sub1:test_val").AsT<test_val>())
	{
		std::cerr << "Failed to pass [TEST8]" << std::endl;
		return 8;
	}
	std::cout << "[TEST8] passed" << std::endl;

	// [TEST9] Saving and loading files + values and section iteration (main test)
	std::stringstream stream;
	stream << ft;
	stream >> ft2;
	if (ft2.SectionsSize() != ft.SectionsSize())
	{
		std::cerr << "Failed to pass [TEST9]" << std::endl;
		return 9;
	}
	for (INI::File::sections_iter it = ft2.SectionsBegin(); it != ft2.SectionsEnd(); it++)
	{
		INI::Section* sect1 = it->second;
		INI::Section* sect2 = ft.GetSection(sect1->FullName());
		if (sect1->ValuesSize() != sect2->ValuesSize() || sect1->Comment() != sect2->Comment())
		{
			std::cerr << "Failed to pass [TEST9]" << std::endl;
			return 9;
		}
		for (INI::Section::values_iter vit = sect1->ValuesBegin(); vit != sect1->ValuesEnd(); vit++)
		{
			if (vit->second != sect2->GetValue(vit->first) || sect1->GetComment(vit->first) != sect2->GetComment(vit->first))
			{
				std::cerr << "Failed to pass [TEST9]" << std::endl;
				return 9;
			}
		}
	}
	std::cout << "[TEST9] passed" << std::endl;

	// [TEST10] Saving and loading sections + Unload feature
	stream << ft.GetSection("MainProg2.Sub1");
	ft2.Unload();
	stream >> ft2;
	// Only 1 section was saved (parent is not created automatically as well as child sections)
	if (ft2.SectionsSize() != 1)
	{
		std::cerr << "Failed to pass [TEST10]" << std::endl;
		return 10;
	}
	INI::Section* sect = ft2.GetSection("MainProg2")->GetSubSection("Sub1");
	if (sect->ValuesSize() != 1 || sect->GetValue("test_val").AsT<test_val>() != tp)
	{
		std::cerr << "Failed to pass [TEST10]" << std::endl;
		return 10;
	}
	std::cout << "[TEST10] passed" << std::endl;

	// [TEST11] Complex arrays test
	ft.SetValue("MainProg2:cmp_array1", "{12,3}, {13,5}, {18,9} ");
	ft.SetValue("MainProg2:cmp_array2", "Str1, Str2, {{Str3,,3}}, Str4\\,\\,\\{\\,\\,\\}, {Str5\\,,\\{\\{}");
	{
		INI::Value val = ft.GetSection("MainProg2")->GetValue("cmp_array1");
		if (val.AsArray()[1].AsArray()[0].AsInt() != 13 || val.AsArray()[2].AsArray()[1].AsInt() != 9)
		{
			std::cerr << "Failed to pass [TEST11] (check 1)" << std::endl;
			return 11;
		}
		val = ft.GetSection("MainProg2")->GetValue("cmp_array2");
		val = val.AsArray().ToValue();
		if (val.AsArray()[1].AsString() != "Str2" || val.AsArray()[2].AsString() != "{Str3,,3}"
			|| val.AsArray()[3].AsString() != "Str4,,{,,}" || val.AsArray()[4].AsString() != "Str5,,{{")
		{
			std::cerr << "Failed to pass [TEST11] (check 2)" << std::endl;
			return 11;
		}
	}
	std::cout << "[TEST11] passed" << std::endl;

	// [TEST12] Map test
	ft.SetValue("MainProg2:map1","1:5, 1:3, 2:9, 3:10");
	ft.SetValue("MainProg2:map2","Str1:5, Str2:3, Str3:{:,,:}");
	ft.SetValue("MainProg2:map3","Str1:{Sub1,{\\,,\\\\{\\\\{Sub2,},{,:Sub3}}, Str2:1");
	{
		INI::Value val = ft.GetSection("MainProg2")->GetValue("map1");
		val = val.AsMap().ToValue();
		if (val.AsMap()[2].AsInt() != 9 || val.AsMap()[1].AsInt() != 3)
		{
			std::cerr << "Failed to pass [TEST12] (check 1)" << std::endl;
			return 12;
		}
		val = ft.GetSection("MainProg2")->GetValue("map2");
		INI::Map mp = val.AsMap();
		if (val.AsMap()["Str2"].AsInt() != 3 || val.AsMap()["Str3"].AsString() != ":,,:")
		{
			std::cerr << "Failed to pass [TEST12] (check 2)" << std::endl;
			return 12;
		}
		val = ft.GetSection("MainProg2")->GetValue("map3");
		mp = val.AsMap().ToValue().AsMap();
		std::string str = val.AsMap()["Str1"].AsArray()[1].AsString();
		if (val.AsMap()["Str1"].AsArray()[1].AsString() != ",,{{Sub2," ||
			val.AsMap()["Str1"].AsArray()[2].AsString() != ",:Sub3" ||
			val.AsMap()["Str2"].AsInt() != 1)
		{
			std::cerr << "Failed to pass [TEST12] (check 3)" << std::endl;
			return 12;
		}
	}
	std::cout << "[TEST12] passed" << std::endl;

	// [TEST13] Optional. File loading
	if (!input_file.empty())
	{
		if (!ft.Load(input_file))
		{
			std::cerr << "Failed to pass optional [TEST13] (file loading). Error: " 
				<< ft.LastResult().GetErrorDesc() << std::endl;
			return 13;
		}
		std::cout << "[TEST13] passed!" << std::endl;
	}
	else
		std::cout << "Optional [TEST13] (file loading) SKIPPED" << std::endl;

	// [TEST14] Optional. File saving
	if (!output_file.empty())
	{
		if (!ft.Save(output_file))
		{
			std::cerr << "Failed to pass optional [TEST14] (file saving). Wrong output file. File contents comes next"
				<< std::endl << ft;
			return 14;
		}
		std::cout << "[TEST14] passed! (Look in " << output_file << " for your file)!" << std::endl;
	}
	else
		std::cout << "Optional [TEST14] (file saving) SKIPPED" << std::endl;

	// [SUCCESS]
	std::cout << "!!! [SUCCESS] !!! " << std::endl;
    return 0;
}
