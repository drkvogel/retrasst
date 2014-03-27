
/**
 * VA20_QueryServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_queryservice;

    /**
     *  VA20_QueryServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_QueryServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_QueryServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_QueryServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for getGenericContainer method
            * override this method for handling normal response from getGenericContainer operation
            */
           public void receiveResultgetGenericContainer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getGenericContainer operation
           */
            public void receiveErrorgetGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForGenericFile method
            * override this method for handling normal response from getDirectAccessUrlForGenericFile operation
            */
           public void receiveResultgetDirectAccessUrlForGenericFile(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForGenericFile operation
           */
            public void receiveErrorgetDirectAccessUrlForGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsDicomSeries method
            * override this method for handling normal response from existsDicomSeries operation
            */
           public void receiveResultexistsDicomSeries(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsDicomSeries operation
           */
            public void receiveErrorexistsDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsVisit method
            * override this method for handling normal response from existsVisit operation
            */
           public void receiveResultexistsVisit(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsVisit operation
           */
            public void receiveErrorexistsVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getVisit method
            * override this method for handling normal response from getVisit operation
            */
           public void receiveResultgetVisit(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getVisit operation
           */
            public void receiveErrorgetVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getEntity method
            * override this method for handling normal response from getEntity operation
            */
           public void receiveResultgetEntity(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getEntity operation
           */
            public void receiveErrorgetEntity(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomSeriesForDicomStudy method
            * override this method for handling normal response from getDicomSeriesForDicomStudy operation
            */
           public void receiveResultgetDicomSeriesForDicomStudy(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomSeriesForDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomSeriesForDicomStudy operation
           */
            public void receiveErrorgetDicomSeriesForDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findGenericDocumentsForEntities method
            * override this method for handling normal response from findGenericDocumentsForEntities operation
            */
           public void receiveResultfindGenericDocumentsForEntities(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindGenericDocumentsForEntitiesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findGenericDocumentsForEntities operation
           */
            public void receiveErrorfindGenericDocumentsForEntities(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInDicomImage method
            * override this method for handling normal response from existsUidInDicomImage operation
            */
           public void receiveResultexistsUidInDicomImage(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInDicomImage operation
           */
            public void receiveErrorexistsUidInDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInGenericContainer method
            * override this method for handling normal response from existsUidInGenericContainer operation
            */
           public void receiveResultexistsUidInGenericContainer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInGenericContainer operation
           */
            public void receiveErrorexistsUidInGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getPatient method
            * override this method for handling normal response from getPatient operation
            */
           public void receiveResultgetPatient(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetPatientResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getPatient operation
           */
            public void receiveErrorgetPatient(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findVisits method
            * override this method for handling normal response from findVisits operation
            */
           public void receiveResultfindVisits(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindVisitsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findVisits operation
           */
            public void receiveErrorfindVisits(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findVisit method
            * override this method for handling normal response from findVisit operation
            */
           public void receiveResultfindVisit(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findVisit operation
           */
            public void receiveErrorfindVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForVisit method
            * override this method for handling normal response from getDirectAccessUrlForVisit operation
            */
           public void receiveResultgetDirectAccessUrlForVisit(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForVisit operation
           */
            public void receiveErrorgetDirectAccessUrlForVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsDicomImage method
            * override this method for handling normal response from existsDicomImage operation
            */
           public void receiveResultexistsDicomImage(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsDicomImage operation
           */
            public void receiveErrorexistsDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findGenericDocuments method
            * override this method for handling normal response from findGenericDocuments operation
            */
           public void receiveResultfindGenericDocuments(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindGenericDocumentsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findGenericDocuments operation
           */
            public void receiveErrorfindGenericDocuments(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForDicomStudy method
            * override this method for handling normal response from getDirectAccessUrlForDicomStudy operation
            */
           public void receiveResultgetDirectAccessUrlForDicomStudy(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForDicomStudy operation
           */
            public void receiveErrorgetDirectAccessUrlForDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getWagoUrl method
            * override this method for handling normal response from getWagoUrl operation
            */
           public void receiveResultgetWagoUrl(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetWagoUrlResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getWagoUrl operation
           */
            public void receiveErrorgetWagoUrl(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsGenericContainer method
            * override this method for handling normal response from existsGenericContainer operation
            */
           public void receiveResultexistsGenericContainer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsGenericContainer operation
           */
            public void receiveErrorexistsGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getGenericFile method
            * override this method for handling normal response from getGenericFile operation
            */
           public void receiveResultgetGenericFile(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getGenericFile operation
           */
            public void receiveErrorgetGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForEntity method
            * override this method for handling normal response from getDirectAccessUrlForEntity operation
            */
           public void receiveResultgetDirectAccessUrlForEntity(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForEntity operation
           */
            public void receiveErrorgetDirectAccessUrlForEntity(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsPatient method
            * override this method for handling normal response from existsPatient operation
            */
           public void receiveResultexistsPatient(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsPatientResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsPatient operation
           */
            public void receiveErrorexistsPatient(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsDocument method
            * override this method for handling normal response from existsDocument operation
            */
           public void receiveResultexistsDocument(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsDocument operation
           */
            public void receiveErrorexistsDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findDicomDocumentsForEntities method
            * override this method for handling normal response from findDicomDocumentsForEntities operation
            */
           public void receiveResultfindDicomDocumentsForEntities(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindDicomDocumentsForEntitiesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findDicomDocumentsForEntities operation
           */
            public void receiveErrorfindDicomDocumentsForEntities(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInGenericFile method
            * override this method for handling normal response from existsUidInGenericFile operation
            */
           public void receiveResultexistsUidInGenericFile(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInGenericFile operation
           */
            public void receiveErrorexistsUidInGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForDicomSeries method
            * override this method for handling normal response from getDirectAccessUrlForDicomSeries operation
            */
           public void receiveResultgetDirectAccessUrlForDicomSeries(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForDicomSeries operation
           */
            public void receiveErrorgetDirectAccessUrlForDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForPatient method
            * override this method for handling normal response from getDirectAccessUrlForPatient operation
            */
           public void receiveResultgetDirectAccessUrlForPatient(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForPatientResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForPatient operation
           */
            public void receiveErrorgetDirectAccessUrlForPatient(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsGenericFile method
            * override this method for handling normal response from existsGenericFile operation
            */
           public void receiveResultexistsGenericFile(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsGenericFile operation
           */
            public void receiveErrorexistsGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for isDocumentOnline method
            * override this method for handling normal response from isDocumentOnline operation
            */
           public void receiveResultisDocumentOnline(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.IsDocumentOnlineResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from isDocumentOnline operation
           */
            public void receiveErrorisDocumentOnline(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInDicomStudy method
            * override this method for handling normal response from existsUidInDicomStudy operation
            */
           public void receiveResultexistsUidInDicomStudy(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInDicomStudy operation
           */
            public void receiveErrorexistsUidInDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDocument method
            * override this method for handling normal response from getDocument operation
            */
           public void receiveResultgetDocument(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDocument operation
           */
            public void receiveErrorgetDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findPatient method
            * override this method for handling normal response from findPatient operation
            */
           public void receiveResultfindPatient(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindPatientResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findPatient operation
           */
            public void receiveErrorfindPatient(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsReferencePointer method
            * override this method for handling normal response from existsReferencePointer operation
            */
           public void receiveResultexistsReferencePointer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsReferencePointerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsReferencePointer operation
           */
            public void receiveErrorexistsReferencePointer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomStudy method
            * override this method for handling normal response from getDicomStudy operation
            */
           public void receiveResultgetDicomStudy(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomStudy operation
           */
            public void receiveErrorgetDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomSeries method
            * override this method for handling normal response from getDicomSeries operation
            */
           public void receiveResultgetDicomSeries(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomSeries operation
           */
            public void receiveErrorgetDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomImage method
            * override this method for handling normal response from getDicomImage operation
            */
           public void receiveResultgetDicomImage(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomImage operation
           */
            public void receiveErrorgetDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getGenericFilesForGenericContainer method
            * override this method for handling normal response from getGenericFilesForGenericContainer operation
            */
           public void receiveResultgetGenericFilesForGenericContainer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetGenericFilesForGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getGenericFilesForGenericContainer operation
           */
            public void receiveErrorgetGenericFilesForGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForGenericContainer method
            * override this method for handling normal response from getDirectAccessUrlForGenericContainer operation
            */
           public void receiveResultgetDirectAccessUrlForGenericContainer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForGenericContainer operation
           */
            public void receiveErrorgetDirectAccessUrlForGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getGenericContainerForDocument method
            * override this method for handling normal response from getGenericContainerForDocument operation
            */
           public void receiveResultgetGenericContainerForDocument(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetGenericContainerForDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getGenericContainerForDocument operation
           */
            public void receiveErrorgetGenericContainerForDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findEntity method
            * override this method for handling normal response from findEntity operation
            */
           public void receiveResultfindEntity(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findEntity operation
           */
            public void receiveErrorfindEntity(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDirectAccessUrlForDicomImage method
            * override this method for handling normal response from getDirectAccessUrlForDicomImage operation
            */
           public void receiveResultgetDirectAccessUrlForDicomImage(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDirectAccessUrlForDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDirectAccessUrlForDicomImage operation
           */
            public void receiveErrorgetDirectAccessUrlForDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInDicomSeries method
            * override this method for handling normal response from existsUidInDicomSeries operation
            */
           public void receiveResultexistsUidInDicomSeries(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInDicomSeries operation
           */
            public void receiveErrorexistsUidInDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findPatientVisit method
            * override this method for handling normal response from findPatientVisit operation
            */
           public void receiveResultfindPatientVisit(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindPatientVisitResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findPatientVisit operation
           */
            public void receiveErrorfindPatientVisit(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getReferencePointer method
            * override this method for handling normal response from getReferencePointer operation
            */
           public void receiveResultgetReferencePointer(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetReferencePointerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getReferencePointer operation
           */
            public void receiveErrorgetReferencePointer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getPatientVisits method
            * override this method for handling normal response from getPatientVisits operation
            */
           public void receiveResultgetPatientVisits(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetPatientVisitsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getPatientVisits operation
           */
            public void receiveErrorgetPatientVisits(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomStudyForDocument method
            * override this method for handling normal response from getDicomStudyForDocument operation
            */
           public void receiveResultgetDicomStudyForDocument(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomStudyForDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomStudyForDocument operation
           */
            public void receiveErrorgetDicomStudyForDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findEntities method
            * override this method for handling normal response from findEntities operation
            */
           public void receiveResultfindEntities(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindEntitiesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findEntities operation
           */
            public void receiveErrorfindEntities(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findPatients method
            * override this method for handling normal response from findPatients operation
            */
           public void receiveResultfindPatients(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindPatientsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findPatients operation
           */
            public void receiveErrorfindPatients(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsDicomStudy method
            * override this method for handling normal response from existsDicomStudy operation
            */
           public void receiveResultexistsDicomStudy(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsDicomStudy operation
           */
            public void receiveErrorexistsDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getWadoUrl method
            * override this method for handling normal response from getWadoUrl operation
            */
           public void receiveResultgetWadoUrl(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetWadoUrlResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getWadoUrl operation
           */
            public void receiveErrorgetWadoUrl(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getDicomImagesForDicomSeries method
            * override this method for handling normal response from getDicomImagesForDicomSeries operation
            */
           public void receiveResultgetDicomImagesForDicomSeries(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GetDicomImagesForDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getDicomImagesForDicomSeries operation
           */
            public void receiveErrorgetDicomImagesForDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsUidInDocument method
            * override this method for handling normal response from existsUidInDocument operation
            */
           public void receiveResultexistsUidInDocument(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsUidInDocumentResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsUidInDocument operation
           */
            public void receiveErrorexistsUidInDocument(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for existsEntity method
            * override this method for handling normal response from existsEntity operation
            */
           public void receiveResultexistsEntity(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.ExistsEntityResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from existsEntity operation
           */
            public void receiveErrorexistsEntity(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for findDicomDocuments method
            * override this method for handling normal response from findDicomDocuments operation
            */
           public void receiveResultfindDicomDocuments(
                    com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.FindDicomDocumentsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from findDicomDocuments operation
           */
            public void receiveErrorfindDicomDocuments(java.lang.Exception e) {
            }
                


    }
    