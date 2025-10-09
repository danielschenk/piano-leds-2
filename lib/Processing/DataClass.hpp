#ifndef PROCESSING_DATACLASS_HPP
#define PROCESSING_DATACLASS_HPP

#include <map>
#include <string>
#include <tuple>
#include <variant>

#include "Color.hpp"
#include "Logging.hpp"

namespace processing
{

template <typename T>
struct Property
{
    Property() = default;
    Property(T value) : value(value) {}

    explicit operator T&()
    {
        return value;
    }

    explicit operator const T&() const
    {
        return value;
    }

    T& operator=(T value)
    {
        this->value = value;
        return this->value;
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

    static constexpr const char* LOGGING_COMPONENT = "DataClass";

    template <typename T>
    void set(std::string name, T value)
    {
        auto pair = descriptors.find(name);
        if (pair == descriptors.end())
        {
            LOG_ERROR_PARAMS("property %s not found", name.c_str());
            return;
        }

        if (!std::holds_alternative<Resolver<T>>(pair->second))
        {
            LOG_ERROR_PARAMS("type of property %s does not match type of value", name.c_str());
            return;
        }
        std::get<Resolver<T>>(pair->second).resolve(this) = value;
    }

    void set(std::string name, const char* value)
    {
        set(name, std::string(value));
    }

    template <typename T>
    struct Resolver
    {
        const std::size_t offset;
        Property<T>& resolve(DataClass<Child>* object) const
        {
            Property<T>* property =
                reinterpret_cast<Property<T>*>(reinterpret_cast<uint8_t*>(object) + offset);
            return *property;
        }
    };

    using Descriptor =
        std::variant<Resolver<int>, Resolver<bool>, Resolver<std::string>, Resolver<RgbColor>>;

  private:
    static std::map<std::string, Descriptor> descriptors;
};

template <typename Child>
std::map<std::string, typename DataClass<Child>::Descriptor> DataClass<Child>::descriptors;

template <typename T, typename DataClassChild>
struct RegisteredProperty : public Property<T>
{
    RegisteredProperty(DataClass<DataClassChild>& container, std::string name, std::size_t offset)
    {
        container.template addDescriptor<T>(name, offset);
    }

    RegisteredProperty(DataClass<DataClassChild>& container, std::string name, std::size_t offset,
                       T value)
        : Property<T>(value)
    {
        container.template addDescriptor<T>(name, offset);
    }

    using Property<T>::operator=;
    using Property<T>::operator==;
    using Property<T>::operator T&;
};

#define PROPERTY(container, type, name) \
    processing::RegisteredProperty<type, container> name{*this, #name, offsetof(container, name)};

}  // namespace processing

#endif
