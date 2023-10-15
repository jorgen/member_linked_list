/******************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2015 Jørgen Lind
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in
 ** all copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 ** THE SOFTWARE.
 ******************************************************************************/

#ifndef MEMBERLINKEDLIST_H
#define MEMBERLINKEDLIST_H

#include <string>
#include <assert.h>

template <typename T>
class MemberListNode
{
public:
    MemberListNode()
        : _next()
        , _prev()
    {
    }

    MemberListNode(MemberListNode const&) = delete;
    MemberListNode& operator=(MemberListNode const&) = delete;

    T *next() const { return _next; }
    T *previous() const { return _prev; }
private:
    T *_next;
    T *_prev;
    template<typename U, MemberListNode<U> U::*>
    friend class MemberList;
};

template <typename T, MemberListNode<T> T::* Member>
class MemberList
{
public:
    class iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
    public:
        iterator()
            : current()
            , list()
        {}

        iterator(T *data, const MemberList *list)
            : current(data)
            , list(list)
        {}

        iterator& operator++()
        {
            assert(current);
            current = (current->*Member).next();
            return *this;
        }

        iterator operator++(int)
        {
            assert(current);
            MemberList::iterator self = *this;
            current = (current->*Member).next();
            return self;
        }

        iterator& operator--()
        {
            if (current)
                current = (current->*Member).previous();
            else if (list)
                current = list->_last;
            return *this;
        }

        iterator operator--(int)
        {
            MemberList::iterator self = *this;
            if (current)
                current = (current->*Member).previous();
            else if (list)
                current = list->_last;
            return *this;
        }

        bool operator==(const iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const iterator &other) const
        {
            return current != other.current;
        }

        T *operator->() const
        {
            return current;
        }

        MemberListNode<T> operator*() const
        {
            return current->*Member;
        }
    private:
        T *current;
        const MemberList *list;
        friend class MemberList;
    };

    MemberList()
        : _first()
        , _last()
    { }

    void push_front(T *data)
    {
        assert(data);
        if (_first)
          (_first->*Member)._prev = data;
        (data->*Member)._next = _first;
        _first = data;
        (data->*Member)._prev = nullptr;
        if (!_last)
            _last = data;
    }

    void prepend(T *data) { push_front(data); }

    void pop_front()
    {
        assert(_first);
        T *next = (_first->*Member)._next;
        (_first->*Member)._next = nullptr;
        (_first->*Member)._prev = nullptr;
        _first = next;
        if (_first)
            (_first->*Member)._prev = nullptr;
        else
            _last = nullptr;
    }

    void push_back(T *data)
    {
        (data->*Member)._prev = _last;
        if (_last)
            (_last->*Member)._next = data;
       (data->*Member)._next = nullptr;
        _last = data;
        if (!_first)
            _first = data;
    }

    void append(T *data) { push_back(data); }

    void pop_back()
    {
        assert(_last);
        T *newLast = (_last->*Member)._prev;
        (_last->*Member)._prev = nullptr;
        if (newLast)
        {
            _last = newLast;
            (newLast->*Member)._next = nullptr;
        }
        else
        {   _last = nullptr;
            _first = nullptr;
        }

    }

    static const MemberListNode<T> &getListNode(const T *data)
    {
        return data->*Member;
    }

    static MemberListNode<T> &getListNode(T *data)
    {
        return data->*Member;
    }

    T *first() const { return _first; }
    T *last() const { return _last; }

    iterator insert(const iterator &it, T *data)
    {
        assert(it.list == this);
        if (it.current) {
            MemberListNode<T> &dataNode = data->*Member;
            MemberListNode<T> &currentNode = it.current->*Member;
            if (currentNode._prev)
                (currentNode._prev->*Member)._next = data;
            else
                _first = data;
            dataNode._prev = currentNode._prev;
            dataNode._next = it.current;
            currentNode._prev = data;
        } else {
            push_back(data);
        }
        return iterator(data, this);
    }

    iterator moveList(const iterator &it, MemberList &list)
    {
        assert(it.list == this);
        if (!list._first)
            return it;
        if (it.current) {
            MemberListNode<T> &currentNode = it.current->*Member;
            if (currentNode._prev)
                (currentNode._prev->*Member)._next = list._first;
            else
                _first = list._first;
            (list._first->*Member)._prev = currentNode._prev;
            (list._last->*Member)._next = it.current;
            currentNode._prev = list._last;
        } else {
            if (!_first) {
                _first = list._first;
                _last = list._last;
            } else {
                (_last->*Member)._next = list._first;
                (list._first->*Member)._prev = _last;
                _last = list._last;
            }
        }
        iterator retIt(list._first, this);
        list._first = nullptr;
        list._last = nullptr;
        return retIt;
    }

    iterator erase(const iterator &it)
    {
        assert(it.list == this);
        assert(it.current);
        MemberListNode<T> &currentNode = it.current->*Member;
        if (currentNode._prev)
            (currentNode._prev->*Member)._next = currentNode._next;
        else
            _first = currentNode._next;
        if (currentNode._next)
            (currentNode._next->*Member)._prev = currentNode._prev;
        else
            _last = currentNode._prev;

        iterator retIt(currentNode._next, this);
        currentNode._next = nullptr;
        currentNode._prev = nullptr;
        return retIt;
    }

    typedef std::reverse_iterator<iterator> reverse_iterator;

    iterator begin() const { return iterator(_first, this); }
    iterator end() const { return iterator(nullptr, this); }
private:
    T *_first;
    T *_last;
};

#endif //MEMBERLINKEDLIST_H
