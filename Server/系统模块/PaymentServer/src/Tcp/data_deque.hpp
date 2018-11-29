#ifndef _DATA_DEQUE_HPP_
#define _DATA_DEQUE_HPP_

#pragma pack(push, 1)

#include <boost/shared_ptr.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#include "ConstTypes.h"
#include "packet.h"

namespace tcp_con {

using namespace boost::interprocess;

//线程安全队列
class Data_queue
{
public:
    Data_queue() 
    { 
    };

    virtual ~Data_queue()
    { 
    };

    void push_back(TCP_Buffer& tcp_buffer)
    {
        throw(std::runtime_error)("Unimplemented.");
        //use std::swap() to steal buffer.
    }

    void push(size_t dataSize) 
    {

        if (MAX_PACKAGE_DEQUE == _size) {
            throw(std::out_of_range)("Push data when max data queue size.");
            return;
        }

        scoped_lock<interprocess_mutex> lock(_wr_mtx);
        if (!lock) {
            throw(std::runtime_error)("push data mutex error.");
            return;
        }

        _data[_end].finalSize = static_cast<WORD>(dataSize);
        _size++;
        _end = (_end + 1) % MAX_PACKAGE_DEQUE;

    }

    TCP_Buffer& pop()
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
     
    TCP_Buffer& front()
    {

        return _data[_front];

    }

    size_t front_idx()
    {

        return _front;

    }

    TCP_Buffer& end()
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

protected:

    size_t _front = 0;
    size_t _end = 0;
    size_t _size = 0;

    interprocess_mutex      _wr_mtx; //写入锁
    interprocess_mutex      _rd_mtx; //读取锁

    TCP_Buffer _data[MAX_PACKAGE_DEQUE];

};
}

#pragma pack(pop)

#endif