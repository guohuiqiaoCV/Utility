#ifndef GH_MAXCONCURRENTQUEUE_HPP
#define GH_MAXCONCURRENTQUEUE_HPP
/* MaxConcurrentQueue enables both push block and pop block. A maximum lenght is
 * instialized for the push block.
 */
#include "ConcurrentQueue.hpp"

namespace gh {
  template <typename T>
  class MaxConcurrentQueue: public ConcurrentQueue<T> {
    using ConcurrentQueue<T>::_cvar;
    using ConcurrentQueue<T>::_queue;
    using ConcurrentQueue<T>::_mutex;
  public:
    MaxConcurrentQueue(uint maxSize = 1): _maxSize(maxSize) {}

    /// Push element to the deque. This blocks until there is free space.
    /// @param elem element to add
    virtual void push(std::shared_ptr<T> elem) {

          std::unique_lock<std::mutex> locker(_mutex);
          _cvar.wait(locker, [ this ]() { return _queue.size() < _maxSize; });

          _queue.push(elem);
          locker.unlock();
          _cvar.notify_all();
    }

  protected:
    uint _maxSize;
  };
}
#endif // GH_MAXCONCURRENTQUEUE_HPP
