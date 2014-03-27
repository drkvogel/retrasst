
/**
 * VA20_KeywordServiceCallbackHandler.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

    package com.icoserve.www.va20_keywordservice;

    /**
     *  VA20_KeywordServiceCallbackHandler Callback class, Users can extend this class and implement
     *  their own receiveResult and receiveError methods.
     */
    public abstract class VA20_KeywordServiceCallbackHandler{



    protected Object clientData;

    /**
    * User can pass in any object that needs to be accessed once the NonBlocking
    * Web service call is finished and appropriate method of this CallBack is called.
    * @param clientData Object mechanism by which the user can pass in user data
    * that will be avilable at the time this callback is called.
    */
    public VA20_KeywordServiceCallbackHandler(Object clientData){
        this.clientData = clientData;
    }

    /**
    * Please use this constructor if you don't want to set any clientData
    */
    public VA20_KeywordServiceCallbackHandler(){
        this.clientData = null;
    }

    /**
     * Get the client data
     */

     public Object getClientData() {
        return clientData;
     }

        
           /**
            * auto generated Axis2 call back method for retrieveKeywordsForDicomSeries method
            * override this method for handling normal response from retrieveKeywordsForDicomSeries operation
            */
           public void receiveResultretrieveKeywordsForDicomSeries(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from retrieveKeywordsForDicomSeries operation
           */
            public void receiveErrorretrieveKeywordsForDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordsFromDicomStudy method
            * override this method for handling normal response from removeKeywordsFromDicomStudy operation
            */
           public void receiveResultremoveKeywordsFromDicomStudy(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RemoveKeywordsFromDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordsFromDicomStudy operation
           */
            public void receiveErrorremoveKeywordsFromDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordsToDicomImage method
            * override this method for handling normal response from addKeywordsToDicomImage operation
            */
           public void receiveResultaddKeywordsToDicomImage(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordsToDicomImage operation
           */
            public void receiveErroraddKeywordsToDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordChildren method
            * override this method for handling normal response from getKeywordChildren operation
            */
           public void receiveResultgetKeywordChildren(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.GetKeywordChildrenResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordChildren operation
           */
            public void receiveErrorgetKeywordChildren(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for retrieveKeywordsForDicomImages method
            * override this method for handling normal response from retrieveKeywordsForDicomImages operation
            */
           public void receiveResultretrieveKeywordsForDicomImages(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomImagesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from retrieveKeywordsForDicomImages operation
           */
            public void receiveErrorretrieveKeywordsForDicomImages(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordsFromDicomSeries method
            * override this method for handling normal response from removeKeywordsFromDicomSeries operation
            */
           public void receiveResultremoveKeywordsFromDicomSeries(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RemoveKeywordsFromDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordsFromDicomSeries operation
           */
            public void receiveErrorremoveKeywordsFromDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordsFromGenericContainer method
            * override this method for handling normal response from removeKeywordsFromGenericContainer operation
            */
           public void receiveResultremoveKeywordsFromGenericContainer(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RemoveKeywordsFromGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordsFromGenericContainer operation
           */
            public void receiveErrorremoveKeywordsFromGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordsToDicomStudy method
            * override this method for handling normal response from addKeywordsToDicomStudy operation
            */
           public void receiveResultaddKeywordsToDicomStudy(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToDicomStudyResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordsToDicomStudy operation
           */
            public void receiveErroraddKeywordsToDicomStudy(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for retrieveKeywordsForGenericContainers method
            * override this method for handling normal response from retrieveKeywordsForGenericContainers operation
            */
           public void receiveResultretrieveKeywordsForGenericContainers(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForGenericContainersResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from retrieveKeywordsForGenericContainers operation
           */
            public void receiveErrorretrieveKeywordsForGenericContainers(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordsToDicomSeries method
            * override this method for handling normal response from addKeywordsToDicomSeries operation
            */
           public void receiveResultaddKeywordsToDicomSeries(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToDicomSeriesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordsToDicomSeries operation
           */
            public void receiveErroraddKeywordsToDicomSeries(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getCatalogChildren method
            * override this method for handling normal response from getCatalogChildren operation
            */
           public void receiveResultgetCatalogChildren(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.GetCatalogChildrenResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getCatalogChildren operation
           */
            public void receiveErrorgetCatalogChildren(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordGroups method
            * override this method for handling normal response from getKeywordGroups operation
            */
           public void receiveResultgetKeywordGroups(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.GetKeywordGroupsResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordGroups operation
           */
            public void receiveErrorgetKeywordGroups(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordsToGenericFile method
            * override this method for handling normal response from addKeywordsToGenericFile operation
            */
           public void receiveResultaddKeywordsToGenericFile(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordsToGenericFile operation
           */
            public void receiveErroraddKeywordsToGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordsFromDicomImage method
            * override this method for handling normal response from removeKeywordsFromDicomImage operation
            */
           public void receiveResultremoveKeywordsFromDicomImage(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RemoveKeywordsFromDicomImageResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordsFromDicomImage operation
           */
            public void receiveErrorremoveKeywordsFromDicomImage(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for addKeywordsToGenericContainer method
            * override this method for handling normal response from addKeywordsToGenericContainer operation
            */
           public void receiveResultaddKeywordsToGenericContainer(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.AddKeywordsToGenericContainerResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from addKeywordsToGenericContainer operation
           */
            public void receiveErroraddKeywordsToGenericContainer(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for retrieveKeywordsForGenericFiles method
            * override this method for handling normal response from retrieveKeywordsForGenericFiles operation
            */
           public void receiveResultretrieveKeywordsForGenericFiles(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForGenericFilesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from retrieveKeywordsForGenericFiles operation
           */
            public void receiveErrorretrieveKeywordsForGenericFiles(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for removeKeywordsFromGenericFile method
            * override this method for handling normal response from removeKeywordsFromGenericFile operation
            */
           public void receiveResultremoveKeywordsFromGenericFile(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RemoveKeywordsFromGenericFileResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from removeKeywordsFromGenericFile operation
           */
            public void receiveErrorremoveKeywordsFromGenericFile(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for getKeywordPath method
            * override this method for handling normal response from getKeywordPath operation
            */
           public void receiveResultgetKeywordPath(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.GetKeywordPathResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from getKeywordPath operation
           */
            public void receiveErrorgetKeywordPath(java.lang.Exception e) {
            }
                
           /**
            * auto generated Axis2 call back method for retrieveKeywordsForDicomStudies method
            * override this method for handling normal response from retrieveKeywordsForDicomStudies operation
            */
           public void receiveResultretrieveKeywordsForDicomStudies(
                    com.icoserve.www.va20_keywordservice.VA20_KeywordServiceStub.RetrieveKeywordsForDicomStudiesResponse result
                        ) {
           }

          /**
           * auto generated Axis2 Error handler
           * override this method for handling error response from retrieveKeywordsForDicomStudies operation
           */
            public void receiveErrorretrieveKeywordsForDicomStudies(java.lang.Exception e) {
            }
                


    }
    