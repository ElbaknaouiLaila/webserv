#include <iostream>
#include <string>

// int main() {
//     std::string url = "/file/file2/file3/text";
//     while (url.find('/') != std::string::npos) {
//         std::cout<<"befor : "<<url<<std::endl;
//             // break;
//         if (url == "/")
//             url.erase(url.find_last_of('/'));
//         std::cout<<"after : "<<url<<std::endl;
//     }
//     // std::cout << url << std::endl;
//     return 0;
// }
// #include <iostream>
// #include <string>

// int main() {
//     std::string url = "/desktop/file/file2/file3/text";
//     while (url.find_last_of('/') != 0) {
//         url.erase(url.find_last_of('/'));
//         std::cout << url << std::endl;
//     }
//     // std::cout << url << std::endl;
//     return 0;
// }
// #include <iostream>
// #include <string>

// int main() {
//     std::string url = "/desktop/file/file2/file3/text";
//     while (url != "/" && url.find_last_of('/') != 0) {
//         url.erase(url.find_last_of('/'));
//     }
//     std::cout << url << std::endl;
//     return 0;
// }
// #include <sys/stat.h>
// bool isDirectory(const char* path) {
//     struct stat info;
//     if (stat(path, &info) != 0) {
//         // Error occurred
//         return false;
//     }
//     return (info.st_mode & S_IFDIR) != 0;
// }
// #include <iostream>
// #include <string>

// int main() {
//     std::string url = "/desktop/file/file2/file3/text";
//     bool flag = false;
//     while (url != "/" && url.find_last_of('/') != 0) {
//         if (flag == false)
//         {
//             std::cout << url << std::endl;
//             flag == true;
//         }
//         url.erase(url.find_last_of('/'));
//         std::cout << url << std::endl;
//         if (url.find_last_of('/') == 0) {
//             url = "/";
//             std::cout << url << std::endl;
//         }
//     }
//     return 0;
// }
std::string find_string(const std::string& str, const std::string& substr) {
    size_t pos = str.find(substr);
    std::cout << pos << std::endl;
    if (pos != std::string::npos) {
        std::cout << "pppp\n";
        return str.substr(substr.size()+1);
    }
    return "";
}
#include <unistd.h>
// bool ft()
// {
//     int a = 1;
//     int b = 2;
//     if (a > b)
//     {
//         return false;
//     }
//     return true;
    
// }
std::string ft()
{
    std::string a = "sds";
    return a;
    
}
#include <sys/stat.h>

bool is_directory(const char* path)
{
    struct stat st;

    if (stat(path, &st) != 0)
    {
        // stat() failed, path does not exist
        return false;
    }

    return S_ISDIR(st.st_mode);
}

// int main(int argc, char const *argv[])
// {
//     const char* path = "/upload";
//     if (is_directory(path))
//     {
//         std::cout<<"DIR";
//     }
//     else
//     {
//         std::cout<<"not DIR";
//         // Handle the case where the path does not exist or is not a directory
//     }
//     // if(ft().size())
//     //     std::cout << "pppp\n";
// //    std::string find =  find_string("GET_files/test" , "GET_files");
//     // std::string str = "/GET_files/test";
//     // str.erase(0);
//     // std::cout << str.size() << std::endl;
//     // std::cout << access("/Users/lelbakna/Desktop", F_OK) <<std::endl;
//     // if (str.find_last_of('/') != 0)
//     //     std::cout<<"find";
//     return 0;
// }
#include <errno.h>
#include <iostream>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/time.h>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
std::string find_extension(std::string filename)
{
    std::string extractedExtension;
    std::size_t dotPos = filename.rfind(".");
    if (filename.empty())
        return ("");
    if (dotPos == std::string::npos) {
        std::cout << "File has no extension" << std::endl;
    } else {
        extractedExtension = filename.substr(dotPos + 1);
        if ((extractedExtension == "php")) {
            // set_cgiExtension("php");
            return "php";
        } else if((extractedExtension == "py")){
            // set_cgiExtension("py");
            return "py";
        }
    }
    return extractedExtension;
}
int main() {

   const char* sourceFile = "file.txt";
//     std::fstream   requestBody;
//     requestBody.open(sourceFile, std::ios::out | std::ios::app | std::ios::binary);

//    const char* destinationDirectory = "/upload/";
//    std::string path = "webservteam/GET_files";
//    std::string destinationFile = path + destinationDirectory + std::string(sourceFile) ;
   
//    int result = rename(sourceFile, destinationFile.c_str());
//    if (result != 0) {
//       std::cerr << "Error moving file : " << destinationFile <<std::endl;
//       exit(EXIT_FAILURE);
//    }
   std::cout << find_extension("file.py") << std::endl;
   return 0;
}