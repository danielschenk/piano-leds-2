#ifndef MODEL_H
#define MODEL_H

#include "ObserverList.h"

#include <mutex>

/**
 * Base class for observable objects
 */
class Model
{
public:
    /**
     * Constructor
     */
    Model() = default;

    /**
     * Destructor
     */
    virtual ~Model() = default;

    // Prevent implicit copy constructor & assignment operator
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    typedef std::function<void()> TUpdateCallback;
    typedef ObserverList<> UpdateObserverList;

    /**
     * Subscribe to model updates.
     *
     * @param callback  The callback to call on a model update
     * @return          The token which can be used to unsubscribe
     */
    UpdateObserverList::TSubscriptionToken subscribe(TUpdateCallback callback) const;

    /**
     * Unsubscribe from model updates.
     *
     * @param token     The token received for the subscription
     */
    void unsubscribe(UpdateObserverList::TSubscriptionToken token) const;

protected:
    /**
     * Set a value and notify observers.
     *
     * @param member    Reference to the member to set
     * @param value     Value to set
     */
    template<typename T>
    void set(T& member, T value)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            member = value;
        }

        // Outside data lock, to prevent deadlock when observer calls a getter
        notifyObservers();
    }

    /**
     * Get a value.
     *
     * @param member    Reference to the member to get
     */
    template<typename T>
    T get(const T& member) const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return member;
    }

private:
    /**
     * Notify all observers about a model update.
     */
    void notifyObservers() const;

    /**
     * List of observers
     */
    mutable UpdateObserverList observers;

    /**
     * Mutex guarding the observers
     */
    mutable std::mutex observersMutex;

    /**
     * Mutex guarding the data
     */
     mutable std::mutex mutex;
};


#endif //MODEL_H
