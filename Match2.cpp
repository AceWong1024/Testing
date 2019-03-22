#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
using namespace std;
#define PI 3.1415926

class Match{	
	private:
		//阈值 Dst越小评分越严格 
		int Dst = 65;
		int Sst = 80;
		int M = 30;
		
		//模板特征向量与实时特征向量的余弦值
		double cosine(const int& Mx0,const int& My0,const int& Mx1,const int& My1,const int& Ax0,const int& Ay0,const int& Ax1,const int& Ay1){
			//向量积 
			int product = (Mx0 - Mx1)*(Ax0 - Ax1) + (My0 - My1)*(Ay0 - Ay1);
			double Mlength = sqrt((Mx0 - Mx1)*(Mx0 - Mx1) + (My0 - My1)*(My0 - My1));
			double Alength = sqrt((Ax0 - Ax1)*(Ax0 - Ax1) + (Ay0 - Ay1)*(Ay0 - Ay1));
			if(Mlength == 0 || Alength == 0)
				return 0;
			return acos(static_cast<float>(product/(Mlength*Alength)))*180/PI;
		}
		
		//计算权重 
		vector<double> calcuW(const vector<double>& a){
			double asum = accumulate(a.begin() , a.end() , 0.0);
			vector<double> weight(9,0);
			if(asum == 0)
				return weight;
			int i;
			for(i=0;i<weight.size();i++){
				double temp = -a[i]/asum;
				weight[i] = 1 - exp(temp);
			}
			
			double wsum = accumulate(weight.begin() , weight.end() , 0.0);
			for(i=0;i<weight.size();i++){
				weight[i] /= wsum;
			}
			return weight;
		}
		
		//计算S
		double calcuS(const double& D,const double& amax){
			if(D > Dst){
				return 0.0;
			}else{
				if(D >= 0){
					//输出f=()进行测试 
//					cout << "ffunction() = "<< ffunction(amax) << endl;
					return ((Dst - D)*(100 - Sst)/Dst + Sst)*ffunction(amax);
				}else{
					cout << "Invalid S!" << endl; 
					return -1;
				}
			}
		}
		
		//计算amax
		double findmax(const vector<double>& data){
			double max = 0;
			for(auto it=data.begin();it!=data.end();++it){
				if(*it > max)
					max = *it;
			}
			return max;
		}
		
		//计算f()
		double ffunction(const double& amax){
			if(amax > 1.82574*M){
				return 0;
			}else{
				if(amax >= 0){
					return 1-(0.3*amax*amax/(M*M));
				}else{
					cout << "Invalid f()!" << endl; 
					return -1;
				}
			}
		} 
	public:
		//单例模式				
		static Match& Instance(){
			static Match instance;
			return instance;
		}
		
		double calculate(const vector<double>& data,const vector<double>& modle){
			//计算模板与数据向量夹角 
			vector<double> a(9);
			a[0] = cosine(modle[22],modle[23],modle[20],modle[21],data[22],data[23],data[20],data[21]);	//11-12
			a[1] = cosine(modle[12],modle[13],modle[24],modle[25],data[12],data[13],data[24],data[25]);	//13-7
			a[2] = cosine(modle[14],modle[15],modle[26],modle[27],data[14],data[15],data[26],data[27]);	//14-8
			a[3] = cosine(modle[8],modle[9],modle[12],modle[13],data[8],data[9],data[12],data[13]);	  	//7-5
			a[4] = cosine(modle[10],modle[11],modle[14],modle[15],data[10],data[11],data[14],data[15]);	//8-6
			a[5] = cosine(modle[4],modle[5],modle[0],modle[1],data[4],data[5],data[0],data[1]);			//1-3
			a[6] = cosine(modle[6],modle[7],modle[2],modle[3],data[6],data[7],data[2],data[3]);			//2-4
			a[7] = cosine(modle[16],modle[17],modle[4],modle[5],data[16],data[17],data[4],data[5]);		//3-9
			a[8] = cosine(modle[18],modle[19],modle[6],modle[7],data[18],data[19],data[6],data[7]);		//4-10
//			测试代码 
//						for(auto it=a.begin();it != a.end();++it){
//							cout << "vector" << ":" << (*it) << endl;
//						}
			
			//计算权重 
			vector<double> weight = calcuW(a);
			
			//D:归一化角度值 
			double D = 0.0;
			for(int i = 0;i < a.size();++i)
				D+=weight[i]*a[i];
//			cout << "D = " << D << endl;
			//			cout << "max = " << findmax(a) << endl;
			//评分
			return calcuS(D,findmax(a));
		}
};

