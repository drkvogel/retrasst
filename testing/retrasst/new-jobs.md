 
# New Test jobs following 2.7.2 database rebuild

## 978253 "Retrieval_978253", "Retrieving THRIVE samples (with secondary)"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

No storage details found.

## 978238 "Retrieval_978238", "Several REVEAL boxes for Chris"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

unknown aliquot type -211291 for this job


## 978243 "Retrieval_978243", "Reveal boxes of three types"

    primary: [-31781] EDTA_1 secondary: [-31782] EDTA_2

unknown aliquot type -211291 for this job


---

## StoreDAO

Anything useful? Should be using?

---

    select retrieval_cid, external_name, description, primary_aliquot, secondary_aliquot, project_cid, status, reason  from c_retrieval_job

---



## Universal exception handling

What I want to do is have a consistent way of dealing with exceptions that doesn't involve messy multiple catches for different types, and hopefully has a simple syntax for generating on-the-fly exception messages, so you can include some useful data. 

In C++11, which we are now using, a function `std::to_string()` turns numeric values into strings which can be concatenated, so you could do things like:

    throw my_exception("this is " + to_string(1) + " exception");

except that there is no overload for int, only for `long long`, `unsigned long long` and `long double` so ints must be cast to `long long`:

    throw my_exception("this is " + to_string((long long)1) + " exception");

### (...) catch-all

You can rethrow from within a catch which may help in some situations, but you can't get much (any?) info about the exception at this stage

### std::exception

Thrown by many std library functions, class hierarchy allows dealing with subsets of similar exceptions. Unfortunately (stupidly?), doesn't have a `std::string` constructor, so this:

    throw std::exception("this is " + to_string(1) + " exception");

doesn't work. This:

    throw std::exception(("this is " + to_string(1) + " exception").c_str());

will compile, but what might have happened to the memory that the temporary string was using when the exception is caught and the string has gone out of scope? Maybe that's why it doesn't have a string constructor.

### (const) char *

OK, but difficult to put parameters in the message. You'd have to allocate some memory to hold the string. How would you free it?

### std::string

Good, but won't catch any `std::exception`s, so we would still need an extra `catch`.

### std::runtime_error

Has std::string constructor, and is a descendant of std::exception. So:

## Conclusion

Throw exceptions of type `std::runtime_error`, which can be given a concatenated string message argument, and catch `std::exception`s, which include `std::runtime_error`.

## Aside

Why doesn't C++ Builder show the string that a `char *`, `std::exception`, `std::runtime_error` exception represents? What the popup alert actually says is (e.g.):

    Project Storeman.exe raised exception class std::runtime_error with message 'Exception Object Address: 0x7B0F088'.

Well, that's:
1) not very helpful
2) not what the string argument to `std::runtime_error` was, and
3) not what comes out of `e.what()`. 

How come?