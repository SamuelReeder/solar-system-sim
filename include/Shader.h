#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    bool load(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    
    // uniform setters
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat2(const std::string& name, const glm::mat2& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);

private:
    unsigned int programId;
    std::unordered_map<std::string, int> uniformLocations;
    
    int getUniformLocation(const std::string& name);
    bool checkCompileErrors(unsigned int shader, const std::string& type);
};