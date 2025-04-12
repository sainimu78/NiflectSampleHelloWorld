#pragma once
#include "Niflect/Serialization/RwTree.h"
#include <iomanip>
#include <limits>

namespace RwTree
{
	constexpr const char JsonSyntaxPair_CurlyBrackets[] = { '{', '}' };
	constexpr const char JsonSyntaxPair_SquareBrackets[] = { '[', ']' };
	constexpr const char JsonSyntaxPair_StringQuotes[] = { '\"', '\"' };
	constexpr const char JsonSyntax_ElementDelimiter = ',';
	constexpr const char JsonSyntax_ObjectColon = ':';
	constexpr const char* JsonSyntax_Indent = "\t";
	constexpr const char* JsonSyntax_True = "true";
	constexpr const char* JsonSyntax_False = "false";

	class CJsonFormat
	{
	public:
		static void Write(const CRwNode* rwNode, std::ostream& stm)
		{
			WriteJsonRecurs(rwNode, stm);
		}
		static void Read(CRwNode* rwNode, std::istream& stm)
		{
			SkipWhitespace(stm);
			ParseValue(rwNode, stm);
		}

	private:
		static void ConvertNumberToString(const CRwValue* rwValue, ERwValueType type, Niflect::CString& str)
		{
			double val;
			Niflect::CStringStream ss;
			switch (type)
			{
			case ERwValueType::Float:
				val = rwValue->GetFloat();
				ss << std::setprecision(std::numeric_limits<float>::max_digits10) << val;
				break;
			case ERwValueType::Double:
				val = rwValue->GetDouble();
				ss << std::setprecision(std::numeric_limits<double>::max_digits10) << val;
				break;
			default:
				ASSERT(false);
				break;
			}
			str = ss.str();
		}
		static void WriteJsonRecurs(const CRwNode* rwNode, std::ostream& stm, uint32 arrayItemIdx = INDEX_NONE, Niflect::CString strIndent = Niflect::CString())
		{
			stm << strIndent;
			if (arrayItemIdx == INDEX_NONE)
			{
				auto& name = rwNode->GetName();
				if (!name.empty())
				{
					stm << JsonSyntaxPair_StringQuotes[0] << rwNode->GetName() << JsonSyntaxPair_StringQuotes[1];
					if (arrayItemIdx == INDEX_NONE)
						stm << JsonSyntax_ObjectColon << " ";
				}
			}
			else
			{
				ASSERT(rwNode->GetName().empty());
			}
			if (rwNode->IsValue())
			{
				Niflect::CString str;
				auto rwValue = rwNode->GetValue();
				switch (rwValue->GetType())
				{
				case ERwValueType::Bool:
					str = rwValue->GetBool() ? JsonSyntax_True : JsonSyntax_False;
					break;
				case ERwValueType::Float:
				{
					Niflect::CStringStream ss;
					ss << std::setprecision(std::numeric_limits<float>::max_digits10) << rwValue->GetFloat();
					str = ss.str();
					break;
				}
				case ERwValueType::Double:
				{
					//begin, 现转换方法无法保证从其它Writer读取的数据原样输出, 如无法保证原样输出rapidjson写的double
					//const auto str0 = "0.4000000059604645";
					//auto val = std::stod(str0);
					//Niflect::CStringStream ss;
					//ss << std::setprecision(std::numeric_limits<double>::max_digits10) << val;
					//auto str1 = ss.str();
					//ASSERT(str0 == str1);
					//end

					Niflect::CStringStream ss;
					ss << std::setprecision(std::numeric_limits<double>::max_digits10) << rwValue->GetDouble();
					str = ss.str();
					break;
				}
				case ERwValueType::Int32:
					str = std::to_string(rwValue->GetInt32()).c_str();
					break;
				case ERwValueType::String:
					str = (JsonSyntaxPair_StringQuotes[0] + rwValue->GetString() + JsonSyntaxPair_StringQuotes[1]);
					break;
				default:
					ASSERT(false);
					break;
				}
				stm << str;
			}
			else
			{
				Niflect::CString blockClose;
				const auto isArray = rwNode->IsArray();
				if (isArray)
				{
					stm << JsonSyntaxPair_SquareBrackets[0] << std::endl;
					blockClose = strIndent + JsonSyntaxPair_SquareBrackets[1];
				}
				else
				{
					stm << JsonSyntaxPair_CurlyBrackets[0] << std::endl;
					blockClose = strIndent + JsonSyntaxPair_CurlyBrackets[1];
				}
				auto cnt = rwNode->GetNodesCount();
				if (cnt > 0)
				{
					strIndent += JsonSyntax_Indent;
					uint32 idxInc = 0;
					for (uint32 idx = 0; idx < cnt; ++idx)
					{
						WriteJsonRecurs(rwNode->GetNode(idx), stm, isArray ? idx : INDEX_NONE, strIndent);
						if (idx != cnt - 1)
							stm << JsonSyntax_ElementDelimiter << std::endl;
					}
					stm << std::endl;
				}
				stm << blockClose;
			}
		}

