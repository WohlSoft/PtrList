/*
PtrList - A vector-like container template which stores elements as array of pointers
to each element to allow taking pointers to each element of container

Copyright (c) 2017-2025 Vitaliy Novichkov <admin@wohlnet.ru>

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

#ifndef PTRLIST_H
#define PTRLIST_H

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _MSC_VER
#ifdef _WIN64
typedef long long ssize_t;
#else
typedef int ssize_t;
#endif
#endif

template<class T>
class PtrList
{
    T** m_data;
    size_t m_size;
    size_t m_sizeReserved;
public:
    class iterator
    {
        friend class PtrList;
        T**     m_data;
        size_t  m_pos;
        iterator(T** d, size_t pos) :
            m_data(d),
            m_pos(pos)
        {}
    public:
        iterator():
            m_data(nullptr),
            m_pos(0)
        {}
        iterator(const iterator& it) :
            m_data(it.m_data),
            m_pos(it.m_pos)
        {}
        bool operator == (const iterator& i)
        {
            return (m_pos == i.m_pos);
        }
        bool operator != (const iterator& i)
        {
            return (m_pos != i.m_pos);
        }
        friend iterator operator+(const iterator &it, int inc)
        {
            iterator i = it;
            i.m_pos += inc;
            return i;
        }
        friend iterator operator-(const iterator &it, int inc)
        {
            iterator i = it;
            i.m_pos -= inc;
            return i;
        }
        iterator& operator++()
        {
            m_pos++;
            return *this;
        }
        iterator& operator++(int)
        {
            m_pos++;
            return *this;
        }
        iterator& operator+=(int inc)
        {
            m_pos += inc;
            return *this;
        }
        iterator& operator-=(int dec)
        {
            m_pos -= dec;
            return *this;
        }
        T& operator*()
        {
            return *(m_data[m_pos]);
        }
    };

    iterator begin() { return iterator(m_data, 0u); }
    iterator end()   { return iterator(m_data, m_size); }

    PtrList() :
        m_data(nullptr),
        m_size(0),
        m_sizeReserved(0)
    {}

    PtrList(size_t size) :
        m_data(nullptr),
        m_size(0),
        m_sizeReserved(0)
    {
        resize(size);
    }

    PtrList(const PtrList& o):
        m_data(nullptr),
        m_size(0),
        m_sizeReserved(0)
    {
        reserve(m_size);
        for(size_t i=0; i < o.m_size; i++)
        {
            m_data[i] = new T(*o.m_data[i]);
        }
        m_size = o.m_size;
    }

    PtrList& operator=(const PtrList& o)
    {
        if (this != &o)
        {
            clear();
            m_size = o.m_size;
            reserve(o.m_size);
            for(size_t i = 0; i < o.m_size; i++)
                m_data[i] = new T(*o.m_data[i]);
        }
        return *this;
    }

    ~PtrList()
    {
        clear();
    }

    void clear()
    {
        if(m_data)
        {
            for(size_t i = 0; i < m_size; i++)
            {
                T* p = m_data[i];
                delete p;
            }
            free(m_data);
        }
        m_data = nullptr;
        m_size = 0;
        m_sizeReserved = 0;
    }

    size_t size() const
    {
        return m_size;
    }

    size_t count() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_sizeReserved;
    }

    void resize(size_t sz)
    {
        if(sz == m_size)
            return;

        if(sz < m_size)
        {
            while(sz < m_size)
            {
                T* p = m_data[(m_size--) - 1];
                delete p;
            }
        }
        else
        {
            if(sz > m_sizeReserved)
                reserve(sz - m_sizeReserved);
            while( m_size < sz )
                m_data[m_size++] = new T;
        }
    }

    void reserve(size_t sz)
    {
        sz += m_sizeReserved;
        size_t newSize = sizeof(T*) * sz;
        if(m_data == 0)
            m_data = (T**)malloc(newSize);
        else
            m_data = (T**)realloc(m_data, newSize);
        assert(m_data);
        m_sizeReserved = sz;
    }

    bool contains(const T& item)
    {
        for(size_t i = 0; i < m_size; i++)
        {
            if(*m_data[i] == item)
                return true;
        }
        return false;
    }

    ssize_t indexOf(const T& item)
    {
        for(size_t i = 0; i < m_size; i++)
        {
            if(*m_data[i] == item)
                return ssize_t(i);
        }
        return -1;
    }

    iterator find(const T& item)
    {
        iterator i = begin();
        for(; i != end(); i++)
        {
            if(*i == item)
                break;
        }
        return i;
    }

    void removeOne(const T& item)
    {
        size_t i = 0, moveTo = m_size;
        for(; i < m_size; i++)
        {
            if(*m_data[i] == item)
            {
                delete m_data[i];
                moveTo = i;
                i++;
                break;
            }
        }
        for(; i < m_size; i++)
        {
            m_data[moveTo++] = m_data[i];
        }
        m_size--;
        m_data[m_size] = nullptr;
    }

    void removeAll(const T& item)
    {
        size_t i = 0, moveTo = m_size;
        for(; i < m_size; i++)
        {
            if(*m_data[i] == item)
            {
                delete m_data[i];
                if(moveTo == m_size)
                    moveTo = i;
                continue;
            }
            if(moveTo != m_size)
                m_data[moveTo++] = m_data[i];
        }
        m_size -= (i - moveTo);
        m_data[m_size] = nullptr;
    }

    iterator erase(iterator pos)
    {
        assert(pos.m_pos <= m_size);
        removeAt(pos.m_pos, 1);
        return iterator(m_data, pos.m_pos);
    }

    iterator erase(iterator from, iterator to)
    {
        assert(from.m_pos <= m_size);
        assert(to.m_pos <= m_size);
        assert(to.m_pos >= from.m_pos);
        if(from == to)
            return from;
        removeAt(from.m_pos, to.m_pos - from.m_pos);
        return iterator(m_data, from.m_pos);
    }

    void removeAt(size_t at)
    {
        assert(m_size > 0);
        size_t i = at + 1, moveTo = at;
        delete m_data[at];
        for(; i < m_size; i++)
        {
            if(moveTo == m_size)
                break;
            m_data[moveTo++] = m_data[i];
        }
        m_size--;
        m_data[m_size] = nullptr;
    }

    void removeAt(size_t at, size_t num)
    {
        assert(m_size > 0);
        assert(m_size >= at + num);
        size_t i = at, del_to = (at + num), moveTo = at;
        while(i < del_to)
            delete m_data[i++];
        for(; i < m_size; i++)
        {
            if(moveTo == m_size)
                break;
            m_data[moveTo++] = m_data[i];
        }
        m_size -= num;
        memset(m_data + m_size, 0, sizeof(T*)*num);
    }

    void pop_back()
    {
        removeAt(m_size - 1);
    }

    void pop_front()
    {
        removeAt(0);
    }

    void swap(size_t from, size_t to)
    {
        assert(m_size > from);
        assert(m_size > to);
        if(from == to)
            return;
        T* tmp       = m_data[from];
        m_data[from] = m_data[to];
        m_data[to]   = tmp;
    }

    void move(size_t from, size_t to)
    {
        assert(m_size > from);
        assert(m_size > to);
        if(from == to)
            return;
        if(from < to)
        {
            T* it = m_data[from];
            while(from < to)
            {
                m_data[from] = m_data[from+1];
                from++;
            }
            m_data[to] = it;
        } else {
            T* it = m_data[from];
            while(from > to)
            {
                m_data[from] = m_data[from-1];
                from--;
            }
            m_data[to] = it;
        }
    }

    void push_back(const T& item)
    {
        if((m_size + 1) >= m_sizeReserved)
            reserve(4);
        m_data[m_size] = new T(item);
        m_size++;
        m_data[m_size] = nullptr;
    }

    void push_front(const T& item)
    {
        insert(0, item);
    }

    void append(const T& item)
    {
        if( (m_size + 1) >= m_sizeReserved)
            reserve(4);
        m_data[m_size] = new T(item);
        m_size++;
        m_data[m_size] = nullptr;
    }

    void append(const PtrList<T> &array)
    {
        reserve(array.size());
        for(const T& t : array)
            this->push_back(t);
    }

    void insert(size_t at, const T& item)
    {
        if(at >= m_size)
            at = m_size;
        if((m_size+1) >= m_sizeReserved)
            reserve(4);
        if(at < m_size)
        {
            unsigned int insertAt = m_size;
            do{
                insertAt--;
                m_data[insertAt+1] = m_data[insertAt];
            }while(insertAt > at);
        }
        m_data[at] = new T(item);
        m_size++;
        m_data[m_size] = nullptr;
    }

    T& last()
    {
        assert(m_size > 0);
        return *(m_data[m_size-1]);
    }

    T& back()
    {
        assert(m_size > 0);
        return *(m_data[m_size-1]);
    }

    T& first()
    {
        assert(m_size > 0);
        return *(m_data[0]);
    }

    T& front()
    {
        assert(m_size>0);
        return *(m_data[0]);
    }

    T &operator[](unsigned int index)
    {
        assert(index < (unsigned int)(m_size));
        return *(m_data[index]);
    }

    T &operator[](int index)
    {
        assert(index >= 0);
        assert(index < int(m_size));
        return *(m_data[index]);
    }

    T &operator[](unsigned long index)
    {
        assert(index < (unsigned long)(m_size));
        return *(m_data[index]);
    }

    T &operator[](long index)
    {
        assert(index >= 0);
        assert(index < long(m_size));
        return *(m_data[index]);
    }

    T &operator[](unsigned long long index)
    {
        assert(index < (unsigned long long)(m_size));
        return *(m_data[index]);
    }

    T &operator[](long long index)
    {
        assert(index >= 0);
        assert(index < (long long)(m_size));
        return *(m_data[index]);
    }

    bool operator==(const PtrList<T>& o2)
    {
        if(m_size != o2.m_size)
            return false;
        for(int i=0; i<m_size; i++)
            if( (*m_data[i]) != (*o2.m_data[i]))
                return false;
        return true;
    }

    bool operator!=(const PtrList<T>& o2)
    {
        return !operator==(o2);
    }
};

#endif // PTRLIST_H
