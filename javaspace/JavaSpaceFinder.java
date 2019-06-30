// File: JavaSpaceFinder.java
// Name: Kevin Zink

// This class deals with finding the JavaSpace server.
// This is taken directly from the dietel book.

// This application unicast discovers the JavaSpaces service.

// Jini core packages
import net.jini.core.discovery.LookupLocator;
import net.jini.core.lookup.*;

// Jini extension package
import net.jini.space.JavaSpace;

import java.rmi.*;
import java.net.*;

public class JavaSpaceFinder {
	
    private JavaSpace space;
    
    public JavaSpaceFinder( String jiniURL )
    {
        LookupLocator locator = null;
        ServiceRegistrar registrar = null;
        System.setSecurityManager( new RMISecurityManager() );
        
        //get lookup service locator at "jini://hostname"
        // use default port and registrar of the locator
        try {  
            locator = new LookupLocator( jiniURL );
            registrar = locator.getRegistrar();
        } 
        
        // handle exception invalid jini URL
        catch ( MalformedURLException malformedURLException ) {
            malformedURLException.printStackTrace();
        }
        
        // handle exception I/O
        catch ( java.io.IOException ioException ) {
            ioException.printStackTrace(); 
        }
        
        // handle exception finding class
        catch ( ClassNotFoundException classNotFoundException ) {
            classNotFoundException.printStackTrace();
        }
        
        // specify the service requirement
        Class[] types = new Class[] { JavaSpace.class };
        ServiceTemplate template = 
            new ServiceTemplate( null, types, null );
        
        // find service       
        try {
            space = 
                ( JavaSpace ) registrar.lookup( template );
        }
        
        // handle exception getting JavaSpace
        catch ( RemoteException remoteException ) {
            remoteException.printStackTrace();
        }
        
        // if does not find any matching service
        if ( space == null ) {
            System.out.println( "No matching service" );
        }
        
    } // end JavaSpaceFinder constructor
    
    public JavaSpace getJavaSpace()
    {
        return space;
    }
}
