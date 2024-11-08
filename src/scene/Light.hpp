#include "../observers/Observable.hpp"
#include <glm/ext/vector_float3.hpp>

struct LightData {
    glm::vec3 color;
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

class Light : public Observable {
protected:
    glm::vec3 color{0.0f};
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    size_t id = 0;

public:
    Light() {}
    Light(glm::vec3 color, size_t id = 0) : color(color) {}
    void set_color(glm::vec3 color);
    void set_id(size_t id);
    virtual LightData get_data() = 0;
};

namespace lights {
class PositionedLight : public Light {
private:
    glm::vec3 position{0.0f};

public:
    PositionedLight(glm::vec3 color, glm::vec3 position, size_t id = 0) : Light(color, id), position(position) {}
    void notify_observers() override;
    void set_position(glm::vec3 position);
    LightData get_data() override;
};
} // namespace lights
