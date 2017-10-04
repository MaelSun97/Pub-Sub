CSE.30341.FA17: Project 03
==========================

This is the documentation for [Project 03] of [CSE.30341.FA17].

Members
-------

1. Domer McDomerson (dmcdomer@nd.edu)
2. Belle Fleur (bfleur@nd.edu)

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

Response.

> 2. The client library needs to provide a `Callback` class.
>
>   - What does the `Callback` class need to keep track of?
>
>   - How will applications use this `Callback` class?

Response.

> 3. The client library needs to provide a `Message` struct.
>
>   - What does the `Message` struct need to keep track of?
>
>   - What methods would be useful to have in processing `Messages`?

Response.

> 4. The client library needs to provide a `Thread` class.
>
>   - What does the `Thread` class need to keep track of?
>
>   - What POSIX thread functions will need to utilize?


Response.

> 5. You will need to perform testing on your client library.
>
>   - How will you test your client library?
>
>   - What will `echo_test` tell you?
>
>   - How will you use the Google Test framework?
>
>   - How will you incorporate testing at every stage of development?

Response.

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

Response.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].

Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.

[Project 03]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project03.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
