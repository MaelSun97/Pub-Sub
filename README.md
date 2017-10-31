CSE.30341.FA17: Project 03
==========================

This is the documentation for [Project 03] of [CSE.30341.FA17].

Members
-------

1. Alison Lui (alui@nd.edu)
2. Ivy Wang (jwang28@nd.edu)
3. Ruochen Sun (rsun2@nd.edu)

Design
------

> 1. The client library needs to provide a `Client` class.
>
>   - What does the `Client` class need to keep track of?
>
>   - How will the `Client` connect to the server?
>
>   - How will it implement concurrent publishing, retrieval, and processing of
>     messages?
>
>   - How will it pass messages between different threads?
>
>   - What data members need to have access synchronized? What primitives will
>     you use to implement these?
>
>   - How will threads know when to block or to quit?
>
>   - How will the `Client` determine which `Callbacks` belong to which topics?

The Client class needs to keep track of the server host, server port, client identify, a queue for outgoing messagesi, and another queue for incoming messages.
Through network sockets.
Using threads.
Using concurrent queues.
Queues. Locks + condition variables
When the user call disconnect on the client a flag will be set and the threads will check that flag using a function with locks.
The subscribe method will associate topics with callbacks using a map.

> 2. The client library needs to provide a `Callback` class.
>
>   - What does the `Callback` class need to keep track of?
>
>   - How will applications use this `Callback` class?

One method.
They will write handlers by creating inherited classes of Callback.

> 3. The client library needs to provide a `Message` struct.
>
>   - What does the `Message` struct need to keep track of?
>
>   - What methods would be useful to have in processing `Messages`?

message type, message topic, message sender, sender's nonce, message body
We prefer not to have any
> 4. The client library needs to provide a `Thread` class.
>
>   - What does the `Thread` class need to keep track of?
>
>   - What POSIX thread functions will need to utilize?

A single thread
pthread\_create, pthread\_wait, pthread\_join

> 5. You will need to perform testing on your client library.
>
>   - How will you test your client library?
>
>   - What will `echo_test` tell you?
>
>   - How will you use the Google Test framework?
>
>   - How will you incorporate testing at every stage of development?

Use google test to unit test the functions. Echo test will tell make sure the client constructor/publish/subscribe/unsubscribe/disconnect functions are working, that the queue is working, and that the publish/retrieve/callback threads are working and if it works consistently should help confirm that there are no race conditions. We will use google test to run functions and make assertions/expectations to make sure that individual functions/classes are working as intended. We will test functions and classes as they are completed incrementally.

> 6. You will need to create an user application that takes advantage of the
>    pub/sub system.
>
>   - What distributed and parallel application do you plan on building?
>
>   - How will utilize the client library?
>
>   - What topics will you need?
>
>   - What callbacks will you need?
>
>   - What additional threads will you need?

We plan on making a chat app that allows two users to run the program at the same time and communicate with each other through the server. Each user will have one instantiation of the client and will publish messages to a topic specific to that user. It will subscribe to the topic of the other user. The callback will just be echocallback because we just want to dump the message to the screen. We will need a thread that receives that messages and one that constantly waits for input from the user and sends any messages that the user types.

Demonstration
-------------

> <https://docs.google.com/a/nd.edu/presentation/d/1ioWgWBQU5V-ueQI8lhIlPnUwuuXSFaBYEiVGzKDyoN4/edit?usp=sharing>

Errata
------

> This meets all the requirements to the best of our knowledge. (2 valgrind errors were okay'd by Professor Bui) 

Extra Credit
------------

> No extra credit was attempted.

[Project 03]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project03.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
