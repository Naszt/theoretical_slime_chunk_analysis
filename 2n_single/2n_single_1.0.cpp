/*
链接：
-fwrapv -Ofast -march=native -mtune=native -flto -funroll-loops
暂不需要：
-fopenmp -fopenmp-simd

版本信息：
1.0 初始版本-是从 2n2.0 改的。
*/
#include <bits/stdc++.h>
using i8=unsigned long long;
using i4=unsigned int;
using i1=unsigned char;
const int Z=1246;
const i4 n=9;
inline i8 f(int x){return 0ll+x*x*4987142+x*5947611;}
inline i8 g(int z){return z*z*4392871ll+z*389711;}
inline bool H(i8 r){return (i4(((r^0x5E434E432)*0x5DEECE66D)>>16)>>1)%10==0;}
struct Node{i8 c;i4 up,down;};
std::deque<Node>pos;
inline std::optional<std::pair<i4,i4>>join(i8 c){
  if(!H(c+g(Z)))return std::nullopt;
  i4 up=1,down=1;
  while(H(c+g(Z+up  )))up  ++;up  --;
  while(H(c+g(Z-down)))down++;down--;
  if(up+down+1<n)return std::nullopt;
  return std::pair{up,down};
}
i4 solve(i8 c1,i8 c2,i4 cnt){
  i4 delta=c2-c1,ans=0;
  if((delta&3)!=1){
    delta=-delta,std::swap(c1,c2);
    if((delta&3)!=1)return 0;
  }
  i4 x0=(((delta-10934753)>>2)*381395499)&((1<<30)-1);
  for(i4 k=0;k<4;++k){
    int X=x0+(k<<30);
    i8 fx=f(X),fx1=f(X+1);
    if(fx1-fx==c2-c1) {
      printf("seed=%lld,x=%lld,z=%lld,n=%u\n",c1-fx,X*16ll,Z*16ll,cnt);
      ans++;
    }
  }return ans;
}
int main(){
  freopen("result.txt","w",stdout);
  for(i8 c1=0,ans=0;;c1++){
    if((c1&((1<<(19+n))-1))==0)
      fprintf(stderr,"c:%lf<<33,元素:%lld,解:%lld\r",1.0*c1/(1ll<<33),pos.size(),ans);
    if((!pos.empty())&&pos.front().c+(1ll<<33)<=c1)pos.pop_front();
    auto result=join(c1);
    if(!result.has_value())continue;
    auto [up1,down1]=result.value();
    pos.push_back({c1,up1,down1});
    for(auto [c2,up2,down2]:pos){
      i4 cnt=std::min(up1,up2)+std::min(down1,down2)+1;
      if(cnt<n)continue;
      ans+=solve(c1,c2,cnt);
    }
  }
  return 0;
}
