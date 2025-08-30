#include "Model.h"

#include <utility>

Model::UpdateObserverList::TSubscriptionToken Model::subscribe(Model::TUpdateCallback callback) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    return observers.subscribe(std::move(callback));
}

void Model::unsubscribe(UpdateObserverList::TSubscriptionToken token) const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    observers.unsubscribe(token);
}

void Model::notifyObservers() const
{
    std::lock_guard<std::mutex> lock(observersMutex);

    observers.notifyObservers();
}
