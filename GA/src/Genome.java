import java.util.Vector;

public class Genome {   //基因的载体

	Vector <Double> vecGenome = new Vector<Double>() ;  //装载基因的容器 (使用浮点数编码)
	double fitness ;  //适应度
	
//	friend class GenAlg;
//	friend class GenEngine;
	public Genome() {
		fitness = 0 ;
		// TODO Auto-generated constructor stub
	}

	public Genome(Vector <Double> vec , double f){
		vecGenome = vec ;
		fitness = f ;
	}
}
