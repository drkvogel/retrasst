
/**
 * WsFault.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis2 version: 1.6.2  Built on : Apr 17, 2012 (05:33:49 IST)
 */

package com.icoserve.www.va20_organizationalunitadministrationservice;

public class WsFault extends java.lang.Exception{

    private static final long serialVersionUID = 1390473883411L;
    
    private com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.WsFaultE faultMessage;

    
        public WsFault() {
            super("WsFault");
        }

        public WsFault(java.lang.String s) {
           super(s);
        }

        public WsFault(java.lang.String s, java.lang.Throwable ex) {
          super(s, ex);
        }

        public WsFault(java.lang.Throwable cause) {
            super(cause);
        }
    

    public void setFaultMessage(com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.WsFaultE msg){
       faultMessage = msg;
    }
    
    public com.icoserve.www.va20_organizationalunitadministrationservice.VA20_OrganizationalUnitAdministrationServiceStub.WsFaultE getFaultMessage(){
       return faultMessage;
    }
}
    