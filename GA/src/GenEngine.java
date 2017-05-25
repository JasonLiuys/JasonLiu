import java.util.Vector;

public class GenEngine {  //运算引擎

	private 
	GenAlg genAlg = new GenAlg();
	Curve curve = new Curve();
	Vector <Genome> m_population = new Vector<Genome>();
	int g_popsize; //*
	double g_dMutationRate; //*
	double g_dCrossoverRate; //*
	int g_numGen; //每条染色体的编码个数，这里是1个
	int g_Generation; //*
	double g_LeftPoint; //*
	double g_RightPoint; //*
	double bestFitness;
    double bestSearch;
	double averageFitness;
	
	public GenEngine(int popsize, double mutationRate, double crossoverRate, int numGen, int generation
			, double leftPoint, double rightPoint) {
		if (popsize%2 != 1) //保证个体总数为单数
			popsize ++ ;
		g_popsize=popsize;
		g_dMutationRate=mutationRate;
		g_dCrossoverRate=crossoverRate;
		g_numGen=numGen;
		g_Generation=generation;
		g_LeftPoint=leftPoint;
		g_RightPoint=rightPoint;
	    bestFitness=0;
        bestSearch=0;
	}
	
	void OnStartGenAlg(){
		
		//初始化遗传算法引擎
		genAlg.init(g_popsize, g_dMutationRate, g_dCrossoverRate, g_numGen,g_LeftPoint,g_RightPoint);
		m_population.clear();  //清空种群容器
		m_population = genAlg.vecPop;  //种群容器装进经过随机初始化的种群
		
		Vector <Double> input = new Vector<Double>() ;
		//double input;
		double output;
		//定义两个容器，以装进函数的输入与及输出
		//我们这个函数是单输入单输出的，但是以后往往不会那么简单，所以我们这里先做好这样的准备。
		//input.add((double) 0);

		for(int Generation = 1; Generation <= g_Generation; Generation++ ){ //循环种群代数
			//里面是对每一条染色体进行操作
			for( int i=0 ; i < g_popsize ; i ++ ){     //对整个种群的适应性进行评价
				input = m_population.get(i).vecGenome;   //获取第i个个体的染色体的基因序列

				//为每一个个体做适应性评价，如之前说的，评价分数就是函数值。其
				//Function函数的作用是输入自变量返回函数值，读者可以参考其代码。
				output = (double)curve.function(input);
				
				m_population.get(i).fitness = output;  //将输出值赋给适应性评价
//				System.out.println(m_population.get(i).vecGenome);
//				System.out.println(m_population.get(i).fitness);		
			}

			
			m_population = genAlg.Epoch(m_population);  //由父代种群进化出子代种群
			//if(genAlg.GetBestFitness().fitness>=bestFitness)
			bestSearch=genAlg.GetBestFitness().vecGenome.get(0);
			bestFitness=genAlg.GetBestFitness().fitness;
			averageFitness=genAlg.GetAverageFitness();
			report(Generation);
			
		}
	}

	void report( int Generation ){	//报告每一代的运行情况
		System.out.println("第"+Generation+"代");
		System.out.println("最佳适应度"+bestFitness);
		System.out.println("最佳适应度基因取值"+bestSearch);
		System.out.println("平均适应度"+averageFitness);
	}
	
}
