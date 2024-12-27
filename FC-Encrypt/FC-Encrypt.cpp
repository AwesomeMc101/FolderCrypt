// FC-Encrypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
By AwesomeMc101
MIT
Requires C++17 to compile.
*/

#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <fstream>
void encrypt(std::string pass, std::string path) {

    /* Read File Data */
    std::ifstream mf(path);
    if (!mf.is_open()) {
        printf("Fail");
        mf.close();
        return;
    }
    std::string alldat;
    mf.seekg(0, mf.end);
    int len = mf.tellg();
    mf.seekg(0, mf.beg);
    while (alldat.size() < len) {
        const size_t def_rs = 512;
        size_t readsz = (len - alldat.size() > def_rs) ? def_rs : (len - alldat.size());
        std::cout << readsz << "\n";
        bool tag_end = 0;
        if (readsz < def_rs) { tag_end = true; }

        char* buf = new char[readsz + tag_end];
        mf.read(buf, readsz);
        if (tag_end) {
            buf[readsz] = '\0';
        }
        std::cout << buf << "\n";
        alldat.append(buf);
        delete[] buf;
    }
    std::cout << "\n" << alldat;
    mf.close();

    /* Transfer Data */
    std::vector<double> moved(alldat.size());
    std::copy(alldat.begin(), alldat.end(), moved.begin());
    alldat.clear();

    /* Encrypt */
    for (const char& c : pass) {
        /* Reverse Op */
        if (!(c % 3)) {
            std::reverse(moved.begin(), moved.end());
        }

        for (double& d : moved) {
            /* Sqrt-M Op */
            d *= (sqrtl(c) * cbrtl(c) / pass.size());
            std::cout << "\n1. " << d;

            /* Exp Op */
            d += exp(c / std::powl(pass.size(),2));
            std::cout << "\n2. " << d;

            /* Div Ops */
            d /= cbrt(log(c));
            std::cout << "\n3. " << d;


            
        }
    }

    std::ofstream of(path);
    for (double& d : moved) {
       of << d << ' ';
    }
    of.close();
}

void correct_string(std::string& dir) {
    dir.erase(std::remove_if(dir.begin(), dir.end(), [](char c) {
        return (c == '"');
        }));
    if (dir.back() == '"') {
        dir.pop_back();
    }
}

int main()
{
    std::string hy;
    std::cout << "Enter File Directory: ";
    std::getline(std::cin, hy);
    correct_string(hy);
    std::cout << hy;

    std::cout << "Enter Password (REMEMBER THIS): ";
    std::string pass;
    std::getline(std::cin, pass);

    std::filesystem::directory_iterator di(hy);
    for (auto& x : di) {
        std::cout << x.path();
        encrypt(pass, x.path().string());
    }

    char dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, dir);

    std::ofstream bat(hy + "\\decrypt.bat");
    std::string dire = dir;
    bat << "cd \"" << dir << "\"" << std::endl;
    bat << "FC-Decrypt.exe %\"" << hy << "\"";

    bat.close();
   
    
}

