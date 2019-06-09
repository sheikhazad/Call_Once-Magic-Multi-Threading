
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;
//For simplicity, entire code is put in single file
class MutexWrongUsage
{
   public:
       MutexWrongUsage(string iFileName): _sFileName(iFileName){}
       void thread_func();
       void writeToFile(const string& sThreadName, const int& iCounter);

       ~MutexWrongUsage()
       {
       	 cout<< "\nFinished writing to file - MutexWrongUsage.txt by all threads, closing file." << endl;
       	 _ofile.close();
	   }

   private:
        ofstream   _ofile;
        string     _sFileName;
        std::mutex _openMutex;
        std::mutex _writeMutex;
};
void MutexWrongUsage::writeToFile(const string& sThreadName, const int& iCounter)
{
	//Wrong use of Mutex (_openMutex) here:
	//1. Lock before checking if file is open.
	//2. Open file ONLY if file is not opened by other thread i.e. file should be opened by ONLY one thread.
	//3. All threads will wait on this lock and check if file is opened even though file is opened only ONCE by ONE thread
	//4. It will cause multiple threads waiting on mutex, lock and unlock unnecessarily,
    //   it wastes CPU cycles and hinders the program from being run concurrently.
	//5. Wrong use of mutex (_openMutex) can be resolved by use of once_flag, which is shown in CallOnceUsage.cpp
	unique_lock<mutex>openLocker(_openMutex);
	if(!_ofile.is_open())
	{
	    cout<< "Opening file - MutexWrongUsage.txt only ONCE by only ONE thread by using mutex." << endl;
		_ofile.open(_sFileName.c_str());
	}
	openLocker.unlock();

   //Lock before writing to file
   lock_guard<mutex>writeLocker(_writeMutex);
   _ofile <<"\n" << sThreadName << " : " << iCounter <<" :: \n" <<endl;
}

void MutexWrongUsage::thread_func()
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
    cout<< "DEMO: Mutex wrong usage:" << endl;
    MutexWrongUsage aMWR("MutexWrongUsage.txt");

    thread th1(&MutexWrongUsage::thread_func, &aMWR );

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
