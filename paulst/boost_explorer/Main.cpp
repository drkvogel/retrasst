#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <iostream>
#include <iterator>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/regex.hpp>
#include <boost/variant.hpp>

class TimesTwo : public boost::static_visitor<>
{
public:
    void operator()( int&         i ) const { i *= 2; }
    void operator()( std::string& s ) const { s += s;   }
};

class Person
{
public:
    Person( const std::string& name ) : m_name( name ) {}
    Person( const Person& p ) : m_name( p.m_name ) {}
    Person& operator=( const Person& p ) { m_name = p.m_name; return *this; }
    void setName( const std::string& str ) { m_name = str; }
    std::string getName() const { return m_name; }
private:
    std::string m_name;
};

std::ostream& operator<<( std::ostream& o, const Person* p )
{
    o << p->getName();
    return o;
}

void pause()
{
    std::cout << "\nHit [return] to continue...";
    char c;
    std::cin >> std::noskipws >> c;
}

void demoVariant()
{
    typedef boost::variant< int, std::string > IntOrString;
    typedef std::vector< IntOrString > IntOrStringCollection;

    IntOrStringCollection c;
    c.push_back( IntOrString( 1   ) );
    c.push_back( IntOrString( "1" ) );

    std::copy( c.begin(), c.end(),
        std::ostream_iterator<IntOrString>( std::cout, "\n" ) );

    boost::apply_visitor( TimesTwo(), c[0] );
    boost::apply_visitor( TimesTwo(), c[1] );

    std::copy( c.begin(), c.end(),
        std::ostream_iterator<IntOrString>( std::cout, "\n" ) );

    TimesTwo v;
    std::for_each( c.begin(), c.end(), boost::apply_visitor(v) );

    std::copy( c.begin(), c.end(),
        std::ostream_iterator<IntOrString>( std::cout, "\n" ) );
}

void demoBind2nd()
{
    std::vector< Person* > people;
    people.push_back( new Person("Paul") );
    people.push_back( new Person("Jim")  );

    std::copy( people.begin(), people.end(),
        std::ostream_iterator<Person*>( std::cout, "\n" ) );

    std::for_each( people.begin(), people.end(),
        boost::bind2nd( boost::mem_fun(&Person::setName), "Kate") );

    std::copy( people.begin(), people.end(),
        std::ostream_iterator<Person*>( std::cout, "\n" ) );

    BOOST_FOREACH( Person* p, people )
    {
        delete p;
    }
}

void demoForEach()
{
    int array[] = { 1, 2, 3 };

    BOOST_FOREACH( int i, array )
    {
        std::cout << i << ",";
    }
    std::cout << "\n";
}

void demoCaseConv()
{
    std::string input("Hello!");

    std::cout
        << input                         << " "
        << boost::to_upper_copy( input ) << " "
        << boost::to_lower_copy( input ) << "\n";
}

void reportFind( const std::string& text, const std::string& target )
{
    if ( boost::ifind_first( text, target ) )
    {
        std::cout << "Found '"          << target << "' in '" << text << "'\n";
    }
    else
    {
        std::cout << "Failed to find '" << target << "' in '" << text << "'\n";
    }
}

void demoCaseInsensitiveFind()
{
    std::string text("The quick brown fox called Bob jumped over "
        "farmer Dave's fence.");
    std::string targetExists("Called bob"), targetNotExist("calledBob");

    reportFind( text, targetExists );
    reportFind( text, targetNotExist );
}

void demoPredicate()
{
    std::string input("The quick brown fox died.");

    std::cout << "Input: " << input << "\n"
        << "Starts with 'the' (case-insensitive)? "
        << ( boost::istarts_with( input, "the" ) ? "true" : "false" )
        << "\n";

    input = "1234567890";

    std::cout << "Input: " << input << "\n"
        << "All digits? "
        << ( boost::all( input, boost::is_digit() ) ? "true" : "false" )
        << "\n";
}

void demoTrim()
{
    std::string input(" \twhen\tever\t  ");

    std::cout
        << input << "\n"
        << "[" << boost::trim_left_copy ( input ) << "]\n"
        << "[" << boost::trim_right_copy( input ) << "]\n"
        << "[" << boost::trim_copy      ( input ) << "]\n";
}

void demoRegex()
{
    std::string ex("^\\s*(\\d+)\\s*,\\s*(\\d+)$");
    std::string input("1234 , 89");

    std::cout << "Input: " << input << "\n";
    std::cout << "Regex: " << ex    << "\n";

    boost::regex e(ex.c_str());
	boost::smatch matches;

	if ( regex_search( input, matches, e ) )
    {
        std::cout << matches.size() << " matches.\n";

        if ( matches.size() )
        {
            for ( unsigned int i = 0; i < matches.size(); ++i )
            {
                std::string result( matches[i].first, matches[i].second );
                std::cout << result << "\n";
            }
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    demoRegex();
    pause();
    demoCaseConv();
    pause();
    demoTrim();
    pause();
    demoCaseInsensitiveFind();
    pause();
    demoPredicate();
    pause();
    demoForEach();
    pause();
    demoBind2nd();
    pause();
    demoVariant();
    pause();
   	return 0;
}
