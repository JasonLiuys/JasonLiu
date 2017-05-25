import java.util.Iterator;
import java.util.Vector;


public class GenAlg { //遗传算法引擎

	public Vector <Genome> vecPop = new Vector<Genome>(); //储存每一个个体的染色体
	public int popSize; //种群数量
	public int chromoLength; //每一条染色体的基因的总数目
	public double totalFitness; //所有个体对应的适应性评分的总和
	public double bestFitness; //在所有个体当中最适应的个体的适应性评分
	public double averageFitness; //所有个体的适应性评分的平均值
	public double worstFitness; //在所有个体当中最不适应的个体的适应性评分
	public Genome fittestGenome; //最适应的个体在m_vecPop容器里面的索引号
	public double mutationRate; //基因突变的概率,一般介于0.05和0.3之间
	public double crossoverRate; //基因交叉的概率一般设为0.7
	public int generation; //记录种群的代数
	public double maxPerturbation,leftPoint,rightPoint;
	//最大变异步长，左端点，右端点
	public GenAlg() {
		
		// TODO Auto-generated constructor stub
	}
	//初始化变量
	void Reset(){
		totalFitness=0;
		//bestFitness=0;
	    //worstFitness=9999;
		averageFitness=0;
	}
	
	//初始化函数
	void init(int popsize, double MutRate, double CrossRate, int GenLenght,double LeftPoint,double RightPoint){
		
		popSize = popsize;  
		mutationRate = MutRate;
		crossoverRate = CrossRate;
		chromoLength = GenLenght;
		totalFitness = 0;
		generation = 0;
		//fittestGenome = 0;
		bestFitness = 0.0;
		worstFitness = 99999999;
		averageFitness = 0;
		maxPerturbation=0.004;
		leftPoint=LeftPoint;
		rightPoint=RightPoint;
		vecPop.clear();  //清空种群容器，以初始化

		for (int i=0; i<popSize; i++){     
			
			Genome gen = new Genome();
			//类的构造函数已经把适应性评分初始化为0
			vecPop.add(gen);

			//把所有的基因编码初始化为函数区间内的随机数。

			for (int j=0; j<chromoLength; j++){
				vecPop.get(i).vecGenome.add( Math.random() * (rightPoint - leftPoint) + leftPoint);
			}
		}
	}
	
	
	//计算TotalFitness, BestFitness, WorstFitness, AverageFitness等变量
	void CalculateBestWorstAvTot(){
		for (int i=0; i<popSize; i++){
			//累计适应性分数.
			totalFitness += vecPop.get(i).fitness;

			if(vecPop.get(i).fitness >= bestFitness){
				bestFitness=vecPop.get(i).fitness;
				fittestGenome=vecPop.get(i);
			}
			if(vecPop.get(i).fitness <= worstFitness)
				worstFitness=vecPop.get(i).fitness;
		}

		averageFitness=totalFitness/popSize;
	}
		
	
	//轮盘赌选择函数
	Genome GetChromoRoulette(){
		
		double Slice = ( Math.random() )* totalFitness;  //产生一个0到总适应性评分总和之间的随机数.
		Genome TheChosenOne = null;  //保存转盘所选出来的个体的基因.
		double FitnessSoFar = 0;  //累计适应性分数的和.

		//遍历总人口里面的每一条染色体
		for (int i=0; i<popSize; i++){
			FitnessSoFar += vecPop.get(i).fitness;  //累计适应性分数.
			if (FitnessSoFar >= Slice){  			//如果累计分数大于随机数,就选择此时的基因.
				TheChosenOne = vecPop.get(i);
				break;
			}
			else{
				TheChosenOne = vecPop.get(i);
			}
		}
		//返回转盘选出来的个体基因
		return TheChosenOne;
	}

	//基因变异函数
	void Mutate(Vector <Double> chromo){
		//遵循预定的突变概率,对基因进行突变
		for (int i=0; i<chromo.size(); i++){			
			if ( Math.random() < mutationRate){  //如果发生突变的话
				//使该权值增加或者减少一个很小的随机数值
				double temp = chromo.get(i) + (( Math.random()-0.5 ) * maxPerturbation);
				chromo.set(i, temp);

				//保证袋鼠不至于跳出自然保护区.
				if(chromo.get(i) < leftPoint){
					chromo.set(i, rightPoint);
				}
				else if(chromo.get(i) > rightPoint){
					chromo.set(i, leftPoint);
				}
			}
		}
	}
	
	//Epoch函数产生新一代基因
	Vector<Genome> Epoch(Vector <Genome> vecNewPop){
		//用类的成员变量来储存父代的基因组(在此之前vecPop储存的是不带估值的所有基因组)	
		vecPop = vecNewPop;	
//		vecPop.clear();
//		for ( int i = 0 ; i < vecNewPop.size() ; i ++ ){
//			Genome value = vecNewPop.get(i);
//			vecPop.add(value);
//		}
		
		Vector <Genome> vecNewPopTemp = new Vector<Genome> ();
		Reset(); //初始化相关变量
		CalculateBestWorstAvTot();  //为相关变量赋值
		System.out.println(totalFitness);
//		vecNewPop.clear();  //清空装载新种群的容器
		
		
		while (vecNewPopTemp.size() < popSize){ 	//产生新一代的所有基因组
			Genome mum = GetChromoRoulette();	//转盘随机抽出两个基因
			Genome dad = GetChromoRoulette();
//			System.out.println("mum" + mum.vecGenome);
//			System.out.println("dad" + dad.vecGenome);
			Vector <Double> baby1, baby2;	//创建两个子代基因组
			baby1 = mum.vecGenome;  		//先把他们分别设置成父方和母方的基因
			baby2 = dad.vecGenome;
			Mutate(baby1);   //使子代基因发生基因突变
			Mutate(baby2);
			Genome gen1 = new Genome(baby1, 0);
			Genome gen2 = new Genome(baby2, 0);
			vecNewPopTemp.add( gen1 );  //把两个子代基因组放到新的基因组容器里面
			vecNewPopTemp.add( gen2 );
				
		}//子代产生完毕

		System.out.println("-----please wait");
//		//如果你设置的人口总数非单数的话,就会出现报错
//		if(vecNewPop.size() != popSize){
//			//MessageBox("你的人口数目不是单数!!!");
//			cout<<"error"<<endl;
//			return;
//		}
		return vecNewPopTemp;

	}
		
	Genome GetBestFitness(){
		return fittestGenome;
	}

	double GetAverageFitness(){
		return averageFitness;
	}
}



//import java.util.Vector;
//
//public class GenAlg {
//
//	private Vector vecPop = new Vector() ;  //装载个体的容器 
//	double totalFitness ;  //适应性分数的总和
//	int popSize ; //全部个体的总和
//
//	public GenAlg() {
//		// TODO Auto-generated constructor stub
//	}
//	
//	public Genome Roulette_wheel_selection() {
//		double FitnessSoFar = 0;   //累计适应性分数的和.  
//		double Slice = Math.random()*totalFitness ;
//		Genome TheChosenOne;   //这个基因将承载转盘所选出来的那个个体.  
//		for (int i=0; i<popSize; ++i) {  
////	        FitnessSoFar = FitnessSoFar + vecPop.get(i);  
////		        if (FitnessSoFar >= Slice) {
////		            TheChosenOne = i;  
//		            break;  
//		        }  
//		    }  
//		return TheChosenOne;  
//	}	
//}
//
