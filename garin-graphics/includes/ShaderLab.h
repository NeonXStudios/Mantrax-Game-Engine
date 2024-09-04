#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

class ShaderElement
{
public:
    virtual std::string TranslateToGLSL(bool isVertexShader) const = 0;
    virtual ~ShaderElement() = default;
};

class ShaderVariable : public ShaderElement
{
public:
    std::string Name;
    std::string Type;
    std::string StorageQualifier;
    int Location;

    ShaderVariable(std::string name, std::string type, std::string storageQualifier = "", int location = -1)
        : Name(name), Type(type), StorageQualifier(storageQualifier), Location(location) {}

    std::string TranslateToGLSL(bool isVertexShader) const override
    {
        std::string glslType = (Type == "Texture2D") ? "sampler2D" : Type;
        std::string qualifier = isVertexShader ? TranslateVertexQualifier() : TranslateFragmentQualifier();
        std::ostringstream oss;
        if (Location >= 0)
        {
            oss << "layout (location = " << Location << ") ";
        }
        oss << qualifier << " " << glslType << " " << Name << ";";
        return oss.str();
    }
    

private:
    std::string TranslateVertexQualifier() const
    {
        if (StorageQualifier == "in")
            return "in";
        if (StorageQualifier == "out")
            return "out";
        return StorageQualifier;
    }

    std::string TranslateFragmentQualifier() const
    {
        if (StorageQualifier == "in")
            return "in";
        if (StorageQualifier == "out")
            return "out";
        return StorageQualifier;
    }
};

class ShaderFunction : public ShaderElement
{
public:
    std::string Name;
    std::string ReturnType;
    std::vector<ShaderVariable> Parameters;
    std::vector<std::string> Body;

    ShaderFunction(std::string name, std::string returnType = "void")
        : Name(name), ReturnType(returnType) {}

    void AddParameter(const ShaderVariable &parameter)
    {
        Parameters.push_back(parameter);
    }

    void AddLineToBody(const std::string &line)
    {
        Body.push_back(line);
    }

    std::string TranslateToGLSL(bool isVertexShader) const override
    {
        std::ostringstream oss;
        oss << ReturnType << " " << Name << "(";
        for (size_t i = 0; i < Parameters.size(); ++i)
        {
            if (i > 0)
                oss << ", ";
            oss << Parameters[i].Type << " " << Parameters[i].Name;
        }
        oss << ") {\n";

        for (const auto &line : Body)
        {
            std::string translatedLine = line;
            ReplaceAll(translatedLine, "TXT2D", "texture");
            ReplaceAll(translatedLine, "_pos", "gl_Position");
            oss << "    " << translatedLine << ";\n";
        }

        oss << "}";
        return oss.str();
    }

private:
    static void ReplaceAll(std::string &str, const std::string &from, const std::string &to)
    {
        size_t startPos = 0;
        while ((startPos = str.find(from, startPos)) != std::string::npos)
        {
            str.replace(startPos, from.length(), to);
            startPos += to.length();
        }
    }
};

class CustomShader
{
public:
    std::string Version = "330 core";
    std::vector<std::unique_ptr<ShaderElement>> VertexElements;
    std::vector<std::unique_ptr<ShaderElement>> FragmentElements;

    std::string VERTEX;
    std::string FRAGMENT;

    void AddElement(std::unique_ptr<ShaderElement> element, bool isVertex)
    {
        if (isVertex)
        {
            VertexElements.push_back(std::move(element));
        }
        else
        {
            FragmentElements.push_back(std::move(element));
        }
    }

    std::string TranslateToGLSL(bool isVertex) const
    {
        std::ostringstream oss;
        oss << "#version " << Version << " core" << "\n";

        const auto &elements = isVertex ? VertexElements : FragmentElements;
        for (const auto &element : elements)
        {
            oss << element->TranslateToGLSL(isVertex) << "\n";
        }

        return oss.str();
    }

