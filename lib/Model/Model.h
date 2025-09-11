#ifndef MODEL_H
#define MODEL_H

#include <mutex>

class Model
{
public:
    Model() = default;
    virtual ~Model() = default;

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

protected:
    template<typename T>
    void set(T& member, T value)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            member = value;
        }

        notifyObservers();
    }

    template<typename T>
    T get(const T& member) const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return member;
    }

private:
    void notifyObservers() const;
    mutable std::mutex mutex;
};


#endif //MODEL_H
