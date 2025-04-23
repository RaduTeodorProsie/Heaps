#include "src/Quaternary.cpp"
#include "src/Rheap.cpp"
#include "src/pairing.cpp"
#include "src/priority_queue.cpp"
#include <iostream>
#include <fstream>

#include <chrono>
#include <iomanip>


template <typename T>
void Solve(T A, T B,std::string file)
{
  std::ifstream fin(file.c_str());
  
  int n;
  int operatie, unde, val, x;
  fin >> n;
  for (int i = 1; i <= n; i++)
  {
    fin >> operatie;
    if (operatie == 1) /// PUSH
    {
      fin >> unde >> val;
      if (!unde)
        A.push(val);
      else
        B.push(val);
    }
    else if (operatie == 2) /// POP
    {
      if (!A.empty())
        A.pop();
    }
    else if (operatie == 3) /// TOP
    {
      if (!A.empty())
        x = A.top();
    }
    else if (operatie == 4) /// SWAP
    {
      A.swap(B);
    }
    else if (operatie == 5) /// MELD
    {
      A.meld(B);
      T bb;
      B = bb;
    }
  }
  fin.close();
}

template <typename heap>
void Test(heap a,heap b)
{
  std::string path = "teste/fisiere/test",ending = ".txt",to_open; 

  for(int i=1;i<=6;i++)
  { 
    to_open = path + std::to_string(i) + ending;
  
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    start = std::chrono::high_resolution_clock::now();
    
    Solve(a,b,to_open);
  
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << std::setprecision(4) << 1.0 * elapsed.count() / 1000 << " seconds for test" << std::to_string(i) << "\n";
  }
}

int main()
{

  /*declarare : nume_heap<clasa, comparator (nu e obligatoriu)> nume
   *functii:
   *push, pop, top
   *heap1.swap(heap2) -> face swap intre cele doua heapuri
   *heap1.meld(heap2) -> insereaza heapul 2 in heapul 1. nu se garanteaza nimic
   *in legatura cu heap2 dupa aceasta operatie
   */
  
  // PriorityQueue<int> pq1,pq2;
  // std::cout << "PriorityQueue\n";
  // Test(pq1,pq2);
  // std::cout << "-------------\n\n";

  // PairingHeap<int> ph1,ph2;              ///  crapa pe T5
  // std::cout << "Pairing Heap\n";
  // Test(ph1,ph2);
  // std::cout << "-------------\n\n";

  // Rheap<int> rh1,rh2;                    /// seg fault
  // std::cout << "Randomized Heap\n";
  // Test(rh1,rh2);
  // std::cout << "-------------\n\n";

  // Quaternary<int> qh1,qh2;
  // std::cout << "Quaternary Heap\n";
  // Test(qh1,qh2);
  // std::cout << "-------------\n\n";
}