#ifndef _DATA_DEQUE_HPP_
#define _DATA_DEQUE_HPP_

#pragma pack(push, 1)

#include "ConstTypes.h"

namespace tool {

using namespace boost::interprocess;

//线程安全队列
template<typename T>
class Data_queue
{
public:

	typedef  void push_func( tool::Data_queue<T>*, size_t );

public:
    Data_queue() 
    { 
    };

    virtual ~Data_queue()
    { 
    };

    void push_back(T& tcp_buffer)
    {
        throw(std::runtime_error)("Unimplemented.");
        //use std::swap() to steal buffer.
    }

    size_t push_back(size_t dataSize, push_func* func = nullptr)
    {

        if (MAX_PACKAGE_DEQUE == _size) {
            throw(std::out_of_range)("Push data when max data queue size.");
            return -1;
        }

        scoped_lock<interprocess_mutex> lock(_wr_mtx);
        if (!lock) {
            throw(std::runtime_error)("push data mutex error.");
            return -1;
        }

		if (func) {
			func(this, dataSize);
		}

		size_t idx = _end;
        _size++;
        _end = (_end + 1) % MAX_PACKAGE_DEQUE;
		return idx;
    }

	T& pop_front()
    {

        if (_size == 0) {
            throw(std::out_of_range)("pop empty data queue.");
            return _data[_front];
        }

        scoped_lock<interprocess_mutex> lock(_rd_mtx);

        if (!lock) {
            throw(std::runtime_error)("pop data mutex error.");
            return _data[_front];
        }

        _size--;
        int idx = _front;
        _front = ((_front + 1) % MAX_PACKAGE_DEQUE);
        return _data[idx];

    }

	T& front()
    {

        return _data[_front];

    }

    size_t front_idx()
    {

        return _front;

    }

	T& end()
    {

        return _data[_end];

    }

    size_t end_idx()
    {

        return _end;

    }

    bool isEmpty()
    {

        return _size == 0;

    }

    bool isFull()
    {

        return _size == MAX_PACKAGE_DEQUE;

    }

    size_t size() 
    {

        return _size;

    }

	T& at(size_t index)
	{

		assert(index >= 0 && index < MAX_PACKAGE_DEQUE);
		if (index < 0 || index >= MAX_PACKAGE_DEQUE) {
			return _data[_end];
		}
		return _data[index];

	}

protected:

    size_t _front = 0;
    size_t _end = 0;
    size_t _size = 0;

    interprocess_mutex      _wr_mtx; //写入锁
    interprocess_mutex      _rd_mtx; //读取锁

	T _data[MAX_PACKAGE_DEQUE];

};
}


#pragma pack(pop)

#endif