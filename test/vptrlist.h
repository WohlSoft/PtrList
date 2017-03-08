/*
VPtrList - implementation of PtrList based on std::vector<std::shared_ptr<T>> inherence
           Experimental, created for benchmarking

Copyright (c) 2017 Vitaliy Novichkov <admin@wohlnet.ru>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef VPTRLIST_H
#define VPTRLIST_H

#include <vector>
#include <memory>

template<class T>
class VPtrList : public std::vector<std::unique_ptr<T>>
{
public:
    typedef std::unique_ptr<T> SHptr;
    typedef std::vector<SHptr> vecPTR;
    typedef typename vecPTR::iterator iterator;
    typedef typename vecPTR::const_iterator const_iterator;
    VPtrList() : vecPTR()
    {}

    VPtrList(size_t size) : vecPTR(size)
    {}

    iterator begin() { return vecPTR::begin(); }
    iterator end()   { return vecPTR::end(); }

    void push_back(const T& item)
    {
        vecPTR::push_back(SHptr(new T(item)));
    }

    bool contains(const T& item)
    {
        size_t s = vecPTR::size();
        SHptr* d = vecPTR::data();
        for(size_t i = 0; i < s; i++)
        {
            if(*d[i] == item)
                return true;
        }
        return false;
    }
};



#endif // VPTRLIST_H
