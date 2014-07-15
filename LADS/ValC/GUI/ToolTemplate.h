#ifndef TOOLTEMPLATEH
#define TOOLTEMPLATEH

#include <FMX.Types.hpp>
#include "IdleService.h"
#include "Model.h"
#include <string>
#include "ValCDialogs.h"

namespace valcui
{

/*
In this context, a 'Tool' is a visual component that can be added to the user interface at runtime.

Addition of a Tool to the user interface is achieved by constructing an instance of Tool.
Removing a Tool from the user interface is achieved by deleting the Tool instance.
*/
class Tool
{
public:
    Tool() {}
    virtual ~Tool() {}
    virtual std::string getName() const = 0;
    virtual void onResize() = 0;
private:
    Tool( const Tool& );
    Tool& operator=( const Tool& );
};

/*
An implementation of the Tool interface which consists of a 
View and a Controller.

Takes care of:
    - instantiating the View
    - hooking the View into the user interface
    - associating the View with a Controller
    - linking the Controller to the Model

And the destructor takes care of:
    - unhooking the View from the user interface
    - detaching the Controller from the Model
    - deleting the View
    - deleting the Controller
*/
template<typename Controller, typename View, typename Container>
class ToolTemplate : public Tool
{
public:

    /*
    Note that the View is not passed in as an argument to the constructor. Rather, the View is instantiated by the constructor.
    */
    ToolTemplate( 
        const std::string& name,
        Controller* controller, 
        Container* container, // A visual space which the View should occupy
        Model* model, 
        IdleService* idleService,
        TAlignLayout alignment ) // Describes how the View should occupy the space provided by the Container
        :
        m_name( name ),
        m_controller( controller ),
        m_container( container ),
        m_model( model ),
        m_view( new View( container ) ),
        m_idleService( idleService )
    {
        m_view->Align = alignment;
        m_view->Parent = m_container;
        m_container->AddObject( m_view );
        m_controller->setModel( m_model );
        m_controller->setView( m_view );
        m_controller->init();
        m_model->registerModelEventListener( m_controller->getModelEventListenerInterface() );
        m_idleService->registerUser( m_controller->getIdleServiceUserInterface() );
    }

    std::string getName() const
    {
        return m_name;
    }

    void onResize()
    {
        m_controller->onResize();
    }

    ~ToolTemplate()
    {
        try
        {
            m_model->unregisterModelEventListener( m_controller->getModelEventListenerInterface() );
            m_idleService->unregisterUser( m_controller->getIdleServiceUserInterface() );
            m_container->RemoveComponent( m_view ); // Resume responsibility for lifetime of View
        }
        catch ( const Exception& e )
        {
            showErrorMsg( AnsiString( e.Message.c_str() ).c_str() );
        }
        delete m_view;
        delete m_controller;
    }

private:
    const std::string         m_name;
    Controller*         const m_controller;
    Container*          const m_container;
    Model*              const m_model;
    View*               const m_view;
    IdleService*        const m_idleService;
};

}

#endif

