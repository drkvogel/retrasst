#ifndef NOTIFIABLEH
#define NOTIFIABLEH

namespace paulst
{

class Notifiable
{
public:
    virtual ~Notifiable() {}
    virtual void notify() = 0;
};

}

#endif

