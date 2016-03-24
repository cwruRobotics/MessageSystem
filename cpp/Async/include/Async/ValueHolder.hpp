#ifndef ASYNC_VALUEHOLDER_HPP
#define ASYNC_VALUEHOLDER_HPP

// SYSTEM INCLUDES
#include <type_traits>


// C++ PROJECT INCLUDES


namespace Async
{

    template<typename VALUE>
    class ValueHolder :
        public std::enable_shared_from_this<ValueHolder<VALUE> >
    {
    private:

        struct ValueTriviallyDestructible
        {
            union
            {
                VALUE _value;
            };
            bool _hasValue;

            ValueTriviallyDestructible() : _hasValue(false)
            {}

            void Clear()
            {
                this->_hasValue = false;
            }
        };

        struct ValueNonTriviallyDestructible
        {
            union
            {
                VALUE _value;
            };
            bool _hasValue;

            ValueNonTriviallyDestructible() : _hasValue(false)
            {}

            void Clear()
            {
                if(this->_hasValue)
                {
                    this->_hasValue = false;
                    this->_value.~VALUE();
                }
            }
        };

        using ValueStored =
            typename std::conditional<std::is_trivially_destructible<VALUE>::value,
                                      ValueTriviallyDestructible,
                                      ValueNonTriviallyDestructible>::type;

        ValueStored _valueStored;

    private:

        void RequireContents() const
        {
            if (!this->_valueStored._hasValue)
            {
                throw AsyncValueHolderException();
            }
        }

        template<typename... Args>
        void Construct(Args&&... args)
        {
            const void* ptr = &this->_valueStored._value;
            new(const_cast<void*>(ptr)) VALUE(std::forward<Args>(args)...);
            this->_valueStored._hasValue = true;
        }

    public:

        static_assert(!std::is_reference<VALUE>::value,
                      "ValueHolder cannot store Reference types");
        static_assert(!std::is_abstract<VALUE>::value,
                      "ValueHolder cannot store Abstract types");

        ValueHolder()
        {
        }

        ValueHolder(const VALUE& val)
        {
            static_assert(std::is_nothrow_copy_constructible<VALUE>::value);
            this->Construct(val);
        }

        ValueHolder(VALUE&& val)
        {
            static_assert(std::is_nothrow_move_constructible<VALUE>::value);
            this->Construct(std::move(val));
        }

        bool HasValue() const
        {
            return this->_valueStored._hasValue;
        }

        VALUE* GetPointer()
        {
            return this->_valueStored._hasValue ? &this->_valueStored._value : nullptr;
        }

        const VALUE* GetPointer() const
        {
            return this->_valueStored._hasValue ? &this->_valueStored._value : nullptr;
        }

        explicit operator bool() const
        {
            return this->HasValue();
        }

        VALUE GetValue()
        {
            this->RequireContents();
            return this->_valueStored._value;
        }

        const VALUE& GetValue() const
        {
            this->RequireContents();
            return this->_valueStored._value;
        }

        void Clear()
        {
            this->_valueStored.Clear();
        }

        void Assign(VALUE&& val)
        {
            if(this->HasValue())
            {
                this->_valueStored._value = std::move(val);
            }
            else
            {
                this->Construct(std::move(val));
            }
        }

        void Assign(const VALUE& val)
        {
            if(this->HasValue())
            {
                this->_valueStored._value = val;
            }
            else
            {
                this->Construct(val);
            }
        }

        template<typename Arg>
        ValueHolder& operator = (Arg&& arg)
        {
            this->Assign(std::forward<Arg>(arg));
            return *this;
        }

        const VALUE& operator * () const {return this->GetValue();}
              VALUE  operator * ()       {return this->GetValue();}

        const VALUE* operator -> () const {return &this->GetValue();}
              VALUE* operator -> ()       {return &this->GetValue();}

    };

    template<typename VALUE>
    using ValueHolderPtr = std::shared_ptr<ValueHolder<VALUE> >;

} // end of namespace Async

#endif // end of ASYNC_VALUEHOLDER_HPP
