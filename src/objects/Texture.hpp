#include "ShaderProgram.hpp"
#include <string>

class Texturable {
protected:
    uint texture_id;

public:
    virtual void bind() = 0;
    virtual void apply_uniform(ShaderProgram &shader_program) = 0;
};

class Texture : public Texturable {
public:
    Texture(std::string filename);
    void bind() override;
    void apply_uniform(ShaderProgram &shader_program) override;
};

class CubeMap : public Texturable {
public:
    CubeMap(std::string face1, std::string face2, std::string face3, std::string face4, std::string face5,
            std::string face6);
    void bind() override;
    void apply_uniform(ShaderProgram &shader_program) override;
};
