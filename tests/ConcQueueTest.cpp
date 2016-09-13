#include <iostream>
#include <chrono>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "../ghUtility/MaxConcurrentQueue.hpp"

using namespace std;

uint N = 10000;
uint Step = N * 0.1;

// block pop thread
bool BlockPopTest() {

  std::cout << ">>> Start block pop test..." << std::endl;
  gh::MaxConcurrentQueue<string> q(1);

  thread popThread( [ & ] {
    cout << "Fast pop thread ..." << endl;
    for ( uint i( 0 ) ; i < N ; ++ i ) {
      string str = *q.pop();
      if(i%Step == 0)
        std::cout << "Poped "<< i/Step*10 << " percent..." << std::endl;
    }
  } );

  thread pushThread([&] {
    cout << "slowly push thread ..." << endl;
    for(uint i(0);i<N;++i) {
        this_thread::sleep_for( chrono::milliseconds( 1 ) );
        q.push( make_shared<string>( "Tossy Halfcakes" ) );
        assert(q.size()<=1);
        if(i%Step == 0)
          std::cout << "Pushed "<< i/Step*10 << " percent..." << std::endl;
    }
  });


  popThread.join();
  pushThread.join();
  assert(q.empty());

  std::cout << ">>> Finish block push test. Succeed!" << std::endl;

  return true;
}

// block push thread
bool BlockPushTest() {

  std::cout << ">>> Start block push test..." << std::endl;

  gh::MaxConcurrentQueue<string> q(1);

  thread popThread( [ & ] {
    //cout << "Instant pop thread ..." << endl;
    for ( uint i( 0 ) ; i < N ; ++ i ) {
      this_thread::sleep_for( chrono::milliseconds( 1 ) );
      string str = *q.pop();
      if(i%Step == 0)
        std::cout << "Poped "<< i/Step*10 << " percent..." << std::endl;
    }
  });

  thread pushThread([&] {

    for(uint i(0);i<N;++i) {
        q.push( make_shared<string>( "Tossy Halfcakes" ) );
        assert(q.size()<=1);
        if(i%Step == 0)
          std::cout << "Pushed " << i/Step*10 << " percent..." << std::endl;
    }
  });

  popThread.join();
  pushThread.join();
  assert(q.empty());

  std::cout << ">>> Finish block push test. Succeed!" << std::endl;
  return true;
}

// random block push or pop
bool RandomBlockTest() {

    std::srand(std::time(0)); // set seed with current time

    std::cout << ">>> Start random block test..." << std::endl;
    gh::MaxConcurrentQueue<string> q(1);

    thread popThread( [ & ] {
      cout << "Spawn pop thread ..." << endl;
      for ( uint i( 0 ) ; i < N ; ++ i ) {
        this_thread::sleep_for( chrono::milliseconds( std::rand()%10+1 ) );
        string str = *q.pop();
        if(i%Step == 0)
          std::cout << "Poped "<< i/Step*10 << " percent..." << std::endl;
      }
    } );

    thread pushThread([&] {
      cout << "Spawn push thread ..." << endl;
      for(uint i(0);i<N;++i) {
          this_thread::sleep_for( chrono::milliseconds( std::rand()%10+1 ) );
          q.push( make_shared<string>( "Tossy Halfcakes" ) );
          assert(q.size()<=1);
          if(i%Step == 0)
            std::cout << "Pushed "<< i/Step*10 << " percent..." << std::endl;
      }
    });

    popThread.join();
    pushThread.join();
    assert(q.empty());

    std::cout << ">>> Finish random block test. Succeed!" << std::endl;

    return true;

}

int main(){

  BlockPushTest();

  BlockPopTest();

  RandomBlockTest();

  std::cout << "<<< Passed All Tests!!! <<<" << std::endl;

  return 0;
}
