#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
class Transformation {
public:
    virtual void apply(glm::mat4 &matrix) = 0;
};

namespace transf {
class Rotate : public Transformation {
private:
    float angle;
    glm::vec3 axis;

public:
    Rotate(float angle, glm::vec3 axis) : angle(angle), axis(axis) {}
    void apply(glm::mat4 &matrix) override;
};

class Translate : public Transformation {
private:
    glm::vec3 translation_vec;

public:
    Translate(glm::vec3 translation_vec) : translation_vec(translation_vec) {}
    void apply(glm::mat4 &matrix) override;
};

class Scale : public Transformation {
private:
    glm::vec3 scale_ratio;

public:
    Scale(glm::vec3 scale_ratio) : scale_ratio(scale_ratio) {}
    void apply(glm::mat4 &matrix) override;
};
} // namespace transf
