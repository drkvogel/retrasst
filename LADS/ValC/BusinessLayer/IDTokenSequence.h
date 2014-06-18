#ifndef IDTOKENSEQUENCEH
#define IDTOKENSEQUENCEH

#include "API.h"
#include <vector>

namespace valc
{

class IDTokenSequence
{
private:
    std::vector< IDToken > m_sequence;
public:

    typedef decltype(m_sequence)::const_iterator const_iterator;
    typedef decltype(m_sequence)::iterator iterator;

    IDTokenSequence();
    IDTokenSequence( const IDTokenSequence& other );
    IDTokenSequence& operator=( const IDTokenSequence& other );
    void            push_back( const IDToken& idt );
    bool            contains( const IDToken& idt ) const;
    const_iterator  begin() const;
    iterator        begin();
    void            clear();
    const_iterator  end() const;
    iterator        end();
    void            erase( iterator i );
    iterator        find( const IDToken& idt );
    const_iterator  find( const IDToken& idt ) const;
};

}

#endif