    void SaveToFiles(const std::string &baseFileName) const
    {
        try
        {
            std::string glslVertexCode = TranslateToGLSL(true);
            std::string glslFragmentCode = TranslateToGLSL(false);

            std::ofstream vertexFile(baseFileName + ".vertex");
            vertexFile << glslVertexCode;
            vertexFile.close();

            std::ofstream fragmentFile(baseFileName + ".fragment");
            fragmentFile << glslFragmentCode;
            fragmentFile.close();

            std::cout << "Shaders compiled platform (OpenGL)\n";
        }
        catch (const std::exception &err)
        {
            std::cerr << "Error: " << err.what() << "\n";
            throw;
        }
    }

    void SaveToVariables()
    {
        std::string glslVertexCode = TranslateToGLSL(true);
        std::string glslFragmentCode = TranslateToGLSL(false);

        VERTEX = glslVertexCode;
        FRAGMENT = glslFragmentCode;
    }
};

class CustomShaderParser
{
public:
    static CustomShader ParseShaderFile(const std::string &filePath)
    {
        CustomShader shader;
        std::ifstream file(filePath);
        std::string line;
        ShaderFunction *currentFunction = nullptr;
        bool isVertex = false;

        while (std::getline(file, line))
        {
            std::string trimmedLine = Trim(line);

            if (trimmedLine.find(":Core") == 0)
            {
                shader.Version = trimmedLine.substr(6);
            }
            else if (trimmedLine == "front")
            {
                isVertex = false;
            }
            else if (trimmedLine == "back")
            {
                isVertex = true;
            }
            else if (trimmedLine.find("param_out") == 0 || trimmedLine.find("param_in") == 0)
            {
                auto parts = Split(trimmedLine, " =()");
                if (parts.size() >= 4)
                {
                    std::string qualifier = trimmedLine.find("param_in") == 0 ? "in" : "out";
                    std::string type = parts[2];
                    std::string name = parts[3];
                    int location = -1;

                    shader.AddElement(std::make_unique<ShaderVariable>(name, type, qualifier, location), isVertex);
                }
            }
            else if (trimmedLine.find("param") == 0)
            {
                auto parts = Split(trimmedLine, " =");
                if (parts.size() >= 4)
                {
                    shader.AddElement(std::make_unique<ShaderVariable>(parts[3], parts[2], "uniform"), isVertex);
                }
            }
            else if (trimmedLine.find("layer") == 0)
            {
                auto parts = Split(trimmedLine, " =()");
                if (parts.size() >= 6)
                {
                    int location = std::stoi(parts[1]);
                    shader.AddElement(std::make_unique<ShaderVariable>(parts[5], parts[4], "in", location), isVertex);
                }
            }
            else if (trimmedLine.find("var") == 0)
            {
                auto parts = Split(trimmedLine, " ");
                if (parts.size() >= 3)
                {
                    shader.AddElement(std::make_unique<ShaderVariable>(parts[2], parts[1]), isVertex);
                }
            }
            else if (trimmedLine.find("func") == 0)
            {
                auto parts = Split(trimmedLine, " ()");
                if (parts.size() >= 2)
                {
                    auto func = std::make_unique<ShaderFunction>(parts[1]);
                    currentFunction = func.get();
                    shader.AddElement(std::move(func), isVertex);
                }
            }
            else if (trimmedLine == "{")
            {
                // Start of function body
            }
            else if (trimmedLine == "}")
            {
                // End of function body
                currentFunction = nullptr;
            }
            else if (currentFunction)
            {
                currentFunction->AddLineToBody(trimmedLine);
            }
        }

        return shader;
    }

private:
    static std::string Trim(const std::string &str)
    {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first)
        {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

    static std::vector<std::string> Split(const std::string &str, const std::string &delimiters)
    {
        std::vector<std::string> tokens;
        size_t start = str.find_first_not_of(delimiters), end = 0;

        while ((end = str.find_first_of(delimiters, start)) != std::string::npos)
        {
            tokens.push_back(str.substr(start, end - start));
            start = str.find_first_not_of(delimiters, end);
        }
        if (start != std::string::npos)
            tokens.push_back(str.substr(start));

        return tokens;
    }
};

// int main(int argc, char* argv[]) {
//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <shader file path>\n";
//         return 1;
//     }

//     CustomShader shader = CustomShaderParser::ParseShaderFile(argv[1]);

//     shader.SaveToFiles("output");

//     return 0;
// }
