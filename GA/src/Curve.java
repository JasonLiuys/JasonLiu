import java.util.Vector;

public class Curve {
	private double pai = 3.1415926;
	public Curve() {
		// TODO Auto-generated constructor stub
	}
	public double function(Vector <Double> input){
		double x=input.get(0);
		double output;
		output=x*(Math.sin(10*pai*x))+2.0;
		return output;
	}
}
