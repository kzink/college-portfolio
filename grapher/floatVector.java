// File: floatVector.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 8/9, Programming Problem 1
//
// This class contains is a modified Vector specifically designed for floating numbers.
//
// It converts floating numbers from their base type to their object type so they can 
// easily be used with the default Java Vector class.

import java.util.Vector;

/** This class contains a float vector.
 */
class floatVector {
    
    private Vector vector;
    
    public floatVector() {
        vector = new Vector();
    }

    public boolean add( float f ) {
        return vector.add( new Float( f ) );
    }
    
    public boolean add( Float f ) {
        return vector.add( f );
    }

    public float get( int i ) {
        return ((Float) vector.get( i )).floatValue();
    }

    public void set( int i, float value ) {
        vector.set(i, new Float( value ) );
    }

    public int size() {
        return vector.size();
    }
    
    public void clear() {
        vector.clear();
    }
}

