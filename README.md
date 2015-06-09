# translate
A command line translator for Unix-like systems. It consists of a server part (backend) and a client part (frontend).

The backend is build on top of [PostgreSQL](http://www.postgresql.org/ ) and offers a [RESTful](http://en.wikipedia.org/wiki/Representational_state_transfer) API that is used by the frontend to get the requested translations.

The language material is originally taken from http://www.dict.cc/. Unfortunatelly it is not allowed to share it with the sources, i.e. for getting the language resources it is required to download them from [here](http://www1.dict.cc/translation_file_request.php?l=e).

A finaly remark: this is just a hobby project to play a little bit around with C++ and Unix. Please don't expect to get entire sentences translated! What you will get is an easy- and fast-to-use command line translator for single words.
