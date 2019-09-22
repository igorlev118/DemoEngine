#include "Precompiled.hpp"
#include "Utility.hpp"

std::string Utility::GetFileDirectory(std::string filePath)
{
    std::string path;

    std::size_t it = filePath.find_last_of("/\\");
    if(it != std::string::npos)
    {
        path = filePath.substr(0, it + 1);
    }

    return path;
}

std::string Utility::GetFileExtension(std::string filePath)
{
    std::string extension;

    std::size_t it = filePath.find_last_of(".");
    if(it != std::string::npos)
    {
        extension = filePath.substr(it + 1);
    }

    return extension;
}

std::string Utility::GetTextFileContent(std::string filePath)
{
    std::ifstream file(filePath);
    std::string content;

    if(file)
    {
        file.seekg(0, std::ios::end);
        content.resize((unsigned int)file.tellg());
        file.seekg(0, std::ios::beg);

        file.read(&content[0], content.size());
    }

    return content;
}

std::vector<char> Utility::GetBinaryFileContent(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    std::vector<char> content;

    if(file)
    {
        file.seekg(0, std::ios::end);
        content.resize((unsigned int)file.tellg());
        file.seekg(0, std::ios::beg);

        file.read(&content[0], content.size());
    }

    return content;
}

std::vector<std::string> Utility::TokenizeString(std::string text, char delimeter)
{
    std::vector<std::string> result;

    auto begin = text.begin();
    auto it = text.begin();

    while(it != text.end())
    {
        if(*it == delimeter)
        {
            result.push_back(std::string(begin, it));
            begin = ++it;
        }
        else
        {
            ++it;
        }

        if(it == text.end())
        {
            result.push_back(std::string(begin, it));
        }
    }

    return result;
}
