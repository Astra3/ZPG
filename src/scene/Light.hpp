#include "../observers/Observable.hpp"
#include <glm/ext/vector_float3.hpp>

struct LightData {
    glm::vec3 color;
    glm::vec3 position;
};

class Light : public Observable {
protected:
    glm::vec3 color{0.0f};

public:
    Light() {}
    Light(glm::vec3 color) : color(color) {}
    void set_color(glm::vec3 color);
    virtual LightData get_data() = 0;
};

namespace lights {
class PositionedLight : public Light {
private:
    glm::vec3 position{0.0f};

public:
    PositionedLight(glm::vec3 color, glm::vec3 position) : Light(color), position(position) {}
    void notify_observers() override;
    void set_position(glm::vec3 position);
    LightData get_data() override;
};
} // namespace lights
