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
int main(int argc, char const *argv[])
{
//    std::string find =  find_string("GET_files/test" , "GET_files");
    // std::string str = "/GET_files/test";
    // str.erase(0);
    // std::cout << str.size() << std::endl;
    std::cout << access("/Users/lelbakna/Desktop", F_OK) <<std::endl;
    // if (str.find_last_of('/') != 0)
    //     std::cout<<"find";
    return 0;
}