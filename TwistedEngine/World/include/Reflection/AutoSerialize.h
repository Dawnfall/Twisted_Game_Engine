#pragma once

#include "PropertyInfo.h"
#include <yaml-cpp/yaml.h>

namespace Twisted
{
    template<typename T>
    YAML::Node AutoYamlSerialize(const T& obj)
    {
        YAML::Node node;
        for (const PropertyInfo& prop : GetTypeProperties<T>())
        {
            void* ptr = prop.accessor(const_cast<T*>(&obj));
            switch (prop.type)
            {
            case PropertyType::Float:  node[prop.yamlKey] = *static_cast<float*>(ptr);       break;
            case PropertyType::Int:    node[prop.yamlKey] = *static_cast<int*>(ptr);         break;
            case PropertyType::Bool:   node[prop.yamlKey] = *static_cast<bool*>(ptr);        break;
            case PropertyType::String: node[prop.yamlKey] = *static_cast<std::string*>(ptr); break;
            case PropertyType::Vec2f:  node[prop.yamlKey] = *static_cast<Vec2f*>(ptr);       break;
            case PropertyType::Vec3f:  node[prop.yamlKey] = *static_cast<Vec3f*>(ptr);       break;
            case PropertyType::Vec4f:  node[prop.yamlKey] = *static_cast<Vec4f*>(ptr);       break;
            case PropertyType::Quat:   node[prop.yamlKey] = *static_cast<Quat*>(ptr);        break;
            }
        }
        return node;
    }

    template<typename T>
    void AutoYamlDeserialize(T& obj, const YAML::Node& node)
    {
        for (const PropertyInfo& prop : GetTypeProperties<T>())
        {
            if (!node[prop.yamlKey])
                continue;
            void* ptr = prop.accessor(&obj);
            switch (prop.type)
            {
            case PropertyType::Float:  *static_cast<float*>(ptr)       = node[prop.yamlKey].as<float>();       break;
            case PropertyType::Int:    *static_cast<int*>(ptr)         = node[prop.yamlKey].as<int>();         break;
            case PropertyType::Bool:   *static_cast<bool*>(ptr)        = node[prop.yamlKey].as<bool>();        break;
            case PropertyType::String: *static_cast<std::string*>(ptr) = node[prop.yamlKey].as<std::string>(); break;
            case PropertyType::Vec2f:  *static_cast<Vec2f*>(ptr)       = node[prop.yamlKey].as<Vec2f>();       break;
            case PropertyType::Vec3f:  *static_cast<Vec3f*>(ptr)       = node[prop.yamlKey].as<Vec3f>();       break;
            case PropertyType::Vec4f:  *static_cast<Vec4f*>(ptr)       = node[prop.yamlKey].as<Vec4f>();       break;
            case PropertyType::Quat:   *static_cast<Quat*>(ptr)        = node[prop.yamlKey].as<Quat>();        break;
            }
        }
    }
}
