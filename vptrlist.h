/*
VPtrList - implementation of PtrList based on std::vector<std::shared_ptr<T>> inherence
           implements same API as PtrList, but works faster and has things as std::vector has

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
#include <assert.h>

template<class T>
class VPtrList : public std::vector<std::unique_ptr<T>>
{
public:
    typedef std::unique_ptr<T> SHptr;
    typedef std::vector<SHptr> vecPTR;
    typedef typename vecPTR::iterator       S_iterator;
    typedef typename vecPTR::const_iterator S_const_iterator;

    template<typename TT, class SIterator>
    class VPtrIterator : public SIterator
    {
    public:
        VPtrIterator(const SIterator&o) : SIterator(o) {}
        virtual ~VPtrIterator() {}

        TT& operator*()
        {
            return *(SIterator::operator*());
        }

        friend VPtrIterator operator+(const VPtrIterator &it, int inc)
        {
            S_iterator i = it;
            i += inc;
            return iterator(i);
        }
        friend VPtrIterator operator-(const VPtrIterator &it, int inc)
        {
            S_iterator i = it;
            i -= inc;
            return iterator(i);
        }
        friend VPtrIterator operator+(const VPtrIterator &it1, const VPtrIterator &it2)
        {
            return iterator(it1 + it2);
        }
        friend VPtrIterator operator-(const VPtrIterator &it1, const VPtrIterator &it2)
        {
            return iterator(it1 - it2);
        }
        VPtrIterator& operator++()
        {
            SIterator::operator++();
            return *this;
        }
        VPtrIterator& operator++(int)
        {
            SIterator::operator++();
            return *this;
        }
        VPtrIterator& operator+=(int inc)
        {
            SIterator::operator+=(inc);
            return *this;
        }
        VPtrIterator& operator-=(int dec)
        {
            SIterator::operator-=(dec);
            return *this;
        }
        VPtrIterator& operator+=(const VPtrIterator &inc)
        {
            SIterator::operator+=(inc);
            return *this;
        }
        VPtrIterator& operator-=(const VPtrIterator &dec)
        {
            SIterator::operator-=(dec);
            return *this;
        }
    };

    typedef VPtrIterator<T, S_iterator>             iterator;
    typedef VPtrIterator<const T, S_const_iterator> const_iterator;

    VPtrList() : vecPTR()
    {}

    VPtrList(size_t size) : vecPTR(size)
    {}

    VPtrList& operator=(const VPtrList& o)
    {
        if (this != &o)
        {
            vecPTR::clear();
            this->append(o);
        }
        return *this;
    }

    iterator begin() { return vecPTR::begin(); }
    iterator end()   { return vecPTR::end(); }
    const_iterator begin() const { return vecPTR::begin(); }
    const_iterator end() const   { return vecPTR::end(); }

    size_t count() const
    {
        return vecPTR::size();
    }

    bool contains(const T& item)
    {
        return indexOf(item) >= 0;
    }

    ssize_t indexOf(const T& item)
    {
        size_t s = vecPTR::size();
        SHptr* d = vecPTR::data();
        size_t i = 0;
        for(; i < s; i++)
        {
            if(*d[i] == item)
                return ssize_t(i);
        }
        return -1;
    }

    iterator find(const T& item)
    {
        iterator i = begin();
        for(; i != end(); i++)
        {
            if(*i->get() == item)
                break;
        }
        return i;
    }

    void removeOne(const T& item)
    {
        iterator i = begin();
        for(; i != end(); i++)
        {
            if(*(i->get()) == item)
            {
                vecPTR::erase(i);
                break;
            }
        }
    }

    void removeAll(const T& item)
    {
        iterator i = begin();
        for(; i != end();)
        {
            if(*i->get() == item)
            {
                i == vecPTR::erase(i);
            } else {
                i++;
            }
        }
    }

    iterator erase(iterator pos)
    {
        assert(pos < end());
        return iterator(vecPTR::erase(pos));
    }

    iterator erase(iterator from, iterator to)
    {
        assert(from < end());
        assert(to < end());
        assert(from <= to);
        if(from == to)
            return from;
        return iterator(vecPTR::erase(from, to));
    }

    void removeAt(size_t at)
    {
        vecPTR::erase(begin() + at);
    }

    void removeAt(size_t at, size_t num)
    {
        vecPTR::erase(begin() + at, begin() + (at+num));
    }

    void pop_front()
    {
        vecPTR::erase(begin(), begin() + 1);
    }

    void swap(size_t from, size_t to)
    {
        if(from == to)
            return;
        std::swap(*(vecPTR::begin() + from), *(vecPTR::begin() + to));
    }

    void move(size_t from, size_t to)
    {
        size_t m_size = vecPTR::size();
        SHptr* m_data = vecPTR::data();
        assert(m_size > from);
        assert(m_size > to);
        if(from == to)
            return;
        if(from < to)
        {
            SHptr it = std::move(m_data[from]);
            while(from < to)
            {
                m_data[from] = std::move(m_data[from+1]);
                from++;
            }
            m_data[to] = std::move(it);
        } else {
            SHptr it = std::move(m_data[from]);
            while(from > to)
            {
                m_data[from] = std::move(m_data[from-1]);
                from--;
            }
            m_data[to] = std::move(it);
        }
    }

    void push_back(const T& item)
    {
        vecPTR::push_back(SHptr(new T(item)));
    }

    void push_front(const T& item)
    {
        vecPTR::insert(begin(), SHptr(new T(item)));
    }

    void append(const T& item)
    {
        vecPTR::push_back(SHptr(new T(item)));
    }

    void append(const VPtrList<T> &array)
    {
        vecPTR::reserve(array.size());
        for(const T& t : array)
            vecPTR::push_back(SHptr(new T(t)));
    }

    void insert(size_t at, const T& item)
    {
        vecPTR::insert(begin() + at, SHptr(new T(item)));
    }

    T& last()
    {
        assert(vecPTR::size() > 0);
        return *vecPTR::back();
    }

    T& first()
    {
        assert(vecPTR::size() > 0);
        return *vecPTR::front();
    }

    T& back()
    {
        assert(vecPTR::size() > 0);
        return *vecPTR::back();
    }

    T& front()
    {
        assert(vecPTR::size() > 0);
        return *vecPTR::front();
    }

    T &operator[](unsigned int index)
    {
        assert(index < static_cast<unsigned int>(vecPTR::size()));
        return *(vecPTR::at(index));
    }

    T &operator[](int index)
    {
        assert(index >= 0);
        assert(index < static_cast<int>(vecPTR::size()));
        return *(vecPTR::at(index));
    }

    T &operator[](unsigned long index)
    {
        assert(index < static_cast<unsigned long>(vecPTR::size()));
        return *(vecPTR::at(index));
    }

    T &operator[](long index)
    {
        assert(index >= 0);
        assert(index < static_cast<unsigned long>(vecPTR::size()));
        return *(vecPTR::at(index));
    }

    T &operator[](unsigned long long index)
    {
        assert(index < static_cast<unsigned long long>(vecPTR::size()));
        return *(vecPTR::at(index));
    }

    T &operator[](long long index)
    {
        assert(index >= 0);
        assert(index < static_cast<long long>(vecPTR::size()));
        return *(vecPTR::at(index));
    }
};

#endif // VPTRLIST_H
