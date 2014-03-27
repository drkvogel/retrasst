
/**
 * VA20_UserAdministrationServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_useradministrationservice;

    /**
     *  VA20_UserAdministrationServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_UserAdministrationServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_UserAdministrationServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_UserAdministrationServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for renameUser method
            * override this method for handling normal response from renameUser operation
            */
           public void receiveResultrenameUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.RenameUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renameUser operation
           */
            public void receiveErrorrenameUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for userExists method
            * override this method for handling normal response from userExists operation
            */
           public void receiveResultuserExists(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.UserExistsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from userExists operation
           */
            public void receiveErroruserExists(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for setUserExternallyManaged method
            * override this method for handling normal response from setUserExternallyManaged operation
            */
           public void receiveResultsetUserExternallyManaged(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.SetUserExternallyManagedResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from setUserExternallyManaged operation
           */
            public void receiveErrorsetUserExternallyManaged(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findUsers method
            * override this method for handling normal response from findUsers operation
            */
           public void receiveResultfindUsers(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.FindUsersResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findUsers operation
           */
            public void receiveErrorfindUsers(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for setOrganizationalUnitRolesForUser method
            * override this method for handling normal response from setOrganizationalUnitRolesForUser operation
            */
           public void receiveResultsetOrganizationalUnitRolesForUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.SetOrganizationalUnitRolesForUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from setOrganizationalUnitRolesForUser operation
           */
            public void receiveErrorsetOrganizationalUnitRolesForUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getRoles method
            * override this method for handling normal response from getRoles operation
            */
           public void receiveResultgetRoles(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.GetRolesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getRoles operation
           */
            public void receiveErrorgetRoles(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateUser method
            * override this method for handling normal response from updateUser operation
            */
           public void receiveResultupdateUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.UpdateUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateUser operation
           */
            public void receiveErrorupdateUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for setPassword method
            * override this method for handling normal response from setPassword operation
            */
           public void receiveResultsetPassword(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.SetPasswordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from setPassword operation
           */
            public void receiveErrorsetPassword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getUsersForOrganizationalUnit method
            * override this method for handling normal response from getUsersForOrganizationalUnit operation
            */
           public void receiveResultgetUsersForOrganizationalUnit(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.GetUsersForOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getUsersForOrganizationalUnit operation
           */
            public void receiveErrorgetUsersForOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createUser method
            * override this method for handling normal response from createUser operation
            */
           public void receiveResultcreateUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.CreateUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createUser operation
           */
            public void receiveErrorcreateUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitsForUser method
            * override this method for handling normal response from getOrganizationalUnitsForUser operation
            */
           public void receiveResultgetOrganizationalUnitsForUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.GetOrganizationalUnitsForUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitsForUser operation
           */
            public void receiveErrorgetOrganizationalUnitsForUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitRolesForUser method
            * override this method for handling normal response from getOrganizationalUnitRolesForUser operation
            */
           public void receiveResultgetOrganizationalUnitRolesForUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.GetOrganizationalUnitRolesForUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitRolesForUser operation
           */
            public void receiveErrorgetOrganizationalUnitRolesForUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for setUserInternallyManaged method
            * override this method for handling normal response from setUserInternallyManaged operation
            */
           public void receiveResultsetUserInternallyManaged(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.SetUserInternallyManagedResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from setUserInternallyManaged operation
           */
            public void receiveErrorsetUserInternallyManaged(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeAllOrganizationalUnitRolesForUser method
            * override this method for handling normal response from removeAllOrganizationalUnitRolesForUser operation
            */
           public void receiveResultremoveAllOrganizationalUnitRolesForUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.RemoveAllOrganizationalUnitRolesForUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeAllOrganizationalUnitRolesForUser operation
           */
            public void receiveErrorremoveAllOrganizationalUnitRolesForUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteUser method
            * override this method for handling normal response from deleteUser operation
            */
           public void receiveResultdeleteUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.DeleteUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteUser operation
           */
            public void receiveErrordeleteUser(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getUser method
            * override this method for handling normal response from getUser operation
            */
           public void receiveResultgetUser(
                    com.icoserve.www.va20_useradministrationservice.VA20_UserAdministrationServiceStub.GetUserResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getUser operation
           */
            public void receiveErrorgetUser(java.lang.Exception e) {
            }
                


    }
    