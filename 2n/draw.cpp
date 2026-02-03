#include<bits/stdc++.h>
#include<windows.h>
using i8=unsigned long long;
using i4=unsigned int;
inline bool check(i8 seed,int x,int z){//=H(seed+f(x)+g(z))
  i8 rng=(seed+x*x*4987142+x*5947611+z*z*4392871ll+z*389711);
  return (i4(((rng^0x5E434E432)*0x5DEECE66D)>>16)>>1)%10==0;
}
inline i8 f(int x){return 0ll+x*x*4987142+x*5947611;}
long long seed,x,z,n;
int main(){
  while(1){
    scanf(" seed=%lld,x=%lld,z=%lld,n=%lld",&seed,&x,&z,&n);
    x/=16,z/=16;
    i4 cnt1=0,cnt2=0;
    while(check(seed,x,z-cnt1))cnt1++;
    while(check(seed,x+1,z-cnt2))cnt2++;
    printf("列1=%u,列2=%u\n",cnt1,cnt2);
    cnt1=0,cnt2=0;
    while(check(seed,x,z+cnt1))cnt1++;
    while(check(seed,x+1,z+cnt2))cnt2++;
    printf("列1=%u,列2=%u\n",cnt1-1,cnt2-1);
    x-=7,z-=15;
    for(i4 rz=0;rz!=25;rz++){
      for(i4 rx=0;rx!=15;rx++){
        printf(check(seed,x+rx,z+rz)?"■":"□");
      }printf("\n");
    }printf("\n\n");
    Sleep(500);
  }
  return 0;
}
/*
seed=560409260,x=9158562752,z=1441728528
*/
