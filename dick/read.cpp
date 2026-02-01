#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int main() {
  std::ofstream outputFile("result.txt");
  if (!outputFile.is_open()) {
    std::cerr << "无法创建输出文件 result.txt" << std::endl;
    return 1;
  }
  
  int fileCount = 0;
  bool filesExist = true;
  
  // 依次读取文件，直到找不到更多文件
  while (filesExist) {
    std::stringstream filename;
    filename << "result_" << fileCount << ".txt";
    
    std::ifstream inputFile(filename.str());
    
    if (!inputFile.is_open()) {
      // 如果找不到文件，尝试几种命名方式
      if (fileCount == 0) {
        // 可能是不同的命名格式，尝试 result.txt 或 data.txt
        std::string alternativeNames[] = {"result.txt", "data.txt", "output.txt"};
        bool found = false;
        
        for (const auto& name : alternativeNames) {
          inputFile.open(name);
          if (inputFile.is_open()) {
            found = true;
            break;
          }
        }
        
        if (!found) {
          filesExist = false;
          break;
        }
      } else {
        filesExist = false;
        break;
      }
    }
    
    // 复制文件内容
    std::string line;
    while (std::getline(inputFile, line)) {
      outputFile << line << std::endl;
    }
    
    inputFile.close();
    fileCount++;
    
    // 安全限制：最多处理1000个文件
    if (fileCount > 1000) {
      std::cerr << "警告：已达到最大文件处理数量限制" << std::endl;
      break;
    }
  }
  
  outputFile.close();
  
  std::cout << "成功合并了 " << fileCount << " 个文件到 result.txt" << std::endl;
  
  return 0;
}
