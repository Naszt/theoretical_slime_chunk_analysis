/*
链接：
-fwrapv -fopenmp -fopenmp-simd -Ofast -march=native -mtune=native -flto -funroll-loops

版本信息：
1.0 初始版本
*/
#include <bits/stdc++.h>
using i8=unsigned long long;
using i4=unsigned int;
inline bool check(i8 seed,int x,int z){//=H(seed+f(x)+g(z))
  i8 rng=(seed+x*x*4987142+x*5947611+z*z*4392871ll+z*389711);
  return (i4(((rng^0x5E434E432)*0x5DEECE66D)>>16)>>1)%10==0;
}
inline i8 f(int x){return 0ll+x*x*4987142+x*5947611;}
inline i8 g(int z){return z*z*4392871ll+z*389711;}
inline bool H(i8 r){return (i4(((r^0x5E434E432)*0x5DEECE66D)>>16)>>1)%10==0;}
i4 n=7;int bound;
std::unordered_multimap<int,i4>pos;
std::vector<int>join(i4 c){
  std::vector<int>vec;i4 cnt=0;
  for(int z=-bound;z<=bound;++z){
    if(!H(c+g(z))){cnt=0;continue;}
    if(++cnt<n)continue;
    if(pos.count(z)){vec.push_back(z);continue;}
    pos.insert({z,c});
  }return vec;
}
/*
f(x0)=c1 && f(x0+1)=c2
-> 9974284x+10934753=c2-c1 (mod 1<<32)
-> x=(c2-c1-10934753)/4*381395499 (mod 1<<30)
*/
bool solve(i4 c1,i4 c2){
  i4 delta=c2-c1;
  if((delta&3)!=1)return 0;
  i4 x0=(((delta-10934753)>>2)*381395499)&((1<<30)-1);
  for (i4 k=0;k^4;++k){
    int X=x0+(k<<30);
    i8 fx=f(X),fx1=f(X+1);
    if (fx1-fx==(i8)c2-c1) {
      printf("seed=%lld,x=%lld",c1-fx,16ll*X);
      return 1;
    }
  }return 0;
}
int main(){
  bound=pow(10,std::min(8u,n));
  std::mt19937 rnd(time(0));
  for(i4 cnt=0,coll=0,ans=0;;cnt++){
    printf("种子:%u,元素:%u,碰撞:%u,解:%u\r",cnt,pos.size(),coll,ans);
    i4 c1=cnt;
    auto all_z=join(c1);
    for(auto z:all_z){
      coll+=pos.count(z);
      auto [l,r]=pos.equal_range(z);
      for(auto it=l;it!=r;++it) {
        if(solve(c1,it->second))printf(",z=%lld\n",z*16ll),ans++;
        if(solve(it->second,c1))printf(",z=%lld\n",z*16ll),ans++;
      }pos.insert({z,c1});
    }
  }
  return 0;
}
/*
种子:519,元素:1015,碰撞:115,解:93
种子:519,元素:1307,碰撞:408,解:170


seed=-1410005562,x=17082890240,z=1401298256
seed=-1930307088,x=15454497216,z=-293327728
seed=560409832,x=9158562752,z=-509726416
seed=560408058,x=9158562752,z=401984464
seed=560408192,x=9158562752,z=598699760
seed=560408272,x=9158562752,z=-1140347648
seed=560408358,x=9158562752,z=-821698352
seed=560409074,x=9158562752,z=1127141968
seed=560409260,x=9158562752,z=1441728528
seed=560409352,x=9158562752,z=352791968
seed=560409690,x=9158562752,z=-759571152
seed=560409692,x=9158562752,z=-283151904
*/
