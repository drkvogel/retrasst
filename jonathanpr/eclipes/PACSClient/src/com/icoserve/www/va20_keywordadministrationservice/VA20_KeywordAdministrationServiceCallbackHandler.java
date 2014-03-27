
/**
 * VA20_KeywordAdministrationServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_keywordadministrationservice;

    /**
     *  VA20_KeywordAdministrationServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_KeywordAdministrationServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_KeywordAdministrationServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_KeywordAdministrationServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for getKeywordDependenciesForCatalog method
            * override this method for handling normal response from getKeywordDependenciesForCatalog operation
            */
           public void receiveResultgetKeywordDependenciesForCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordDependenciesForCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordDependenciesForCatalog operation
           */
            public void receiveErrorgetKeywordDependenciesForCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordPath method
            * override this method for handling normal response from getKeywordPath operation
            */
           public void receiveResultgetKeywordPath(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordPathResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordPath operation
           */
            public void receiveErrorgetKeywordPath(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renameKeywordGroup method
            * override this method for handling normal response from renameKeywordGroup operation
            */
           public void receiveResultrenameKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.RenameKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renameKeywordGroup operation
           */
            public void receiveErrorrenameKeywordGroup(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createFieldType method
            * override this method for handling normal response from createFieldType operation
            */
           public void receiveResultcreateFieldType(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateFieldTypeResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createFieldType operation
           */
            public void receiveErrorcreateFieldType(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsKeyword method
            * override this method for handling normal response from existsKeyword operation
            */
           public void receiveResultexistsKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ExistsKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsKeyword operation
           */
            public void receiveErrorexistsKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findCatalogs method
            * override this method for handling normal response from findCatalogs operation
            */
           public void receiveResultfindCatalogs(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.FindCatalogsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findCatalogs operation
           */
            public void receiveErrorfindCatalogs(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordGroup method
            * override this method for handling normal response from getKeywordGroup operation
            */
           public void receiveResultgetKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordGroup operation
           */
            public void receiveErrorgetKeywordGroup(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordChildren method
            * override this method for handling normal response from getKeywordChildren operation
            */
           public void receiveResultgetKeywordChildren(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordChildrenResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordChildren operation
           */
            public void receiveErrorgetKeywordChildren(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordDependenciesForKeyword method
            * override this method for handling normal response from getKeywordDependenciesForKeyword operation
            */
           public void receiveResultgetKeywordDependenciesForKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordDependenciesForKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordDependenciesForKeyword operation
           */
            public void receiveErrorgetKeywordDependenciesForKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteKeywordDependency method
            * override this method for handling normal response from deleteKeywordDependency operation
            */
           public void receiveResultdeleteKeywordDependency(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteKeywordDependencyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteKeywordDependency operation
           */
            public void receiveErrordeleteKeywordDependency(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findKeywordGroups method
            * override this method for handling normal response from findKeywordGroups operation
            */
           public void receiveResultfindKeywordGroups(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.FindKeywordGroupsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findKeywordGroups operation
           */
            public void receiveErrorfindKeywordGroups(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for undeleteKeyword method
            * override this method for handling normal response from undeleteKeyword operation
            */
           public void receiveResultundeleteKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.UndeleteKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from undeleteKeyword operation
           */
            public void receiveErrorundeleteKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteKeywordGroup method
            * override this method for handling normal response from deleteKeywordGroup operation
            */
           public void receiveResultdeleteKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteKeywordGroup operation
           */
            public void receiveErrordeleteKeywordGroup(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createKeywordDependency method
            * override this method for handling normal response from createKeywordDependency operation
            */
           public void receiveResultcreateKeywordDependency(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateKeywordDependencyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createKeywordDependency operation
           */
            public void receiveErrorcreateKeywordDependency(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renameFieldType method
            * override this method for handling normal response from renameFieldType operation
            */
           public void receiveResultrenameFieldType(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.RenameFieldTypeResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renameFieldType operation
           */
            public void receiveErrorrenameFieldType(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getFieldTypes method
            * override this method for handling normal response from getFieldTypes operation
            */
           public void receiveResultgetFieldTypes(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetFieldTypesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getFieldTypes operation
           */
            public void receiveErrorgetFieldTypes(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateKeywordField method
            * override this method for handling normal response from updateKeywordField operation
            */
           public void receiveResultupdateKeywordField(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.UpdateKeywordFieldResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateKeywordField operation
           */
            public void receiveErrorupdateKeywordField(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeyword method
            * override this method for handling normal response from getKeyword operation
            */
           public void receiveResultgetKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeyword operation
           */
            public void receiveErrorgetKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeKeywordParent method
            * override this method for handling normal response from changeKeywordParent operation
            */
           public void receiveResultchangeKeywordParent(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ChangeKeywordParentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeKeywordParent operation
           */
            public void receiveErrorchangeKeywordParent(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createKeyword method
            * override this method for handling normal response from createKeyword operation
            */
           public void receiveResultcreateKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createKeyword operation
           */
            public void receiveErrorcreateKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteKeywordField method
            * override this method for handling normal response from deleteKeywordField operation
            */
           public void receiveResultdeleteKeywordField(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteKeywordFieldResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteKeywordField operation
           */
            public void receiveErrordeleteKeywordField(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for undeleteCatalog method
            * override this method for handling normal response from undeleteCatalog operation
            */
           public void receiveResultundeleteCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.UndeleteCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from undeleteCatalog operation
           */
            public void receiveErrorundeleteCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordGroupToOrganizationalUnit method
            * override this method for handling normal response from addKeywordGroupToOrganizationalUnit operation
            */
           public void receiveResultaddKeywordGroupToOrganizationalUnit(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.AddKeywordGroupToOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordGroupToOrganizationalUnit operation
           */
            public void receiveErroraddKeywordGroupToOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createKeywordField method
            * override this method for handling normal response from createKeywordField operation
            */
           public void receiveResultcreateKeywordField(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateKeywordFieldResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createKeywordField operation
           */
            public void receiveErrorcreateKeywordField(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getOrganizationalUnitNamesForKeywordGroup method
            * override this method for handling normal response from getOrganizationalUnitNamesForKeywordGroup operation
            */
           public void receiveResultgetOrganizationalUnitNamesForKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetOrganizationalUnitNamesForKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getOrganizationalUnitNamesForKeywordGroup operation
           */
            public void receiveErrorgetOrganizationalUnitNamesForKeywordGroup(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createKeywordGroup method
            * override this method for handling normal response from createKeywordGroup operation
            */
           public void receiveResultcreateKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createKeywordGroup operation
           */
            public void receiveErrorcreateKeywordGroup(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for createCatalog method
            * override this method for handling normal response from createCatalog operation
            */
           public void receiveResultcreateCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.CreateCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from createCatalog operation
           */
            public void receiveErrorcreateCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getCatalogChildren method
            * override this method for handling normal response from getCatalogChildren operation
            */
           public void receiveResultgetCatalogChildren(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetCatalogChildrenResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getCatalogChildren operation
           */
            public void receiveErrorgetCatalogChildren(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getCatalog method
            * override this method for handling normal response from getCatalog operation
            */
           public void receiveResultgetCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getCatalog operation
           */
            public void receiveErrorgetCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsFieldType method
            * override this method for handling normal response from existsFieldType operation
            */
           public void receiveResultexistsFieldType(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ExistsFieldTypeResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsFieldType operation
           */
            public void receiveErrorexistsFieldType(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsCatalog method
            * override this method for handling normal response from existsCatalog operation
            */
           public void receiveResultexistsCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ExistsCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsCatalog operation
           */
            public void receiveErrorexistsCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsKeywordPath method
            * override this method for handling normal response from existsKeywordPath operation
            */
           public void receiveResultexistsKeywordPath(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ExistsKeywordPathResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsKeywordPath operation
           */
            public void receiveErrorexistsKeywordPath(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeKeywordValue method
            * override this method for handling normal response from changeKeywordValue operation
            */
           public void receiveResultchangeKeywordValue(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ChangeKeywordValueResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeKeywordValue operation
           */
            public void receiveErrorchangeKeywordValue(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteCatalog method
            * override this method for handling normal response from deleteCatalog operation
            */
           public void receiveResultdeleteCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteCatalog operation
           */
            public void receiveErrordeleteCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordGroupFromOrganizationalUnit method
            * override this method for handling normal response from removeKeywordGroupFromOrganizationalUnit operation
            */
           public void receiveResultremoveKeywordGroupFromOrganizationalUnit(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.RemoveKeywordGroupFromOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordGroupFromOrganizationalUnit operation
           */
            public void receiveErrorremoveKeywordGroupFromOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renameCatalog method
            * override this method for handling normal response from renameCatalog operation
            */
           public void receiveResultrenameCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.RenameCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renameCatalog operation
           */
            public void receiveErrorrenameCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateCatalog method
            * override this method for handling normal response from updateCatalog operation
            */
           public void receiveResultupdateCatalog(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.UpdateCatalogResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateCatalog operation
           */
            public void receiveErrorupdateCatalog(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteKeyword method
            * override this method for handling normal response from deleteKeyword operation
            */
           public void receiveResultdeleteKeyword(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteKeywordResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteKeyword operation
           */
            public void receiveErrordeleteKeyword(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteFieldType method
            * override this method for handling normal response from deleteFieldType operation
            */
           public void receiveResultdeleteFieldType(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.DeleteFieldTypeResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteFieldType operation
           */
            public void receiveErrordeleteFieldType(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordGroupNamesForOrganizationalUnits method
            * override this method for handling normal response from getKeywordGroupNamesForOrganizationalUnits operation
            */
           public void receiveResultgetKeywordGroupNamesForOrganizationalUnits(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.GetKeywordGroupNamesForOrganizationalUnitsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordGroupNamesForOrganizationalUnits operation
           */
            public void receiveErrorgetKeywordGroupNamesForOrganizationalUnits(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsKeywordGroup method
            * override this method for handling normal response from existsKeywordGroup operation
            */
           public void receiveResultexistsKeywordGroup(
                    com.icoserve.www.va20_keywordadministrationservice.VA20_KeywordAdministrationServiceStub.ExistsKeywordGroupResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsKeywordGroup operation
           */
            public void receiveErrorexistsKeywordGroup(java.lang.Exception e) {
            }
                


    }
    