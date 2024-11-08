#include "generators.hpp"
#include "objects/DrawableObject.hpp"
#include <memory>
#include <random>

std::random_device RAND_DEV;
std::default_random_engine E1(RAND_DEV());
std::uniform_real_distribution<float> DEGREES(0, 360);
std::uniform_real_distribution<float> POSITION(-80, 80);
std::uniform_real_distribution<float> SCALE(0.5, 4);

namespace generators {
void trees_bushes(std::vector<DrawableObject> &objects, std::shared_ptr<ShaderProgram> shader, size_t count) {
    auto model_tree = std::make_shared<models::Tree>();
    auto model_bush = std::make_shared<models::Bush>();

    objects.reserve(objects.size() + count);

    for (size_t i = 0; i < 100; i++) {
        TransformationType t[3];
        for (size_t j = 0; j < 3; j++) {
            t[j] = {std::make_shared<transf::Translate>(glm::vec3(POSITION(E1), -10.0f, POSITION(E1))),
                    std::make_shared<transf::Scale>(glm::vec3(SCALE(E1))),
                    std::make_shared<transf::Rotate>(DEGREES(E1), glm::vec3(0.0f, 1.0f, 0.0f))};
        }
        objects.push_back(DrawableObject(model_tree, shader, std::move(t[0])));
        objects.push_back(DrawableObject(model_bush, shader, std::move(t[1])));
        objects.push_back(DrawableObject(model_bush, shader, std::move(t[2])));
    }
}
} // namespace generators
