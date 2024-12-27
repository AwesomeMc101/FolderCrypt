// FC-Decrypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
By AwesomeMc101
MIT
Requires C++17 to compile.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

void decrypt(std::string pass, std::string path) {

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
    std::vector<double> moved;
    std::string buf;
    for (char c : alldat)
    {
        if (c == ' ') {
            moved.emplace_back(std::stold(buf)); //Encrypt leaves a trailing space!
            buf.clear();
        }
        else {
            buf.push_back(c);
            
        }
    }
    /* Encrypt */
    std::reverse(pass.begin(), pass.end());
    for (const char& c : pass) {
        /* Reverse Op */
        if (!(c % 3)) {
            std::reverse(moved.begin(), moved.end());
        }

        for (double& d : moved) {
            d *= cbrt(log(c));
            std::cout << "\n3. " << d;
            /* Exp Op */
            d -= exp(c / std::powl(pass.size(), 2));
            std::cout << "\n2. " << d;

            /* Sqrt-M Op */
            d /= (sqrtl(c) * cbrtl(c) / pass.size());
            std::cout << "\n1. " << d;

           

            /* Div Ops */
            


        }
    }

    std::ofstream of(path);
    for (double& d : moved) {
        of << (char)round(d);
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
int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please use decrypt.bat in your folder.");
        std::cin.get();
    }
    std::string dir = argv[1];
    correct_string(dir);
    system("cls");
entry: //dgaf, goto is valid
    std::cout << "If you enter your password incorrectly, you will need to re-encrypt with the incorrect password\nand then decrypt with the correct one.\nData can be lost- enter correctly!\n\n";
    std::cout << "Please Enter Your Password: ";
    std::string ln1;
    std::getline(std::cin, ln1);
    std::cout << std::endl << "Please Confirm Your Password: ";
    std::string ln2;
    std::getline(std::cin, ln2);
    if (ln1 != ln2) {
        system("cls");
        std::cout << "Passwords don't match!\n";
        goto entry;
    }

    std::filesystem::directory_iterator di(dir);
    for (auto& x : di) {
        std::cout << x.path();
        if (x.path().string().find("decrypt.bat") != std::string::npos) {
            continue;
        }
        decrypt(ln1, x.path().string());
    }

    //Done.
}

