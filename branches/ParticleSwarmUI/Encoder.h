#include <vector>
#include <string>

class Encoder
{
	/// 为Shape文件编码，结果写到新建的字段中
	static void StartEncode(string filePath, vector<string> newFieldNames, string tip);
}