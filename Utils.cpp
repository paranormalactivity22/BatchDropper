#include "Utils.h"
#include <iomanip>
#include <sstream>

string ToHex(const string& s)
{
    ostringstream buffer;
    for (size_t i = 0; i < s.length(); ++i)
    {
        int ch = s[i] & 0xFF; // 255
        buffer << std::hex << std::setfill('0') << std::setw(2) << ch;
    }
    return buffer.str();
}

void parse_error(const string& s)
{
    cout << s << endl;
    exit(0);
}

string get_file_name(char* path)
{
    int len = strlen(path);
    int flag = 0;
    for (size_t i = len - 1; i > 0; i--)
    {
        if (path[i] == '\\' || path[i] == '//' || path[i] == '/')
        {
            flag = 1;
            path = path + i + 1;
            break;
        }
    }
    return string(path);
}