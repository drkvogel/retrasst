#ifndef TutCallbackH
#define TutCallbackH

#include <iostream>
#include <iomanip>
#include <string>
#include "tut.h"

class Tally
{
public:

    int value;
          
    Tally()
     : value(0) 
    {
    }
    
    Tally( const Tally& other )
     : value(other.value) 
    {
    }
    
    Tally& operator=( const Tally& other )
    {
        value = other.value;
        return *this;
    }
    
    void increment()
    {
        ++value;
    }
};

class TutCallback : public tut::callback
{
    Tally total, passed, failed;
    
    public:
        void run_started(){ std::cout << "\n"; }
        void group_started(const std::string& name) 
        { 
            std::cout << std::endl << "Test Group: " 
                << name << std::endl;
        }

        void test_completed(const tut::test_result& tr)
        {
            std::cout << std::left << std::setw(8) << resultAsFriendlyString(tr.result)
                <<  ": " << tr.test << ". " << tr.name << std::endl;
            if ( tr.result != tut::test_result::ok )
            {
                std::cout << std::string(10, ' ') << tr.message << std::endl;
            }
            std::cout << std::flush;
            
            total.increment();
            
            if ( tr.result == tut::test_result::ok )
            {
                passed.increment();
            }
            else
            {
                failed.increment();
            }
        }

        void run_completed()
        {
            std::cout
                << "\n"
                <<    "Total: "  << total.value
                << "; passed: "  << passed.value
                << "; failed: "  << failed.value
                << "\n\n"; }

    private:
        std::string resultAsFriendlyString(int result)
        {
            switch(result)
            {
                case tut::test_result::ok: return "ok";
                case tut::test_result::fail: return "fail";
                case tut::test_result::ex: return "ex";
                case tut::test_result::warn: return "warn";
                case tut::test_result::term: return "term";
                case tut::test_result::ex_ctor: return "ex_ctor";
                default: return "UNRECOGNISED RESULT";
            }
        }
};


#endif

