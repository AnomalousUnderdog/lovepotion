#pragma once

#include "objects/object.h"

namespace love
{
    class Variant
    {
        static const int MAX_SMALL_STRING_LENGTH = 15;

        class SharedString : public Object
        {
            public:
                SharedString(const char * string, size_t length) : length(length)
                {
                    this->string = new char[length + 1];
                    this->string[length] = '\0';
                    memcpy(this->string, string, length);
                }

                virtual ~SharedString() {
                    delete [] this->string;
                }

                char * string;
                size_t length;
        };

        struct SmallString
        {
            char string[MAX_SMALL_STRING_LENGTH];
            uint8_t length;
        };

        private:
            std::variant<std::monostate, bool, float, Variant::SharedString *, Variant::SmallString, void *, Proxy, Nil> variant;

        public:

            enum Type
            {
                UNKNOWN = 0,
                BOOLEAN,
                NUMBER,
                STRING,
                SMALLSTRING,
                LUSERDATA,
                LOVE_OBJECT,
                NIL,
                TABLE
            };

            Variant() : variant(std::monostate{}) {}
            Variant(std::monostate v) : variant(v) {}
            Variant(bool v) : variant(v) {}
            Variant(love::Type * type, Object * object);
            Variant(void * v) : variant(v) {}
            Variant(Nil v) : variant(v) {}
            Variant(float v) : variant(v) {}
            Variant(const std::string & v);
            Variant(const char * v, size_t length);

            Variant(const Variant & v);

            Variant & operator= (const Variant & v);

            Variant(Variant && other);

            ~Variant();

            static Proxy * TryExtractProxy(lua_State * L, size_t index);

            Type GetType() const;

            template <Type type>
            const std::variant_alternative_t<size_t(type), decltype(variant)> & GetValue() const
            {
                return std::get<size_t(type)>(variant);
            }

            template <Type type>
            std::variant_alternative_t<size_t(type), decltype(variant)> & GetValue()
            {
                return std::get<size_t(type)>(variant);
            }

            std::string GetTypeString() const;

            static Variant FromLua(lua_State * L, int n);
            void ToLua(lua_State * L) const;

    };
}
