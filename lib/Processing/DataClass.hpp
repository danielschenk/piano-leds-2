#ifndef PROCESSING_DATACLASS_HPP
#define PROCESSING_DATACLASS_HPP

#include <map>
#include <tuple>
#include <variant>

namespace processing
{

template <typename T>
struct Property
{
    Property() = default;
    Property(T value) : value(value) {}

    explicit operator T() const
    {
        return value;
    }

    T& operator=(T value)
    {
        this->value = value;
    }

    bool operator==(const T& other) const
    {
        return other == value;
    }

  private:
    T value;
};

template <typename Child>
class DataClass
{
  public:
    DataClass() = default;

    template <typename T>
    void addDescriptor(std::string name, size_t offset)
    {
        descriptors.emplace(name, Resolver<T>{offset});
    }

    template <typename T>
    void set(std::string name, T value)
    {
    }

    template <typename T>
    struct Resolver
    {
        const std::size_t offset;
        Property<T>& resolve(Child* object) const
        {
            Property<T>* property = static_cast<void*>(object) + offset;
            return *property;
        }
    };

    using Descriptor = std::variant<Resolver<int>, Resolver<bool>>;

  private:
    static std::map<std::string, Descriptor> descriptors;

    // static_assert(std::is_pod<Child>(), "child must be a POD type");
};

template <typename Child>
std::map<std::string, typename DataClass<Child>::Descriptor> DataClass<Child>::descriptors;

template <typename T, typename Container>
struct RegisteredProperty : public Property<T>
{
    RegisteredProperty(Container& container, std::string name, std::size_t offset)
    {
        container.template addDescriptor<T>(name, offset);
    }

    RegisteredProperty(Container& container, std::string name, std::size_t offset, T value)
        : Property<T>(value)
    {
        container.template addDescriptor<T>(name, offset);
    }

    using Property<T>::operator=;
    using Property<T>::operator==;
    using Property<T>::operator T;
};

#define PROPERTY(container, type, name) \
    RegisteredProperty<type, container> name{*this, #name, offsetof(container, name)};

}  // namespace processing

#endif
