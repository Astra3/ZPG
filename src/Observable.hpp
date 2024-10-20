#include <memory>
#include <vector>
#include "Observer.hpp"

#pragma once

class Observable {
protected:
    std::vector<std::shared_ptr<Observer>> observers;
public:
    void attach(std::shared_ptr<Observer> observer);
    virtual void notify_observers() = 0;
};
