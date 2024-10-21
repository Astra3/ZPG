#include "Observer.hpp"
#include <memory>
#include <vector>

#pragma once

class Observable {
protected:
    std::vector<std::shared_ptr<Observer>> observers;

public:
    void attach(std::shared_ptr<Observer> observer);
    virtual void notify_observers() = 0;
};
