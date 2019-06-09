# Call_Once-Magic-Multi-Threading
This project shows how std::call_once magically restrict only one thread to call a block of code and avoid wrong use of mutex to protect data, thus increase efficiency. Once a thread calls call_once, then rest of the thread will not call it.

USE OF CALL ONCE INSTEAD OF MUTEX:

1. No need to lock before checking if file is open.
2. Call_once will be called by ONLY one thread and so file will be opened by ONLY one thread.
3. It will avoid multiple threads waiting on mutex, lock and unlock unnecessarily,
   thus avoid waste of CPU cycles and avoid hindering the program from being run concurrently.
4. Compare it with use of mutex in in MutexWrongUsage.cpp

WRONG USE OF MUTEX INSTEAD OF CALL_ONCE:

1. All threads will wait on this lock and check if file is opened even though file is already opened by one of the thread
2. It will cause multiple threads waiting on mutex, lock and unlock unnecessarily,
   it wastes CPU cycles and hinders the program from being run concurrently.
3. Wrong use of mutex can be resolved by use of once_flag, which is shown in CallOnceUsage.cpp

