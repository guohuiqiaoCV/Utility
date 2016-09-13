#ifndef GH_CONCURRENTQUEUE_HPP
#define GH_CONCURRENTQUEUE_HPP

/* ConcurrentQueue:
 *
 * A thread-safe queue. `pop` could be blocked when it is empty.
 *
 * This code is mainly adopted from this git repository: https://gist.github.com/204fdc5f1afa76f5f8e17b4e660eb6e3.git, with bug fixed in the `pop` function.
 */

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


namespace gh {

  typedef unsigned int uint;
  // ----------------------------------------

  template <typename T>
  struct IQueueWriter
  {
    virtual ~IQueueWriter() {}
    virtual void push( std::shared_ptr<T> ) = 0;
  };
  // ----------------------------------------

  template <typename T>
  struct IQueueReader
  {
    virtual ~IQueueReader() {}
    virtual const bool empty() const = 0;
    virtual const size_t size() const = 0;
    virtual std::shared_ptr<T> pop() = 0;
  };
  // ----------------------------------------

  /* Blocking, concurrency-safe queue. The method that blocks is pop(),
   * which makes the current thread wait until there is a value to pop
   * from the queue.
   */
  template <typename T>
  struct ConcurrentQueue : public IQueueWriter<T>, public IQueueReader<T>
  {
    ConcurrentQueue() {}
    ConcurrentQueue( const ConcurrentQueue & ) = delete;
    ConcurrentQueue & operator= ( const ConcurrentQueue & ) = delete;

    /* Concurrency-safe check if queue empty.
     */
    virtual const bool empty() const
    {
      std::lock_guard<std::mutex> lk( _mutex );
       return _queue.empty();
    }

    virtual const size_t size() const
    {
      std::lock_guard<std::mutex> lk( _mutex );
      return _queue.size();
    }

    /* Concurrency-safe push to queue.
     */
    virtual void push( std::shared_ptr<T> val )
    {
      std::lock_guard<std::mutex> lk( _mutex );
      _queue.push( val );
      _cvar.notify_one();
    }

    /* Waiting, concurrency-safe pop of value. If there are no values in
     * the queue, then this method blocks the current thread until there
     * are.
     */
    virtual std::shared_ptr<T> pop()
    {
      std::unique_lock<std::mutex> lk( _mutex );
      _cvar.wait( lk, [ this ] { return ! _queue.empty(); } );
      auto value(
      _queue.front() );
      _queue.pop();
      lk.unlock();
      _cvar.notify_all();
      return value;
    }

  protected:
    mutable std::mutex _mutex;
    std::queue<std::shared_ptr<T>> _queue;
    std::condition_variable _cvar;
  };

}
#endif //GH_CONCURRENTQUEUE_HPP



