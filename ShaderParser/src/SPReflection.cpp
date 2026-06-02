#include "ShaderParser/SPReflection.h"
#include "Utils.h"

SPReflection ParseShaderRef(std::string_view refBlock)
{
    SPReflection result;
    int    lineNum = 0;
    size_t pos     = 0;

    while (pos < refBlock.size())
    {
        size_t eol        = refBlock.find('\n', pos);
        bool   hasNewline = eol != std::string_view::npos;
        std::string_view line = refBlock.substr(pos, hasNewline ? eol - pos : refBlock.size() - pos);
        pos = hasNewline ? eol + 1 : refBlock.size();
        ++lineNum;

        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        line = Trim(line);
        if (line.empty())
            continue;

        size_t typeEnd = line.find_first_of(" \t");
        if (typeEnd == std::string_view::npos)
        {
            result.ok           = false;
            result.errorMessage = "Line " + std::to_string(lineNum) + ": expected 'type shaderName editorName'";
            return result;
        }
        std::string_view typeName = line.substr(0, typeEnd);
        std::string_view rest     = TrimLeft(line.substr(typeEnd));

        size_t shaderNameEnd        = rest.find_first_of(" \t");
        std::string_view shaderName = shaderNameEnd != std::string_view::npos ? rest.substr(0, shaderNameEnd) : rest;
        rest                        = shaderNameEnd != std::string_view::npos ? TrimLeft(rest.substr(shaderNameEnd)) : std::string_view{};

        if (shaderName.empty())
        {
            result.ok           = false;
            result.errorMessage = "Line " + std::to_string(lineNum) + ": missing shader variable name";
            return result;
        }

        std::string_view editorName = TrimRight(rest);
        if (editorName.empty())
        {
            result.ok           = false;
            result.errorMessage = "Line " + std::to_string(lineNum) + ": missing editor display name";
            return result;
        }

        ShaderPropertyType type;
        if      (typeName == "float")   type = ShaderPropertyType::Float;
        else if (typeName == "double")  type = ShaderPropertyType::Double;
        else if (typeName == "int")     type = ShaderPropertyType::Int;
        else if (typeName == "vec2")    type = ShaderPropertyType::Vec2;
        else if (typeName == "vec3")    type = ShaderPropertyType::Vec3;
        else if (typeName == "vec4")    type = ShaderPropertyType::Vec4;
        else if (typeName == "color")   type = ShaderPropertyType::Color;
        else if (typeName == "texture") type = ShaderPropertyType::Texture;
        else
        {
            result.ok           = false;
            result.errorMessage = "Line " + std::to_string(lineNum) + ": unknown type '" + std::string(typeName) + "'";
            return result;
        }

        result.properties.push_back({ type, std::string(shaderName), std::string(editorName) });
    }

    return result;
}
