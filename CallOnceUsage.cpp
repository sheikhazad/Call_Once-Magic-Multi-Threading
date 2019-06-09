
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;
//For simplicity, entire code is put in single file
class CallOnceUsage
{
   public:
       CallOnceUsage(string iFileName): _sFileName(iFileName){}
       void thread_func();
       void writeToFile(const string& sThreadName, const int& iCounter);

       ~CallOnceUsage()
       {
       	 cout<< "\nFinished writing to file - CallOnceUsage.txt by all threads, closing file." << endl;
       	 _ofile.close();
	   }

   private:
        ofstream        _ofile;
        string          _sFileName;
        std::once_flag  _oflag;
        std::mutex      _writeMutex;
};
void CallOnceUsage::writeToFile(const string& sThreadName, const int& iCounter)
{
	//Use of call_once instead of mutex:
	//1. No need to lock before checking if file is open.
	//2. Call_once will be called by ONLY one thread and so file will be opened by ONLY one thread.
	//3. It will avoid multiple threads waiting on mutex, lock and unlock unnecessarily,
	//   thus avoid waste of CPU cycles and avoid hindering the program from being run concurrently.
	//4. Compare it with use of mutex in in MutexWrongUsage.cpp
    std::call_once(_oflag, [&](){
                                  cout<< "Opening file - CallOnceUsage.txt only ONCE by only ONE thread by using call_once." << endl;
                                  _ofile.open(_sFileName.c_str());
                               });


   //Lock before writing to file
   lock_guard<mutex>writeLocker(_writeMutex);
   _ofile <<"\n" << sThreadName << " : " << iCounter <<" :: \n" <<endl;
}

void CallOnceUsage::thread_func()
{
    cout<< "\nWriting from thread 2 [From 0 to 100]: " << endl;
    int i=0;
    while( i<=100 )
    {
        writeToFile("Thread-2", i);
        ++i;
    }
}

int main(int argc, char** argv)
{
    cout<< "DEMO: Using call_once instead of mutex:" << endl;
    CallOnceUsage aMWR("CallOnceUsage.txt");

    thread th1(&CallOnceUsage::thread_func, &aMWR );

    cout<< "Writing from main thread [From -1 to -100]: " << endl;
    int i= -1;
    while( i >= -100 )
    {
        aMWR.writeToFile("Thread-1", i);
        --i;
    }

    th1.join();

    return EXIT_SUCCESS;
}
