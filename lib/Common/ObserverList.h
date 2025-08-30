#ifndef COMMON_OBSERVERLIST_H_
#define COMMON_OBSERVERLIST_H_

#include <functional>
#include <vector>
#include <algorithm>

/**
 * Template for observer lists.
 */
template<class... CallbackArgs>
class ObserverList
{
public:
    typedef std::function<void(CallbackArgs... args)> TCallbackFunction;
    typedef unsigned int TSubscriptionToken;

    ObserverList() = default;

    ObserverList(const ObserverList&) = delete;
    ObserverList& operator=(const ObserverList&) = delete;

    /**
     * Subscribe for events.
     *
     * @param[in]   callback    The callback function to register.
     */
    TSubscriptionToken subscribe(TCallbackFunction callback)
    {
        // Check for a free slot in the vector first
        bool foundSlot = false;
        TSubscriptionToken token(-1);
        for(auto it = subscriptions.begin(); it < subscriptions.end(); ++it)
        {
            if(*it == nullptr)
            {
                *it = callback;
                foundSlot = true;
                token = std::distance(subscriptions.begin(), it);
                break;
            }
        }

        if(!foundSlot)
        {
            // No empty slot found, extend vector
            token = subscriptions.insert(subscriptions.cend(), callback) - subscriptions.cbegin();
        }

        return token;
    }

    /**
     * Unsubscribe from events.
     *
     * @param[in]  token   The token of the subscription to cancel.
     */
    void unsubscribe(TSubscriptionToken token)
    {
        if(token < subscriptions.size())
        {
            // Invalidate entry instead of removing it. This keeps previously handed out tokens valid.
            subscriptions[token] = nullptr;
        }
    }

    /**
     * Notify all observers.
     */
    void notifyObservers(CallbackArgs... args) const
    {
        for(const auto& it : subscriptions)
        {
            if(it != nullptr)
            {
                it(args...);
            }
        }
    }

private:
    /** The collection of subscriptions. Using a vector for optimal traversal. */
    std::vector<TCallbackFunction> subscriptions;
};

#endif /* COMMON_OBSERVERLIST_H_ */
