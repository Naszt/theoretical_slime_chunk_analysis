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
i4 n=6;int bound;
std::unordered_multimap<int,i4>pos;
//std::vector<int>join(i4 c){
//  std::vector<int>vec;i4 cnt=0;
//  for(int z=-bound;z<=bound;++z){
//    if(H(c+g(z))){++cnt;continue;}
//    if(cnt<n){cnt=0;continue;}cnt=0;
//    if(pos.count(z-1)){vec.push_back(z-1);continue;}
//    pos.insert({z-1,c});
//  }return vec;
//}
std::vector<int> join(i4 c) {
  const int total_range = 2 * bound + 1;
  const int num_threads = std::thread::hardware_concurrency();
  const int chunk_size = total_range / num_threads + 1;
  
  std::vector<std::future<std::vector<int>>> futures;
  std::vector<int> result;
  std::mutex result_mutex;
  std::mutex pos_mutex;  // 如果 pos 需要线程安全访问
  
  auto worker = [&](int start_z, int end_z) -> std::vector<int> {
    std::vector<int> local_vec;
    i4 local_cnt = 0;
    
    for (int z = start_z; z <= end_z; ++z) {
      if (H(c + g(z))) {
        ++local_cnt;
        continue;
      }
      
      if (local_cnt < n) {
        local_cnt = 0;
        continue;
      }
      local_cnt = 0;
      
      // 线程安全的 pos 访问
      bool has_pos;
      {
        std::lock_guard<std::mutex> lock(pos_mutex);
        has_pos = pos.count(z - 1);
      }
      
      if (has_pos) {
        local_vec.push_back(z - 1);
        continue;
      }
      
      {
        std::lock_guard<std::mutex> lock(pos_mutex);
        pos.insert({z - 1, c});
      }
    }
    return local_vec;
  };
  
  // 启动线程
  for (int t = 0; t < num_threads; ++t) {
    int start_z = -bound + t * chunk_size;
    int end_z = std::min(start_z + chunk_size - 1, bound);
    if (start_z <= bound) {
      futures.push_back(std::async(std::launch::async, worker, start_z, end_z));
    }
  }
  
  // 收集结果
  for (auto& future : futures) {
    auto partial_result = future.get();
    std::lock_guard<std::mutex> lock(result_mutex);
    result.insert(result.end(), partial_result.begin(), partial_result.end());
  }
  
  return result;
}
bool solve(i4 c1,i4 c2,int z){
  i4 delta=c2-c1;
  if((delta&3)!=1)return 0;
  i4 x0=(((delta-10934753)>>2)*381395499)&((1<<30)-1);
  for (i4 k=0;k^4;++k){
    int x=x0+(k<<30);
    i8 fx=f(x),fx1=f(x+1);
    if(fx1-fx==(i8)c2-c1){
      i8 s=c1-fx;i4 cnt=0;
      cnt =check(s,x-1,z+1)+check(s,x+2,z+1);
      if(cnt<2)return 1;
      cnt+=check(s,x-1,z+2)+check(s,x-2,z+1)+check(s,x-2,z+2)+check(s,x+2,z+2)+check(s,x+3,z+1)+check(s,x+3,z+2);
      if(cnt<=7)return 1;
      printf("seed=%lld,x=%lld",s,16ll*x);
      printf(",z=%lld            \n",z*16ll);
//      printf(",z=%lld:check=%u\n",z*16ll,cnt);
      return 1;
    }
  }return 0;
}
int main(){
  bound=pow(10,std::min(8u,n+1));
  std::mt19937 rnd(time(0));
  for(i4 cnt=0,coll=0,ans=0;;cnt++){
    if(cnt%100==0)printf("种子:%u,元素:%u,碰撞:%u,解:%u\r",cnt,pos.size(),coll,ans);
    i4 c1=cnt;
    auto all_z=join(c1);
    for(auto z:all_z){
      coll+=pos.count(z);
      auto [l,r]=pos.equal_range(z);
      for(auto it=l;it!=r;++it){
        if(solve(c1,it->second,z))ans++;
        if(solve(it->second,c1,z))ans++;
      }pos.insert({z,c1});
    }
  }
  return 0;
}
/*
种子:14600500,元素:290236896,碰撞:936602187,解:475296228
*/
