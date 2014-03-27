
/**
 * VA20_UtilityServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_utilityservice;

    /**
     *  VA20_UtilityServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_UtilityServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_UtilityServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_UtilityServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for getEntityTypes method
            * override this method for handling normal response from getEntityTypes operation
            */
           public void receiveResultgetEntityTypes(
                    com.icoserve.www.va20_utilityservice.VA20_UtilityServiceStub.GetEntityTypesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getEntityTypes operation
           */
            public void receiveErrorgetEntityTypes(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitPermissions method
            * override this method for handling normal response from getOrganizationalUnitPermissions operation
            */
           public void receiveResultgetOrganizationalUnitPermissions(
                    com.icoserve.www.va20_utilityservice.VA20_UtilityServiceStub.GetOrganizationalUnitPermissionsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitPermissions operation
           */
            public void receiveErrorgetOrganizationalUnitPermissions(java.lang.Exception e) {
            }
                


    }
    