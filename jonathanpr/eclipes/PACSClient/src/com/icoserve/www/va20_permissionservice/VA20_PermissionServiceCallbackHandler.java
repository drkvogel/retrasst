
/**
 * VA20_PermissionServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_permissionservice;

    /**
     *  VA20_PermissionServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_PermissionServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_PermissionServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_PermissionServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for authenticateUser method
            * override this method for handling normal response from authenticateUser operation
            */
           public void receiveResultauthenticateUser(
                    com.icoserve.www.va20_permissionservice.VA20_PermissionServiceStub.AuthenticateUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from authenticateUser operation
           */
            public void receiveErrorauthenticateUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitPermissionsForUser method
            * override this method for handling normal response from getOrganizationalUnitPermissionsForUser operation
            */
           public void receiveResultgetOrganizationalUnitPermissionsForUser(
                    com.icoserve.www.va20_permissionservice.VA20_PermissionServiceStub.GetOrganizationalUnitPermissionsForUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitPermissionsForUser operation
           */
            public void receiveErrorgetOrganizationalUnitPermissionsForUser(java.lang.Exception e) {
            }
                


    }
    