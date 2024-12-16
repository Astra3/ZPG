#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
class Transformation {
public:
    virtual void apply(glm::mat4 &matrix) = 0;
};

namespace transf {
class Rotate : public Transformation {
protected:
    float angle;
    glm::vec3 axis;

public:
    Rotate(float angle, glm::vec3 axis) : angle(angle), axis(axis) {}
    void set_angle(float angle);
    void apply(glm::mat4 &matrix) override;
};

class RotateTime : public Rotate {
private:
    float speed;
public:
    RotateTime(float speed = 2.f) : Rotate(0.f, glm::vec3(0, 1, 0)), speed(speed) {}
    void apply(glm::mat4 &matrix) override;
};

class RotateAround : public Rotate {
    protected:
        float speed;
    public:
        RotateAround(float speed) : Rotate(0.f, glm::vec3(0, 1, 0)), speed(speed) {}
        void apply(glm::mat4 &matrix) override;
};

class RotateMoon : public RotateAround {
    public:
    RotateMoon(float speed) : RotateAround(speed) {}
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
