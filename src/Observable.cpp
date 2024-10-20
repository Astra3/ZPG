#include "Observable.hpp"
#include <memory>

void Observable::attach(std::shared_ptr<Observer> observer) {
    this->observers.push_back(observer);
}

