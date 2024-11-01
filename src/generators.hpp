#include "objects/DrawableObject.hpp"
#include "objects/ShaderProgram.hpp"
#include <cstddef>
#include <functional>

namespace generators {
using GENERATOR_FUNCTION = std::function<void(std::vector<DrawableObject> &, std::shared_ptr<ShaderProgram>, size_t)>;

void trees_bushes(std::vector<DrawableObject> &objects, std::shared_ptr<ShaderProgram> camera, size_t count);
}; // namespace generators
