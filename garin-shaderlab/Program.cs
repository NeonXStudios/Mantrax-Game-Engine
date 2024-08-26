using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

abstract class ShaderElement
{
    public abstract string TranslateToGLSL(bool isVertexShader);
}

class ShaderVariable : ShaderElement
{
    public string Name { get; set; }
    public string Type { get; set; }
    public string StorageQualifier { get; set; }
    public int? Location { get; set; }

    public ShaderVariable(string name, string type, string storageQualifier = "", int? location = null)
    {
        Name = name;
        Type = type;
        StorageQualifier = storageQualifier;
        Location = location;
    }

    public override string TranslateToGLSL(bool isVertexShader)
    {
        string glslType = Type == "Texture2D" ? "sampler2D" : Type;
        string qualifier = isVertexShader ? TranslateVertexQualifier() : TranslateFragmentQualifier();
        return $"{(Location.HasValue ? $"layout (location = {Location.Value}) " : "")}{qualifier} {glslType} {Name};";
    }

    private string TranslateVertexQualifier()
    {
        return StorageQualifier switch
        {
            "in" => "in",
            "out" => "out",
            _ => StorageQualifier
        };
    }

    private string TranslateFragmentQualifier()
    {
        return StorageQualifier switch
        {
            "in" => "in",
            "out" => "out",
            _ => StorageQualifier
        };
    }
}

class ShaderFunction : ShaderElement
{
    public string Name { get; set; }
    public string ReturnType { get; set; }
    public List<ShaderVariable> Parameters { get; set; }
    public List<string> Body { get; set; }

    public ShaderFunction(string name, string returnType = "void")
    {
        Name = name;
        ReturnType = returnType;
        Parameters = new List<ShaderVariable>();
        Body = new List<string>();
    }

    public void AddParameter(ShaderVariable parameter)
    {
        Parameters.Add(parameter);
    }

    public void AddLineToBody(string line)
    {
        Body.Add(line);
    }

    public override string TranslateToGLSL(bool isVertexShader)
    {
        var glsl = new StringBuilder();
        glsl.Append($"{ReturnType} {Name}(");
        glsl.Append(string.Join(", ", Parameters.Select(p => $"{p.Type} {p.Name}")));
        glsl.AppendLine(") {");

        foreach (var line in Body)
        {
            string translatedLine = line.Replace("TXT2D", "texture");
            translatedLine = translatedLine.Replace("_pos", "gl_Position");
            glsl.AppendLine($"    {translatedLine};");
        }

        glsl.AppendLine("}");
        return glsl.ToString();
    }
}

class CustomShader
{
    public string Version { get; set; } = "330 core";
    public List<ShaderElement> VertexElements { get; set; } = new List<ShaderElement>();
    public List<ShaderElement> FragmentElements { get; set; } = new List<ShaderElement>();

    public void AddElement(ShaderElement element, bool isVertex)
    {
        if (isVertex)
            VertexElements.Add(element);
        else
            FragmentElements.Add(element);
    }

    public string TranslateToGLSL(bool isVertex)
    {
        var code = new StringBuilder();
        code.AppendLine($"#version {Version}");

        var elements = isVertex ? VertexElements : FragmentElements;
        foreach (var element in elements)
        {
            code.AppendLine(element.TranslateToGLSL(isVertex));
        }

        return code.ToString();
    }

    public void SaveToFiles(string baseFileName)
    {
        try
        {
            string glslVertexCode = TranslateToGLSL(true);
            string glslFragmentCode = TranslateToGLSL(false);

            File.WriteAllText($"{baseFileName}.vertex", glslVertexCode);
            File.WriteAllText($"{baseFileName}.fragment", glslFragmentCode);

            Console.WriteLine("Shaders compiled platform (OpenGL)");
        }
        catch (Exception err)
        {
            Console.WriteLine("Error: " + err.Message);
            throw;
        }
    }
}

class CustomShaderParser
{
    public static CustomShader ParseShaderFile(string filePath)
    {
        var shader = new CustomShader();
        var lines = File.ReadAllLines(filePath);

        ShaderFunction currentFunction = null;
        bool isVertex = false;

        foreach (var line in lines)
        {
            var trimmedLine = line.Trim();

            if (trimmedLine.StartsWith(":Core"))
            {
                shader.Version = trimmedLine.Split(' ')[1];
            }
            else if (trimmedLine == "front")
            {
                isVertex = false;
            }
            else if (trimmedLine == "back")
            {
                isVertex = true;
            }
            else if (trimmedLine.StartsWith("param_out") || trimmedLine.StartsWith("param_in"))
            {
                var parts = trimmedLine.Split(new[] { ' ', '=', '(', ')' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 5)
                {
                    var qualifier = parts[0].Replace("param_", "");
                    var location = int.Parse(parts[1]);
                    var param = new ShaderVariable(parts[5], parts[3], qualifier, location);
                    shader.AddElement(param, isVertex);
                }
            }
            else if (trimmedLine.StartsWith("param"))
            {
                var parts = trimmedLine.Split(new[] { ' ', '=' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 4)
                {
                    var param = new ShaderVariable(parts[3], parts[2], "uniform");
                    shader.AddElement(param, isVertex);
                }
            }
            else if (trimmedLine.StartsWith("layer"))
            {
                var parts = trimmedLine.Split(new[] { ' ', '=', '(', ')' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 6)
                {
                    var location = int.Parse(parts[1]);
                    var variableType = parts[4];
                    var paramName = parts[5];
                    var param = new ShaderVariable(paramName, variableType, "in", location);
                    shader.AddElement(param, isVertex);
                }
            }

            else if (trimmedLine.StartsWith("var"))
            {
                var parts = trimmedLine.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 3)
                {
                    var variable = new ShaderVariable(parts[2], parts[1]);
                    shader.AddElement(variable, isVertex);
                }
            }
            else if (trimmedLine.StartsWith("func"))
            {
                var parts = trimmedLine.Split(new[] { ' ', '(', ')' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 2)
                {
                    currentFunction = new ShaderFunction(parts[1], "void");
                    shader.AddElement(currentFunction, isVertex);
                }
            }
            else if (trimmedLine == "{")
            {
                // INICIO DE LA FUNCIÓN
            }
            else if (trimmedLine == "}")
            {
                // FIN DE LA FUNCIÓN
                currentFunction = null;
            }
            else if (currentFunction != null)
            {
                // AGREGAR LÍNEA AL CUERPO DE LA FUNCIÓN
                currentFunction.AddLineToBody(trimmedLine);
            }
        }

        return shader;
    }
}

class Program
{
    static void Main(string[] args)
    {
        var shader = CustomShaderParser.ParseShaderFile(args[0]);

        shader.SaveToFiles("output");
    }
}
