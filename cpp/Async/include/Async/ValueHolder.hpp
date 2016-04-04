#pragma once

#ifndef ASYNC_VALUEHOLDER_HPP
#define ASYNC_VALUEHOLDER_HPP

// SYSTEM INCLUDES
#include <type_traits>
#include <stdexcept>

// C++ PROJECT INCLUDES


namespace Async
{

    template<typename VALUE>
    class ValueHolder :
        public std::enable_shared_from_this<ValueHolder<VALUE> >
    {
    private:

        VALUE   _value;
        bool    _hasValue;

    private:

        void RequireContents() const
        {
            if (!this->_value)
            {
                throw std::logic_error("No Value in ValueHolder");
            }
        }

        template<typename... Args>
        void Construct(Args&&... args)
        {
            const void* ptr = &this->_value;
            new(const_cast<void*>(ptr)) VALUE(std::forward<Args>(args)...);
            this->_hasValue = true;
        }

    public:

        static_assert(!std::is_reference<VALUE>::value,
                      "ValueHolder cannot store Reference types");
        static_assert(!std::is_abstract<VALUE>::value,
                      "ValueHolder cannot store Abstract types");

        ValueHolder() : _hasValue(false)
        {
        }

        ValueHolder(const VALUE& val) : _hasValue(false)
        {
            this->Construct(val);
        }

        ValueHolder(VALUE&& val)
        {
            this->Construct(std::move(val));
        }

        bool HasValue() const
        {
            return this->_hasValue;
        }

        VALUE* GetPointer()
        {
            return this->_hasValue ? &this->_value : nullptr;
        }

        const VALUE* GetPointer() const
        {
            return this->_hasValue ? &this->_value : nullptr;
        }

        explicit operator bool() const
        {
            return this->HasValue();
        }

        VALUE GetValue()
        {
            this->RequireContents();
            return this->_value;
        }

        const VALUE& GetValue() const
        {
            this->RequireContents();
            return this->_value;
        }

        void Clear()
        {
            this->_hasValue = false;
        }

        void Assign(VALUE&& val)
        {
            if(this->HasValue())
            {
                this->_value = std::move(val);
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
                this->_value = val;
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
