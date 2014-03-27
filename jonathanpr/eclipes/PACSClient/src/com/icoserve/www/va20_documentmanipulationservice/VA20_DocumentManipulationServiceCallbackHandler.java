
/**
 * VA20_DocumentManipulationServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_documentmanipulationservice;

    /**
     *  VA20_DocumentManipulationServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_DocumentManipulationServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_DocumentManipulationServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_DocumentManipulationServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for restoreDocument method
            * override this method for handling normal response from restoreDocument operation
            */
           public void receiveResultrestoreDocument(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.RestoreDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from restoreDocument operation
           */
            public void receiveErrorrestoreDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeDocumentOrganizationalUnit method
            * override this method for handling normal response from changeDocumentOrganizationalUnit operation
            */
           public void receiveResultchangeDocumentOrganizationalUnit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentOrganizationalUnitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeDocumentOrganizationalUnit operation
           */
            public void receiveErrorchangeDocumentOrganizationalUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeDocumentVisit method
            * override this method for handling normal response from changeDocumentVisit operation
            */
           public void receiveResultchangeDocumentVisit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeDocumentVisit operation
           */
            public void receiveErrorchangeDocumentVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateDocument method
            * override this method for handling normal response from updateDocument operation
            */
           public void receiveResultupdateDocument(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DocumentUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateDocument operation
           */
            public void receiveErrorupdateDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for exportDicomImage method
            * override this method for handling normal response from exportDicomImage operation
            */
           public void receiveResultexportDicomImage(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ExportDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from exportDicomImage operation
           */
            public void receiveErrorexportDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateDicomStudy method
            * override this method for handling normal response from updateDicomStudy operation
            */
           public void receiveResultupdateDicomStudy(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomStudyUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateDicomStudy operation
           */
            public void receiveErrorupdateDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for importGenericFileIntoOrgUnit method
            * override this method for handling normal response from importGenericFileIntoOrgUnit operation
            */
           public void receiveResultimportGenericFileIntoOrgUnit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericOrgUnitImportResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from importGenericFileIntoOrgUnit operation
           */
            public void receiveErrorimportGenericFileIntoOrgUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteDicomImage method
            * override this method for handling normal response from deleteDicomImage operation
            */
           public void receiveResultdeleteDicomImage(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomImageDeleteResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteDicomImage operation
           */
            public void receiveErrordeleteDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for exportGenericFile method
            * override this method for handling normal response from exportGenericFile operation
            */
           public void receiveResultexportGenericFile(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ExportGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from exportGenericFile operation
           */
            public void receiveErrorexportGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateDicomImage method
            * override this method for handling normal response from updateDicomImage operation
            */
           public void receiveResultupdateDicomImage(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomImageUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateDicomImage operation
           */
            public void receiveErrorupdateDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for importGenericFileForEntityIntoOrgUnit method
            * override this method for handling normal response from importGenericFileForEntityIntoOrgUnit operation
            */
           public void receiveResultimportGenericFileForEntityIntoOrgUnit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericOrgUnitImportForEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from importGenericFileForEntityIntoOrgUnit operation
           */
            public void receiveErrorimportGenericFileForEntityIntoOrgUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteDicomSeries method
            * override this method for handling normal response from deleteDicomSeries operation
            */
           public void receiveResultdeleteDicomSeries(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomSeriesDeleteResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteDicomSeries operation
           */
            public void receiveErrordeleteDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for importDicomImageIntoOrgUnit method
            * override this method for handling normal response from importDicomImageIntoOrgUnit operation
            */
           public void receiveResultimportDicomImageIntoOrgUnit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomOrgUnitImportResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from importDicomImageIntoOrgUnit operation
           */
            public void receiveErrorimportDicomImageIntoOrgUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteGenericFile method
            * override this method for handling normal response from deleteGenericFile operation
            */
           public void receiveResultdeleteGenericFile(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericFileDeleteResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteGenericFile operation
           */
            public void receiveErrordeleteGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateGenericContainer method
            * override this method for handling normal response from updateGenericContainer operation
            */
           public void receiveResultupdateGenericContainer(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericContainerUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateGenericContainer operation
           */
            public void receiveErrorupdateGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renderDicomImage method
            * override this method for handling normal response from renderDicomImage operation
            */
           public void receiveResultrenderDicomImage(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.RenderDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renderDicomImage operation
           */
            public void receiveErrorrenderDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for exportFilesForReferencePointer method
            * override this method for handling normal response from exportFilesForReferencePointer operation
            */
           public void receiveResultexportFilesForReferencePointer(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ExportFilesForReferencePointerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from exportFilesForReferencePointer operation
           */
            public void receiveErrorexportFilesForReferencePointer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for renderGenericFile method
            * override this method for handling normal response from renderGenericFile operation
            */
           public void receiveResultrenderGenericFile(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.RenderGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from renderGenericFile operation
           */
            public void receiveErrorrenderGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeDocumentEntity method
            * override this method for handling normal response from changeDocumentEntity operation
            */
           public void receiveResultchangeDocumentEntity(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeDocumentEntity operation
           */
            public void receiveErrorchangeDocumentEntity(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for importDicomImageForEntityIntoOrgUnit method
            * override this method for handling normal response from importDicomImageForEntityIntoOrgUnit operation
            */
           public void receiveResultimportDicomImageForEntityIntoOrgUnit(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomOrgUnitImportForEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from importDicomImageForEntityIntoOrgUnit operation
           */
            public void receiveErrorimportDicomImageForEntityIntoOrgUnit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateGenericFile method
            * override this method for handling normal response from updateGenericFile operation
            */
           public void receiveResultupdateGenericFile(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericFileUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateGenericFile operation
           */
            public void receiveErrorupdateGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for deleteDocument method
            * override this method for handling normal response from deleteDocument operation
            */
           public void receiveResultdeleteDocument(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DocumentDeleteResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from deleteDocument operation
           */
            public void receiveErrordeleteDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for updateDicomSeries method
            * override this method for handling normal response from updateDicomSeries operation
            */
           public void receiveResultupdateDicomSeries(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomSeriesUpdateResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from updateDicomSeries operation
           */
            public void receiveErrorupdateDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for changeDocumentPatient method
            * override this method for handling normal response from changeDocumentPatient operation
            */
           public void receiveResultchangeDocumentPatient(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.ChangeDocumentPatientResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from changeDocumentPatient operation
           */
            public void receiveErrorchangeDocumentPatient(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for copyDocument method
            * override this method for handling normal response from copyDocument operation
            */
           public void receiveResultcopyDocument(
                    com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.CopyDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from copyDocument operation
           */
            public void receiveErrorcopyDocument(java.lang.Exception e) {
            }
                


    }
    