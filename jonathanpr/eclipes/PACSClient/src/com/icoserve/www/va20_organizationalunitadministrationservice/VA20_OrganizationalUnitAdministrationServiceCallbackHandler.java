
/**
 * VA20_OrganizationalUnitAdministrationServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_organizationalunitadministrationservice;

    /**
     *  VA20_OrganizationalUnitAdministrationServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_OrganizationalUnitAdministrationServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_OrganizationalUnitAdministrationServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_OrganizationalUnitAdministrationServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for createOrganizationalUnit method
            * override this method for handling normal response from createOrganizationalUnit operation
            */
           public void receiveResultcreateOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.CreateOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createOrganizationalUnit operation
           */
            public void receiveErrorcreateOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findOrganizationalUnits method
            * override this method for handling normal response from findOrganizationalUnits operation
            */
           public void receiveResultfindOrganizationalUnits(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.FindOrganizationalUnitsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findOrganizationalUnits operation
           */
            public void receiveErrorfindOrganizationalUnits(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteOrganizationalUnit method
            * override this method for handling normal response from deleteOrganizationalUnit operation
            */
           public void receiveResultdeleteOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.DeleteOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteOrganizationalUnit operation
           */
            public void receiveErrordeleteOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnit method
            * override this method for handling normal response from getOrganizationalUnit operation
            */
           public void receiveResultgetOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.GetOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnit operation
           */
            public void receiveErrorgetOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsOrganizationalUnit method
            * override this method for handling normal response from existsOrganizationalUnit operation
            */
           public void receiveResultexistsOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.ExistsOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsOrganizationalUnit operation
           */
            public void receiveErrorexistsOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitPath method
            * override this method for handling normal response from getOrganizationalUnitPath operation
            */
           public void receiveResultgetOrganizationalUnitPath(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.GetOrganizationalUnitPathResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitPath operation
           */
            public void receiveErrorgetOrganizationalUnitPath(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitChildren method
            * override this method for handling normal response from getOrganizationalUnitChildren operation
            */
           public void receiveResultgetOrganizationalUnitChildren(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.GetOrganizationalUnitChildrenResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitChildren operation
           */
            public void receiveErrorgetOrganizationalUnitChildren(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renameOrganizationalUnit method
            * override this method for handling normal response from renameOrganizationalUnit operation
            */
           public void receiveResultrenameOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.RenameOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renameOrganizationalUnit operation
           */
            public void receiveErrorrenameOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateOrganizationalUnit method
            * override this method for handling normal response from updateOrganizationalUnit operation
            */
           public void receiveResultupdateOrganizationalUnit(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.UpdateOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateOrganizationalUnit operation
           */
            public void receiveErrorupdateOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitTree method
            * override this method for handling normal response from getOrganizationalUnitTree operation
            */
           public void receiveResultgetOrganizationalUnitTree(
                    com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.GetOrganizationalUnitTreeResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitTree operation
           */
            public void receiveErrorgetOrganizationalUnitTree(java.lang.Exception e) {
            }
                


    }
    