	private:
		static void ParseValue(CRwNode* rwNode, std::istream& stm)
		{
			SkipWhitespace(stm);
			char ch = stm.peek();
			if (ch == JsonSyntaxPair_CurlyBrackets[0])
			{
				ParseObject(rwNode, stm);
			}
			else if (ch == JsonSyntaxPair_SquareBrackets[0])
			{
				ParseArray(rwNode, stm);
			}
			else if (ch == JsonSyntaxPair_StringQuotes[0])
			{
				auto str = ParseString(stm);
				auto rwValue = rwNode->ToValue();
				rwValue->SetString(str);
			}
			else if (MatchKeyword(stm, JsonSyntax_True))
			{
				auto rwValue = rwNode->ToValue();
				rwValue->SetBool(true);
			}
			else if (MatchKeyword(stm, JsonSyntax_False))
			{
				auto rwValue = rwNode->ToValue();
				rwValue->SetBool(false);
			}
			else if (isdigit(ch) || ch == '-')
			{
				ParseNumber(rwNode, stm);
			}
			else
			{
				throw std::logic_error("Invalid JSON value");
			}
		}
		static void ParseObject(CRwNode* rwNode, std::istream& stm)
		{
			stm.get(); // Skip '{'
			SkipWhitespace(stm);
			while (stm.peek() != JsonSyntaxPair_CurlyBrackets[1])
			{
				SkipWhitespace(stm);
				auto key = ParseString(stm);
				SkipWhitespace(stm);
				stm.get(); // Skip ':'
				auto rwChild = CreateRwNode();
				ParseValue(rwChild.Get(), stm);
				rwChild->SetName(key);
				rwNode->AddNode(rwChild);
				SkipWhitespace(stm);
				if (stm.peek() == JsonSyntax_ElementDelimiter)
				{
					stm.get(); // Skip ','
				}
			}
			stm.get(); // Skip '}'
		}
		static void ParseArray(CRwNode* rwNode, std::istream& stm)
		{
			auto rwArray = rwNode->ToArray();
			stm.get(); // Skip '['
			SkipWhitespace(stm);
			while (stm.peek() != JsonSyntaxPair_SquareBrackets[1])
			{
				SkipWhitespace(stm);
				auto rwItem = CreateRwNode();
				ParseValue(rwItem.Get(), stm);
				rwArray->AddItem(rwItem);
				SkipWhitespace(stm);
				if (stm.peek() == JsonSyntax_ElementDelimiter)
				{
					stm.get(); // Skip ','
				}
			}
			stm.get(); // Skip ']'
		}
		static Niflect::CString ParseString(std::istream& stm)
		{
			stm.get(); // Skip '\"'
			Niflect::CString str;
			char ch;
			while (true)
			{
				ch = stm.get();
				if (ch == '\\')
				{
					str += ch;
					ch = stm.get();
				}
				else if (ch == JsonSyntaxPair_StringQuotes[1])
				{
					break;
				}
				str += ch;
			}
			return str;
		}
		static void ParseNumber(CRwNode* rwNode, std::istream& stm)
		{
			std::string numStr;
			bool isFloat = false;
			while (true)
			{
				auto ch = stm.get();
				if (ch == '-' || isdigit(ch))
				{
					numStr += ch;
				}
				else if (ch == '.' || ch == 'e' || ch == 'E')
				{
					numStr += ch;
					isFloat = true;
				}
				else
				{
					break;
				}
			}

			auto rwValue = rwNode->ToValue();
			if (isFloat)
			{
				auto decimalPos = numStr.find('.');
				ASSERT(decimalPos != std::string::npos);//如失败可能是科学计数格式, 认为有必要则实现解析
				auto decimalPlaces = numStr.length() - decimalPos - 1;
				if (decimalPlaces <= std::numeric_limits<float>::max_digits10)
					rwValue->SetFloat(stof(numStr));
				else
					rwValue->SetDouble(stod(numStr));

				//rwValue->SetDouble(std::stod(numStr));
			}
			else
			{
				rwValue->SetInt32(std::stoi(numStr));
			}
		}
		static void SkipWhitespace(std::istream& stm)
		{
			char ch;
			while (isspace(ch = stm.peek()))
			{
				stm.get();
			}
		}
		static bool MatchKeyword(std::istream& stm, const Niflect::CString& keyword)
		{
			Niflect::CString buffer;
			for (char ch : keyword)
			{
				if (stm.peek() == ch)
				{
					buffer += stm.get();
				}
				else
				{
					// Rollback if match fails
					int cnt = static_cast<int>(buffer.size());
					for (int i = cnt - 1; i >= 0; --i)
					{
						stm.unget();
					}
					return false;
				}
			}
			return true;
		}
	};
}