int main(int argc,char** argv){
//	vector<int> data = {3,5,1,3,3,3,5,2,2,1,4,1};
//	vector<int> modle = {3,5,1,2,3,3,5,2,2,1,4,1};
//	vector<int> data = {
//	207,304,
//	155,302,
//	225,398,
//	136,391,
//	290,204,
//	116,157,
//	283,177,
//	86,193,
//	231,474,
//	128,481,
//	185,50,
//	182,117,
//	233,148,
//	136,143};
//	vector<int> modle = {
//	 208 ,297 ,
//  158 ,294,
//  228 ,399,
//  136 ,385,
//  291 ,202,
//  114 ,153,
//  285 ,179,
//   88 ,187,
//  233 ,467,
//  127 ,478,
//  187  ,43,
//  187 ,114,
//  237 ,141,
//  138 ,139	
//	};
//	vector<int> modle = {633,780,543,770,640,947,506,961,692,551,613,656,676,612,492,608,639,1131,493,1151,610,319,595,441,671,488,504,486};
//	vector<int> modle = { 
// 174 ,260,
// 141 ,258,
// 182 ,317,
// 135 ,317,
// 189 ,189,
// 121 ,182,
// 231 ,184,
//  87 ,185,
// 191 ,370,
// 132 ,365,
// 157 ,109,
// 155 ,155,
// 189 ,169,
// 126 ,171};
//	vector<int> modle = {
//	231 ,338 ,
//  180 ,336 ,
//  244 ,439 ,
//  150 ,424 ,
//  268 ,312 ,
//  143 ,294 ,
//  292 ,252 ,
//  128 ,250 ,
//  239 ,511 ,
//  127 ,520 ,
//  213 , 82 ,
//  218 ,158 ,
//  268 ,181 ,
//  163 ,184 
//	};
//	vector<int> modle = {
//	244 ,432,
// 138 ,412,
// 376 ,572,
// 259 ,518,
// 490 ,523,
// 190 ,532,
// 412 ,391,
// 210 ,393,
// 350 ,710,
// 144 ,686,
// 393 ,105,
// 376 ,229,
// 405 ,269,
// 288 ,273};
//	vector<int> data = {595.19655262,  421.24224447,  492.68112784,  821.98435949,  585.87696855,  853.04963973,  359.10042283,  924.49978427,  567.23780041, 1042.54784916,  234.83930189,  564.14253355,  949.34074729,  573.46211762,  374.63306295,  576.56864564,  809.54698624,  585.88822971,  216.20013375, 1045.65437718,  545.49210424, 1191.66119429,  598.30308064,  545.50336541,  502.00071191,  567.24906157,  685.2858653 ,  564.14253355};
	vector<double> modle = {442.71933922,  422.12087414,  529.88333801,  988.68686629,  467.62333887,  907.74886741,  601.48233702, 1225.27486301,  293.29534129,  907.74886741,  262.16534172,  534.18887259,  287.06934137,  555.97987229,  293.29534129,  661.82187082,  308.86034107,  664.93487078,  875.42633322, 1153.675864  ,  283.95634142, 1119.43286448,  461.39733896,  599.56187168,  492.52733853,  593.33587177,  492.52733853,  590.22287181};
    vector<double> data = {368.00734025,  282.03587608,  501.8663384 ,  976.23486646,  476.96233874,  904.63586745,  517.43133818, 1306.21286189,  221.69634228,  929.53986711,  181.22734284,  471.92887345,  209.24434245,  468.8158735 ,  234.14834211,  664.93487078,  271.50434159,  618.23987142,  775.8103346 , 1200.37086336,  252.82634185, 1172.35386374,  452.05833909,  515.51087285,  405.36333973,  527.96287268,  402.25033978,  521.73687276};
//	vector<int> data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cout << "评分: "<< Match::Instance().calculate(data,modle) << endl;
}

