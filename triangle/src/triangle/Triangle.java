package triangle;

public class Triangle {
	
	public static boolean isTriangle (int a, int b, int c){
		if ((a+b)>c&&(a+c)>b&&(b+c)>a){//the three number can constitute a triangle
			return true;
		}
		else {
			return false;
		}
	}
	
	public static boolean isEquilateral (int a, int b, int c){
		if (isTriangle(a,b,c)){
			if ( a == b && b == c ){
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	
	public static boolean isIsosceles (int a, int b, int c){
		if (isTriangle(a,b,c)){
			if ( a == b || a == c || b == c ){
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	
	public static boolean isRightTriangle (int a, int b, int c){
		if (isTriangle(a,b,c)){
			if ( a*a==b*b+c*c||b*b==a*a+c*c||c*c==a*a+b*b ){
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	
	 public static void Triangle (int a, int b, int c) { 
		 	if (isTriangle(a,b,c)){  //the three number can constitute a triangle
		        if ( isEquilateral(a,b,c) ){
		        	System.out.println ("The triangle is a equilateral!"); 
		        }
		        else if ( isIsosceles(a,b,c) ){
		        	if (isRightTriangle(a,b,c)){
		        		System.out.println ("The triangle is a isosceles right triangle!");
		        	}
		        	else{
		        		System.out.println ("The triangle is a isosceles!"); 
		        	}
		        }
		        else if (isRightTriangle(a,b,c)){
		        	System.out.println ("The triangle is a Right triangle!");
		        }
		        else {
		        	System.out.println ("The triangle is a scalene!");
		        }
		 	}
		 	else {
		 		System.out.println ("Can't constitute a triangle!");
		 	}
	    } // end Triangle
	}