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

### Exception

Has the added bonus of being displayed properly by C++ Builder projects:

    catch (Exception &exception) {
        Application->ShowException(&exception);
    } catch (...) {
        try {
            throw Exception("Unknown exception");
        } catch (Exception &exception) {
            Application->ShowException(&exception);
        }
    }

but not standard C++, and again, won't catch `std::exception`, requiring an extra `catch`.

### std::runtime_error

Has std::string constructor, and is a descendant of std::exception. 

<http://stackoverflow.com/questions/1569726/difference-stdruntime-error-vs-stdexception>

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

##