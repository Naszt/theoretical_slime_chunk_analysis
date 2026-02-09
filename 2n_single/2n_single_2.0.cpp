/*
链接：
-fwrapv -Ofast -march=native -mtune=native -flto -funroll-loops
暂不需要：
-fopenmp -fopenmp-simd

版本信息：
1.0 初始版本 - 是从 2n2.0 改的。
2.0 分离了解方程与找长条，再做了一个漂亮的输出。
    顺便还优化了几处的常数。
*/
#include <bits/stdc++.h>
using i8=unsigned long long;
using i4=unsigned int;
const int Z=1246;
const i4 n=7,Del=30;
inline i8 f(int x){return 0ll+x*x*4987142+x*5947611;}
inline i8 g(int z){return z*z*4392871ll+z*389711;}
inline bool H(i8 r){return (i4(((r^0x5E434E432)*0x5DEECE66D)>>16)>>1)%10==0;}
struct Node{i8 c;i4 up,down;};
FILE*file;
std::deque<Node>pos;
std::vector<Node>posB;
void checkB(const i8 B){
  i8 gZ=g(Z)+(B<<30),g0[Del],g1[Del];
  for(i8 i=1;i<Del;i++)g0[i]=g(Z+i),g1[i]=g(Z-i);
  for(i8 c=B<<33;c<((B+1)<<33);c++){
    if((c&((1<<28)-1))==0)
      printf("\e[u进度:%.2lf,个数:%lld\e[K",1.0*c/(1ll<<33),posB.size());
    if(!H(gZ++))continue;
    i4 up=1,down=1;
    while(H(c+g0[up  ]))up  ++;up  --;
    while(H(c+g1[down]))down++;down--;
    if(up+down+1<n)continue;
    posB.push_back({c,up,down});
  }
}
i4 solve(i8 c1,i8 c2,i4 cnt){
  i4 del=c2-c1,ans=0;
  if((del&3)!=1){
    del=-del,std::swap(c1,c2);
    if((del&3)!=1)return 0;
  }
  for(i4 k=0,X=(((del-10934753)>>2)*381395499)&((1<<30)-1);k<4;++k,X+=(1<<30))
    if(f(X+1)-f(X)==c2-c1)++ans,
      fprintf(file,"seed=%lld,x=%lld,z=%lld,n=%u\n",c1-f(X),X*16ll,Z*16ll,cnt);
  return ans;
}
void slove(i8&ans){
  i8 size=pos.size();
  for(auto [c1,up1,down1]:posB){
    if(size&&pos.front().c+(1ll<<33)<=c1)--size,pos.pop_front();
    for(auto [c2,up2,down2]:pos){
      i4 cnt=std::min(up1,up2)+std::min(down1,down2)+1;
      if(cnt<n)continue;
      ans+=solve(c1,c2,cnt);
    }pos.push_back({c1,up1,down1});
  }posB={};
}
int main(){
  file=fopen("result.txt","w");
  printf("\e[?25l");
  for(i8 B=0,ans=0;;++B){
    printf("\e[H\e[36m〔找长条〕\e[0m\e[s");checkB(B);
    printf("\r〔找长条〕进度:%lld.00,个数:%lld\033[K\n",B+1,posB.size());
    printf("\e[36m〔解方程〕\e[0m");slove(ans);
    printf("\r〔解方程〕解:%lld\e[K\r",ans);
  }
  return 0;
}
