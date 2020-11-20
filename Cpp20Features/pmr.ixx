module;

// Type your code here, or load an example.
#include <iostream>
#include <memory_resource>
#include <vector>
#include <string>
#include <chrono>

import Fibo.MemoryTracker;

export module Fibo.Pmr;

void print(std::string_view msg)
{
    static int numspc = 0;
    auto loop = (msg.at(0) == '~') ? --numspc : numspc++;
    for (int i = 0; i < loop; ++i) std::cout << "  ";
    std::cout << msg << std::endl << std::flush;
}

struct Foo 
{
private:
    int mId{};
    std::pmr::string mName;

public:
    Foo() noexcept 
    {
        //std::cout << "default ctor " << this << std::endl;
    }

    template<typename T>
    Foo(int id, T&& name) noexcept :
        mId{ id },
        mName(std::forward<T>(name))
    {
        //std::cout << "ctor with argument " << this << std::endl;
    }

    Foo(Foo const& o) : mId{ o.mId }, mName{ o.mName }
    {
        //std::cout << "copy ctor " << this << " <= " << &o << std::endl;
    }

    Foo& operator=(Foo const& o) 
    {
        //std::cout << "copy operator " << this << " <= " << &o << std::endl;
        Foo tmp(o);
        *this = std::move(tmp);
        return *this;
    }

    Foo(Foo&& o) noexcept : 
        mId{ o.mId },
        mName{ std::exchange(o.mName, std::pmr::string{}) }
    {
        //std::cout << "move ctor " << this << " <= " << &o << std::endl;
    }

    Foo& operator=(Foo&& o) noexcept 
    {
        if (this != &o) {
            mId = o.mId;
            mName = std::exchange(o.mName, std::pmr::string{});
            //std::cout << "move operator " << this << " <= " << &o << std::endl;
        }
        return *this;
    }
};

export void testMonotoicBuffer()
{
}

export void monotonic_unreusable()
{
    std::cout << "\n\n*********************************************\n";
    std::cout << "* monotoic buffer resource                  *\n";
    std::cout << "*********************************************\n";
 
    print("monotonic_buffer_resource");
    std::pmr::monotonic_buffer_resource pool{};
    {
        {
            print("vector");
            std::pmr::vector<Foo> vec{ &pool };
            vec.reserve(2);
            {
                print("emplace_back");
                vec.emplace_back(1, "The string should be big enought to avoid the SSO");
                vec.emplace_back(2, "The string should be big enought to avoid the SSO");
                print("~emplace_back");
            }
            print("~vector");
        }
    }
    std::cout << "Can re-use ???? => NO\n";
    {
        {
            print("vector");
            std::pmr::vector<Foo> vec{ &pool };
            vec.reserve(1);
            {
                print("emplace_back");
                vec.emplace_back(3, "The string should be big enought to avoid the SSO");
                print("~emplace_back");
            }
            print("~vector");
        }
    }
    print("~monotonic_buffer_resource");
}


export void synchronized_reusable()
{
    std::cout << "\n\n*********************************************\n";
    std::cout << "* synchronized pool resource                *\n";
    std::cout << "*********************************************\n";

    print("synchronized_pool_resource");
    std::pmr::synchronized_pool_resource pool{};
    {
        {
            print("vector");
            std::pmr::vector<Foo> vec{ &pool };
            vec.reserve(2);
            {
                print("emplace_back");
                vec.emplace_back(1, "The string should be big enought to avoid the SSO");
                vec.emplace_back(2, "The string should be big enought to avoid the SSO");
                print("~emplace_back");
            }
            print("~vector");
        }
    }
    std::cout << "Can re-use ???? => YES\n";
    {
        {
            print("vector");
            std::pmr::vector<Foo> vec{ &pool };
            vec.reserve(1);
            {
                print("emplace_back");
                vec.emplace_back(3, "The string should be big enought to avoid the SSO");
                print("~emplace_back");
            }
            print("~vector");
        }
    }
    print("~synchronized_pool_resource");
}